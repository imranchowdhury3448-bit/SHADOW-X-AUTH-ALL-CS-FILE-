package com.shadowxauth;

import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.time.Duration;

/**
 * SHADOW X AUTH - Java SDK
 * GitHub: https://github.com/imranchowdhury3448-bit/SHADOW-X-AUTH-ALL-CS-FILE-
 * Discord: https://discord.gg/WwHrs5RVS
 */
public class ShadowXAuth {
    private final String ownerid;
    private final String secret;
    private final String version;
    private final HttpClient client;
    private static final String API_URL = "https://shadowxauth.onrender.com/api/auth";

    public boolean isAuthenticated = false;
    public String username = "";
    public String subscription = "";
    public String expiry = "";
    public String message = "";

    public ShadowXAuth(String ownerid, String secret, String version) {
        this.ownerid = ownerid;
        this.secret = secret;
        this.version = version != null ? version : "1.0";
        this.client = HttpClient.newBuilder().connectTimeout(Duration.ofSeconds(35)).build();
    }

    public boolean login(String user, String pass) {
        try {
            String json = String.format("{\"ownerid\":\"%s\",\"secret\":\"%s\",\"username\":\"%s\",\"password\":\"%s\",\"hwid\":\"JAVA-HWID\",\"version\":\"%s\"}",
                    ownerid, secret, user, pass, version);

            HttpRequest request = HttpRequest.newBuilder()
                    .uri(URI.create(API_URL + "/login"))
                    .header("Content-Type", "application/json")
                    .POST(HttpRequest.BodyPublishers.ofString(json))
                    .build();

            HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());
            String body = response.body();

            if (body.contains("\"success\":true")) {
                this.isAuthenticated = true;
                this.username = user;
                this.message = "Authenticated successfully";
                return true;
            }
            this.message = "Authentication failed";
            return false;
        } catch (Exception e) {
            this.message = "Connection error: " + e.getMessage();
            return false;
        }
    }
}
