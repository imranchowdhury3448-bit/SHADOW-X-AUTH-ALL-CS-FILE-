// ========================================================================
// SHADOW X AUTH - NODE.JS WORKING SAMPLE PROJECT
// ========================================================================

const readline = require('readline');
const ShadowXAuth = require('./shadowxauth');

console.log("====================================================");
console.log("        SHADOW X AUTH - NODE.JS SAMPLE PROJECT      ");
console.log("====================================================\n");

const auth = new ShadowXAuth({
    name:    'MY_APP',
    ownerid: '25D2EFE1BE',
    secret:  '39f8e1a629524947a1767597cd3159967742aed0116a49529e508bba70ad081c',
    version: '1.0'
});

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

rl.question('Enter License Key: ', async (key) => {
    const res = await auth.login('username', 'password', key);
    if (res.success) {
        console.log(`\n[+] Access Granted! Welcome ${res.username}`);
    } else {
        console.log(`\n[-] Access Denied: ${res.message}`);
    }
    rl.close();
});
