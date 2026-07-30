/*
 * SHADOW X AUTH - C++ SDK
 * Requires: libcurl, nlohmann/json, OpenSSL (for HWID hashing)
 *
 * Usage:
 *   ShadowXAuth auth("My App", "ownerid", "secret", "1.0");
 *   auto result = auth.login("username", "password");
 *   if (result.success) { std::cout << "Welcome " << result.username; }
 */

#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#ifdef _WIN32
  #include <windows.h>
  #include <comdef.h>
  #include <Wbemidl.h>
  #pragma comment(lib, "wbemuuid.lib")
  #pragma comment(lib, "comsuppw.lib")
  #pragma comment(lib, "libcurl.lib")
#endif

using json = nlohmann::json;

namespace ShadowXAuthSDK {

// ── Result struct ─────────────────────────────────────────────────────────────
struct AuthResult {
    bool        success;
    std::string message;
    std::string token;
    std::string username;
    std::string subscription;
    std::string expiry;
};

// ── HTTP helper ───────────────────────────────────────────────────────────────
static std::string _response_body;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

static std::string httpPost(const std::string& url, const std::string& body) {
    CURL* curl = curl_easy_init();
    std::string response;

    if (curl) {
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL,            url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS,     body.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER,     headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,  WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA,      &response);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT,        10L);

        curl_easy_perform(curl);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    return response;
}

// ── HWID ──────────────────────────────────────────────────────────────────────
static std::string simpleHash(const std::string& input) {
    // Simple FNV-1a hash (replace with SHA256 via OpenSSL for production)
    uint64_t hash = 14695981039346656037ULL;
    for (char c : input) {
        hash ^= static_cast<uint64_t>(c);
        hash *= 1099511628211ULL;
    }
    std::ostringstream ss;
    ss << std::hex << hash;
    std::string h = ss.str();
    while (h.length() < 32) h += "0";
    return h.substr(0, 32);
}

static std::string getHWID() {
#ifdef _WIN32
    try {
        HRESULT hr;
        hr = CoInitializeEx(0, COINIT_MULTITHREADED);

        IWbemLocator* pLoc = nullptr;
        hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER,
                              IID_IWbemLocator, (LPVOID*)&pLoc);

        IWbemServices* pSvc = nullptr;
        hr = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &pSvc);

        // Get disk serial
        std::string diskSerial;
        IEnumWbemClassObject* pEnum = nullptr;
        pSvc->ExecQuery(bstr_t("WQL"),
                        bstr_t("SELECT SerialNumber FROM Win32_DiskDrive"),
                        WBEM_FLAG_FORWARD_ONLY, NULL, &pEnum);
        if (pEnum) {
            IWbemClassObject* pObj = nullptr;
            ULONG uRet = 0;
            pEnum->Next(WBEM_INFINITE, 1, &pObj, &uRet);
            if (pObj) {
                VARIANT v;
                pObj->Get(L"SerialNumber", 0, &v, 0, 0);
                diskSerial = _bstr_t(v.bstrVal);
                VariantClear(&v);
                pObj->Release();
            }
            pEnum->Release();
        }

        // Get CPU ID
        std::string cpuId;
        IEnumWbemClassObject* pEnum2 = nullptr;
        pSvc->ExecQuery(bstr_t("WQL"),
                        bstr_t("SELECT ProcessorId FROM Win32_Processor"),
                        WBEM_FLAG_FORWARD_ONLY, NULL, &pEnum2);
        if (pEnum2) {
            IWbemClassObject* pObj = nullptr;
            ULONG uRet = 0;
            pEnum2->Next(WBEM_INFINITE, 1, &pObj, &uRet);
            if (pObj) {
                VARIANT v;
                pObj->Get(L"ProcessorId", 0, &v, 0, 0);
                cpuId = _bstr_t(v.bstrVal);
                VariantClear(&v);
                pObj->Release();
            }
            pEnum2->Release();
        }

        pSvc->Release();
        pLoc->Release();
        CoUninitialize();

        return simpleHash(diskSerial + cpuId);
    } catch (...) {
        char buf[256];
        DWORD len = 256;
        GetComputerNameA(buf, &len);
        return simpleHash(std::string(buf));
    }
#else
    // Linux/Mac fallback
    return simpleHash("linux_machine");
#endif
}

// ── Main class ────────────────────────────────────────────────────────────────
class ShadowXAuth {
private:
    std::string _name;
    std::string _ownerid;
    std::string _secret;
    std::string _version;
    std::string _hwid;
    std::string _baseUrl;

public:
    std::string token;
    std::string username;
    std::string subscription;
    std::string expiry;
    bool isLoggedIn = false;

    ShadowXAuth(
        const std::string& name,
        const std::string& ownerid,
        const std::string& secret,
        const std::string& version,
        const std::string& serverUrl = "http://localhost:5000/api/auth"
    ) : _name(name), _ownerid(ownerid), _secret(secret), _version(version), _baseUrl(serverUrl) {
        _hwid = getHWID();
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }

    ~ShadowXAuth() { curl_global_cleanup(); }

    AuthResult login(const std::string& uname, const std::string& password) {
        json payload = {
            {"ownerid",  _ownerid},
            {"secret",   _secret},
            {"version",  _version},
            {"username", uname},
            {"password", password},
            {"hwid",     _hwid}
        };

        std::string body = payload.dump();
        std::string resp = httpPost(_baseUrl + "/login", body);

        if (resp.empty())
            return {false, "Connection failed — is the server running?", "", "", "", ""};

        try {
            json j = json::parse(resp);
            bool success = j.value("success", false);

            if (success) {
                token        = j.value("token",        "");
                username     = j.value("username",     "");
                subscription = j.value("subscription", "default");
                expiry       = j.value("expiry",       "");
                isLoggedIn   = true;
            }

            return {
                success,
                j.value("message", ""),
                j.value("token",    ""),
                j.value("username", ""),
                j.value("subscription", ""),
                j.value("expiry",   "")
            };
        } catch (...) {
            return {false, "Invalid server response", "", "", "", ""};
        }
    }

    bool verifySession() {
        if (token.empty()) return false;
        json payload = {{"token", token}};
        std::string resp = httpPost(_baseUrl + "/verify", payload.dump());
        if (resp.empty()) return false;
        try {
            return json::parse(resp).value("success", false);
        } catch (...) { return false; }
    }

    std::string getLocalHWID() const { return _hwid; }
};

} // namespace ShadowXAuthSDK
