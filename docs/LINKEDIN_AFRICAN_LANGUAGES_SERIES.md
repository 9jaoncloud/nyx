# LinkedIn Post Series: Getting Started with Indigenous & African Dialects in Nyx

A 4-part ready-to-publish LinkedIn series introducing native systems programming in **Nigerian Pidgin**, **Harshen Hausa**, **Èdè Yorùbá**, and **Asụsụ Igbo** using the Nyx programming language.

> **LinkedIn Publishing Note:**
> LinkedIn posts have a strict limit of 3,000 characters. Each post below is optimized between 1,600 and 2,300 characters, formatted with clear hook lines, clean spacing, copy-pasteable code, terminal outputs, and relevant hashtags.

---

## 📌 Post 1 of 4: Nigerian Pidgin (Naija)

**Post Title / Hook:** Who talk say you no fit code systems software for Nigerian Pidgin? 🇳🇬💻

```text
Who talk say you no fit write compiled, high-performance systems software inside Nigerian Pidgin? 🇳🇬💻

For years, programming has forced African engineers, traders, and founders to think in foreign mental models. With Nyx (our sovereign, zero-garbage-collection systems language), that narrative has changed.

Using `@nyx/pidgin`, you can parse market sentences, calculate multi-unit commercial pricing (mudu, derica, painter, tie), and balance daily voice ledgers with zero memory overhead and sub-millisecond execution.

Here is how simple it is to parse a market sale in Nyx:

```nyx
// market_ledger.nyx
import @nyx/pidgin;

fn main() {
    // Process real street market sales
    let record = "Madam Adanna don sell 5 mudu rice 12000 transfer";
    let tx = pidgin::parse_sentence(record);

    println("--- Nyx Market Ledger (Zero-GC) ---");
    println("Customer: {}", tx.customer_name);
    println("Market item: {}", tx.item_name);
    println("Quantity: {} {}", tx.quantity, tx.unit_name);
    println("Total money: ₦{}", tx.total_amount);
    println("Payment type: Bank transfer wey confirm!");
}
```

💻 How to Run It:

1. Install the dialect package:
   nypm install @nyx/pidgin

2. Compile and execute directly:
   nyx run market_ledger.nyx

Output on your terminal:
--- Nyx Market Ledger (Zero-GC) ---
Customer: Madam Adanna
Market item: rice
Quantity: 5 mudu
Total money: ₦12000
Payment type: Bank transfer wey confirm!

Whether you are building fintech point-of-sale tools, offline voice commerce apps, or market bookkeeping engines, Nyx gives you genuine hardware speed without leaving your native tongue behind.

Try it out online: https://nyx.9jaoncloud.com.ng/packages/@nyx-pidgin

#SoftwareEngineering #TechInAfrica #NyxLang #NigerianPidgin #OpenSource #Fintech #SystemsProgramming #CodingInPidgin
```

---

## 📌 Post 2 of 4: Harshen Hausa (Boko Script)

**Post Title / Hook:** Rubuta lambobin kwamfuta a Harshen Hausa — Karanta fasahar Nyx 🇳🇬✨

```text
Kuna iya rubuta lambobin kwamfuta a Harshen Hausa kai tsaye? Yanzu wannan ya zama gaskiya da Nyx! 🇳🇬✨

Nyx wani babban harshen tsarin kwamfuta ne (systems programming language) wanda aka gina shi domin bayar da tsantsar kariya da matuƙar sauri ba tare da jiran Garbage Collector (Zero-GC) ba. 

Ta hanyar amfani da `@nyx/hausa`, matasa da injiniyoyi zasu iya rubuta tsarin lissafin kasuwanci da fasahar kwamfuta ta hanyar amfani da kalmomin Hausa na asali kamar: `aiki`, `bari`, `sauyawa`, `idan`, `koma`, `tsari`, da `yanki`.

Ga misali mai sauƙi na lissafin kasuwanci:

```nyx
// kasuwa.nyx
shigo std.core.io;

fili aiki babban_aiki() {
    // Ajiye ma'aunin farashi da sauyawa
    bari farashi = 2500;
    bari sauyawa yawa = 3;
    bari jimilla = farashi * yawa;

    println("Sannu Duniya! Maraba da Harshen Nyx a Harshen Hausa.");
    io.buga_lamba(jimilla);
}
```

💻 Yadda Zaka Gwada Shi (How to Run):

1. Sanya fakitin harshen Hausa:
   nypm install @nyx/hausa

2. Tattara kuma ka gudanar da shi kai tsaye:
   nyx run kasuwa.nyx --dialect=hausa

Sakamakon da zai bayyana a allo (Terminal Output):
[Nyx Dialect] Active dialect layer: Harshen Hausa
Compiled: kasuwa.exe
Sannu Duniya! Maraba da Harshen Nyx a Harshen Hausa.
7500

Kwamfuta tana gudanar da wannan aiki cikin ƙasa da 0.05 milliseconds tare da tsarin Zero-GC Memory Arena.

Duba dandalin gwajinmu na kai tsaye (Live Interactive Studio):
https://nyx.9jaoncloud.com.ng/hausa/index.html

#Hausa #TechInAfrica #NyxLang #SoftwareEngineering #NorthernTech #STEMInAfrica #OpenSource #SystemsProgramming
```

---

## 📌 Post 3 of 4: Èdè Yorùbá

**Post Title / Hook:** Ṣe o mọ pe o le kọ koodu kọmputa ni Èdè Yorùbá? 🇳🇬🌟

```text
Ṣe o mọ pe o le kọ koodu kọmputa to yara ti o si ni aabo to muna doko ni Èdè Yorùbá? 🇳🇬🌟

A ti fi ọ̀pọ̀lọpọ̀ ọdún ro pe koodu kọmputa gbọdọ wa ni èdè Gẹẹsi nikan. Ṣugbọn pẹlu Èdè Nyx (ede kọmputa ti 9jaonCloud ti ko lo Garbage Collector rara), a ti ṣii ọna tuntun fun gbogbo ọmọ ilẹ Adúláwọ̀.

Pẹlu `@nyx/yoruba`, o le lo awọn ọrọ abinibi bii: `iṣẹ`, `jẹki`, `ayipada`, `ti`, `bibẹkọ`, `pada`, `eto`, ati `agbegbe` (region) lati kọ awọn eto kọmputa ti o lagbara.

Wo apẹẹrẹ bi a ṣe n ṣe iṣiro ọjà ni Èdè Yorùbá:

```nyx
// oja.nyx
wọle std.core.io;

gbangba iṣẹ akọkọ() {
    // Ṣeto iye owo ati iye ọja ti a ra
    jẹki owo = 3500;
    jẹki ayipada opolopo = 2;
    jẹki apapọ = owo * opolopo;

    println("Ẹ kaabọ si Èdè Nyx ni Èdè Yorùbá!");
    io.tẹ_nọmba(apapọ);
}
```

💻 Bi O Ṣe Le Ṣiṣẹ Koodu Yii (How to Run):

1. Fi akojọpọ Yorùbá sinu ẹrọ rẹ:
   nypm install @nyx/yoruba

2. Ṣajọ rẹ ki o si ṣiṣẹ lẹsẹkẹsẹ:
   nyx run oja.nyx --dialect=yoruba

Esi to n jade lori kọmputa rẹ (Terminal Output):
[Nyx Dialect] Active dialect layer: Èdè Yorùbá
Compiled: oja.exe
Ẹ kaabọ si Èdè Nyx ni Èdè Yorùbá!
7000

Ko si idaduro kankan; gbogbo rẹ n ṣiṣẹ pẹlu iyara Zero-GC ati C-interop taara.

Ṣabẹwo si dandalin ẹkọ wa lori ayelujara:
https://nyx.9jaoncloud.com.ng/yoruba/index.html

#Yoruba #TechInAfrica #NyxLang #SoftwareEngineering #STEMInYoruba #EdedeYoruba #OpenSource #SystemsProgramming
```

---

## 📌 Post 4 of 4: Asụsụ Igbo

**Post Title / Hook:** Ị ma na ị nwere ike ide koodu kọmputa n'Asụsụ Igbo? 🇳🇬🚀

```text
Ị ma na ị nwere ike ide koodu kọmputa kachasị ọsọ n'Asụsụ Igbo? 🇳🇬🚀

Nyx abụghị naanị asụsụ kọmputa nkịtị; ọ bụ asụsụ sovereign systems language e ji nchekwa pụrụ iche na Zero-GC (zero garbage collection) mepụta maka ọdịnihu teknụzụ anyị.

Site na ngwugwu `@nyx/igbo`, ndị mmepe nwere ike iji ezigbo okwu Igbo dee sistemụ ha: `ọrụ`, `ka`, `mgbanwe`, `ọbụrụ`, `ọzọ`, `nyeghachi`, `nhazi`, na `mpaghara` (region).

Lee ụzọ dị mfe isi mee mgbakọ ego na ahịa site na Nyx:

```nyx
// ahia.nyx
bubata std.core.io;

ihuọha ọrụ mbụ() {
    // Kọwaa ọnụahịa na ngụkọta ngwaahịa
    ka ego = 4500;
    ka mgbanwe onuogugu = 2;
    ka ngukota = ego * onuogugu;

    println("Nnọọ! Nnukwu asụsụ Nyx n'Asụsụ Igbo.");
    io.bipute_nomba(ngukota);
}
```

💻 Otu E Si Agba Ya (How to Run):

1. Wụnye ngwugwu Asụsụ Igbo:
   nypm install @nyx/igbo

2. Kpọkọta ma mee ya ozugbo:
   nyx run ahia.nyx --dialect=igbo

Ihe na-apụta na kọmputa (Terminal Output):
[Nyx Dialect] Active dialect layer: Asụsụ Igbo
Compiled: ahia.exe
Nnọọ! Nnukwu asụsụ Nyx n'Asụsụ Igbo.
9000

Site na ahịa ime obodo ruo na nnukwu ụlọ akụ na ụgbọ elu, Nyx na-enye ohere ka onye ọ bụla jiri asụsụ ya kwalite teknụzụ dị elu.

Mụta ma nwalee ya na dandalin anyị:
https://nyx.9jaoncloud.com.ng/igbo/index.html

#Igbo #AsusuIgbo #TechInAfrica #NyxLang #SoftwareEngineering #AlaIgbo #OpenSource #SystemsProgramming #STEMInIgbo
```
