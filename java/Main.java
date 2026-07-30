// ========================================================================
// SHADOW X AUTH - JAVA WORKING SAMPLE PROJECT
// ========================================================================

import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        System.out.println("====================================================");
        System.out.println("        SHADOW X AUTH - JAVA SAMPLE PROJECT         ");
        System.out.println("====================================================\n");

        ShadowXAuth auth = new ShadowXAuth("MY_APP", "25D2EFE1BE", "39f8e1a629524947a1767597cd3159967742aed0116a49529e508bba70ad081c", "1.0");

        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter License Key: ");
        String key = scanner.nextLine();

        AuthResponse res = auth.validateLicense(key);

        if (res.isSuccess()) {
            System.out.println("\n[+] Access Granted!");
            System.out.println("[+] Welcome, " + res.getUsername());
        } else {
            System.out.println("\n[-] Access Denied: " + res.getMessage());
        }
    }
}
