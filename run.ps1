# to run: .\run.ps1

$ErrorActionPreference = "Stop"

$processName = "missile_defense_backend"

$existingProcess = Get-Process $processName -ErrorAction SilentlyContinue

if ($existingProcess) {
    Write-Host "Stopping existing backend process..."
    Stop-Process -Name $processName -Force
}

Write-Host "Configuring project..."
cmake -S . -B .\backend\build

Write-Host "Building and running project..."
cmake --build .\backend\build --config Debug --target run