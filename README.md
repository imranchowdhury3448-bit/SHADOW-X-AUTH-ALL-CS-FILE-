# 🔐 SHADOW X AUTH — Multi-Language SDK & Examples

Official SDKs and integration code for **SHADOW X AUTH v2.0** — High Performance Software Authentication Platform.

- **Discord**: [Join Community & Buy Plans](https://discord.gg/WwHrs5RVS)
- **Website**: [shadowxauth.onrender.com](https://shadowxauth.onrender.com/)
- **GitHub**: [SHADOW-X-AUTH-ALL-CS-FILE-](https://github.com/imranchowdhury3448-bit/SHADOW-X-AUTH-ALL-CS-FILE-.git)

---

## 🚀 Supported Languages

| Language | Folder | Features |
|----------|--------|----------|
| **C# (.NET / WinForms / Native AOT)** | `/csharp/` | AOT Compatible, HWID Lock, Json.NET Linq |
| **C++** | `/cpp/` | libcurl + nlohmann/json, HWID support |
| **Python** | `/python/` | Requests, HWID locking, full async/sync support |
| **Java** | `/java/` | HttpClient + org.json |
| **JavaScript / Node.js** | `/nodejs/` | Fetch API / Axios |
| **PHP** | `/php/` | cURL wrapper |
| **Go** | `/go/` | net/http |
| **Rust** | `/rust/` | reqwest + serde |

---

## ⚡ Quick Start Example (C#)

```csharp
var auth = new ShadowXAuth("YOUR_OWNERID", "YOUR_SECRET", "1.0");

if (await auth.LoginAsync("username", "password")) {
    Console.WriteLine($"Logged in! Subscription: {auth.Subscription}");
} else {
    Console.WriteLine($"Login failed: {auth.Message}");
}
```

---

## 🛡️ Features Included
- **Native AOT Compatible**: No reflection trimming bugs on .NET 7/8/9
- **HWID Protection**: Locks user to their physical hardware ID on first login
- **Reset HWID**: Owner / Staff can reset hardware lock anytime from Dashboard
- **Plan Limits Enforced**: Basic (5 apps, unlimited keys), Premium (unlimited apps, unlimited keys)
- **Discord Direct Purchasing**: Integrated with `discord.gg/WwHrs5RVS`
