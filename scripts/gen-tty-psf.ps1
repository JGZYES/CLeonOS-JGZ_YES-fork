param(
    [string]$OutputPath = "ramdisk/system/tty.psf"
)

Add-Type -AssemblyName System.Drawing

$width = 8
$height = 16
$glyphCount = 256
$headerSize = 4
$bytesPerGlyph = $height
$totalSize = $headerSize + ($glyphCount * $bytesPerGlyph)

$fontCandidates = @("Consolas", "Lucida Console", "Courier New")
$font = $null
foreach ($name in $fontCandidates) {
    try {
        $font = New-Object System.Drawing.Font($name, 14, [System.Drawing.FontStyle]::Regular, [System.Drawing.GraphicsUnit]::Pixel)
        if ($font.Name -eq $name) {
            break
        }
    } catch {
        $font = $null
    }
}

if ($null -eq $font) {
    throw "No usable monospace font found for PSF generation"
}

$bytes = New-Object byte[] $totalSize
$bytes[0] = 0x36
$bytes[1] = 0x04
$bytes[2] = 0x00
$bytes[3] = [byte]$bytesPerGlyph

# default all glyphs to blank, avoids noisy '?' for undefined chars.
for ($i = $headerSize; $i -lt $totalSize; $i++) {
    $bytes[$i] = 0x00
}

$white = [System.Drawing.Brushes]::White
$black = [System.Drawing.Color]::Black

for ($code = 32; $code -le 126; $code++) {
    $ch = [char]$code

    $bmp = New-Object System.Drawing.Bitmap $width, $height
    $g = [System.Drawing.Graphics]::FromImage($bmp)
    $g.Clear($black)
    $g.TextRenderingHint = [System.Drawing.Text.TextRenderingHint]::SingleBitPerPixelGridFit

    # Small negative offset keeps glyph centered in 8x16 cell for common monospace fonts.
    $g.DrawString([string]$ch, $font, $white, -1, -2)

    $glyphOffset = $headerSize + ($code * $bytesPerGlyph)

    for ($y = 0; $y -lt $height; $y++) {
        [byte]$rowBits = 0

        for ($x = 0; $x -lt $width; $x++) {
            $p = $bmp.GetPixel($x, $y)
            if (($p.R + $p.G + $p.B) -ge 384) {
                $rowBits = [byte]($rowBits -bor (1 -shl (7 - $x)))
            }
        }

        $bytes[$glyphOffset + $y] = $rowBits
    }

    $g.Dispose()
    $bmp.Dispose()
}

$font.Dispose()

$dir = Split-Path -Parent $OutputPath
if (-not [string]::IsNullOrWhiteSpace($dir)) {
    New-Item -ItemType Directory -Force -Path $dir | Out-Null
}

$fullOutput = Join-Path (Resolve-Path '.') $OutputPath
[System.IO.File]::WriteAllBytes($fullOutput, $bytes)
Write-Output "Generated $OutputPath ($($bytes.Length) bytes)"