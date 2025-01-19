
if (-not (Get-Module -ListAvailable -Name ImportExcel)) {
    Install-Module -Name ImportExcel -Force -Scope CurrentUser
}

$inputFile = "adresy.xlsx"
$outputFile = "wyniki.xlsx"

$sheetName = "IP-Addresses"
$addresses = Import-Excel -Path $inputFile -WorksheetName $sheetName | Select-Object -First 5
if (-not $addresses) {
    Write-Host "Nie znaleziono adresów IP w pliku $inputFile w arkuszu $sheetName." -ForegroundColor Red
    exit 1
}


$results = @()

foreach ($address in $addresses) {
    $ip = $address."Column1" # Załóżmy, że adresy są w pierwszej kolumnie

    try {
        $pingResult = Test-Connection -ComputerName $ip -Count 1 -ErrorAction Stop
        $status = "Odpowiedź: " + $pingResult.Status
    } catch {
        $status = "Błąd: $_"
    }

    # Dodanie wynikuw do listy
    $results += [PSCustomObject]@{
        Adres = $ip
        Wynik = $status
    }
}

$results | Export-Excel -Path $outputFile -WorksheetName "Wyniki" -AutoSize

Write-Host "Wyniki zapisano w pliku $outputFile" -ForegroundColor Green
