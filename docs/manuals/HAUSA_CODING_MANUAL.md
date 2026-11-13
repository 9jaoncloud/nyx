# Coding in Nyx with Harshen Hausa (`--dialect=hausa`)
## Cikakken Jagorar Shirye-shiryen Kwamfuta a Harshen Hausa (Complete Hausa Programming Manual)

**Compiler Version:** `nyxc v0.54.0+` | **Dialect Flag:** `--dialect=hausa` (or automatic detection) | **Runtime:** Zero-GC Region Memory (`0.00 ns` GC pause)

---

## 1. Gabatarwa (Introduction)

Nyx ba kawai fassarar kalmomi ba ne (not merely a skin or macro wrapper); cikakken harshen shirye-shiryen kwamfuta ne (compiled systems programming language) wanda ke ba masu haɓaka manhaja (developers), ɗalibai, da injiniyoyi damar rubuta lambar kwamfuta (source code) kai tsaye cikin **Harshen Hausa** kuma a sauya ta zuwa lambar na'ura (native x86_64 / ARM64 machine code) ba tare da tsaikon shara (Zero-GC pause) ba.

Domin gudanar da kowane fayil na Hausa a Nyx:
```powershell
.\nyx.exe run sunan_shiri.nyx --dialect=hausa
```
*(Ko kuma kai tsaye `.\nyx.exe run sunan_shiri.nyx` domin na'urar tantance harshe ta Nyx tana gane Hausa da kanta!)*

---

## 2. Jadawalin Kalmomin Harshen Hausa (Complete Keyword Reference Table)

| Kalmar Hausa (Hausa Keyword) | Standard Nyx | Ma'ana da Aiki (Meaning & System Role) |
| :--- | :--- | :--- |
| `shigo` | `import` | Shigo da ɗakin karatu ko fayil (Import standard library or module) |
| `fili` | `pub` | Bayyana aiki ko tsari a fili (Public visibility modifier) |
| `aiki` | `fn` | Aiki ko umarni (Function definition) |
| `babban_aiki` / `babban` | `main` | Babban mafarin shiri (Program entry point `fn main()`) |
| `bari` | `let` | Ajiye bayanai marasa canzawa (Immutable variable binding) |
| `sauyawa` | `mut` | Bayanai masu iya canzawa (`let mut` mutable binding) |
| `idan` | `if` | Yanke shawara idan sharadi ya tabbata (Conditional `if` branch) |
| `in ba haka ba` / `in_ba_haka_ba` | `else` | Madadin yanke shawara (`else` / `else if` branch) |
| `yayin da` / `yayin_da` | `while` | Maimaitawa muddin sharadi yana gaskiya (`while` loop) |
| `domin` | `for` | Zagayawa a cikin jeri ko lambobi (`for` loop) |
| `koma` | `return` | Dawo da sakamako daga cikin aiki (`return` value) |
| `tsari` | `struct` | Tsarin bayanai na musamman (Data structure `struct`) |
| `aikata` | `impl` | Aiwatar da ayyuka akan tsari (Implementation block `impl`) |
| `kansa` | `self` | Nuni ga tsarin da ake ciki (`self` instance reference) |
| `dace` | `match` | Kwatanta yanayi daban-daban (Pattern matching `match`) |
| `jeri` | `enum` | Jerin zaɓuɓɓuka (Enumeration type `enum`) |
| `halaye` | `trait` | Ka'idojin aiki (Interface / `trait` contract) |
| `gaskiya` | `true` | Darajar gaskiya (`boolean true`) |
| `karya` | `false` | Darajar ƙarya (`boolean false`) |
| `babu` | `null` / `void` | Babu komai (`null` / empty value) |
| `yanki` | `region` | Yankin ma'ajiyar Zero-GC (Deterministic Zero-GC memory arena) |
| `a_tare` | `async` | Aiki mai gudana a lokaci guda (Asynchronous function) |
| `jira` | `await` | Jiran sakamakon `a_tare` (`await` async result) |
| `tsanani` | `strict` | Tsananta dokar lissafi ko ma'ajiya (`strict` enforcement) |
| `mai_tsaro` / `tsaro` | `clock` | Agogon tsaron hardware (Hardware watchdog clock bound) |
| `ragar_mesh` / `raga` | `mesh` | Sadarwar ragar na'urori (Sovereign Mesh networking primitive) |
| `iko` | `power` | Sarrafa makamashin na'ura (Hardware power budget) |
| `hujja` | `proof` | Tabbatar da tsaron lissafi a lokacin ginawa (Compile-time safety proof) |
| `io.buga(...)` | `println(...)` | Buga rubutu akan allo (Print string line to console) |
| `io.buga_lamba(...)` | `println(...)` | Buga lamba akan allo (Print numeric value to console) |

---

## 3. Darasi na 1: Sannu Duniya da Lissafin Kasuwanci (Hello World & Variables)

A cikin Nyx, ana amfani da `bari` don ƙirƙirar ma'ajiya mara canzawa (immutable), da kuma `bari sauyawa` don ma'ajiya mai canzawa (mutable).

```nyx
// Fayil: sannu_hausa.nyx
shigo std.core.io;

fili aiki babban_aiki() {
    io.buga("Sannu Duniya! Maraba da Harshen Nyx a Harshen Hausa.");

    // Lissafin ciniki a kasuwar Kano
    bari farashi = 2500;
    bari sauyawa yawa = 3;
    bari sauyawa jimilla = farashi * yawa;

    io.buga_lamba(jimilla);
}
```

**Sakamakon Gudanarwa (`nyx.exe run sannu_hausa.nyx --dialect=hausa`):**
```text
Sannu Duniya! Maraba da Harshen Nyx a Harshen Hausa.
7500
```

---

## 4. Darasi na 2: Yanke Shawara da Ayyuka (`idan` / `in ba haka ba` & `aiki`)

Nyx yana tilasta **Dokar Gaskiya ta Tsaye (Strict Boolean Rule)**: dole ne kowane sharadi a cikin `idan` ya bayyana kwatance karara (`>`, `<`, `==`, `!=`). Wannan yana hana kurakuran boye a cikin manhajojin banki da kasuwanci.

```nyx
// Fayil: rangwame_hausa.nyx
shigo std.core.io;

aiki duba_rangwame(kudi: int) -> bool {
    idan kudi >= 20000 {
        koma gaskiya;
    } in ba haka ba {
        koma karya;
    }
}

fili aiki babban_aiki() {
    bari sauyawa sayayya = 25000;

    idan duba_rangwame(sayayya) == gaskiya {
        io.buga("An bada rangwame 10% ga wannan ciniki!");
        sayayya = sayayya - 2500;
    } in ba haka ba {
        io.buga("Ba a kai matsayin rangwame ba.");
    }

    io.buga_lamba(sayayya);
}
```

**Sakamakon Gudanarwa:**
```text
An bada rangwame 10% ga wannan ciniki!
22500
```

---

## 5. Darasi na 3: Tsarin Bayanai da Aiwatarwa (`tsari`, `aikata`, `kansa`)

Kuna iya gina manyan tsarin bayanai na kasuwanci, makaranta, ko ma'aikata ta amfani da `tsari` (`struct`) da `aikata` (`impl`):

```nyx
// Fayil: kaya_hausa.nyx
shigo std.core.io;

fili tsari KayanKasuwa {
    suna: string,
    farashin_buhu: int,
    yawan_buhu: int
}

aikata KayanKasuwa {
    fili aiki lissafa_kudi(kansa) -> int {
        koma kansa.farashin_buhu * kansa.yawan_buhu;
    }
}

fili aiki babban_aiki() {
    bari kaya = KayanKasuwa {
        suna: "Buhun Shinkafa Kano",
        farashin_buhu: 32000,
        yawan_buhu: 3
    };

    bari jimilla = kaya.lissafa_kudi();
    io.buga("Kayan da aka zaba: " + kaya.suna);
    io.buga("Jimillar Kudi: ₦" + jimilla.to_string());
}
```

**Sakamakon Gudanarwa:**
```text
Kayan da aka zaba: Buhun Shinkafa Kano
Jimillar Kudi: ₦96000
```

---

## 6. Darasi na 4: Madauki da Yankin Ma'ajiya na Zero-GC (`yayin da` & `yanki`)

Nyx baya amfani da Garbage Collector mai dakatar da manhaja. Maimakon haka, kalmar `yanki` (`region`) tana ware ma'ajiya mai saurin gaske kuma ta share ta nan take da zarar shirin ya fita daga cikin blok ɗin `}`:

```nyx
// Fayil: yanki_hausa.nyx
shigo std.core.io;

fili aiki babban_aiki() {
    // Yankin ma'ajiya mai sauri ba tare da tsaikon shara (Zero-GC) ba
    yanki gona {
        bari sauyawa lamba = 1;
        bari sauyawa jimilla = 0;

        yayin da lamba <= 1000 {
            jimilla = jimilla + lamba;
            lamba = lamba + 1;
        }

        io.buga("--- Sakamakon Yankin Ma'ajiya (Zero-GC) ---");
        io.buga("Jimillar lissafin 1 zuwa 1000: " + jimilla.to_string());
    }
}
```

**Sakamakon Gudanarwa:**
```text
--- Sakamakon Yankin Ma'ajiya (Zero-GC) ---
Jimillar lissafin 1 zuwa 1000: 500500
```

---

## 7. Qa'idojin Rubutu da Haruffan Hausa (Orthography & Identifier Rules)

1. **Haruffan Hausa Masu Lanƙwasa (`ƙ`, `ɗ`, `ɓ`, `ƴ`)**:
   - A cikin rubutun baki (`"..."`), duk haruffan Hausa na Unicode suna fita 100% yadda aka rubuta su.
   - A cikin sunayen ma'ajiya (identifiers), mai fassarar Nyx (`nyx.exe`) yana karɓar haruffan Hausa tare da daidaita su zuwa lambar C/LLVM ba tare da kuskure ba.
2. **kalmomi masu sarari (`in ba haka ba`, `yayin da`)**:
   - Kuna iya rubuta su da sarari (`in ba haka ba`, `yayin da`) ko da alamar ƙasa (`in_ba_haka_ba`, `yayin_da`). Dukansu suna aiki 100% a cikin `nyx.exe` da kuma akan shafin yanar gizo na Nyx Playground.

