<?php
/**
 * SHADOW X AUTH - PHP SDK
 * GitHub: https://github.com/imranchowdhury3448-bit/SHADOW-X-AUTH-ALL-CS-FILE-
 * Discord: https://discord.gg/WwHrs5RVS
 */
class ShadowXAuth {
    private $ownerid;
    private $secret;
    private $version;
    private $apiUrl = 'https://shadowxauth.onrender.com/api/auth';

    public $isAuthenticated = false;
    public $message = '';
    public $username = '';

    public function __construct($ownerid, $secret, $version = '1.0') {
        $this->ownerid = $ownerid;
        $this->secret  = $secret;
        $this->version = $version;
    }

    public function login($username, $password) {
        $payload = json_encode([
            'ownerid'  => $this->ownerid,
            'secret'   => $this->secret,
            'username' => $username,
            'password' => $password,
            'hwid'     => gethostname(),
            'version'  => $this->version
        ]);

        $ch = curl_init($this->apiUrl . '/login');
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
        curl_setopt($ch, CURLOPT_POST, true);
        curl_setopt($ch, CURLOPT_POSTFIELDS, $payload);
        curl_setopt($ch, CURLOPT_HTTPHEADER, ['Content-Type: application/json']);
        curl_setopt($ch, CURLOPT_TIMEOUT, 35);

        $response = curl_exec($ch);
        curl_close($ch);

        $data = json_decode($response, true);
        if (isset($data['success']) && $data['success'] === true) {
            $this->isAuthenticated = true;
            $this->username = $data['user']['username'] ?? $username;
            $this->message = $data['message'] ?? 'Authenticated';
            return true;
        }

        $this->message = $data['message'] ?? 'Login failed';
        return false;
    }
}
