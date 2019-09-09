while($true)
{
    # get the token from input
    $session_id = Read-Host -Prompt 'Input token'
Get-Date -Format HH:mm:ss.fff

    try {
        # First we create the request.
        $HTTP_Request = [System.Net.WebRequest]::Create('https://access.please-open.it/access/')
        $HTTP_Request.Headers.Add("Authorization", $session_id)
        $HTTP_Request.Headers.Add("ControllerId", <doorId>)
        $HTTP_Status = 401
        # We then get a response from the site.
        $HTTP_Response = $HTTP_Request.GetResponse()

        # We then get the HTTP code as an integer.
        $HTTP_Status = [int]$HTTP_Response.StatusCode

        If ($HTTP_Status -eq 200) {
Get-Date -Format HH:mm:ss.fff

            $PlayWav=New-Object System.Media.SoundPlayer
            $PlayWav.SoundLocation="$pwd\success.wav"
            $PlayWav.play()
            Write-Host "Access Granted !"

            $port= new-Object System.IO.Ports.SerialPort COM15,9600,None,8,one
            $port.open()
            #$port.Write('on', 0, 1)
            #Start-Sleep -Milliseconds 1500
            #$port.Write('off', 0, 1)
            $port.Close()
        }
    } Catch {
Get-Date -Format HH:mm:ss.fff

        $PlayWav=New-Object System.Media.SoundPlayer
        $PlayWav.SoundLocation="$pwd\error.wav"
        $PlayWav.playsync()
        Write-Host "unauthorized"
    }

    # Finally, we clean up the http request by closing it.
    $HTTP_Response.Close()
}
