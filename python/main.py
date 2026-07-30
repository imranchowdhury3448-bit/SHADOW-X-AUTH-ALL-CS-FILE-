# ========================================================================
# SHADOW X AUTH - PYTHON WORKING SAMPLE PROJECT
# ========================================================================

from shadowxauth import ShadowXAuth

print("====================================================")
print("       SHADOW X AUTH - PYTHON SAMPLE PROJECT        ")
print("====================================================\n")

auth = ShadowXAuth(
    name    = "MY_APP",
    ownerid = "25D2EFE1BE",
    secret  = "39f8e1a629524947a1767597cd3159967742aed0116a49529e508bba70ad081c",
    version = "1.0"
)

key = input("Enter License Key: ")

result = auth.validate_license(key)

if result.get("success"):
    print(f"\n[+] Access Granted! Welcome {result.get('username')}")
else:
    print(f"\n[-] Access Denied: {result.get('message')}")
