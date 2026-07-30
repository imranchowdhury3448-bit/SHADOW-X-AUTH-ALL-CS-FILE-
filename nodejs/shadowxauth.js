const axios = require('axios');
const os = require('os');

/**
 * SHADOW X AUTH - Node.js SDK
 * GitHub: https://github.com/imranchowdhury3448-bit/SHADOW-X-AUTH-ALL-CS-FILE-
 * Discord: https://discord.gg/WwHrs5RVS
 */
class ShadowXAuth {
  constructor(ownerid, secret, version = '1.0') {
    this.ownerid = ownerid;
    this.secret = secret;
    this.version = version;
    this.apiUrl = 'https://shadowxauth.onrender.com/api/auth';
    this.isAuthenticated = false;
    this.message = '';
  }

  getHWID() {
    return `${os.hostname()}-${os.userInfo().username}`;
  }

  async login(username, password) {
    try {
      const res = await axios.post(`${this.apiUrl}/login`, {
        ownerid: this.ownerid,
        secret: this.secret,
        username,
        password,
        hwid: this.getHWID(),
        version: this.version
      }, { timeout: 35000 });

      if (res.data && res.data.success) {
        this.isAuthenticated = true;
        this.username = res.data.user.username;
        this.subscription = res.data.user.subscription;
        this.message = res.data.message || 'Authenticated';
        return true;
      }
      this.message = res.data.message || 'Authentication failed';
      return false;
    } catch (err) {
      this.message = err.response?.data?.message || err.message;
      return false;
    }
  }
}

module.exports = ShadowXAuth;
