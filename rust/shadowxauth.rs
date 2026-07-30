// SHADOW X AUTH - Rust SDK Example
// Add dependencies to Cargo.toml:
// [dependencies]
// reqwest = { version = "0.11", features = ["json"] }
// tokio = { version = "1.0", features = ["full"] }
// serde = { version = "1.0", features = ["derive"] }

use serde::{Deserialize, Serialize};

#[derive(Serialize)]
struct LoginPayload<'a> {
    ownerid: &'a str,
    secret: &'a str,
    username: &'a str,
    password: &'a str,
    hwid: &'a str,
    version: &'a str,
}

#[derive(Deserialize, Debug)]
struct AuthResponse {
    success: bool,
    message: Option<String>,
    token: Option<String>,
}

pub struct ShadowXAuth<'a> {
    pub ownerid: &'a str,
    pub secret: &'a str,
    pub version: &'a str,
    pub api_url: String,
}

impl<'a> ShadowXAuth<'a> {
    pub fn new(ownerid: &'a str, secret: &'a str, version: &'a str) -> Self {
        Self {
            ownerid,
            secret,
            version,
            api_url: "https://shadowxauth.onrender.com/api/auth".to_string(),
        }
    }

    pub async fn login(&self, username: &'a str, password: &'a str) -> Result<bool, String> {
        let client = reqwest::Client::new();
        let payload = LoginPayload {
            ownerid: self.ownerid,
            secret: self.secret,
            username,
            password,
            hwid: "RUST-HWID-KEY",
            version: self.version,
        };

        let res = client
            .post(format!("{}/login", self.api_url))
            .json(&payload)
            .send()
            .await
            .map_err(|e| e.to_string())?;

        let auth_res: AuthResponse = res.json().await.map_err(|e| e.to_string())?;
        if auth_res.success {
            Ok(true)
        } else {
            Err(auth_res.message.unwrap_or_else(|| "Auth failed".to_string()))
        }
    }
}

#[tokio::main]
async fn main() {
    let auth = ShadowXAuth::new("YOUR_OWNERID", "YOUR_SECRET", "1.0");
    match auth.login("testuser", "testpass").await {
        Ok(_) => println!("Successfully logged in!"),
        Err(e) => println!("Error: {}", e),
    }
}
