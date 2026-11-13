# LinkedIn Series: Coding in Harshen Hausa with Nyx
## 4-Part Dedicated Masterclass (Boko Script)

A 4-part ready-to-publish LinkedIn masterclass introducing compiled systems programming in native Hausa using `@nyx/hausa` and Nyx's Zero-GC toolchain.

---

### 📌 Post 1 of 4: Sannu Duniya & Farawa (Syntax Fundamentals)

**Hook & Preview:**
```text
Kuna iya rubuta lambobin kwamfuta a Harshen Hausa kai tsaye? Yanzu wannan ya zama gaskiya da Nyx! 🇳🇬✨ (Kashi na 1/4)

Shin kun san cewa yaren kwamfuta na farko da zai ba ku damar rubuta lambobin tsarin kwamfuta (systems programming) a Harshen Hausa tare da cikakken saurin Zero-GC yana nan?

Da Nyx da fakitin `@nyx/hausa`, ba sai ka fara koyon kalmomin Turanci ba kafin ka gane dabarun kimiya da fasahar kera manhaja.

Ga misalin shirin "Sannu Duniya" da lissafi mai sauƙi:

```nyx
// sannu.nyx
shigo std.core.io;

fili aiki babban_aiki() {
    // Bayyana sabbin lambobi ta amfani da bari da sauyawa
    bari farashi = 2500;
    bari sauyawa yawa = 3;
    bari jimilla = farashi * yawa;

    println("Sannu Duniya! Maraba da Harshen Nyx a Harshen Hausa.");
    io.buga_lamba(jimilla);
}
```

💻 Yadda Zaka Gwada Shi (How to Run):

1. Sanya fakitin Hausa:
   nypm install @nyx/hausa

2. Tattara kuma ka gudanar da shi:
   nyx run sannu.nyx --dialect=hausa

Sakamako a allo (Terminal Output):
[Nyx Dialect] Active dialect layer: Harshen Hausa
Compiled: sannu.exe
Sannu Duniya! Maraba da Harshen Nyx a Harshen Hausa.
7500

Wannan shiri yana aiki ne ta hanyar C-interop kai tsaye ba tare da jinkirin Garbage Collector ba.

Gobe a Kashi na 2: Sharadi da Yanke Shawara ta amfani da `idan` da `in ba haka ba`.

Duba dandalin gwaji: https://nyx.9jaoncloud.com.ng/hausa/index.html

#Hausa #TechInAfrica #NyxLang #SoftwareEngineering #NorthernTech #STEMInAfrica #OpenSource #SystemsProgramming
```

---

### 📌 Post 2 of 4: Sharadi da Yanke Shawara (Conditionals & Logic)

**Hook & Preview:**
```text
Yanke shawara a manhajar kwamfuta ta hanyar amfani da kalmomin Hausa: `idan` da `in ba haka ba`. 🇳🇬⚖️ (Kashi na 2/4)

Duk wata manhajar kasuwanci ko banki tana buƙatar duba sharadi (conditions)—misali: shin mai sayayya ya cancanci rangwame? Ko kuma kudin da ke asusunsa ya isa biyan buƙata?

A cikin Nyx, zaka iya amfani da `idan` (if), `in ba haka ba` (else), da kuma `koma` (return) cikin sauƙi da tsari:

```nyx
// sharadi.nyx
shigo std.core.io;

fili aiki duba_rangwame(kudi: i32) -> bool {
    // Idan kudin sayayya ya kai 10,000, a bada rangwame
    idan kudi >= 10000 {
        println("An bada rangwame 10% ga wannan ciniki!");
        koma gaskiya;
    } in ba haka ba {
        println("Babu rangwame a wannan adadin.");
        koma karya;
    }
}

fili aiki babban_aiki() {
    bari ciniki_1 = 12500;
    duba_rangwame(ciniki_1);
}
```

💻 Yadda Zaka Gwada Shi (How to Run):

nyx run sharadi.nyx --dialect=hausa

Sakamako a allo (Terminal Output):
[Nyx Dialect] Active dialect layer: Harshen Hausa
Compiled: sharadi.exe
An bada rangwame 10% ga wannan ciniki!

Ma'anar kalmomin:
• `idan` $\rightarrow$ `if`
• `in ba haka ba` $\rightarrow$ `else`
• `gaskiya` $\rightarrow$ `true`
• `karya` $\rightarrow$ `false`
• `koma` $\rightarrow$ `return`

Gobe a Kashi na 3: Tsarukan Bayanai (`tsari` da `aikata`) da madaukin lissafi (`yayin da`).

#Hausa #CodingInHausa #NyxLang #TechInAfrica #SoftwareEngineering #Logic #AfricanTech
```

---

### 📌 Post 3 of 4: Madaukin Lissafi da Tsarukan Bayanai (Loops & Structs)

**Hook & Preview:**
```text
Gina Tsarukan Bayanai (Structs & Object Methods) a Harshen Hausa tare da Nyx. 🇳🇬📦 (Kashi na 3/4)

A fannin kimiyyar kwamfuta ta zamani, tsarukan bayanai (structs) sune kashin bayan gina manhajoji. Suna hada bayanai da ayyukan da suka shafi abu guda.

A cikin Nyx, muna amfani da kalmar `tsari` (struct) da `aikata` (impl) domin tsara kayayyaki da ayyukansu:

```nyx
// tsari.nyx
shigo std.core.io;

tsari KayanKasuwa {
    suna: string,
    farashi: i32,
    yawa: i32,
}

aikata KayanKasuwa {
    fili aiki jimillar_kudi(kansa) -> i32 {
        koma kansa.farashi * kansa.yawa;
    }
}

fili aiki babban_aiki() {
    bari shinkafa = KayanKasuwa {
        suna: "Buhun Shinkafa Kano",
        farashi: 48000,
        yawa: 2,
    };

    println("Kayan da aka zaba: {}", shinkafa.suna);
    println("Jimillar Kudi: ₦{}", shinkafa.jimillar_kudi());
}
```

💻 Yadda Zaka Gwada Shi (How to Run):

nyx run tsari.nyx --dialect=hausa

Sakamako a allo (Terminal Output):
[Nyx Dialect] Active dialect layer: Harshen Hausa
Compiled: tsari.exe
Kayan da aka zaba: Buhun Shinkafa Kano
Jimillar Kudi: ₦96000

Wannan tsari yana adana bayanai ne a kan Memory Stack kai tsaye ba tare da cinye ma'ajiyar RAM a banza ba.

Gobe a Kashi na 4: Yankin Ma'ajiya ta Musamman (`yanki`) domin sarrafa lissafi mai tarin yawa.

#Hausa #NyxLang #DataStructures #SystemsProgramming #SoftwareEngineering #STEMInAfrica
```

---

### 📌 Post 4 of 4: Yankin Ma'ajiyar Kwamfuta ta Zero-GC (Region Arenas)

**Hook & Preview:**
```text
Cikakken saurin kwamfuta ba tare da tsaiko ba: Yankin Ma'ajiya (`yanki`) a Nyx. 🇳🇬⚡ (Kashi na 4/4)

A yawancin yarukan kwamfuta kamar Java, Python, ko Go, kwamfuta tana yawan tsayawa domin goge ma'ajiyar RAM (Garbage Collection pauses). A manhajar asibiti ko banki, wannan tsayawar tana iya kawo asara.

A cikin Nyx, an tsara ma'ajiya ne ta hanyar `yanki` (memory region arena) wanda yake goge dubban bayanai a lokaci guda cikin sifili nanoseconds (0.00 ns):

```nyx
// yanki.nyx
shigo std.core.io;

fili aiki babban_aiki() {
    // Gina yankin ma'ajiya mai sauri
    yanki gona {
        bari sauyawa lissafi = 0;
        bari sauyawa i = 1;

        // Madaukin lissafi mai sauri
        yayin da i <= 1000 {
            lissafi = lissafi + i;
            i = i + 1;
        }

        println("--- Sakamakon Yankin Ma'ajiya (Zero-GC) ---");
        println("Jimillar lissafin 1 zuwa 1000: {}", lissafi);
    }
    // Dukkan ma'ajiyar tana wanke kanta kai tsaye!
}
```

💻 Yadda Zaka Gwada Shi (How to Run):

nyx run yanki.nyx --dialect=hausa

Sakamako a allo (Terminal Output):
[Nyx Dialect] Active dialect layer: Harshen Hausa
Compiled: yanki.exe
--- Sakamakon Yankin Ma'ajiya (Zero-GC) ---
Jimillar lissafin 1 zuwa 1000: 500500

Tare da Nyx:
• Babu matsalar satar bayanai (Use-After-Free protection)
• Babu buƙatar Garbage Collector
• Cikakken goyon bayan Harshen Hausa daga tushe

Ku gwada dandalinmu na ilimi a yanar gizo:
https://nyx.9jaoncloud.com.ng/hausa/index.html

#Hausa #HighPerformance #NyxLang #ComputerScience #MemoryManagement #TechInnovation #Africa
```
