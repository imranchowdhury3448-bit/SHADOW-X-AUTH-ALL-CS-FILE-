; SHADOW X AUTH - AutoHotkey (AHK) Integration
; GitHub: https://github.com/imranchowdhury3448-bit/SHADOW-X-AUTH-ALL-CS-FILE-
; Discord: https://discord.gg/WwHrs5RVS

ShadowXAuthLogin(ownerid, secret, username, password) {
    url := "https://shadowxauth.onrender.com/api/auth/login"
    hwid := A_ComputerName . "-" . A_UserName
    
    json := "{""ownerid"":""" . ownerid . """,""secret"":""" . secret . """,""username"":""" . username . """,""password"":""" . password . """,""hwid"":""" . hwid . """,""version"":""1.0""}"

    http := ComObjCreate("WinHttp.WinHttpRequest.5.1")
    http.Open("POST", url, false)
    http.SetRequestHeader("Content-Type", "application/json")
    http.Send(json)

    response := http.ResponseText
    If InStr(response, """success"":true") {
        MsgBox, 64, Shadow X Auth, Login Successful!
        return true
    } Else {
        MsgBox, 16, Shadow X Auth, Login Failed!
        return false
    }
}

; Usage Example:
; ShadowXAuthLogin("YOUR_OWNERID", "YOUR_SECRET", "testuser", "testpass")
