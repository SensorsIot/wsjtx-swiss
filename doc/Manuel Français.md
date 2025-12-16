# WSJT-SWISS

[![GitHub Actions](https://img.shields.io/badge/Built%20with-GitHub%20Actions-2088FF?logo=github-actions&logoColor=white)](https://github.com/SensorsIot/wsjtx/actions) [![Licence: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) [![Plateforme](https://img.shields.io/badge/Platform-Windows-0078D6?logo=windows)](https://github.com/SensorsIot/wsjtx/releases) [![WSJT-X](https://img.shields.io/badge/Based%20on-WSJT--X%202.7.0-green)](https://wsjt.sourceforge.io/wsjtx.html)

---

## ***Ce logiciel est installé indépendamment de votre logiciel WSJT-X. Il peut être supprimé en effaçant simplement le répertoire C:\WSJTX-SWISS. Il devrait importer vos paramètres WSJT-X standard***

## */P ou /M etc. ainsi que les indicatifs à 4 caractères comme HB9HSLU ne fonctionnent pas*

## 1. Objectif de WSJT-SWISS

WSJT-SWISS est un fork de [WSJT-X](https://wsjt.sourceforge.io/wsjtx.html) développé pour le **Swiss XMAS FT8 Contest**. Il permet aux radioamateurs d'échanger des codes de cantons suisses dans le cadre du protocole de mode numérique FT8.

Le règlement du contest se trouve ici : https://uska.ch/contest/schweizer-contest-kw/

### Protocole d'échange de cantons

WSJT-SWISS est basé sur le protocole ARRL Field Day pour encoder les informations de canton dans la structure de message de 77 bits. Lors d'un QSO de contest, les stations échangent leurs codes de canton à deux lettres (par ex., **ZH** pour Zurich, **BE** pour Berne).

**Exemple de déroulement d'un QSO :**

```
Station A (ZH)              Station B (BE)
─────────────────────────────────────────────
CQ ZH HB9AAA JN47
                            HB9AAA HB9BBB -07
HB9BBB HB9AAA -06
                            HB9AAA HB9BBB 1A BE    ← Échange de canton
HB9BBB HB9AAA 1A ZH         ← Échange de canton
                            HB9AAA HB9BBB RR73
```

Le 1A avant le canton peut être ignoré.

### Journalisation ADIF

Les QSOs du contest suisse sont automatiquement enregistrés avec des champs ADIF supplémentaires :

| Champ | Description | Exemple |
|-------|-------------|---------|
| `MY_CANTON` | Code de son propre canton | `ZH` |
| `HIS_CANTON` | Code du canton de la station correspondante | `BE` |

Ces champs peuvent être utilisés par les logiciels de journalisation de contest pour l'évaluation et la vérification.

---

## 2. Téléchargements

La dernière version se trouve sur la [page des releases](https://github.com/SensorsIot/wsjtx/releases).

| Paquet | Description |
|--------|-------------|
| `wsjtx-swiss-installer` | Installateur Windows (ZIP) |

### Problèmes de téléchargement

Votre navigateur ou antivirus peut bloquer le téléchargement car le fichier n'est pas fréquemment téléchargé. C'est un **faux positif** – le logiciel est sûr.

**Chrome :** Cliquez sur « Conserver » ou sélectionnez « Conserver le fichier dangereux » dans « Téléchargements ».
**Edge :** « Conserver » → « Plus d'informations » → « Conserver quand même ».
**Antivirus :** Ajoutez une exception ou désactivez temporairement la protection en temps réel pendant le téléchargement.

---

## 3. Installation

1. Extraire `wsjtx-swiss-installer.zip`
2. Exécuter `wsjtx-swiss-installer.exe`

### Importation automatique des paramètres

Lors de la première installation, WSJT-SWISS importe automatiquement vos paramètres de WSJT-X si :

- aucune configuration WSJT-SWISS existante n'est trouvée et
- une configuration WSJT-X existe à `%LOCALAPPDATA%\WSJT-X\WSJT-X.ini`

Cela importe l'indicatif, le locator, les paramètres audio et la configuration du rig. Les paramètres WSJT-SWISS existants ne sont **jamais écrasés**.

### Avertissement Microsoft SmartScreen

Windows peut afficher un avertissement SmartScreen car l'application n'est pas signée avec un certificat de signature de code commercial.

**Comment procéder :**
1. Cliquez sur **« Informations complémentaires »**
2. Sélectionnez **« Exécuter quand même »**

C'est courant pour les logiciels open source en dehors du Microsoft Store.

### Avertissements antivirus

Certains programmes antivirus peuvent signaler l'installateur comme suspect. C'est généralement un faux positif. Vous pouvez :

- ajouter une exception pour l'installateur
- désactiver temporairement la protection en temps réel pendant l'installation
- vérifier le téléchargement par rapport aux notes de version en utilisant le hash du fichier

---

## 4. Démarrage de WSJT-SWISS

Après l'installation, vous pouvez démarrer WSJT-SWISS de plusieurs façons :

- **Menu Démarrer :** Entrée **WSJT-X**
- **Bureau :** Raccourci bureau (si créé lors de l'installation)
- **Dossier d'installation :** `C:\WSJTX-SWISS\bin\wsjtx.exe`

---

## 5. Configuration

1. Démarrer **WSJT-SWISS**
2. **File** → **Settings** (ou touche **F2**)
3. Ouvrir l'onglet **Advanced**
4. Sous **Special Operating Activity** → sélectionner **Swiss XMAS**
5. Sélectionner votre canton dans la liste déroulante
6. Enregistrer avec **OK**
7. Entrer `XMAS` dans le champ **Contest name**

![Capture d'écran de configuration](images/setup-screenshot.png)

Les messages TX contiendront désormais automatiquement votre code de canton, et vous appellerez CQ en tant que **CQ XMAS**.

---

## 6. Fonctionnement (comme d'habitude avec FT8)

### Démarrer un QSO

1. Activer le mode **Swiss FT8 Contest** (voir Configuration)
2. Régler la fréquence sur la fréquence du contest (**7,055 MHz**)
3. Cliquer sur **Enable TX** pour appeler CQ, ou double-cliquer sur une station pour répondre
4. L'échange de canton se fait automatiquement pendant le déroulement du QSO

### Flux des messages

| Étape | Action |
|-------|--------|
| 1 | Station A appelle CQ avec canton (par ex., « CQ ZH HB9AAA JN47 ») |
| 2 | Station B répond avec un rapport |
| 3 | Station A envoie un rapport |
| 4 | Station B envoie le code de canton |
| 5 | Station A envoie le code de canton |
| 6 | Station B confirme avec RR73 |

![Capture d'écran de fonctionnement](images/operation-screenshot.png)

### Journalisation

Les QSOs terminés sont automatiquement enregistrés avec :

- Les champs FT8 standard (indicatif, heure, fréquence, mode, rapports)
- Les champs de canton (`MY_CANTON`, `HIS_CANTON`)

Comme d'habitude avec WSJT-X, vous pouvez générer un log Cabrillo et le télécharger sur https://contestlog.uska.ch/submit

![Capture d'écran Cabrillo](images/Cabrillo-Screenshot.png)

Avant le téléchargement, les champs doivent être remplis selon l'exemple suivant :

![Capture d'écran Cabrillo2](images/Cabrillo2-screenshot.png)

Le score est calculé par l'USKA. Il peut donc également être rempli avec 0.

Lors du téléchargement, **sélectionnez Christmas Contest Digital 2 2025** et entrez la catégorie suivante :

![Capture d'écran Cabrillo3](images/Cabrillo3.screenshot.png)

---

## 7. Compatibilité

| Scénario | Compatibilité |
|----------|---------------|
| WSJT-SWISS ↔ WSJT-SWISS | Support complet du contest suisse |
| WSJT-SWISS ↔ WSJT-X | FT8 standard fonctionne ; les messages de canton ne sont pas décodés |
| FT8/FT4/etc. standard | Entièrement compatible avec toutes les versions de WSJT-X |

**Note :** Les messages du contest suisse ne sont correctement décodés que par WSJT-SWISS. Le WSJT-X standard n'affiche pas ces messages.
