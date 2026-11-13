# Coding in Nyx with Nigerian Pidgin (`--dialect=pidgin` & `@nyx/pidgin`)
## Complete Naija Pidgin Systems Programming & Commercial NLP Manual

**Compiler Version:** `nyxc v0.54.0+` | **Dialect Flag:** `--dialect=pidgin` (or automatic detection) | **Built-in Engine:** `@nyx/pidgin` + `runtime/nyx_pidgin.c`

---

## 1. Introduction: Why Naija Pidgin in a Systems Language?

Nigerian Pidgin is spoken by over 100 million people across West Africa as the primary language of commerce, trade, logistics, and daily street engineering. Nyx provides **two complementary layers** of Nigerian Pidgin support:

1. **Native Pidgin Syntax Dialect (`--dialect=pidgin`)**: Write structural Nyx code (`work oga()`, `make`, `make dey`, `suppose` / `if no be so`, `as e dey go`, `arrange`, `area`, `io.talk`) that compiles directly to native x86_64/ARM64 machine code with zero garbage collection pauses.
2. **Native Pidgin Commercial NLP Engine (`import @nyx/pidgin;`)**: Built directly into the C runtime (`runtime/nyx_pidgin.c` & `include/nyx_pidgin.h`), allowing standard or dialect Nyx applications to parse real-world Nigerian market sentences (e.g., *"Madam Kemi buy 3 mudu of garri for 250 each"*) into typed `NyxPidginTransaction` structs in microseconds.

To run any Pidgin program with `nyx.exe`:
```powershell
.\nyx.exe run program_name.nyx --dialect=pidgin
```

---

## 2. Complete Nigerian Pidgin Keyword Reference Table (`--dialect=pidgin`)

| Naija Pidgin Keyword | Alternative Form | Standard Nyx | Meaning & System Role |
| :--- | :--- | :--- | :--- |
| `bring` | `carry` | `import` | Import a module or standard library (`import`) |
| `open` | `open` | `pub` | Public visibility modifier (`pub`) |
| `work` | `yarn` | `fn` | Function declaration (`fn`) |
| `oga` | `oga_work` / `first` | `main` | Program entry point (`fn main()`) |
| `make` | `make` | `let` | Immutable variable declaration (`let`) |
| `dey` | `dey` | `mut` | Mutable variable modifier (`make dey x = 10;` -> `let mut x = 10;`) |
| `suppose` | `if` | `if` | Conditional `if` branch |
| `if no be so` | `if_no_be_so` / `abi na` | `else` | Conditional `else` branch |
| `as e dey go` | `as_e_dey_go` | `while` | `while` loop while condition holds true |
| `for inside` | `for` | `for` | `for` loop iteration |
| `giveback` | `return` | `return` | Return value from a function |
| `arrange` | `struct` | `struct` | Define a custom data structure (`struct`) |
| `do_am` | `impl` | `impl` | Implement methods for a struct (`impl`) |
| `my_own` | `self` | `self` | Struct instance reference (`self`) |
| `wetin` | `match` | `match` | Pattern matching (`match`) |
| `na_true` | `true` | `true` | Boolean `true` |
| `na_lie` | `false` | `false` | Boolean `false` |
| `nothing` | `null` | `null` | Empty / `null` value |
| `area` | `region` | `region` | Deterministic Zero-GC memory arena (`region`) |
| `io.talk(...)` | `talk(...)` | `println(...)` | Print text line to standard output |
| `io.talk_number(...)` | `println(...)` | `println(...)` | Print numeric value to standard output |

---

## 3. Part A: Pure Naija Pidgin Dialect Examples (`--dialect=pidgin`)

### Example 1: Hello Naija & Market Discount Logic
```nyx
// File: market_pidgin.nyx
bring std.core.io;

open work oga() {
    io.talk("How far Naija! Welcome to Nyx Pidgin Programming.");

    make bag_price = 4000;
    make dey quantity = 2;
    make dey total = bag_price * quantity;

    suppose total > 5000 {
        io.talk("Correct customer! You get 500 Naira jara discount.");
        total = total - 500;
    } if no be so {
        io.talk("No jara discount for this amount.");
    }

    io.talk_number(total);
}
```

**Output (`nyx.exe run market_pidgin.nyx --dialect=pidgin`):**
```text
How far Naija! Welcome to Nyx Pidgin Programming.
Correct customer! You get 500 Naira jara discount.
7500
```

### Example 2: Structs (`arrange`), Methods (`do_am`), and Zero-GC Regions (`area`)
```nyx
// File: shop_ledger_pidgin.nyx
bring std.core.io;

open arrange ShopItem {
    item_name: string,
    unit_price: int,
    carton_count: int
}

do_am ShopItem {
    open work calc_total(my_own) -> int {
        giveback my_own.unit_price * my_own.carton_count;
    }
}

open work oga() {
    area alaba_market {
        make goods = ShopItem {
            item_name: "Solar Inverter Battery",
            unit_price: 85000,
            carton_count: 2
        };

        make bill = goods.calc_total();
        io.talk("Item: " + goods.item_name);
        io.talk("Total Bill: ₦" + bill.to_string());
    }
}
```

**Output:**
```text
Item: Solar Inverter Battery
Total Bill: ₦170000
```

---

## 4. Part B: The `@nyx/pidgin` Commercial NLP Engine (LinkedIn Series Examples)

Nyx also includes `@nyx/pidgin`, a zero-allocation C runtime parser (`nyx_pidgin_parse`) that extracts structured commercial fields (`customer`, `item`, `quantity`, `unit`, `unit_price`, `total`) from natural Nigerian Pidgin sentences.

### Example 3: Parsing Street Market Transactions (`pidgin::parse_sentence`)
```nyx
// File: voice_market.nyx
import std.core.io;
import @nyx/pidgin;

pub fn main() {
    let raw_speech = "Madam Kemi buy 3 mudu of garri for 250 each";
    let tx = pidgin::parse_sentence(raw_speech);

    println("--- Commercial Trade Calculation ---");
    println("Customer: " + tx.customer);
    println("Commodity: " + tx.item);
    println("Quantity: " + tx.quantity.to_string() + " " + tx.unit);
    println("Unit price: ₦" + tx.unit_price.to_string());
    println("Total to pay: ₦" + tx.total.to_string());
}
```

**Output:**
```text
--- Commercial Trade Calculation ---
Customer: Madam Kemi
Commodity: garri
Quantity: 3 mudu
Unit price: ₦250
Total to pay: ₦750
```

### Example 4: Oga Daily Ledger & Voice POS Confirmation
```nyx
// File: oga_daily_ledger.nyx
import std.core.io;

pub struct DailyLedger {
    cash_sales: int,
    transfer_sales: int,
    credit_given: int
}

impl DailyLedger {
    pub fn total_revenue(self) -> int {
        return self.cash_sales + self.transfer_sales + self.credit_given;
    }
}

pub fn main() {
    let my_shop = DailyLedger {
        cash_sales: 15000,
        transfer_sales: 45000,
        credit_given: 12000
    };

    let gross = my_shop.total_revenue();

    println("=== OGA DAILY FINANCIAL SUMMARY ===");
    println("Cash wey dey hand: ₦" + my_shop.cash_sales.to_string());
    println("Bank transfer:     ₦" + my_shop.transfer_sales.to_string());
    println("Money wey outside: ₦" + my_shop.credit_given.to_string());
    println("Total gross trade: ₦" + gross.to_string());
}
```

**Output:**
```text
=== OGA DAILY FINANCIAL SUMMARY ===
Cash wey dey hand: ₦15000
Bank transfer:     ₦45000
Money wey outside: ₦12000
Total gross trade: ₦72000
```

