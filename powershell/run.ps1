# ========================================================================
# SHADOW X AUTH - POWERSHELL WORKING SAMPLE PROJECT
# ========================================================================

Import-Module .\ShadowXAuth.ps1

Write-Host "====================================================" -ForegroundColor Cyan
Write-Host "     SHADOW X AUTH - POWERSHELL SAMPLE PROJECT      " -ForegroundColor Cyan
Write-Host "====================================================`n" -ForegroundColor Cyan

$auth = New-ShadowXAuth `
    -AppName "MY_APP" `
    -OwnerId "25D2EFE1BE" `
    -Secret  "39f8e1a629524947a1767597cd3159967742aed0116a49529e508bba70ad081c" `
    -Version "1.0"

$key = Read-Host "Enter License Key"

$res = $auth.ValidateKey($key)

if ($res.Success) {
    Write-Host "`n[+] Access Granted! Welcome $($res.Username)" -ForegroundColor Green
} else {
    Write-Host "`n[-] Access Denied: $($res.Message)" -ForegroundColor Red
}
