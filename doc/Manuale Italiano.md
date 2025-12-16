# WSJT-SWISS

[![GitHub Actions](https://img.shields.io/badge/Built%20with-GitHub%20Actions-2088FF?logo=github-actions&logoColor=white)](https://github.com/SensorsIot/wsjtx/actions) [![Licenza: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) [![Piattaforma](https://img.shields.io/badge/Platform-Windows-0078D6?logo=windows)](https://github.com/SensorsIot/wsjtx/releases) [![WSJT-X](https://img.shields.io/badge/Based%20on-WSJT--X%202.7.0-green)](https://wsjt.sourceforge.io/wsjtx.html)

---

## ***Questo software viene installato indipendentemente dal vostro software WSJT-X. Può essere rimosso semplicemente eliminando la directory C:\WSJTX-SWISS. Dovrebbe importare le impostazioni standard di WSJT-X***

## */P o /M ecc. così come i nominativi a 4 caratteri come HB9HSLU non funzionano*

## 1. Scopo di WSJT-SWISS

WSJT-SWISS è un fork di [WSJT-X](https://wsjt.sourceforge.io/wsjtx.html) sviluppato per il **Swiss XMAS FT8 Contest**. Permette ai radioamatori di scambiare codici dei cantoni svizzeri come parte del protocollo digitale FT8.

Il regolamento del contest si trova qui: https://uska.ch/contest/schweizer-contest-kw/

### Protocollo di scambio cantoni

WSJT-SWISS si basa sul protocollo ARRL Field Day per codificare le informazioni sui cantoni nella struttura del messaggio a 77 bit. Durante un QSO di contest, le stazioni si scambiano i loro codici cantonali a due lettere (ad es., **ZH** per Zurigo, **BE** per Berna).

**Esempio di svolgimento di un QSO:**

```
Stazione A (ZH)             Stazione B (BE)
─────────────────────────────────────────────
CQ ZH HB9AAA JN47
                            HB9AAA HB9BBB -07
HB9BBB HB9AAA -06
                            HB9AAA HB9BBB 1A BE    ← Scambio cantone
HB9BBB HB9AAA 1A ZH         ← Scambio cantone
                            HB9AAA HB9BBB RR73
```

L'1A prima del cantone può essere ignorato.

### Registrazione ADIF

I QSO del contest svizzero vengono salvati automaticamente con campi ADIF aggiuntivi:

| Campo | Descrizione | Esempio |
|-------|-------------|---------|
| `MY_CANTON` | Codice del proprio cantone | `ZH` |
| `HIS_CANTON` | Codice del cantone della stazione corrispondente | `BE` |

Questi campi possono essere utilizzati dai software di log per contest per la valutazione e la verifica.

---

## 2. Download

L'ultima versione si trova nella [pagina delle release](https://github.com/SensorsIot/wsjtx/releases).

| Pacchetto | Descrizione |
|-----------|-------------|
| `wsjtx-swiss-installer` | Installer Windows (ZIP) |

### Problemi di download

Il vostro browser o antivirus potrebbe bloccare il download perché il file non viene scaricato frequentemente. Questo è un **falso positivo** – il software è sicuro.

**Chrome:** Cliccare su "Conserva" o selezionare "Conserva file pericoloso" in "Download".
**Edge:** "Conserva" → "Altre informazioni" → "Conserva comunque".
**Antivirus:** Aggiungere un'eccezione o disabilitare temporaneamente la protezione in tempo reale durante il download.

---

## 3. Installazione

1. Estrarre `wsjtx-swiss-installer.zip`
2. Eseguire `wsjtx-swiss-installer.exe`

### Importazione automatica delle impostazioni

Alla prima installazione, WSJT-SWISS importa automaticamente le impostazioni da WSJT-X se:

- non viene trovata alcuna configurazione WSJT-SWISS esistente e
- esiste una configurazione WSJT-X in `%LOCALAPPDATA%\WSJT-X\WSJT-X.ini`

Questo importa nominativo, locator, impostazioni audio e configurazione del rig. Le impostazioni WSJT-SWISS esistenti non vengono **mai sovrascritte**.

### Avviso Microsoft SmartScreen

Windows potrebbe mostrare un avviso SmartScreen perché l'applicazione non è firmata con un certificato di firma del codice commerciale.

**Come procedere:**
1. Cliccare su **"Ulteriori informazioni"**
2. Selezionare **"Esegui comunque"**

Questo è comune per i software open source al di fuori del Microsoft Store.

### Avvisi antivirus

Alcuni programmi antivirus potrebbero segnalare l'installer come sospetto. Questo è solitamente un falso positivo. Potete:

- aggiungere un'eccezione per l'installer
- disabilitare temporaneamente la protezione in tempo reale durante l'installazione
- verificare il download confrontando l'hash del file con le note di rilascio

---

## 4. Avvio di WSJT-SWISS

Dopo l'installazione, potete avviare WSJT-SWISS in diversi modi:

- **Menu Start:** Voce **WSJT-X**
- **Desktop:** Collegamento sul desktop (se creato durante l'installazione)
- **Cartella di installazione:** `C:\WSJTX-SWISS\bin\wsjtx.exe`

---

## 5. Configurazione

1. Avviare **WSJT-SWISS**
2. **File** → **Settings** (o tasto **F2**)
3. Aprire la scheda **Advanced**
4. Sotto **Special Operating Activity** → selezionare **Swiss XMAS**
5. Selezionare il proprio cantone dalla lista a discesa
6. Salvare con **OK**
7. Inserire `XMAS` nel campo **Contest name**

![Screenshot configurazione](images/setup-screenshot.png)

I messaggi TX conterranno ora automaticamente il vostro codice cantonale, e chiamerete CQ come **CQ XMAS**.

---

## 6. Operatività (come al solito con FT8)

### Avviare un QSO

1. Attivare la modalità **Swiss FT8 Contest** (vedi Configurazione)
2. Impostare la frequenza sulla frequenza del contest (**7,055 MHz**)
3. Cliccare su **Enable TX** per chiamare CQ, o fare doppio clic su una stazione per rispondere
4. Lo scambio dei cantoni avviene automaticamente durante lo svolgimento del QSO

### Flusso dei messaggi

| Passo | Azione |
|-------|--------|
| 1 | Stazione A chiama CQ con cantone (ad es., "CQ ZH HB9AAA JN47") |
| 2 | Stazione B risponde con rapporto |
| 3 | Stazione A invia rapporto |
| 4 | Stazione B invia codice cantone |
| 5 | Stazione A invia codice cantone |
| 6 | Stazione B conferma con RR73 |

![Screenshot operatività](images/operation-screenshot.png)

### Registrazione

I QSO completati vengono salvati automaticamente con:

- Campi FT8 standard (nominativo, ora, frequenza, modo, rapporti)
- Campi cantone (`MY_CANTON`, `HIS_CANTON`)

Come al solito con WSJT-X, potete generare un log Cabrillo e caricarlo su https://contestlog.uska.ch/submit

![Screenshot Cabrillo](images/Cabrillo-Screenshot.png)

Prima del caricamento, i campi devono essere compilati secondo il seguente esempio:

![Screenshot Cabrillo2](images/Cabrillo2-screenshot.png)

Il punteggio viene calcolato dall'USKA. Pertanto, può anche essere compilato con 0.

Al momento del caricamento, **selezionare Christmas Contest Digital 2 2025** e inserire la seguente categoria:

![Screenshot Cabrillo3](images/Cabrillo3.screenshot.png)

---

## 7. Compatibilità

| Scenario | Compatibilità |
|----------|---------------|
| WSJT-SWISS ↔ WSJT-SWISS | Supporto completo del contest svizzero |
| WSJT-SWISS ↔ WSJT-X | FT8 standard funziona; i messaggi dei cantoni non vengono decodificati |
| FT8/FT4/ecc. standard | Pienamente compatibile con tutte le versioni di WSJT-X |

**Nota:** I messaggi del contest svizzero vengono decodificati correttamente solo da WSJT-SWISS. Il WSJT-X standard non visualizza questi messaggi.
