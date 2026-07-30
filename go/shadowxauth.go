package shadowxauth

import (
	"bytes"
	"encoding/json"
	"fmt"
	"net/http"
	"time"
)

/**
 * SHADOW X AUTH - Go SDK
 * GitHub: https://github.com/imranchowdhury3448-bit/SHADOW-X-AUTH-ALL-CS-FILE-
 * Discord: https://discord.gg/WwHrs5RVS
 */
type ShadowXAuth struct {
	OwnerID string
	Secret  string
	Version string
	APIURL  string
}

func New(ownerID, secret, version string) *ShadowXAuth {
	if version == "" {
		version = "1.0"
	}
	return &ShadowXAuth{
		OwnerID: ownerID,
		Secret:  secret,
		Version: version,
		APIURL:  "https://shadowxauth.onrender.com/api/auth",
	}
}

func (s *ShadowXAuth) Login(username, password string) (bool, string) {
	payload := map[string]string{
		"ownerid":  s.OwnerID,
		"secret":   s.Secret,
		"username": username,
		"password": password,
		"hwid":     "GO-HWID-KEY",
		"version":  s.Version,
	}

	body, _ := json.Marshal(payload)
	client := &http.Client{Timeout: 35 * time.Second}
	resp, err := client.Post(s.APIURL+"/login", "application/json", bytes.NewBuffer(body))
	if err != nil {
		return false, fmt.Sprintf("Connection error: %v", err)
	}
	defer resp.Body.Close()

	var result map[string]interface{}
	json.NewDecoder(resp.Body).Decode(&result)

	if success, ok := result["success"].(bool); ok && success {
		return true, "Authenticated"
	}
	return false, fmt.Sprintf("%v", result["message"])
}
