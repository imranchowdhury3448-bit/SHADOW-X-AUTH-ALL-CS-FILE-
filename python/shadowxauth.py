import requests
import platform
import uuid

class ShadowXAuth:
    """
    SHADOW X AUTH - Python SDK
    GitHub: https://github.com/imranchowdhury3448-bit/SHADOW-X-AUTH-ALL-CS-FILE-
    Discord: https://discord.gg/WwHrs5RVS
    """
    API_URL = "https://shadowxauth.onrender.com/api/auth"

    def __init__(self, ownerid: str, secret: str, version: str = "1.0"):
        self.ownerid = ownerid
        self.secret = secret
        self.version = version
        self.is_authenticated = False
        self.username = ""
        self.subscription = ""
        self.expiry = ""
        self.message = ""

    def _get_hwid(self) -> str:
        return f"{platform.node()}-{uuid.getnode()}"

    def login(self, username: str, password: str) -> bool:
        payload = {
            "ownerid": self.ownerid,
            "secret": self.secret,
            "username": username,
            "password": password,
            "hwid": self._get_hwid(),
            "version": self.version
        }
        try:
            r = requests.post(f"{self.API_URL}/login", json=payload, timeout=35)
            data = r.json()
            if data.get("success"):
                self.is_authenticated = True
                user_info = data.get("user", {})
                self.username = user_info.get("username", username)
                self.subscription = user_info.get("subscription", "default")
                self.expiry = user_info.get("expiry", "")
                self.message = data.get("message", "Authenticated")
                return True
            self.message = data.get("message", "Login failed")
            return False
        except Exception as e:
            self.message = f"Connection error: {str(e)}"
            return False

    def register(self, username: str, password: str, email: str = "", license_key: str = "") -> bool:
        payload = {
            "ownerid": self.ownerid,
            "secret": self.secret,
            "username": username,
            "password": password,
            "email": email,
            "license_key": license_key,
            "hwid": self._get_hwid()
        }
        try:
            r = requests.post(f"{self.API_URL}/register", json=payload, timeout=35)
            data = r.json()
            self.message = data.get("message", "")
            return bool(data.get("success"))
        except Exception as e:
            self.message = f"Connection error: {str(e)}"
            return False

if __name__ == "__main__":
    auth = ShadowXAuth("YOUR_OWNERID", "YOUR_SECRET", "1.0")
    if auth.login("testuser", "testpass"):
        print(f"Welcome {auth.username}! Sub: {auth.subscription}")
    else:
        print(f"Error: {auth.message}")
