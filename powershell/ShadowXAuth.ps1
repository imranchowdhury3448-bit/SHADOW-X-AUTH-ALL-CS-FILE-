# SHADOW X AUTH - PowerShell Integration Module
# GitHub: https://github.com/imranchowdhury3448-bit/SHADOW-X-AUTH-ALL-CS-FILE-
# Discord: https://discord.gg/WwHrs5RVS

function Invoke-ShadowXAuthLogin {
    param(
        [Parameter(Mandatory=$true)][string]$OwnerId,
        [Parameter(Mandatory=$true)][string]$Secret,
        [Parameter(Mandatory=$true)][string]$Username,
        [Parameter(Mandatory=$true)][string]$Password,
        [string]$Version = "1.0",
        [string]$ApiUrl = "https://shadowxauth.onrender.com/api/auth"
    )

    $hwid = "$env:COMPUTERNAME-$env:USERNAME"
    $payload = @{
        ownerid  = $OwnerId
        secret   = $Secret
        username = $Username
        password = $Password
        hwid     = $hwid
        version  = $Version
    } | ConvertTo-Json

    try {
        $resp = Invoke-WebRequest -Uri "$ApiUrl/login" -Method POST -Body $payload -ContentType "application/json" -UseBasicParsing
        $data = $resp.Content | ConvertFrom-Json
        if ($data.success) {
            Write-Host "✅ Login Successful! Welcome $($data.user.username)" -ForegroundColor Green
            return $data
        } else {
            Write-Host "❌ Login Failed: $($data.message)" -ForegroundColor Red
            return $null
        }
    } catch {
        Write-Host "❌ Connection Error: $_" -ForegroundColor Red
        return $null
    }
}

# Example usage:
# Invoke-ShadowXAuthLogin -OwnerId "YOUR_OWNERID" -Secret "YOUR_SECRET" -Username "testuser" -Password "testpass"
