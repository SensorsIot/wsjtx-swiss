# Swiss FT8 Contest Mode - Implementation Plan

## Overview
This plan details the implementation of Swiss FT8 Contest mode for WSJT-X, enabling transmission and reception of Swiss canton information in structured FT8 messages.

## Important Note on Message Type
The FSD specifies "Type 8" for Swiss Canton Code, but FT8's i3 field is only 3 bits (values 0-7). We will use **i3=0, n3=7** (available "tbd" slot) for this implementation.

## Swiss Canton Codes (26 cantons, 0-25)
```
0=AG, 1=AI, 2=AR, 3=BE, 4=BL, 5=BS, 6=FR, 7=GE, 8=GL, 9=GR,
10=JU, 11=LU, 12=NE, 13=NW, 14=OW, 15=SG, 16=SH, 17=SO,
18=SZ, 19=TG, 20=TI, 21=UR, 22=VD, 23=VS, 24=ZG, 25=ZH
```

## File Modifications Required

### 1. FT8 Protocol Layer (Fortran)

#### 1.1 lib/77bit/77bit.txt
**Action:** Document the new message type
**Location:** After line 19 (0.6 entry)
**Add:**
```
0.7   HB9BLA HB9XYZ ZH                   28 28 15              71   Swiss FT8 Contest
```

#### 1.2 lib/77bit/packjt77.f90
**Action:** Add packing function for Swiss contest

**Location 1:** In pack77() subroutine, after line 151 (after Field Day check)
**Add:**
```fortran
! Check 0.7 (Swiss FT8 Contest)
  call pack77_07(nwords,w,i3,n3,c77)
  if(i3.ge.0) go to 900
```

**Location 2:** After pack77_06 subroutine (around line 1100)
**Add new subroutine:**
```fortran
subroutine pack77_07(nwords,w,i3,n3,c77)

! Check 0.7 (Swiss FT8 Contest)
! Example message: HB9BLA HB9XYZ ZH       28 28 15    71

  parameter (NCANTON=26)
  character*13 w(19)
  character*77 c77
  character*6 bcall_1,bcall_2
  character*2 ccanton(NCANTON)
  logical ok1,ok2
  data ccanton/                                                         &
       "AG","AI","AR","BE","BL","BS","FR","GE","GL","GR",  &
       "JU","LU","NE","NW","OW","SG","SH","SO",            &
       "SZ","TG","TI","UR","VD","VS","ZG","ZH"/

  if(nwords.ne.3) return
  call chkcall(w(1),bcall_1,ok1)
  call chkcall(w(2),bcall_2,ok2)
  if(.not.ok1 .or. .not.ok2) return

  icanton=-1
  do i=1,NCANTON
     if(ccanton(i).eq.w(3)(1:2)) then
        icanton=i-1  ! 0-based index
        exit
     endif
  enddo
  if(icanton.eq.-1) return

! 0.7   HB9BLA HB9XYZ ZH            28 28 15    71   Swiss FT8 Contest

  i3=0
  n3=7
  call pack28(w(1),n28a)
  call pack28(w(2),n28b)

  ! Pack canton code in lower 5 bits, reserved 10 bits = 0
  ipayload = icanton  ! Bits 0-4: canton (5 bits), Bits 5-14: reserved (10 bits)

  write(c77,1010) n28a,n28b,ipayload,n3,i3
1010 format(2b28.28,b15.15,2b3.3)

  return
end subroutine pack77_07
```

**Location 3:** In unpack77() subroutine (around line 330, after i3=0,n3=4 check)
**Add:**
```fortran
  else if(i3.eq.0 .and. n3.eq.7) then
! 0.7   HB9BLA HB9XYZ ZH            28 28 15    71   Swiss FT8 Contest
     parameter (NCANTON=26)
     character*2 ccanton(NCANTON)
     data ccanton/                                                         &
          "AG","AI","AR","BE","BL","BS","FR","GE","GL","GR",  &
          "JU","LU","NE","NW","OW","SG","SH","SO",            &
          "SZ","TG","TI","UR","VD","VS","ZG","ZH"/

     read(c77,1070) n28a,n28b,ipayload
1070 format(2b28,b15)
     icanton = iand(ipayload, 31)  ! Extract lower 5 bits
     if(icanton.lt.0 .or. icanton.gt.25) then
        unpk77_success=.false.
        return
     endif
     call unpack28(n28a,c28a,success)
     unpk28_success=unpk28_success .and. success
     call unpack28(n28b,c28b,success)
     unpk28_success=unpk28_success .and. success
     msg=trim(c28a)//' '//trim(c28b)//' '//ccanton(icanton+1)
```

### 2. C++/Qt Configuration Layer

#### 2.1 Configuration.hpp
**Action:** Add Swiss FT8 Contest to SpecialOperatingActivity enum

**Location:** Line 203
**Modify:**
```cpp
enum class SpecialOperatingActivity {NONE, NA_VHF, EU_VHF, FIELD_DAY, RTTY, WW_DIGI, FOX, HOUND, ARRL_DIGI, Q65_PILEUP, SWISS_FT8_CONTEST};
```

**Add:** New accessor methods (around line 205)
```cpp
QString swiss_my_canton () const;
void set_swiss_my_canton (QString const&);
```

#### 2.2 Configuration.cpp
**Action:** Add UI handler and storage for Swiss canton

**Location 1:** Add slot declaration (around line 595)
```cpp
Q_SLOT void on_rbSwiss_FT8_Contest_clicked (bool);
Q_SLOT void on_cbSwiss_My_Canton_currentIndexChanged (QString const&);
```

**Location 2:** Add member variable to store canton (in impl class)
```cpp
QString swiss_my_canton_;
```

**Location 3:** Add implementation for accessors
```cpp
QString Configuration::swiss_my_canton () const
{
  return m_->swiss_my_canton_;
}

void Configuration::set_swiss_my_canton (QString const& canton)
{
  m_->swiss_my_canton_ = canton;
}
```

**Location 4:** Add slot implementation
```cpp
void Configuration::impl::on_rbSwiss_FT8_Contest_clicked (bool checked)
{
  if (checked)
    {
      ui_->special_op_activity_stack->setCurrentIndex(10);  // New page index
    }
}

void Configuration::impl::on_cbSwiss_My_Canton_currentIndexChanged (QString const& canton)
{
  swiss_my_canton_ = canton;
}
```

#### 2.3 Configuration.ui (Qt Designer)
**Action:** Add radio button and canton selector in Advanced tab

**Steps:**
1. Open Configuration.ui in Qt Designer
2. Find "Special Operating Activity" group box
3. Add new radio button: `rbSwiss_FT8_Contest` with text "Swiss FT8 Contest"
4. In special_op_activity_stack, add new page (index 10)
5. Add QComboBox named `cbSwiss_My_Canton` with all 26 canton codes
6. Wire up signals to slots

### 3. Message Generation Layer

#### 3.1 widgets/mainwindow.cpp
**Action:** Integrate Swiss contest into message generation

**Location:** In genStdMsgs() function, around line 6725 (after FIELD_DAY check)
**Add:**
```cpp
if(SpecOp::SWISS_FT8_CONTEST==m_specOp) sent=m_config.swiss_my_canton();
```

### 4. ADIF Logging Layer

#### 4.1 logbook/logbook.cpp
**Action:** Add MY_CANTON and HIS_CANTON ADIF fields

**Location:** In QSOToADIF() function, around line 170 (after RTTY check)
**Add:**
```cpp
else if (Configuration::SpecialOperatingActivity::SWISS_FT8_CONTEST == config_->special_op_id ())
  {
    // Extract canton from exchange
    // xSent contains our canton, xRcvd contains their canton
    t += " <MY_CANTON:" + QString::number (xSent.size ()) + '>' + xSent;
    if (words.size () == 1 && words.at(0).size() == 2)
      {
        t += " <HIS_CANTON:" + QString::number (words.at(0).size ()) + '>' + words.at(0);
      }
  }
```

### 5. Application Branding

#### 5.1 Window Title
**Action:** Change application title from "WSJT-X" to "WSJT-SWISS"

**File:** `widgets/mainwindow.cpp` (or where window title is set)
**Change:** Window title from `WSJT-X v2.7.0 by K1JT et al.` to `WSJT-SWISS v2.7.0 by K1JT et al. and HB9BLA`

**File:** `CMakeLists.txt` or version header (if application name is defined there)
**Change:** Application name string if applicable

### 6. Build System
No CMakeLists.txt changes needed - Fortran files in lib/77bit/ are already included.

## QSO Message Sequence

According to FSD:

1. **CQ:** `CQ ZH HB9BLA JN36` (canton in plain text)
2. **Signal reports:** `HB9BLA HB9XYZ -07` / `HB9XYZ HB9BLA -06`
3. **Canton exchange (structured):**
   - `HB9BLA HB9XYZ ZH` (uses i3=0, n3=7 encoding)
   - `HB9XYZ HB9BLA BE` (uses i3=0, n3=7 encoding)
4. **Final:** `RR73`

## Testing Checklist

- [ ] Encode test: Verify `HB9BLA HB9XYZ ZH` packs correctly
- [ ] Decode test: Verify packed message unpacks to `HB9BLA HB9XYZ ZH`
- [ ] UI test: Select Swiss FT8 Contest mode, choose canton
- [ ] Message gen: Generate messages with canton instead of grid
- [ ] ADIF test: Verify MY_CANTON and HIS_CANTON in log export
- [ ] Window title: Verify title shows "WSJT-SWISS v2.7.0 by K1JT et al. and HB9BLA"
- [ ] End-to-end: Complete QSO between two WSJT-SWISS instances

## Implementation Order

1. **Application branding** (mainwindow.cpp - window title)
2. **Fortran encoding/decoding** (packjt77.f90, 77bit.txt)
3. **C++ configuration** (Configuration.hpp/cpp)
4. **UI integration** (Configuration.ui)
5. **Message generation** (mainwindow.cpp)
6. **ADIF logging** (logbook.cpp)
7. **Build and test**

## Potential Issues and Solutions

### Issue 1: Canton Code Validation
**Problem:** Invalid canton codes could cause crashes
**Solution:** Add bounds checking in both pack77_07 and unpack77

### Issue 2: CQ Message Format
**Problem:** FSD shows "CQ ZH HB9BLA JN36" but standard CQ is "CQ HB9BLA JN36"
**Solution:** Modify genCQMsg() to insert canton after "CQ" when Swiss mode active

### Issue 3: Message Ambiguity
**Problem:** 2-letter canton codes could conflict with other message types
**Solution:** Only parse as Swiss contest when mode is active AND format matches exactly

### Issue 4: Backwards Compatibility
**Problem:** Standard WSJT-X users won't decode Swiss messages
**Solution:** This is acceptable - both stations must use WSJT-SWISS

## Notes

- This implementation uses i3=0, n3=7 (not "Type 8" as in FSD)
- 77-bit structure: 28 (call1) + 28 (call2) + 5 (canton) + 10 (reserved) + 3 (n3) + 3 (i3)
- Reserved 10 bits allow future extensions
- CRC protection ensures reliable canton decode
- Both stations MUST use WSJT-SWISS fork
