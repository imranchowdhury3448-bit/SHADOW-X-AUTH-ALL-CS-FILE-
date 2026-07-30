// ========================================================================
// SHADOW X AUTH - C# (.NET 6/7/8/9) WORKING SAMPLE PROJECT
// ========================================================================

using System;
using ShadowXAuthSDK;

namespace ShadowXAuthSample
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("====================================================");
            Console.WriteLine("        SHADOW X AUTH - C# SAMPLE PROJECT          ");
            Console.WriteLine("====================================================\n");

            // Step 1: Initialize SDK
            var auth = new ShadowXAuth("MY_APP", "25D2EFE1BE", "39f8e1a629524947a1767597cd3159967742aed0116a49529e508bba70ad081c", "1.0");

            // Step 2: Prompt user for license key
            Console.Write("Enter License Key: ");
            string key = Console.ReadLine();

            // Step 3: Validate license key
            var res = auth.ValidateLicense(key);

            if (res.Success)
            {
                Console.WriteLine($"\n[+] Access Granted!");
                Console.WriteLine($"[+] Welcome, {res.Username}");
                Console.WriteLine($"[+] Plan Expiry: {res.Expiry}");
            }
            else
            {
                Console.WriteLine($"\n[-] Access Denied: {res.Message}");
            }
        }
    }
}
