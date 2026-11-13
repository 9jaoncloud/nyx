# LinkedIn Series: Coding in Èdè Yorùbá with Nyx
## 4-Part Dedicated Masterclass

A 4-part ready-to-publish LinkedIn masterclass introducing compiled systems programming in native Yorùbá using `@nyx/yoruba` and Nyx's Zero-GC toolchain.

---

### 📌 Post 1 of 4: Bawo Agbaye & Ìbẹ̀rẹ̀ Koodu (Syntax Fundamentals)

**Hook & Preview:**
```text
Ṣe o mọ pe o le kọ koodu kọmputa to yara ti o si ni aabo to muna doko ni Èdè Yorùbá? 🇳🇬🌟 (Apá 1/4)

Fún ọ̀pọ̀lọpọ̀ ọdún, a ti gbàgbọ́ pé èdè Gẹ̀ẹ́sì nìkan ni a lè fi kọ koodu kọmputa. Ṣùgbọ́n pẹ̀lú Èdè Nyx (ede kọmputa ti a ṣe fún aabo ati iyara Zero-GC), àwọn ọ̀dọ́ ati onímọ̀ ẹ̀rọ wa ti lè fi èdè Yorùbá ṣe àgbékalẹ̀ àwọn ètò kọmputa tó lágbára.

Pẹ̀lú `@nyx/yoruba`, o lè lo àwọn ọ̀rọ̀ bíi: `iṣẹ`, `akọkọ`, `jẹki`, `ayipada`, ati `pada`.

Wo àpẹẹrẹ shiri "Bawo Agbaye" ati ìṣirò owó ọjà:

```nyx
// bawo.nyx
wọle std.core.io;

gbangba iṣẹ akọkọ() {
    // Ṣeto iye owo ati iye ọja
    jẹki owo = 3500;
    jẹki ayipada ege = 2;
    jẹki apapọ = owo * ege;

    println("Ẹ kaabọ si Èdè Nyx ni Èdè Yorùbá!");
    io.tẹ_nọmba(apapọ);
}
```

💻 Bí O Ṣe Lè Ṣiṣẹ Koodu Yìí (How to Run):

1. Fi akojọpọ Yorùbá sinu ẹrọ rẹ:
   nypm install @nyx/yoruba

2. Ṣajọ rẹ ki o si ṣiṣẹ lẹsẹkẹsẹ:
   nyx run bawo.nyx --dialect=yoruba

Èsì tí yóò jáde lórí kọmputa (Terminal Output):
[Nyx Dialect] Active dialect layer: Èdè Yorùbá
Compiled: bawo.exe
Ẹ kaabọ si Èdè Nyx ni Èdè Yorùbá!
7000

Koodu yìí ń ṣiṣẹ́ lábẹ́ abẹ́ pẹ̀lú C-interop tààrà láìsí ìdádúró Garbage Collector.

Ọ̀la ní Apá Keji: Ṣíṣe Ìpinnu pẹ̀lú `ti` ati `bibẹkọ`.

Dán an wò lórí ayélujára: https://nyx.9jaoncloud.com.ng/yoruba/index.html

#Yoruba #TechInAfrica #NyxLang #SoftwareEngineering #EdedeYoruba #STEMInYoruba #OpenSource #SystemsProgramming
```

---

### 📌 Post 2 of 4: Ṣíṣe Ìpinnu ati Àṣàyàn (Conditionals & Logic)

**Hook & Preview:**
```text
Bí a ṣe ń ṣe ìpinnu (Logic & Conditionals) nínú ètò kọmputa ní Èdè Yorùbá. 🇳🇬⚖️ (Apá 2/4)

Nínú ètò ìṣirò ifowopamọ́ tàbí kíkọ àbájáde ìdánwò, kọmputa gbọdọ mọ bí a ṣe ń yẹ ipò wò—ṣé akẹ́kọ̀ọ́ kọjá idánwò? Ṣé oníbàárà tóótun láti gba àwìn?

A lè lo `ti` (if), `bibẹkọ` (else), `otitọ` (true), ati `irọ` (false) láti ṣe ìpinnu yìí ní Èdè Nyx:

```nyx
// ipinnu.nyx
wọle std.core.io;

gbangba iṣẹ ṣayẹwo_idanwo(maki: i32) -> bool {
    // Ti maki ba to aadota (50), o ti kọja
    ti maki >= 50 {
        println("Oriire! O ti kọja idanwo yii.");
        pada otitọ;
    } bibẹkọ {
        println("Eforiti si! Maki ko tii to.");
        pada irọ;
    }
}

gbangba iṣẹ akọkọ() {
    jẹki maki_akẹkọọ = 74;
    ṣayẹwo_idanwo(maki_akẹkọọ);
}
```

💻 Bí O Ṣe Lè Ṣiṣẹ Koodu Yìí (How to Run):

nyx run ipinnu.nyx --dialect=yoruba

Èsì lórí iboju (Terminal Output):
[Nyx Dialect] Active dialect layer: Èdè Yorùbá
Compiled: ipinnu.exe
Oriire! O ti kọja idanwo yii.

Itumọ àwọn ọ̀rọ̀ kọmputa:
• `ti` $\rightarrow$ `if`
• `bibẹkọ` $\rightarrow$ `else`
• `otitọ` $\rightarrow$ `true`
• `irọ` $\rightarrow$ `false`
• `pada` $\rightarrow$ `return`

Ọ̀la ní Apá Kẹta: Ètò Data (`eto` ati `ṣe`) ati kíkà lúùpù (`nigbati`).

#Yoruba #CodingInYoruba #NyxLang #SoftwareEngineering #AfricanTech #STEMInAfrica
```

---

### 📌 Post 3 of 4: Kíka Lupu ati Ètò Data (Loops & Structs)

**Hook & Preview:**
```text
Ṣíṣètò akojọpọ data (Structs & Impl) ní Èdè Yorùbá pẹ̀lú Nyx. 🇳🇬📦 (Apá 3/4)

Nígbà tí a bá ń kọ ètò kọmputa ńlá fún ilé-ìwé tàbí ilé-iṣẹ́, a nílò láti kó àwọn ìsọfúnni tí ó jọra jọ sínú `eto` (struct) kí a sì fi àwọn iṣẹ́ tí ó jẹ mọ́ ọn sínú rẹ̀ nípa lilo `ṣe` (impl).

Wo bí a ṣe ń ṣe ètò Akẹ́kọ̀ọ́ ní Èdè Yorùbá:

```nyx
// eto.nyx
wọle std.core.io;

eto Akeko {
    orukọ: string,
    ọjọ_ori: i32,
    owo_ile_iwe: i32,
}

ṣe Akeko {
    gbangba iṣẹ iye_owo_odun(ara_rẹ) -> i32 {
        pada ara_rẹ.owo_ile_iwe * 3; // Sa meta lodun
    }
}

gbangba iṣẹ akọkọ() {
    jẹki tade = Akeko {
        orukọ: "Tade Oladipo",
        ọjọ_ori: 19,
        owo_ile_iwe: 45000,
    };

    println("Orukọ Akẹkọọ: {}", tade.orukọ);
    println("Apapọ Owo Ọdun: ₦{}", tade.iye_owo_odun());
}
```

💻 Bí O Ṣe Lè Ṣiṣẹ Koodu Yìí (How to Run):

nyx run eto.nyx --dialect=yoruba

Èsì tí yóò jáde (Terminal Output):
[Nyx Dialect] Active dialect layer: Èdè Yorùbá
Compiled: eto.exe
Orukọ Akẹkọọ: Tade Oladipo
Apapọ Owo Ọdun: ₦135000

Pẹ̀lú ètò yìí, kọmputa ń ṣiṣẹ́ lórí Stack memory láìsí ìfipamọ́ àjùlọ tàbí jafara.

Ọ̀la ní Apá Kẹrin: Agbègbè Ìṣirò Zero-GC (`agbegbe`) fún ìṣirò to gba microseconds.

#Yoruba #NyxLang #DataStructures #ProgrammingInYoruba #SoftwareEngineering #Nigeria
```

---

### 📌 Post 4 of 4: Agbegbe Ìṣirò Laisi Idaduro Zero-GC (Memory Arenas)

**Hook & Preview:**
```text
Iyara kọmputa to gaju laisi idaduro: Agbegbe Ìṣirò (`agbegbe`) ninu Nyx. 🇳🇬⚡ (Apá 4/4)

Ọ̀pọ̀lọpọ̀ èdè kọmputa bíi Java tàbí Python máa ń ní ìdádúró (Garbage Collection pause) nígbà tí wọ́n bá ń gbá oríṣiríṣi ìdọ̀tí kúrò ní iranti RAM. Èyí kò bójú mu fún ètò ọkọ̀ òfurufú tàbí ìṣòwò gíga lórí ayélujára.

Ní Èdè Nyx, a máa ń lo `agbegbe` (region arena) láti pèsè iranti to ń nu ara rẹ̀ mọ́ ní milliseconds láìsí ìdádúró:

```nyx
// agbegbe.nyx
wọle std.core.io;

gbangba iṣẹ akọkọ() {
    // Agbegbe iṣiro Zero-GC to yara
    agbegbe oja_nla {
        jẹki ayipada apapọ = 0;
        jẹki ayipada i = 1;

        // Kika lupu to gba egberun igba
        nigbati i <= 1000 {
            apapọ = apapọ + i;
            i = i + 1;
        }

        println("--- Èsì Agbègbè Ìṣirò (Zero-GC) ---");
        println("Àpapọ̀ iṣirò lati 1 si 1000: {}", apapọ);
    }
    // Gbogbo iranti ti nu kuro laisi idaduro!
}
```

💻 Bí O Ṣe Lè Ṣiṣẹ Koodu Yìí (How to Run):

nyx run agbegbe.nyx --dialect=yoruba

Èsì lórí iboju (Terminal Output):
[Nyx Dialect] Active dialect layer: Èdè Yorùbá
Compiled: agbegbe.exe
--- Èsì Agbègbè Ìṣirò (Zero-GC) ---
Àpapọ̀ iṣirò lati 1 si 1000: 500500

Àwọn àǹfààní Èdè Nyx:
• Aabo tó péye fún iranti kọmputa (Zero-GC Region Safety)
• Ìbámu pẹ̀lú C-ABI fún iṣẹ́ pẹlu ẹ̀rọ abinibi
• Kíkọ koodu ní Èdè Yorùbá tààrà lórí kọmputa

Dán an wò lórí ayélujára:
https://nyx.9jaoncloud.com.ng/yoruba/index.html

#Yoruba #HighPerformance #NyxLang #ComputerScience #MemoryManagement #TechInnovation #Africa
```
