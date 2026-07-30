; ========================================================================
; SHADOW X AUTH - AUTOHOTKEY WORKING SAMPLE PROJECT
; ========================================================================

#Include ShadowXAuth.ahk

; Step 1: Initialize Shadow X Auth SDK
auth := new ShadowXAuth("MY_APP", "25D2EFE1BE", "39f8e1a629524947a1767597cd3159967742aed0116a49529e508bba70ad081c", "1.0")

; Step 2: Prompt user for license key
InputBox, licenseKey, Shadow X Auth, Please enter your license key:

if (ErrorLevel) {
    MsgBox, Cancellation requested. Exiting application.
    ExitApp
}

; Step 3: Validate license key
res := auth.ValidateKey(licenseKey)

if (res.success) {
    MsgBox, 64, Shadow X Auth Success, Welcome % res.username `nExpiry: % res.expiry
} else {
    MsgBox, 16, Shadow X Auth Error, Access Denied: % res.message
    ExitApp
}
