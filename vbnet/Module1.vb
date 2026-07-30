' ========================================================================
' SHADOW X AUTH - VB.NET WORKING SAMPLE PROJECT
' ========================================================================

Imports System
Imports ShadowXAuthSDK

Module Module1
    Sub Main()
        Console.WriteLine("====================================================")
        Console.WriteLine("       SHADOW X AUTH - VB.NET SAMPLE PROJECT        ")
        Console.WriteLine("====================================================`n")

        Dim auth As New ShadowXAuth("MY_APP", "25D2EFE1BE", "39f8e1a629524947a1767597cd3159967742aed0116a49529e508bba70ad081c", "1.0")

        Console.Write("Enter License Key: ")
        Dim key As String = Console.ReadLine()

        Dim res = auth.ValidateLicense(key)

        If res.Success Then
            Console.WriteLine(vbCrLf & "[+] Access Granted! Welcome " & res.Username)
        Else
            Console.WriteLine(vbCrLf & "[-] Access Denied: " & res.Message)
        End If
    End Sub
End Module
