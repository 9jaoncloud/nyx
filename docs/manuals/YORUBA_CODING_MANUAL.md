# Coding in Nyx with Èdè Yorùbá (`--dialect=yoruba`)
## Ìwé İtọ́sọ́nà Kíkọ Kóòdù Kọ̀mpútà ní Èdè Yorùbá (Complete Yorùbá Programming Manual)

**Compiler Version:** `nyxc v0.54.0+` | **Dialect Flag:** `--dialect=yoruba` (or automatic detection) | **Runtime:** Zero-GC Region Memory (`0.00 ns` GC pause)

---

## 1. Ọ̀rọ̀ Àkọ́kọ́ (Introduction)

Nyx kì í ṣe àwọ̀ lásán tàbí ìtumọ̀ lórí èdè Gẹ̀ẹ́sì (not just a skin or translation layer); ó jẹ́ èdè kọ̀mpútà gidi tí a ń ṣàkójọpọ̀ rẹ̀ sí kóòdù ẹ̀rọ (`native x86_64 / ARM64 machine code`), níbi tí àwọn ọ̀rọ̀ ìbílẹ̀ Yorùbá ti ń darí ìṣirò, ìpinnu, àti ìṣàkóso ìrántí Zero-GC.

Láti ṣàkójọpọ̀ àti ṣiṣẹ́ kóòdù Yorùbá lórí ẹ̀rọ rẹ:
```powershell
.\nyx.exe run oruko_faili.nyx --dialect=yoruba
```
*(Tàbí kí o kàn lo `.\nyx.exe run oruko_faili.nyx` nítorí pé ẹ̀rọ Nyx máa ń dá Èdè Yorùbá mọ̀ fúnra rẹ̀!)*

---

## 2. Àtẹ Àwọn Ọ̀rọ̀ Kókó Èdè Yorùbá (Complete Keyword Reference Table)

Nyx gba àmì ohùn àti ojú-ìwé Yorùbá (`ọ`, `ẹ`, `ṣ`) àti kíkọ láìsí àmì (`o`, `e`, `s`) kí ó rọrùn lórí bọ́tìnì kọ̀mpútà (keyboard) kọ̀ọ̀kan:

| Ọ̀rọ̀ Yorùbá ( pẹ̀lú àmì ) | Láìsí Àmì (ASCII-friendly) | Standard Nyx | Ìtumọ̀ àti Iṣẹ́ Rẹ̀ (Meaning & System Role) |
| :--- | :--- | :--- | :--- |
| `wọle` | `wole` | `import` | Mu ìwé-ikawe tabi faili wọlé (Import module / library) |
| `gbangba` | `gbangba` | `pub` | Jẹ́ kí iṣẹ́ tàbí ètò hàn sí gbogbo ènìyàn (Public visibility) |
| `iṣẹ` | `ise` | `fn` | Ìtumọ̀ iṣẹ́ (Function declaration) |
| `akọkọ` | `akoko` | `main` | Ibẹ̀rẹ̀ iṣẹ́ ètò (`fn main()` entry point) |
| `jẹki` | `jeki` | `let` | Ìpamọ́ dátà tí kò ní yípadà (Immutable variable binding) |
| `ayipada` | `ayipada` | `mut` | Dátà tí ó lè yípadà (`let mut` mutable variable) |
| `ti` | `ti` | `if` | Ṣíṣe ìpinnu tí ipò bá jẹ́ òtítọ́ (`if` conditional) |
| `bibẹkọ` | `bibeko` | `else` | Bí kò bá rí bẹ́ẹ̀ (`else` / `else if` branch) |
| `nigbati` | `nigbati` | `while` | Àtúnṣe nígbà tí ipò bá sì jẹ́ òtítọ́ (`while` loop) |
| `fun` | `fun` | `for` | Yíyípo nínú àtòjọ (`for` loop) |
| `pada` | `pada` | `return` | Da èsì padà láti inú iṣẹ́ (`return` statement) |
| `eto` | `eto` | `struct` | Ètò àkójọpọ̀ dátà (Data structure `struct`) |
| `ṣe` | `se` | `impl` | Ṣíṣe imuse àwọn iṣẹ́ lórí ètò (`impl` block) |
| `ara_rẹ` | `ara_re` | `self` | Ìtọ́kasí sí ètò náà fúnra rẹ̀ (`self` reference) |
| `bamu` | `bamu` | `match` | Ìfiwéra onírúurú ipò (Pattern matching `match`) |
| `orisi` | `orisi` | `enum` | Oríṣiríṣi ẹ̀yà (Enumeration `enum`) |
| `iwa` | `iwa` | `trait` | Àdéhùn ìwà àti iṣẹ́ (`trait` interface) |
| `otitọ` | `otito` | `true` | Òtítọ́ (`boolean true`) |
| `irọ` | `iro` | `false` | Irọ́ (`boolean false`) |
| `ofo` / `òfo` | `ofo` | `null` / `void` | Òfo / Kò sí nǹkan (`null` / `void`) |
| `àgbègbè` | `agbegbe` | `region` | Àgbègbè ìrántí Zero-GC (Deterministic Zero-GC memory arena) |
| `lapapọ` | `lapapo` | `async` | Iṣẹ́ àjùmọ̀ṣe (`async` function) |
| `duro` | `duro` | `await` | Dúró de èsì iṣẹ́ (`await` async result) |
| `muna` | `muna` | `strict` | Òfin líle (`strict` mode) |
| `aago_abò` | `aago` | `clock` | Aago àbò ẹ̀rọ (Hardware watchdog timer) |
| `awon_mesh` / `àwọ̀n` | `awon` | `mesh` | Àwọ̀n ìbánisọ̀rọ̀ ẹ̀rọ (`mesh` network primitive) |
| `agbára` | `agbara` | `power` | Ìṣàkóso agbára ẹ̀rọ (Hardware power budget) |
| `ìdánilẹ́jọ́` | `idanilejo` | `proof` | Ẹ̀rí àbò kóòdù (Compile-time formal proof) |
| `io.tẹ(...)` | `io.te(...)` | `println(...)` | Tẹ ọ̀rọ̀ jáde sí ojú ìwòran (Print text line) |
| `io.tẹ_nọmba(...)` | `io.te_nomba(...)` | `println(...)` | Tẹ nọ́mbà jáde sí ojú ìwòran (Print number) |

---

## 3. Ẹ̀kọ́ Kìíní: "Bawo Agbaye!" àti Ìṣirò Owó (Verified LinkedIn Post Examples)

Àwọn àpẹẹrẹ méjèèjì tí a tẹ̀ jáde lórí LinkedIn (*"Breaking Code Barriers: How Nyx Makes Yoruba a First-Class Programming Language"*) ti jẹ́yẹ̀wò, wọ́n sì ń ṣiṣẹ́ 100% lórí `nyx.exe` àti Playground:

### Àpẹẹrẹ 1: Bawo Agbaye (Hello World)
```nyx
wọle std.core.io;

gbangba iṣẹ akọkọ() {
    io.tẹ("Bawo Agbaye! Ẹ kaabọ si Èdè Nyx.");
}
```

**Èsì (`nyx.exe run bawo.nyx --dialect=yoruba`):**
```text
Bawo Agbaye! Ẹ kaabọ si Èdè Nyx.
```

### Àpẹẹrẹ 2: Ìṣirò Owó àti Ẹ̀bùn (Transactions, Mutation & Conditionals)
```nyx
wọle std.core.io;

gbangba iṣẹ akọkọ() {
    jẹki iye_owo = 3500;
    jẹki ayipada opolopo = 2;
    jẹki ayipada apapọ = iye_owo * opolopo;

    // Ṣiṣe Ipinnu (If / Else Conditionals)
    ti apapọ > 5000 {
        io.tẹ("O gba ẹbùn ẹlẹ́rìndínlógún!");
        apapọ = apapọ - 500;
    } bibẹkọ {
        io.tẹ("Ko si ẹbùn fun iye yii.");
    }

    io.tẹ_nọmba(apapọ);
}
```

**Èsì (`nyx.exe run owo_ebun.nyx --dialect=yoruba`):**
```text
O gba ẹbùn ẹlẹ́rìndínlógún!
6500
```

---

## 4. Ẹ̀kọ́ Kejì: Kíkọ Iṣẹ́ àti Ìdánwò Akẹ́kọ̀ọ́ (`iṣẹ`, `pada`, `otitọ` / `irọ`)

Nínú Nyx, o lè kọ iṣẹ́ (`iṣẹ`) tí ó ń gba dátà wọlé tí ó sì ń da èsì padà (`pada`):

```nyx
// Faili: idanwo_yoruba.nyx
wọle std.core.io;

iṣẹ ṣayẹwo_ami(ami: int) -> bool {
    ti ami >= 50 {
        pada otitọ;
    } bibẹkọ {
        pada irọ;
    }
}

gbangba iṣẹ akọkọ() {
    jẹki ami_akẹkọọ = 78;

    ti ṣayẹwo_ami(ami_akẹkọọ) == otitọ {
        io.tẹ("Oriire! O ti kọja idanwo yii.");
    } bibẹkọ {
        io.tẹ("Gbiyanju siwaju sii ni igba miiran.");
    }
}
```

**Èsì:**
```text
Oriire! O ti kọja idanwo yii.
```

---

## 5. Ẹ̀kọ́ Kẹta: Ètò Dátà àti Ìṣe (`eto`, `ṣe`, `ara_rẹ`)

Láti kọ ètò ilé-ìwé, báńkì, tàbí ilé-iṣẹ́, a máa ń lo `eto` (`struct`) àti `ṣe` (`impl`):

```nyx
// Faili: akekoo_yoruba.nyx
wọle std.core.io;

gbangba eto Akẹkọọ {
    orukọ: string,
    owo_osu: int,
    osu: int
}

ṣe Akẹkọọ {
    gbangba iṣẹ iṣiro_apapọ(ara_rẹ) -> int {
        pada ara_rẹ.owo_osu * ara_rẹ.osu;
    }
}

gbangba iṣẹ akọkọ() {
    jẹki olu = Akẹkọọ {
        orukọ: "Tade Oladipo",
        owo_osu: 15000,
        osu: 9
    };

    jẹki gbogbo_owo = olu.iṣiro_apapọ();
    io.tẹ("Orukọ Akẹkọọ: " + olu.orukọ);
    io.tẹ("Apapọ Owo Ọdun: ₦" + gbogbo_owo.to_string());
}
```

**Èsì:**
```text
Orukọ Akẹkọọ: Tade Oladipo
Apapọ Owo Ọdun: ₦135000
```

---

## 6. Ẹ̀kọ́ Kẹrin: Àgbègbè Ìrántí Zero-GC àti Àtúnṣe (`agbegbe` & `nigbati`)

Nyx máa ń lo `agbegbe` (`region`) láti ṣàkóso ìrántí láìsí ìdádúró Garbage Collector (`0.00 ns` GC pause):

```nyx
// Faili: agbegbe_yoruba.nyx
wọle std.core.io;

gbangba iṣẹ akọkọ() {
    // Ṣíṣi àgbègbè ìrántí Zero-GC (Zero-GC Memory Region)
    agbegbe oja_nla {
        jẹki ayipada ka = 1;
        jẹki ayipada apapọ = 0;

        nigbati ka <= 1000 {
            apapọ = apapọ + ka;
            ka = ka + 1;
        }

        io.tẹ("--- Èsì Agbègbè Ìṣirò (Zero-GC) ---");
        io.tẹ("Àpapọ̀ iṣirò lati 1 si 1000: " + apapọ.to_string());
    }
}
```

**Èsì:**
```text
--- Èsì Agbègbè Ìṣirò (Zero-GC) ---
Àpapọ̀ iṣirò lati 1 si 1000: 500500
```

---

## 7. Àkíyèsí Lori Ìlò Èdè Yorùbá (Native Speaker & Orthography Notes)

1. **Àmì Labẹ́ Harọ́fì (`ẹ`, `ọ`, `ṣ`) àti Orúkọ Dátà**:
   - Nínú Nyx `v0.54.0+`, o lè lo àmì labẹ́ harọ́fì (`apapọ`, `akẹkọọ`, `orukọ`, `iṣiro_apapọ`) nínú orúkọ dátà àti iṣẹ́ rẹ láìsí ìṣòro kankan. Compiler Nyx máa ń ṣe ìṣọ̀kan wọn fún ẹ̀rọ C/LLVM nígbà tí ó sì ń pa gbogbo àmì ohùn Yorùbá mọ́ nínú ọ̀rọ̀ `"..."`.
2. **Ìtọ́sọ́nà fún Ìdàgbàsókè Èdè (Community Linguistic Vetting)**:
   - Bó tilẹ̀ jẹ́ pé àwọn ọ̀rọ̀ kókó bíi `wọle` (`import`), `jẹki` (`let`), `ayipada` (`mut`), `ti` / `bibẹkọ` (`if` / `else`), `nigbati` (`while`), `eto` (`struct`), àti `àgbègbè` (`region`) ti ń ṣiṣẹ́ ní kikun, Nyx ń gbà àwọn onímọ̀ èdè Yorùbá láàyè láti dábàá àwọn ọ̀rọ̀ ìbílẹ̀ míràn tí ó bá ọpọlọ Yorùbá mu jùlọ.

