Imports System.Net.Http
Imports System.Text
Imports System.Threading.Tasks
Imports Newtonsoft.Json.Linq

' SHADOW X AUTH - VB.NET SDK
' Drop into any VB.NET WinForms or Console project (.NET 6/7/8/9)
Public Class ShadowXAuth
    Private Shared ReadOnly _http As New HttpClient With {.Timeout = TimeSpan.FromSeconds(35)}
    Public Shared ServerUrl As String = "https://shadowxauth.onrender.com/api/auth"

    Private ReadOnly _ownerid As String
    Private ReadOnly _secret As String
    Private ReadOnly _version As String

    Public Property Token As String = ""
    Public Property Username As String = ""
    Public Property Subscription As String = ""
    Public Property Expiry As String = ""
    Public Property IsLoggedIn As Boolean = False
    Public Property LastMessage As String = ""

    Public Sub New(name As String, ownerid As String, secret As String, Optional version As String = "1.0")
        _ownerid = ownerid
        _secret = secret
        _version = version
    End Sub

    Public Async Function LoginAsync(user As String, pass As String) As Task(Of Boolean)
        Try
            Dim payload As New JObject From {
                {"ownerid", _ownerid},
                {"secret", _secret},
                {"version", _version},
                {"username", user},
                {"password", pass},
                {"hwid", Environment.MachineName}
            }

            Dim content As New StringContent(payload.ToString(), Encoding.UTF8, "application/json")
            Dim resp As HttpResponseMessage = Await _http.PostAsync(ServerUrl & "/login", content)
            Dim body As String = Await resp.Content.ReadAsStringAsync()
            Dim result As JObject = JObject.Parse(body)

            If result("success")?.Value(Of Boolean)() = True Then
                Token = If(result("token")?.Value(Of String)(), "")
                Username = If(result("user")?("username")?.Value(Of String)(), user)
                Subscription = If(result("user")?("subscription")?.Value(Of String)(), "default")
                Expiry = If(result("user")?("expiry")?.Value(Of String)(), "")
                IsLoggedIn = True
                LastMessage = If(result("message")?.Value(Of String)(), "Login successful")
                Return True
            End If

            LastMessage = If(result("message")?.Value(Of String)(), "Login failed")
            Return False
        Catch ex As Exception
            LastMessage = "Connection error: " & ex.Message
            Return False
        End Try
    End Function
End Class
