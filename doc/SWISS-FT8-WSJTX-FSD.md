# WSJT-SWISS - Functional Specification Document

## Feature Summary

| Feature | Description |
|---------|-------------|
| Swiss FT8 Contest Mode | Exchange Swiss canton codes using ARRL Field Day message format (i3=0, n3=3) |
| 26 Canton Support | All 26 Swiss cantons replace ARRL Field Day sections (isec 1-26) |
| Cabrillo Log Export | "1A" class replaced with "599" signal report |

---

## 1. Swiss FT8 Contest Mode

### 1.1 Purpose
WSJT-SWISS is a fork of WSJT-X designed for the Swiss XMAS FT8 Contest. It enables amateur radio operators to exchange Swiss canton codes as part of the FT8 digital mode protocol.

### 1.2 Mode Selection
- Location: Settings → Advanced → Special Operating Activity
- Radio button: "Swiss XMAS" (repurposed ARRL Field Day)
- Canton dropdown: All 26 cantons in alphabetical order
- Default selection: BL (Basel-Landschaft)

### 1.3 FT8 Protocol
Swiss contest messages use the **ARRL Field Day message format (i3=0, n3=3)** with Swiss cantons replacing the ARRL sections.

Message format examples:
```
HB9BLA HB9XYZ 1A ZH    (TX2: initial exchange)
HB9XYZ HB9BLA R 1A BE  (TX3: roger + exchange)
```

### 1.4 Bit Structure (77 bits total)
Uses ARRL Field Day encoding:

| Bits | Size | Content |
|------|------|---------|
| 0-27 | 28 bits | Callsign 1 |
| 28-55 | 28 bits | Callsign 2 |
| 56 | 1 bit | Roger flag (ir) |
| 57-60 | 4 bits | TX count (intx) - displays as "1" |
| 61-63 | 3 bits | Class (nclass) - displays as "A" |
| 64-70 | 7 bits | Section (isec) - 1-26 for Swiss cantons |
| 71-73 | 3 bits | n3 field (value: 3) |
| 74-76 | 3 bits | i3 field (value: 0) |

### 1.5 QSO Message Sequence

A typical Swiss XMAS FT8 Contest QSO:

| Step | HB9BLA (Bern) | HB9BKT (Zürich) | TX |
|------|---------------|-----------------|-----|
| 0 | `CQ XMAS HB9BLA JN47` | | TX6 |
| 1 | | *Double-clicks CQ* | - |
| 2 | | `HB9BLA HB9BKT 1A ZH` | TX2 |
| 3 | *Receives exchange* | | - |
| 4 | `HB9BKT HB9BLA R 1A BE` | | TX3 |
| 5 | | *Receives R + exchange* | - |
| 6 | | `HB9BLA HB9BKT RR73` | TX4 |
| 7 | *Receives RR73* | | - |
| 8 | `HB9BKT HB9BLA 73` | | TX5 |

---

## 2. Canton Support

### 2.1 Canton Codes
The 26 Swiss cantons replace the ARRL Field Day sections (isec 1-26):

| isec | Canton | isec | Canton | isec | Canton |
|------|--------|------|--------|------|--------|
| 1 | AG (Aargau) | 10 | GR (Graubünden) | 19 | SZ (Schwyz) |
| 2 | AI (Appenzell I.Rh.) | 11 | JU (Jura) | 20 | TG (Thurgau) |
| 3 | AR (Appenzell A.Rh.) | 12 | LU (Luzern) | 21 | TI (Ticino) |
| 4 | BE (Bern) | 13 | NE (Neuchâtel) | 22 | UR (Uri) |
| 5 | BL (Basel-Landschaft) | 14 | NW (Nidwalden) | 23 | VD (Vaud) |
| 6 | BS (Basel-Stadt) | 15 | OW (Obwalden) | 24 | VS (Valais) |
| 7 | FR (Fribourg) | 16 | SG (St. Gallen) | 25 | ZG (Zug) |
| 8 | GE (Genève) | 17 | SH (Schaffhausen) | 26 | ZH (Zürich) |
| 9 | GL (Glarus) | 18 | SO (Solothurn) | | |

### 2.2 Default Canton
Default: **BL** (Basel-Landschaft)

---

## 3. Logging

### 3.1 Cabrillo Log Export
For Cabrillo export, the "1A" class is replaced with "599" signal report:

| Field | Format | Example |
|-------|--------|---------|
| Sent Exchange | `599 <canton>` | `599 BL` |
| Received Exchange | `599 <canton>` | `599 GR` |

**Code locations:**
- `widgets/mainwindow.cpp:5377-5383` - Sent exchange (m_xSent)
- `widgets/mainwindow.cpp:6214-6217` - Received exchange (m_xRcvd)

---

## 4. User Interface

### 4.1 Application Identity
- Window title: "WSJT-SWISS"
- Application name in settings: "WSJT-SWISS"

### 4.2 Settings Dialog
The "ARRL Field Day" option is renamed to "Swiss XMAS" with canton dropdown.

---

## 5. Source Code Changes

### 5.1 Modified Files

| File | Changes |
|------|---------|
| `main.cpp` | Application name set to "WSJT-SWISS" |
| `Configuration.ui` | "ARRL Field Day" renamed to "Swiss XMAS", canton dropdown added |
| `widgets/mainwindow.cpp` | Cabrillo export: "1A" replaced with "599" |
| `lib/77bit/packjt77.f90` | Swiss cantons replace ARRL sections (NSEC=26) |

### 5.2 Fortran Changes (packjt77.f90)

**Key changes:**
- `NSEC` parameter changed from 86 to 26
- `csec` array contains 26 Swiss cantons (AG-ZH) instead of ARRL sections
- Decoding displays canton code in place of ARRL section

---

## 6. Build Process

### 6.1 Dependencies
| Dependency | Version | Source |
|------------|---------|--------|
| Hamlib | 4.6.5 | https://github.com/SensorsIot/hamlib-prebuilds |
| OmniRig | Latest | Local `deps/OmniRig.zip` and `deps/RigIni.zip` |
| Qt | 5.x | MSYS2 packages |
| FFTW3 | 3.x | MSYS2 packages |
| MinGW64 | Latest | MSYS2 packages |
| NSIS | Latest | MSYS2 packages |

### 6.2 Build Environment
- Platform: Windows (MSYS2/MinGW64)
- CMake generator: MinGW Makefiles
- Build type: Release

### 6.3 GitHub Actions Workflow
- File: `.github/workflows/build-windows.yml`
- Triggers: Push to tags `v*`, manual dispatch
- Artifact: `wsjtx-swiss-installer` (NSIS installer .exe)

---

## 7. Installer Features

### 7.1 Settings Import
On first install, settings are automatically imported from WSJT-X:

| Setting | Value |
|---------|-------|
| Source | `%LOCALAPPDATA%\WSJT-X\WSJT-X.ini` |
| Destination | `%LOCALAPPDATA%\WSJT-SWISS\WSJT-SWISS.ini` |
| Condition | Only if destination does not exist |

### 7.2 Uninstall Cleanup
The uninstaller removes old files:
- `C:\WSJTX-SWISS\bin\libhamlib-4_old.dll`

### 7.3 Install Directory
- **Default**: `C:\WSJTX-SWISS`
- **Configuration file**: `CMakeCPackOptions.cmake.in`

---

## 8. Compatibility

| Scenario | Compatibility |
|----------|---------------|
| WSJT-SWISS ↔ WSJT-SWISS | Full Swiss contest support with canton exchange |
| WSJT-SWISS ↔ WSJT-X | Messages decoded but cantons show as unknown sections |
| Standard FT8/FT4/etc. | Fully compatible with all WSJT-X versions |

**Note:** Standard WSJT-X cannot decode Swiss canton codes since they replaced the ARRL sections.
