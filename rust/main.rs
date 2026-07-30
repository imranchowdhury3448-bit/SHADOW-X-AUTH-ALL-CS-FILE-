// ========================================================================
// SHADOW X AUTH - RUST WORKING SAMPLE PROJECT
// ========================================================================

use std::io::{self, Write};

fn main() {
    println!("====================================================");
    println!("        SHADOW X AUTH - RUST SAMPLE PROJECT         ");
    println!("====================================================\n");

    let auth = ShadowXAuth::new("MY_APP", "25D2EFE1BE", "39f8e1a629524947a1767597cd3159967742aed0116a49529e508bba70ad081c", "1.0");

    print!("Enter License Key: ");
    io::stdout().flush().unwrap();

    let mut key = String::new();
    io::stdin().read_line(&mut key).unwrap();

    let res = auth.validate_key(key.trim()).unwrap();
    if res.success {
        println!("\n[+] Access Granted! Welcome {}", res.username);
    } else {
        println!("\n[-] Access Denied: {}", res.message);
    }
}
