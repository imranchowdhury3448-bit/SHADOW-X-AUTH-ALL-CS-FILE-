using System;
using System.Net.Http;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json.Linq;

namespace ShadowXAuth_SDK
{
    /// <summary>
    /// SHADOW X AUTH C# SDK v2.0
    /// 100% Native AOT Compatible (.NET 6, .NET 7, .NET 8, .NET 9)
    /// Supports: WinForms, WPF, Console, Native AOT Compilation.
    /// GitHub: https://github.com/imranchowdhury3448-bit/SHADOW-X-AUTH-ALL-CS-FILE-
    /// Discord: https://discord.gg/WwHrs5RVS
    /// </summary>
    public class ShadowXAuth
    {
        private static readonly HttpClient _http = new HttpClient { Timeout = TimeSpan.FromSeconds(35) };

        public static string ServerUrl = "https://shadowxauth.onrender.com/api/auth";

        private readonly string _name;
        private readonly string _ownerid;
        private readonly string _secret;
        private readonly string _version;
        private readonly string _hwid;

        public string Token        { get; private set; } = "";
        public string Username     { get; private set; } = "";
        public string Expiry       { get; private set; } = "";
        public string Subscription { get; private set; } = "";
        public bool   IsLoggedIn   { get; private set; } = false;
        public string LastMessage  { get; private set; } = "";

        public ShadowXAuth(string name, string ownerid, string secret, string version = "1.0")
        {
            _name    = name;
            _ownerid = ownerid;
            _secret  = secret;
            _version = version;
            _hwid    = GetHWID();
        }

        // ── LOGIN ─────────────────────────────────────────────────────────────
        public async Task<bool> LoginAsync(string username, string password, string? customHwid = null)
        {
            try
            {
                var payload = new JObject
                {
                    ["ownerid"]  = _ownerid,
                    ["secret"]   = _secret,
                    ["version"]  = _version,
                    ["username"] = username,
                    ["password"] = password,
                    ["hwid"]     = customHwid ?? _hwid
                };

                var content = new StringContent(payload.ToString(), Encoding.UTF8, "application/json");
                var resp    = await _http.PostAsync($"{ServerUrl}/login", content);
                var body    = await resp.Content.ReadAsStringAsync();
                var result  = JObject.Parse(body);

                if (result["success"]?.Value<bool>() == true)
                {
                    Token        = result["token"]?.Value<string>() ?? "";
                    Username     = result["user"]?["username"]?.Value<string>() ?? username;
                    Expiry       = result["user"]?["expiry"]?.Value<string>() ?? "";
                    Subscription = result["user"]?["subscription"]?.Value<string>() ?? "default";
                    IsLoggedIn   = true;
                    LastMessage  = result["message"]?.Value<string>() ?? "Login successful";
                    return true;
                }

                LastMessage = result["message"]?.Value<string>() ?? "Authentication failed";
                return false;
            }
            catch (TaskCanceledException)
            {
                LastMessage = "Server connection timed out (cold start in progress, please retry)";
                return false;
            }
            catch (Exception ex)
            {
                LastMessage = $"Connection error: {ex.Message}";
                return false;
            }
        }

        // ── REGISTER ──────────────────────────────────────────────────────────
        public async Task<bool> RegisterAsync(string username, string password, string email = "", string licenseKey = "")
        {
            try
            {
                var payload = new JObject
                {
                    ["ownerid"]     = _ownerid,
                    ["secret"]      = _secret,
                    ["version"]     = _version,
                    ["username"]    = username,
                    ["password"]    = password,
                    ["email"]       = email,
                    ["license_key"] = licenseKey,
                    ["hwid"]        = _hwid
                };

                var content = new StringContent(payload.ToString(), Encoding.UTF8, "application/json");
                var resp    = await _http.PostAsync($"{ServerUrl}/register", content);
                var body    = await resp.Content.ReadAsStringAsync();
                var result  = JObject.Parse(body);

                LastMessage = result["message"]?.Value<string>() ?? "";
                return result["success"]?.Value<bool>() == true;
            }
            catch (Exception ex)
            {
                LastMessage = $"Connection error: {ex.Message}";
                return false;
            }
        }

        // ── VERIFY SESSION ───────────────────────────────────────────────────
        public async Task<bool> VerifySessionAsync()
        {
            if (string.IsNullOrEmpty(Token)) return false;
            try
            {
                var payload = new JObject { ["token"] = Token };
                var content = new StringContent(payload.ToString(), Encoding.UTF8, "application/json");
                var resp    = await _http.PostAsync($"{ServerUrl}/verify", content);
                var body    = await resp.Content.ReadAsStringAsync();
                var result  = JObject.Parse(body);
                return result["success"]?.Value<bool>() == true;
            }
            catch { return false; }
        }

        // ── HWID GENERATOR ───────────────────────────────────────────────────
        public static string GetHWID()
        {
            try
            {
                string disk = "", cpu = "";
                try
                {
                    using var searcher = new System.Management.ManagementObjectSearcher("SELECT SerialNumber FROM Win32_DiskDrive");
                    foreach (var obj in searcher.Get())
                    {
                        var s = obj["SerialNumber"]?.ToString()?.Trim();
                        if (!string.IsNullOrEmpty(s)) { disk = s; break; }
                    }
                }
                catch { }

                try
                {
                    using var searcher = new System.Management.ManagementObjectSearcher("SELECT ProcessorId FROM Win32_Processor");
                    foreach (var obj in searcher.Get())
                    {
                        var s = obj["ProcessorId"]?.ToString()?.Trim();
                        if (!string.IsNullOrEmpty(s)) { cpu = s; break; }
                    }
                }
                catch { }

                string raw = !string.IsNullOrEmpty(disk + cpu) ? (disk + cpu) : Environment.MachineName + "-" + Environment.UserName;
                var hash = SHA256.HashData(Encoding.UTF8.GetBytes(raw));
                return BitConverter.ToString(hash).Replace("-", "")[..32];
            }
            catch
            {
                var hash = SHA256.HashData(Encoding.UTF8.GetBytes(Environment.MachineName));
                return BitConverter.ToString(hash).Replace("-", "")[..32];
            }
        }
    }
}
