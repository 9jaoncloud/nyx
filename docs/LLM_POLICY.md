# NYX LLM USAGE & PROVENANCE POLICY

**Document ID:** `nyx-llm-policy-1.0`  
**Status:** Foundational Normative Policy  
**Effective Date:** 2026-09-18  
**Ratified By:** Nyx Core Maintainers  
**Maintainer / Chief Architect:** Simeon Bala ([9jaonCloud Sovereign Systems Research](https://9jaoncloud.com.ng))  
**Official Contact:** [nyx@9jaoncloud.com.ng](mailto:nyx@9jaoncloud.com.ng)  
**Companion Documents:**  
- [`docs/PROVENANCE_AND_VERIFICATION.md`](PROVENANCE_AND_VERIFICATION.md)  
- [`docs/paper/nyx-provenance-and-verification.html`](paper/nyx-provenance-and-verification.html)  
- [`docs/llm-policy.html`](llm-policy.html)  

---

## Preamble

Nyx is a memory-safe, zero-GC systems programming language, robotics substrate, cognitive architecture, and formal verification framework. It was created during the era of large language models (LLMs). Those models were actively used throughout its development, and this collaboration is disclosed transparently and openly.

This policy exists because the question that matters in systems software engineering is **not** *"Was an LLM used to author this code?"* but rather:

> **"Is the resulting artifact correct, memory-safe, mathematically proven, reproducible, and tested?"**

Every clause below is designed to ensure strict verification, empirical reproducibility, and human accountability rather than unworkable, opaque origin policing.

---

## 1. Core Guiding Principles

1. **Verification is the Arbiter, Not Origin (§1.1):**  
   The origin of a line of code (human, LLM, or mixed) does not determine whether it belongs in Nyx. Correctness, automated testing, reproducibility, and formal verification determine whether it merges.

2. **Every Claim Must Be Independently Verifiable (§1.2):**  
   Every benchmark number, latency metric, and memory safety claim in Nyx documentation, papers, or releases must be reproducible via single-command reproduction scripts. Claims that cannot be reproduced are removed.

3. **Disclosure is Metadata, Not Confession (§1.3):**  
   Every source file in Nyx carries a structured file-level provenance header. Provenance is transparent metadata that preserves engineering context.

4. **Human Sign-Off is Mandatory for Every Merge (§1.4):**  
   An LLM may assist in synthesizing code, proofs, or tests, but an LLM can **never** be a maintainer, author of record, or merge gatekeeper. Every commit is reviewed, signed, and accounted for by a named human engineer.

5. **The Verification Pipeline is the Sole Source of Trust (§1.5):**  
   Trust in Nyx is rooted in deterministic verification (100% unit/integration test coverage, Jude SMT formal proofs, LTO/PGO compiler validation). If the verification pipeline passes, the artifact is trustworthy.

6. **No Involuntary LLM Adoption (§1.6):**  
   No contributor, auditor, or user may ever be required to use an LLM or proprietary AI service to contribute to or build Nyx.

7. **Zero Harassment Policy (§1.7):**  
   Contributors who disclose LLM assistance in accordance with this policy are protected by our Code of Conduct against harassment, gatekeeping, or discrimination.

---

## 2. Comparative Matrix: Nyx vs. Rust LLM Policy

| Dimension | Rust Foundation LLM Policy | Nyx Sovereign Systems Policy |
| :--- | :--- | :--- |
| **Philosophical Premise** | Origin-centric: restrict LLM code creation to protect legacy trust. | **Verification-centric:** Disclose origin, prove correctness via 4-tier pipeline. |
| **Foundational Codebase** | Assumes code was authored human-only before LLMs exist. | **Transparent LLM-era co-creation:** 100% disclosed and verified from day 0. |
| **Circuit Breaker** | Freezes PRs if >50% of PRs in 6 weeks are LLM-assisted. | **100% Test Pass Circuit Breaker:** Any failing test halts subsystem feature merges. |
| **Formal Safety Invariants** | Code review heuristics and manual RFC consensus. | **SMT-checked mathematical proofs (`@verify`) in Jude dialect.** |
| **Provenance Tracking** | Ad-hoc PR tags; no standard file-level metadata. | **Mandatory file-level header metadata validated in CI.** |
| **Reproducibility** | Standard CI tests. | **Strict Provenance Tiers:** `[MEASURED]`, `[SIMULATED]`, `[LITERATURE]`, `[ESTIMATED]`. |

---

## 3. Rules & Permitted Uses

### 3.1 ✅ Allowed (No Special Disclosure Required)
* **Private Learning & Exploration:** Asking an LLM to explain compiler internals, MLIR lowering, or SMT logic for personal understanding.
* **Private Pre-Submission Review:** Prompting an LLM to review your private branch before submitting a PR.
* **Personal Development Tooling:** Writing one-off local scripts, benchmarks, or shell helpers for your workstation.
* **Ideation & Synthesis:** Using an LLM to brainstorm algorithmic approaches, followed by independently authoring clean code.
* **Explicitly Experimental Branches:** Prototypes marked with `[EXPERIMENTAL - DO NOT REVIEW]` or `experimental/*` branch prefixes.

### 3.2 ⚠️ Allowed with Caveats (Disclosure & Verification Required)
* **LLM-Assisted Code PRs:** Permitted if marked with `llm-assisted` label, accompanied by full unit tests, pre-arranged human reviewer, and valid provenance headers.
* **Machine Translation:** Translating technical documentation into additional languages (e.g., Hausa, Yorùbá, Igbo dialects) provided native human review validates grammar.
* **Automated Bug Discovery / Fuzzing:** Using LLM-assisted fuzzing or static scanners, provided you personally reproduce the bug with a minimal test case before filing.
* **Dedicated Review Bots:** Automated bots must post from dedicated, blockable bot accounts and can never block or approve a PR unilaterally.

### 3.3 ❌ Banned (Strictly Prohibited)
* **Impersonating Human Authorship:** Posting raw LLM output as personal comments, PR descriptions, or review feedback.
* **Unreviewed LLM Documentation & Diagnostics:** Merging LLM-generated error diagnostics, CLI help text, or docstrings without human validation.
* **LLMs as Autonomous Gatekeepers:** Permitting an AI bot to autonomously approve or merge PRs.
* **Deliberate Provenance Concealment:** Hiding LLM involvement in PRs or removing file provenance headers (Code of Conduct violation).

---

## 4. The 4-Tier Verification Model

Every artifact in the Nyx ecosystem is subject to one or more rigorous verification tiers:

```
┌─────────────────────────────────────────────────────────────┐
│ Tier 4: Independent External Reproduction (One-Click Kits)   │
├─────────────────────────────────────────────────────────────┤
│ Tier 3: Formal Proof Discharges (Jude SMT / Z3 Invariants)   │
├─────────────────────────────────────────────────────────────┤
│ Tier 2: Single-Command Reproducible Benchmarks (Mean/StdDev) │
├─────────────────────────────────────────────────────────────┤
│ Tier 1: 100% Automated CI Test Pass Rate                    │
└─────────────────────────────────────────────────────────────┘
```

1. **Tier 1 — Automated Regression Tests:**  
   Every subsystem must maintain a 100% pass rate. `nyx test --all` executes unit, integration, and end-to-end compiler test suites.
2. **Tier 2 — Empirical Reproducibility:**  
   Every benchmark contains pinned hardware specs, compiler flags, and seeds. Single command: `nyx bench --provenance`.
3. **Tier 3 — Formal SMT Proofs:**  
   Safety-critical memory routines, region allocators, and lock-free channels are annotated with `@verify` and discharged by Jude.
4. **Tier 4 — Independent Replication Kits:**  
   Turnkey containerized kits allowing external academic and enterprise auditors to independently verify all claims.

---

## 5. Mandatory File Provenance Header

Every source file in the Nyx codebase must carry a machine-readable provenance header:

```nyx
// provenance: ai-assisted
// first-authored: 2026-03-15
// last-human-review: 2026-09-18
// verification-tier: 3
// verified-by: jude-1.2.0/cert-smt-9a0f42
// author: simeonbala
// reviewer: simeonbala
// contact: nyx@9jaoncloud.com.ng
```

CI will reject any pull request containing modified or new files lacking valid provenance metadata.

---

## 6. Official Commitments

### Commitment to Contributors
- Your work will be evaluated exclusively on technical merits and verification results.
- You will never be forced to use an AI tool.
- Transparent disclosure of AI assistance will be respected and welcomed.

### Commitment to Users & Enterprises
- Every claim in Nyx documentation is reproducible.
- Every zero-GC memory safety guarantee is mathematically verified.
- If any claim fails independent reproduction, it will be publicly corrected or retracted immediately.

---

## 7. Ratification & Contact

Adopted on **September 18, 2026** by the Nyx Core Architecture Team.

* **Chief Architect:** Simeon Bala  
* **Organization:** 9jaonCloud Sovereign Systems Research  
* **Official Inquiries & Policy Questions:** [nyx@9jaoncloud.com.ng](mailto:nyx@9jaoncloud.com.ng)  
* **Website:** [https://9jaoncloud.com.ng](https://9jaoncloud.com.ng) | [https://9jaoncloud.com.ng/nyx](https://9jaoncloud.com.ng/nyx)  
* **License:** Creative Commons Attribution 4.0 International (CC BY 4.0)
