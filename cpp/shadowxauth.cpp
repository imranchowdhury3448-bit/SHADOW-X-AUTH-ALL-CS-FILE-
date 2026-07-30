#include <iostream>
#include <string>
#include <curl/curl.h>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

/**
 * SHADOW X AUTH - C++ SDK
 * GitHub: https://github.com/imranchowdhury3448-bit/SHADOW-X-AUTH-ALL-CS-FILE-
 * Discord: https://discord.gg/WwHrs5RVS
 */
class ShadowXAuth {
private:
    std::string ownerid;
    std::string secret;
    std::string version;
    std::string apiUrl = "https://shadowxauth.onrender.com/api/auth";

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    std::string getHWID() {
        return "CPP-HWID-MACHINE-ID";
    }

public:
    bool isAuthenticated = false;
    std::string username;
    std::string subscription;
    std::string expiry;
    std::string message;

    ShadowXAuth(std::string ownerid, std::string secret, std::string version = "1.0") 
        : ownerid(ownerid), secret(secret), version(version) {}

    bool login(const std::string& user, const std::string& pass) {
        CURL* curl = curl_easy_init();
        if (!curl) return false;

        json payload = {
            {"ownerid", ownerid},
            {"secret", secret},
            {"username", user},
            {"password", pass},
            {"hwid", getHWID()},
            {"version", version}
        };

        std::string jsonStr = payload.dump();
        std::string readBuffer;

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, (apiUrl + "/login").c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonStr.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 35L);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            message = "Connection failed";
            return false;
        }

        try {
            json resJson = json::parse(readBuffer);
            if (resJson.value("success", false)) {
                isAuthenticated = true;
                username = resJson["user"].value("username", user);
                subscription = resJson["user"].value("subscription", "default");
                expiry = resJson["user"].value("expiry", "");
                message = resJson.value("message", "Authenticated");
                return true;
            }
            message = resJson.value("message", "Auth failed");
        } catch (...) {
            message = "Invalid server response";
        }
        return false;
    }
};

int main() {
    ShadowXAuth auth("YOUR_OWNERID", "YOUR_SECRET", "1.0");
    if (auth.login("testuser", "testpass")) {
        std::cout << "Welcome " << auth.username << "!\n";
    } else {
        std::cout << "Error: " << auth.message << "\n";
    }
    return 0;
}
