<?php
// ========================================================================
// SHADOW X AUTH - PHP WORKING SAMPLE PROJECT
// ========================================================================

require_once 'shadowxauth.php';

echo "====================================================\n";
echo "        SHADOW X AUTH - PHP SAMPLE PROJECT          \n";
echo "====================================================\n\n";

$auth = new ShadowXAuth(
    'MY_APP',
    '25D2EFE1BE',
    '39f8e1a629524947a1767597cd3159967742aed0116a49529e508bba70ad081c',
    '1.0'
);

$key = readline("Enter License Key: ");

$res = $auth->validateKey($key);

if ($res['success']) {
    echo "\n[+] Access Granted! Welcome " . $res['username'] . "\n";
} else {
    echo "\n[-] Access Denied: " . $res['message'] . "\n";
}
