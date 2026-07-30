// ========================================================================
// SHADOW X AUTH - C++ WORKING SAMPLE PROJECT
// ========================================================================

#include <iostream>
#include <string>
#include "shadowxauth.h"

int main() {
    std::cout << "====================================================\n";
    std::cout << "        SHADOW X AUTH - C++ SAMPLE PROJECT          \n";
    std::cout << "====================================================\n\n";

    // Step 1: Initialize SDK
    ShadowXAuth auth("MY_APP", "25D2EFE1BE", "39f8e1a629524947a1767597cd3159967742aed0116a49529e508bba70ad081c", "1.0");

    // Step 2: Prompt user for license key
    std::cout << "Enter License Key: ";
    std::string key;
    std::cin >> key;

    // Step 3: Validate license key
    auto res = auth.validateLicense(key);

    if (res.success) {
        std::cout << "\n[+] Access Granted!\n";
        std::cout << "[+] Username : " << res.username << "\n";
        std::cout << "[+] Expiry   : " << res.expiry << "\n";
    } else {
        std::cout << "\n[-] Access Denied: " << res.message << "\n";
        return 1;
    }

    return 0;
}
