# Coding in Nyx with Asụsụ Igbo (`--dialect=igbo`)
## Akwụkwọ Ntụziaka Ide Koodu Kọmputa n'Asụsụ Igbo (Complete Igbo Programming Manual)

**Compiler Version:** `nyxc v0.54.0+` | **Dialect Flag:** `--dialect=igbo` (or automatic detection) | **Runtime:** Zero-GC Region Memory (`0.00 ns` GC pause)

---

## 1. Okwu Mmalite (Introduction)

Nyx abụghị naanị ntụgharị okwu n'elu Bekee; ọ bụ ezigbo asụsụ kọmputa a na-achịkọta ozugbo gaa na koodu igwe (`native x86_64 / ARM64 machine code`), ebe okwu Asụsụ Igbo na-achịkwa mgbakọ na mwepụ, nhazi data, na nchekwa ebe nchekwa Zero-GC.

Iji chịkọta ma mee ka faịlụ Igbo gbaa ọsọ na Nyx:
```powershell
.\nyx.exe run aha_faili.nyx --dialect=igbo
```
*(Ma ọ bụ pịa `.\nyx.exe run aha_faili.nyx` n'ihi na Nyx na-amata Asụsụ Igbo n'onwe ya!)*

---

## 2. Tebụl Okwu Igbo Nile (Complete Keyword Reference Table)

Nyx na-anabata ma mkpụrụedemede nwere ntụpọ (`ị`, `ụ`, `ọ`, `ṅ`) ma ndị na-enweghị ntụpọ (`i`, `u`, `o`, `n`) maka ịdị mfe pịnye na kiiboodu ọ bụla:

| Okwu Igbo (Nwere Ntụpọ) | Enweghị Ntụpọ (ASCII-friendly) | Standard Nyx | Nkọwa na Ọrụ Ya (Meaning & System Role) |
| :--- | :--- | :--- | :--- |
| `bubata` | `bubata` | `import` | Bubata ọba akwụkwọ ma ọ bụ faịlụ (`import` module) |
| `ihuọha` | `ihuoha` | `pub` | Mee ka ọrụ ma ọ bụ nhazi pụta ìhè (`pub` visibility) |
| `ọrụ` | `oru` | `fn` | Nkọwa ọrụ (`fn` function definition) |
| `mbụ` | `mbu` | `main` | Isi mmalite usoro (`fn main()` entry point) |
| `ka` | `ka` | `let` | Nchekwa data na-anaghị agbanwe agbanwe (`let` binding) |
| `mgbanwe` | `mgbanwe` | `mut` | Data nwere ike ịgbanwe (`let mut` mutable variable) |
| `ọbụrụ` | `oburu` | `if` | Ọ bụrụ na ọnọdụ bụ eziokwu (`if` conditional) |
| `ọzọ` | `ozo` | `else` | Ma ọ bụghị ya (`else` / `else if` branch) |
| `mgbe` | `mgbe` | `while` | Megharia mgbe ọnọdụ ka bụ eziokwu (`while` loop) |
| `maka` | `maka` | `for` | Gbaa gburugburu n'ime usoro (`for` loop) |
| `nyeghachi` | `nyeghachi` | `return` | Nyeghachi nsonaazụ site n'ọrụ (`return` statement) |
| `nhazi` | `nhazi` | `struct` | Nhazi data pụrụ iche (`struct` definition) |
| `mejuputa` | `mejuputa` | `impl` | Mejuputa ọrụ maka nhazi (`impl` block) |
| `onwe_ya` | `onwe_ya` | `self` | Ntuaka maka nhazi ahụ n'onwe ya (`self` reference) |
| `dakọọ` | `dakoo` | `match` | Dakọọ ọnọdụ dị iche iche (`match` pattern matching) |
| `ụdị` | `udi` | `enum` | Ụdị nhọrọ (`enum` enumeration) |
| `omume` | `omume` | `trait` | Nkwekọrịta omume (`trait` interface) |
| `eziokwu` | `eziokwu` | `true` | Eziokwu (`boolean true`) |
| `ụgha` | `ugha` | `false` | Ụgha (`boolean false`) |
| `efu` | `efu` | `null` / `void` | Efu / Enweghị ihe (`null` / `void`) |
| `mpaghara` | `mpaghara` | `region` | Mpaghara nchekwa Zero-GC (`region` memory arena) |
| `n'otu_oge` / `n_otu_oge` | `n_otu_oge` | `async` | Ọrụ na-aga n'otu oge (`async` function) |
| `chere` | `chere` | `await` | Chere nsonaazụ ọrụ (`await` async result) |
| `ike` | `ike` | `strict` | Iwu siri ike (`strict` enforcement) |
| `nche_oge` | `nche` | `clock` | Elekere nchekwa igwe (`clock` hardware watchdog) |
| `nzikorita_mesh` | `nzikorita` | `mesh` | Nzikọrịta ozi netwọk (`mesh` network primitive) |
| `ike_ọkụ` | `ike_oku` | `power` | Njikwa ike ọkụ (`power` budget constraint) |
| `akaebe` | `akaebe` | `proof` | Akaebe nchekwa koodu (`proof` compile-time verification) |
| `io.bipute(...)` | `io.bipute(...)` | `println(...)` | Bipụta mkpụrụokwu n'ihuenyo (Print string line) |
| `io.bipute_nọmba(...)` | `io.bipute_nomba(...)` | `println(...)` | Bipụta nọmba n'ihuenyo (Print numeric value) |

---

## 3. Nkụzi 1: Nnọọ Ụwa na Mgbakọ Ego (Hello World & Variables)

Na Nyx, a na-eji `ka` emepụta data na-anaghị agbanwe agbanwe, ma jiri `ka mgbanwe` mepụta data nwere ike ịgbanwe:

```nyx
// Faịlụ: nnoo_igbo.nyx
bubata std.core.io;

ihuọha ọrụ mbụ() {
    io.bipute("Nnọọ! Nnukwu asụsụ Nyx n'Asụsụ Igbo.");

    // Mgbakọ ahịa n'Ọnịcha
    ka ọnụ_ego = 4500;
    ka mgbanwe ole = 2;
    ka mgbanwe ngụkọta = ọnụ_ego * ole;

    io.bipute_nọmba(ngụkọta);
}
```

**Nsonaazụ (`nyx.exe run nnoo_igbo.nyx --dialect=igbo`):**
```text
Nnọọ! Nnukwu asụsụ Nyx n'Asụsụ Igbo.
9000
```

---

## 4. Nkụzi 2: Mkpebi na Ọrụ (`ọbụrụ` / `ọzọ` & `ọrụ`)

Nyx na-agbaso **Iwu Eziokwu Siri Ike (Strict Boolean Rule)**: ọnọdụ ọ bụla dị n'ime `ọbụrụ` ga-egosipụta ntụnyere doro anya (`>=`, `<=`, `==`, `!=`).

```nyx
// Faịlụ: nkwughachi_igbo.nyx
bubata std.core.io;

ọrụ lelee_ego_nkwụghachi(ego: int) -> bool {
    ọbụrụ ego >= 50000 {
        nyeghachi eziokwu;
    } ọzọ {
        nyeghachi ụgha;
    }
}

ihuọha ọrụ mbụ() {
    ka mgbanwe ego_ahịa = 65000;

    ọbụrụ lelee_ego_nkwụghachi(ego_ahịa) == eziokwu {
        io.bipute("Enyela onye ahịa a ego nkwụghachi 5%!");
        ego_ahịa = ego_ahịa - 3250;
    } ọzọ {
        io.bipute("Ego ahịa erubeghị maka ego nkwụghachi.");
    }

    io.bipute_nọmba(ego_ahịa);
}
```

**Nsonaazụ:**
```text
Enyela onye ahịa a ego nkwụghachi 5%!
61750
```

---

## 5. Nkụzi 3: Nhazi Data na Mmejuputa (`nhazi`, `mejuputa`, `onwe_ya`)

Ị nwere ike ịkpụzi usoro azụmahịa ma ọ bụ ngwaahịa site n'iji `nhazi` (`struct`) na `mejuputa` (`impl`):

```nyx
// Faịlụ: ngwaahia_igbo.nyx
bubata std.core.io;

ihuọha nhazi NgwaAhịa {
    aha: string,
    ọnụ_ahịa: int,
    ụkpụrụ: int
}

mejuputa NgwaAhịa {
    ihuọha ọrụ gbako_ego_niile(onwe_ya) -> int {
        nyeghachi onwe_ya.ọnụ_ahịa * onwe_ya.ụkpụrụ;
    }
}

ihuọha ọrụ mbụ() {
    ka akwa = NgwaAhịa {
        aha: "Akwa George nke Aba",
        ọnụ_ahịa: 24000,
        ụkpụrụ: 4
    };

    ka ego_niile = akwa.gbako_ego_niile();
    io.bipute("Ngwaahịa: " + akwa.aha);
    io.bipute("Ngụkọta Ego: ₦" + ego_niile.to_string());
}
```

**Nsonaazụ:**
```text
Ngwaahịa: Akwa George nke Aba
Ngụkọta Ego: ₦96000
```

---

## 6. Nkụzi 4: Mpaghara Nchekwa Zero-GC na Ntugharị (`mpaghara` & `mgbe`)

Nyx anaghị eji Garbage Collector na-akwụsị ọrụ. Okwu `mpaghara` (`region`) na-ewepụta ebe nchekwa dị ọsọ ma kpochapụ ya ozugbo ngọngọ `}`mechiri:

```nyx
// Faịlụ: mpaghara_igbo.nyx
bubata std.core.io;

ihuọha ọrụ mbụ() {
    // Mpaghara nchekwa na-enweghị nkwụsị (Zero-GC Memory Region)
    mpaghara ahia_mba {
        ka mgbanwe ọnụọgụ = 1;
        ka mgbanwe ngụkọta = 0;

        mgbe ọnụọgụ <= 1000 {
            ngụkọta = ngụkọta + ọnụọgụ;
            ọnụọgụ = ọnụọgụ + 1;
        }

        io.bipute("--- Nsonaazụ Mpaghara Ncheta (Zero-GC) ---");
        io.bipute("Ngụkọta 1 ruo 1000: " + ngụkọta.to_string());
    }
}
```

**Nsonaazụ:**
```text
--- Nsonaazụ Mpaghara Ncheta (Zero-GC) ---
Ngụkọta 1 ruo 1000: 500500
```

