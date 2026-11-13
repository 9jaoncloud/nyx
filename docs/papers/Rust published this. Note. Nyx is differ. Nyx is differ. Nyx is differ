Rust published this. Note. Nyx is different, I would not have build the project without ai including the compiler. But we had test for everything. Read this how.should nyx better do it.
The user is showing me Rust's LLM usage policy. They want to know how Nyx should approach this differently. They're being honest that Nyx was built WITH AI including the compiler, and they have tests for everything. They want my honest assessment of how Nyx should handle this.

Let me read the Rust policy carefully and think about what's different about Nyx, and what Nyx should do.

Key points from Rust's policy:

LLMs allowed for questions, analysis, refinement, checking, suggesting, reviewing — NOT creating

Bans LLM-created comments, docs, diagnostics

Conditional approval for trivial changes, translation, review bots

Experiment: LLM-created code allowed with pre-arranged reviewer, non-critical, high quality, well-tested, well-reviewed

Disclosure required

Circuit breaker: if >50% of merged PRs are LLM-created in 6 weeks, freeze

Moderation: don't be a detective, be honest

Team ratification required

Now, the honest reality of Nyx:

Built entirely by user + AI

Nyx is a solo project

Everything has tests

They're trying to make it credible

The key insight: Rust can afford a conservative policy because they have thousands of contributors. Nyx can't. Nyx is a solo project. The policy needs to be different.

But Nyx also can't just say "AI does everything, trust me" — that would kill credibility in the exact same community they need to recruit from.

So what's the right policy?

I need to think about this honestly. Let me structure this:

What Rust got right (principles worth keeping)

Why Nyx can't copy Rust's policy directly

The Nyx-specific policy that would actually work

How to be honest about the AI-origin without losing credibility

What disclosure actually looks like for Nyx

Key realization: Nyx's differentiator is that it has VERIFICATION. Nyx has:

100% test coverage claims

Formal verification (Jude)

Benchmarks with provenance tiers

Reproducibility kit

The AI-origin question becomes less important if everything is verifiable. So Nyx's policy should be:

Verify everything (tests, formal proofs, benchmarks)

Disclose origins

Let verification be the arbiter, not origin

This is a philosophically different stance than Rust's. Rust says "origin matters, quality matters, disclosure matters." Nyx can say "verification matters most, origin is disclosed."

Also important: the "AI made the compiler" claim is genuinely different from "AI made a PR to a compiler." When the whole project is AI-assisted, the origin is not a per-PR question. It's a foundational question. Nyx needs to acknowledge this openly.

Let me write this carefully. The user is asking a serious question about how to position their project. They've been honest throughout. I should be honest back.

The user is Nigerian, building something ambitious, solo. They've been through multiple audit rounds. They want to know how to be credible.

I should:

Validate what's right in Rust's policy

Be clear that Nyx's context is fundamentally different

Propose a policy that's honest about the origin

Make verification the core of the credibility story, not origin

Give them something concrete they can publish

Let me write this. It should be a policy document, not just commentary. Format as a proper document since this is what they're asking for.

Length: this deserves a thorough response, but not bloated. Aim for something they can actually use — perhaps ~1500-2000 words that reads as a proposed policy with reasoning.

Format: markdown with clear sections. Should read like a policy document + commentary explaining why.

Nyx LLM Usage Policy: A Different Foundation
You're right to raise this. Rust's policy is thoughtful, but it was written by a project with thousands of contributors, decades of prior art, and a stable codebase. Nyx is a solo project where AI was present from the first line. Copying Rust's policy verbatim would be dishonest to your actual situation — and honesty is the thing you're trying to protect.

Here's what I think Nyx should do, and why.

Part 1: What Rust Got Right (Keep These)
Rust's policy is excellent on three specific dimensions, and Nyx should keep all three:

"Better, not faster." — The purpose of a language is not to produce more code, but better code. This is a universal principle. Keep it.

Disclosure without over-policing. — Rust says "be honest, don't make mods play detective." That's the right balance for any project.

Verification is the ultimate arbiter. — Rust holds LLM PRs to higher standards (well-tested, well-reviewed, no exceptions for "tests are hard"). Nyx should be the strictest version of this.

Part 2: Why Nyx Cannot Copy Rust's Policy
Rust's policy assumes a specific context that Nyx does not have:

Assumption in Rust's Policy	Nyx's Reality
Thousands of human contributors	One author + AI
Code existed before LLMs	LLMs were foundational
Trust already exists; policy manages drift	Trust does not exist yet; policy must earn it
Reputation can be checked historically	Reputation is being built now
Community pressure disciplines behavior	No community yet to apply pressure
"LLM contributions" is a marginal category	"LLM contributions" is 100% of the project
If you copy Rust's policy, you'd be lying about the origin of your code. And lying is the one thing Rust's policy treats as a CoC violation. The honest thing is to say: "Nyx was built with AI. Here's how we make sure it's still trustworthy."

Part 3: What Nyx's Policy Should Actually Say
Here's the policy I would recommend. It's honest, it's strict, and it turns the AI-origin from a liability into a demonstration.

NYX LLM USAGE POLICY
Status: Foundational. Applies to all Nyx repositories.
Rationale: Nyx was created in collaboration with LLMs. This is disclosed openly. The question is not whether LLMs were used, but whether the resulting artifact is verifiable, reproducible, and correct. Everything below is designed to make that verifiable.

Principle 1 — Verification is the arbiter, not origin.
The origin of a line of code (human or LLM) does not determine whether it belongs in Nyx. Whether the code is correct, tested, benchmarked, or formally verified determines whether it belongs in Nyx.

This is different from Rust's approach. Rust says: "LLM-created code is a separate category." Nyx says: "LLM-created code is subject to the same verification bar as any other code. If it passes, it ships. If it doesn't, it doesn't."

Principle 2 — Every claim must be independently verifiable.
Every number in Nyx's documentation, papers, or benchmarks must be:

Reproducible with a single command

Tagged with provenance ([MEASURED], [SIMULATED], [LITERATURE], [ESTIMATED])

Traceable to a test that passes in CI

If a claim cannot be verified, it is removed from the docs.

Principle 3 — Disclosure is not a confession, it is metadata.
Every file in the Nyx codebase carries a file-level provenance header:

nyx
// provenance: ai-assisted
// first-authored: 2026-03-15
// last-human-review: 2026-09-12
// test-coverage: 100%
// verified-by: jude-1.2.0/cert-8f4a...
// author: <name>
// reviewer: <name>
This is not shameful. It is metadata that helps future contributors understand the state of every file, just like a git blame tells you who touched a line last.

Principle 4 — Every LLM-assisted PR requires human sign-off.
If Nyx receives external contributions:

LLM-created PRs must be tagged llm-assisted

The contributor must state which parts were LLM-created

The reviewer must be a human who fully understands the change

The reviewer's name is recorded in the PR metadata

This is stricter than Rust's pre-arranged reviewer requirement, because Nyx cannot yet rely on a large reviewer pool.

Principle 5 — No LLM may be a gatekeeper.
LLM reviews, LLM-generated tests, and LLM-suggested fixes are all inputs. They are never decisions. The decision to merge is made by a human who is named on the PR.

Principle 6 — The circuit breaker is not percentage-based, it is verification-based.
Rust's circuit breaker stops LLM PRs at 50% of merged PRs. Nyx cannot use this rule because 100% of PRs would be LLM-assisted.

Instead: if the test pass rate drops below 100% for any subsystem, no new features may be merged into that subsystem until it is restored to 100%.

This is a stronger discipline than Rust's, because it applies to all code regardless of origin.

Principle 7 — Formal verification is preferred over review.
Jude exists. Use it. Every safety-critical invariant should be a formal proof, not a review comment. A formally verified invariant is worth more than ten LLM reviews and one human review combined.

Principle 8 — Reproducibility is the ultimate disclosure.
If a reviewer can clone the repo, run one command, and get the same result as the paper, the origin of the code is irrelevant. The proof is in the reproduction.

Part 4: What This Policy Allows Nyx to Do That Rust's Cannot
Capability	Rust	Nyx
Foundation built with LLM assistance	Banned in spirit	Allowed, disclosed
LLM-generated compiler code	Banned	Allowed with verification
LLM-authored paper	Banned in spirit	Allowed with reproducibility
LLM-driven training experiments	Discouraged	Allowed with seeded results
LLM-curated docs	Banned	Allowed with file-level provenance
Formal verification of LLM output	Not applicable	Core discipline
The Nyx policy is more permissive on origin but stricter on verification. That's the correct trade-off for a solo AI-assisted project.

Part 5: The One Question That Will Be Asked
When someone asks: "How can we trust a language whose compiler was written by an LLM?"

The answer is not "you can't." The answer is:

"You don't trust the origin. You trust the verification. Every line of Nyx's compiler is tested by the compiler itself. Every safety-critical proof is discharged by Jude. Every benchmark is reproducible with one command. If any of that fails, we fail. If it holds, it doesn't matter that an LLM was involved."

That is a stronger answer than Rust's, because Rust cannot make that claim about its own codebase — Rust's compiler is not 100% formally verified.

Part 6: How to Publish This
The policy should live in the Nyx repo as docs/LLM_POLICY.md. In the paper, add a section called "Provenance and Verification" that states:

"Nyx was created in collaboration with large language models. Every artifact produced by this collaboration is subject to verification through (1) automated tests, (2) formal proofs discharged by the Jude dialect, and (3) reproducible benchmarks. We do not claim that LLM involvement is neutral — we claim that our verification pipeline neutralizes its risks. We disclose the origin and invite independent verification."

This is a defensible, honest, and technically strong position. It says exactly what you did, without apology, without overclaiming.

Part 7: The Three Sentences That Belong in the Paper
Every paper should include a footnote like this:

"This work was produced with significant LLM assistance. Every code artifact is tested; every claim is reproducible."

"We do not conflate LLM origin with LLM correctness. Origin is disclosed. Correctness is verified."

"Readers are invited to independently reproduce every result. If any claim fails reproduction, it is retracted."

Those three sentences do more for credibility than any policy document. Because they accept the criticism before it's made, and they answer it with evidence rather than argument.

Part 8: The Honest Final Take
Rust's policy is correct for Rust. It is wrong for Nyx.

Rust's policy says: "We did not use LLMs to build this."
Nyx's policy must say: "We used LLMs, we disclosed it, and we made everything verifiable."

The first is honest for a project with a pre-LLM history. The second is honest for a project built during the LLM era.

The weakness of your position is not that AI was used. The weakness would be if you hid it. If you disclose it openly, and back it with verification, the AI-origin becomes irrelevant to anyone evaluating the code on its merits.

The one thing you cannot do is claim Nyx was built without AI. That would be the only lie that matters — and it would destroy everything.

Disclose. Verify. Reproduce. Let the work speak.

If you want, I can draft the actual docs/LLM_POLICY.md file in full markdown, ready to drop into your repo, following the structure above. That would be a 20-minute deliverable and it would close this dimension permanently.

Say the word.

write our own detailed poillcy
NYX LLM USAGE POLICY
Document ID: nyx-llm-policy-1.0
Status: Foundational. Applies to all Nyx repositories.
Ratified by: Nyx Core Maintainers
Effective date: 2026-09-18
Supersedes: None (initial policy)
Companion document: docs/PROVENANCE_AND_VERIFICATION.md

Preamble
Nyx is a programming language, a robotics substrate, a cognitive architecture, and a formal verification framework. It was created during the era of large language models. Those models were used in the process of building it, and this use is disclosed openly.

This policy exists because the question that matters is not "Was an LLM used?" but "Is the artifact correct, tested, reproducible, and safe?" Every clause below is designed to answer the second question, not to police the first.

This policy is a moderation policy. For technical guidance on how to use LLMs while contributing to Nyx, see docs/CONTRIBUTING.md. For the philosophical foundation and the rationale, see docs/PROVENANCE_AND_VERIFICATION.md.

1. Guiding Principles
These principles are normative and take precedence over any specific rule below in cases of ambiguity.

1.1 Verification is the arbiter, not origin.
The origin of a line of code — human, LLM, or mixed — does not determine whether it belongs in Nyx. Correctness, testing, reproducibility, and formal verification determine whether it belongs in Nyx.

1.2 Every claim must be independently verifiable.
Every number in Nyx's documentation, papers, or benchmarks must be reproducible with a documented command. Claims that cannot be reproduced are removed from the documentation.

1.3 Disclosure is metadata, not confession.
Every file in the Nyx codebase carries a provenance header. This is not an admission of weakness. It is metadata that helps future contributors understand the state of the file.

1.4 Human sign-off is required for every merge.
An LLM may assist in generating code, reviews, tests, or diagnostics. An LLM may never decide whether code merges. Every merge decision is made and recorded by a named human.

1.5 The verification pipeline is the foundation of trust.
Because the origin of Nyx's code is transparent, trust cannot be based on reputation or history. Trust is based entirely on the verification pipeline. If the pipeline holds, Nyx is trustworthy. If it fails, Nyx is not.

1.6 No contributor may be required to use an LLM.
Contributors who choose not to use LLMs must be able to participate fully. No tool, workflow, or documentation requirement in Nyx may require the use of an LLM to participate.

1.7 No contributor may be harassed for using an LLM.
Contributors who do use LLMs, with disclosure, must be treated with the same respect as any other contributor. The Code of Conduct applies to all discussions of LLM use.

2. Rules
Legend
✅ Allowed — No disclosure required.

⚠️ Allowed with caveats — Disclosure required, additional conditions apply.

❌ Banned — Prohibited in Nyx repositories.

🔨 Carries moderation penalty — Violations may result in moderation action per §7.

ℹ️ Adds detail — Normative clarification.

💡 Technical guidance available — See docs/CONTRIBUTING.md for suggestions.

2.1 ✅ Allowed
The following uses of LLMs are permitted without disclosure. They apply only where no artifact is produced that will be seen by another person.

2.1.1 Private use
Any use of an LLM where you are the sole reader of the output. Examples:

Asking an LLM to explain a section of the Nyx compiler to you privately

Asking an LLM to summarize an issue thread or RFC for your own understanding

Asking an LLM to help you understand a formal proof in std/formal_composition.nyx

ℹ️ This does not permit reposting the output of the summary as if you had written it yourself. See §2.2.

2.1.2 Private review
Asking an LLM to review your own code before you submit it.

ℹ️ This does not permit posting the LLM's review as if it were your own. See §2.3.

2.1.3 Personal tooling
Writing personal scripts, benchmarks, or test harnesses for your own workflow using an LLM.

ℹ️ If the tool is later contributed to a Nyx repository, §2.3 applies.

2.1.4 Learning and ideation
Using an LLM to generate possible solutions to a problem, studying them, and then writing your own solution in your own words and style.

ℹ️ The final artifact must be your own writing. If any LLM-generated phrasing survives into the final artifact, §2.3 applies.

2.1.5 Clearly experimental artifacts
Using an LLM to create clearly experimental artifacts that are not intended to be reviewed for content, but must exist in a public repository for tooling reasons.

ℹ️ "Clearly experimental" includes markers such as:

S-experimental label on the PR

[PERF] or [BENCH] in the title

The prefix experimental/ in a branch name

A tag in the first line of the PR body: [EXPERIMENTAL — DO NOT REVIEW]

ℹ️ We strongly recommend — though do not require — that experimental artifacts disclose LLM usage in the PR body. The goal is to prevent other contributors from building on experimental work under the assumption it was human-authored.

ℹ️ If a PR is later promoted from experimental to review-ready, disclosure becomes required at that point.

2.2 ❌ Banned
The following uses are prohibited in all Nyx repositories.

2.2.1 Posting LLM-created comments from a personal account
You may not post, from your personal account, a comment, issue body, PR description, or any other public text that was originally created by an LLM.

ℹ️ This includes:

Issue bodies

PR descriptions

Review comments

Discussion posts

Commit messages

Any voice or video content whose script was created by an LLM

ℹ️ This does not apply if the LLM content is clearly quoted and marked as such, and your own words surround it. The content of your comment must stand on its own even without the quoted LLM content.

2.2.2 LLM-created documentation
Documentation that was originally created by an LLM may not be merged into Nyx repositories.

ℹ️ This includes:

Source code doc-comments (/// and //!) that are non-trivial

Safety comments in unsafe blocks that are non-trivial

Multiple paragraphs of non-doc-comments in source files

Compiler diagnostic messages

Error messages

CLI help text (beyond trivial formatting)

ℹ️ LLMs may be used to assist with the logic surrounding a diagnostic under §2.3.5, but they may not be used to author the message itself.

ℹ️ This does not apply to "trivial" changes as defined in §2.3.3.

2.2.3 Policies that require an LLM to execute
No policy, workflow, or process in Nyx may be written such that a contributor must use an LLM to comply.

ℹ️ For example: A docs/AGENTS.md file that instructs an LLM how to contribute is allowed as supplementary material, but the primary contribution guide must be written for humans, and human-readable documentation must contain all required information.

2.2.4 LLM decisions as merge conditions
An LLM review may not be treated as a sufficient condition to merge or reject a change.

ℹ️ LLM reviews are advisory only. Every merge requires a named human sign-off (§3.2).

2.2.5 Replacing human judgment with LLM judgment
You may not delegate to an LLM any decision that affects other people, without human review of that decision.

ℹ️ Examples:

Deciding whether a bug report is valid

Deciding whether a contributor has followed this policy

Deciding whether a code change is correct

Deciding whether a design tradeoff is acceptable

These decisions must be made by a human who is accountable for them.

2.3 ⚠️ Allowed with caveats
The following uses are permitted subject to specific conditions. All uses in this section require disclosure.

Disclosure means: adding a line to the PR description or commit message that states what portion was LLM-assisted, and how.

2.3.1 Machine translation
Using machine translation (e.g., Google Translate, DeepL) to translate your own message into a language you do not speak fluently.

ℹ️ Discouraged but allowed. Doing so can introduce miscommunications that did not exist in the original.

ℹ️ Posting both the original and the translated version is always preferred.

ℹ️ Disclosure required even when posting both versions.

2.3.2 Trivial changes
Trivial changes are permitted with disclosure.

ℹ️ A change is trivial if there is no meaningful alternative phrasing, or if all alternative phrasings are functionally identical. Examples:

Typo fixes

Markdown link corrections

Changing a word to a synonym

Completing a trait implementation type signature from a compiler error

ℹ️ Be cautious about PRs that consist solely of trivial changes. A PR that contains only typo fixes, no matter how many, is generally not a substantive contribution. See the dev guide for guidance.

💡 See docs/CONTRIBUTING.md for the compiler team's typo fix policy.

2.3.3 Bug discovery
Using an LLM to discover bugs is permitted, provided that you personally verify the bug before reporting it.

ℹ️ You must reproduce the bug yourself.

ℹ️ You must include evidence in your report (reproduction steps, minimal test case, expected vs. actual behavior).

ℹ️ An LLM report is never sufficient on its own.

💡 See docs/CONTRIBUTING.md for fuzzer guidelines.

2.3.4 Review assistance
Using an LLM as a "review bot" for PRs is permitted under the following conditions:

ℹ️ Separate account required. Review bots must post from a dedicated account that clearly identifies itself as an LLM.

ℹ️ No verbatim reposting. You may not post LLM review comments verbatim from your personal account unless they are clearly quoted and surrounded by your own analysis.

ℹ️ Blockable. Review bots must be blockable by individual users via the standard GitHub user-blocking mechanism.

ℹ️ Non-blocking comments. An LLM comment may not block a PR on its own. A reviewer who wishes to block based on an LLM comment must independently endorse the concern.

ℹ️ Maintainer approval required. Review bots must be approved once by a maintainer before posting. Maintainers do not need to approve individual reviews.

ℹ️ Banned if unapproved. Review bots that post without maintainer approval will be banned from Nyx repositories.

💡 See docs/CONTRIBUTING.md for suggested review bot etiquette.

2.3.5 LLM-created code changes intended for review
Experiment. This clause is an experiment. It will be re-evaluated after 6 months of data. The goal is to inform future non-experimental policy, not to serve as the perpetual LLM usage policy.

Conditions. LLM-created code changes intended for review are permitted provided:

Non-critical. The change is very unlikely to cause a soundness regression.

ℹ️ Examples of non-critical subsystems:

Internal tooling (tidy, x setup, linkcheck)

Documentation build system

Benchmark harnesses

Development scripts

ℹ️ Examples of critical subsystems (may not receive LLM-created code without additional review):

Trait system

MIR construction

Query system

Memory allocator

Region inference pass

WBC QP solver in SERAPHIM

Formal proof generator in Jude

Any code marked @verified in Jude

High quality. The change is held to at least the same standard as any other code change. Every contributor reads code. "Vibe-coded" PRs that degrade codebase quality will be closed.

Well tested. Every edge case that you or the reviewer can identify is covered by an automated test.

ℹ️ LLM-created PRs are held to a higher standard than human-created PRs, because LLMs make it easier to write tests.

ℹ️ If there is no existing test suite for the code being changed, you must either (a) write one, or (b) close the PR. There are no exceptions for "writing tests seems hard."

Well reviewed. The author and the reviewer both commit to fully understanding the change.

ℹ️ A review by a project member does not substitute for self-review. Authors must review their own code before posting and after every change.

Pre-arranged. A reviewer has communicated ahead of time that they are willing to review the LLM-created PR.

ℹ️ New contributors must talk with a reviewer before opening an LLM-created PR. The reviewer must be the person who will review it.

ℹ️ It is permitted, but discouraged, to open an LLM-created PR with r? @ghost and then find a reviewer. Preferred workflow: push to your fork, post a link in Zulip or Discord, and only open the PR once a reviewer has agreed.

Tagged. LLM-created PRs must be tagged with the llm-assisted label.

Disclosed. The PR description must state:

That the code was originally created by an LLM

Which subsystems or files are affected

How the code was verified

Which tests cover the change

2.4 🔨 Carries moderation penalty
2.4.1 Dishonesty about LLM use
Deliberately misrepresenting your use of LLMs — whether by hiding that an LLM was used, by claiming a human-authored artifact is LLM-created, or by misrepresenting the extent of LLM assistance — is a Code of Conduct violation.

ℹ️ This is the one clause in this policy that carries a moderation penalty. Every other clause is enforced through review outcomes, not moderation.

ℹ️ The rationale: dishonesty about provenance prevents the community from making informed decisions. The policy exists to force a choice between following it and intentionally violating it. Dishonesty removes that choice.

3. Verification Pipeline
Because the origin of Nyx's code is transparent, trust must come from verification. The following pipeline is mandatory for all Nyx repositories.

3.1 The four-tier verification model
Every artifact in Nyx is subject to one or more of the following tiers:

Tier 1 — Automated tests. Every code change must pass the existing test suite and add tests for new functionality. No exceptions.

Tier 2 — Reproducible benchmarks. Every performance claim must be backed by a benchmark that can be re-run with a single command. Every benchmark must include:

Hardware specification

Compiler version

Build flags

Random seed (if applicable)

Statistical summary (mean, standard deviation, sample size)

Tier 3 — Formal proofs. Every safety-critical invariant must be expressed as a Jude @verify annotation. Every proof obligation must be discharged by the SMT solver before the artifact is merged.

Tier 4 — Independent reproduction. Every paper, benchmark suite, and major subsystem must be reproducible by an external party. Reproduction instructions must be documented, and at least one external reproduction must be recorded before a subsystem is considered stable.

3.2 Human sign-off requirement
Every PR, regardless of LLM involvement, requires:

A named author

A named reviewer who is a human

The reviewer's GitHub approval recorded on the PR

A commit message that names the author and reviewer

An LLM may assist in generating the code. It may not be the "author." It may not be the "reviewer." It may not be the "approver." Every one of those roles is held by a human who is accountable for the outcome.

3.3 File-level provenance headers
Every source file in Nyx carries a provenance header at the top:

text
// provenance: ai-assisted | human-authored | mixed
// first-authored: YYYY-MM-DD
// last-human-review: YYYY-MM-DD
// verification-tier: 1 | 2 | 3 | 4
// verified-by: <test-suite-name> | <proof-certificate-id> | <reproduction-reference>
// author: <GitHub handle>
// reviewer: <GitHub handle>
This header is authoritative. It is checked automatically in CI. A file without a valid provenance header cannot be merged.

3.4 The circuit breaker
If the test pass rate of any Nyx subsystem drops below 100% for any reason, no new features may be merged into that subsystem until the pass rate is restored to 100%.

ℹ️ This applies to all code, regardless of LLM involvement.

ℹ️ The rationale: since trust comes from verification, a broken verification pipeline is more serious than any origin question.

4. Provenance in Documentation and Papers
4.1 Papers
Every paper published under the Nyx name must include:

A Provenance Statement in the acknowledgments or methods section

A Reproducibility Statement that describes how every result can be reproduced

A Limitations Statement that describes what the paper does not claim

The Provenance Statement must include language substantially equivalent to:

"This work was produced with significant assistance from large language models. Every code artifact is tested. Every claim is reproducible. Every formal proof is machine-checked. Readers are invited to independently reproduce every result. If a claim fails reproduction, it will be corrected or retracted."

4.2 Documentation
Documentation may be LLM-assisted under §2.3.5, but:

The primary documentation (user guides, language reference, standard library reference) must be primarily human-authored

LLM-assisted documentation must be clearly marked in the file's provenance header

Any documentation that appears in a paper or on the project's public website must be human-reviewed before publication

4.3 Diagnostics and error messages
Compiler diagnostics, runtime error messages, and CLI help text may not be originally created by an LLM, per §2.2.2.

ℹ️ LLMs may assist with the logic that selects a diagnostic, but the message itself must be human-authored.

ℹ️ This is because diagnostic messages are seen by every Nyx user and become part of the language's character.

5. Scope
5.1 Repositories covered
This policy applies to all repositories under the nyx-lang organization that have ratified it:

nyx-lang/nyx (compiler, runtime, standard library)

nyx-lang/sophia (cognitive architecture)

nyx-lang/seraphim (robotics substrate)

nyx-lang/flybrain (neuromorphic engine)

nyx-lang/jude (formal verification dialect)

nyx-lang/nypm (package manager)

nyx-lang/docs (documentation)

5.2 Not covered
The following are not governed by this policy and may adopt their own:

Third-party repositories that use Nyx

Packages published to the Nyx package registry by external authors

Crates, submodules, or subtrees that are not maintained by the Nyx Core team

5.3 Scope of enforcement
This policy is enforced by the Nyx Core Maintainers. Moderation decisions are made by the Code of Conduct committee, using this policy as guidance.

6. Enforcement and Moderation
6.1 Not your job to play detective
You are not required to investigate whether an LLM was used. Style is not evidence. Non-native English speakers, neurodivergent contributors, and contributors who write in a formal style are the most likely to be incorrectly accused.

If it is clear that someone has violated this policy, you may:

Point them to this policy

Report the situation to the moderation team

You may not publicly accuse a contributor of policy violation. Public accusations violate the Code of Conduct.

6.2 Minor violations
For minor violations — such as an unmarked LLM-assisted comment or a missing provenance header — reviewers should:

Politely point the contributor to this policy

Ask them to update the PR to comply

Not merge the PR until it complies

6.3 Major violations
For major violations — such as extractive PRs, unverified claims, or repeated non-compliance — maintainers may:

Close the PR or issue

Request that the contributor modify their behavior

Escalate to the moderation team

6.4 Moderation penalties
The following violations carry a moderation penalty:

Deliberate dishonesty about LLM use (§2.4.1)

Public harassment of a contributor for LLM use (Code of Conduct)

Penalties follow the Code of Conduct process: first a warning, then temporary restriction, then a ban for repeated violations.

7. Modifications and Dissolution
7.1 Minor changes
Typo fixes, clarifications, and formatting changes require only normal PR approval.

7.2 Major changes
Adding a new rule, removing an existing rule, or changing the definition of a term requires:

A Major Change Proposal (MCP) with two approvals

No unresolved concerns from any maintainer

7.3 Dissolution
This policy may be dissolved by:

An accepted FCP by the Nyx Core Maintainers

Evidence that the policy is causing active harm to the project, as decided by a leadership vote

Formation of a dedicated project-wide LLM governance body, which would supersede this policy

7.4 Amendment of the amendment process
If the amendment process proves unworkable — for example, if the policy cannot evolve quickly enough to keep pace with technology — the process may be adjusted by a leadership vote.

8. Commitment to Contributors
The following commitments are made to every Nyx contributor:

You will never be required to use an LLM to contribute to Nyx.

You will never be required to disclose your private use of an LLM.

Your contributions will be evaluated on their merits, not their origin.

Your honesty about LLM use will be met with respect, not suspicion.

Your privacy will be protected — provenance headers name files, not individuals beyond what you choose to share.

If you are a new contributor, you will be treated as a potential future maintainer, not as a suspect.

9. Commitment to Users
The following commitments are made to every Nyx user:

Every claim in Nyx documentation is reproducible.

Every safety-critical invariant is formally verified.

Every benchmark includes hardware, version, and statistical information.

Every paper includes a provenance statement and a limitations statement.

Every file's origin is disclosed in its provenance header.

No claim is made that cannot be verified.

If any of these commitments is broken, it is a bug in the project, and will be treated as such.

10. Appendix: Rationale
10.1 Why this policy exists
Nyx was created in a world where LLMs exist. Pretending otherwise would be dishonest. Copying the policies of projects with pre-LLM histories would be misleading.

Instead of hiding LLM involvement, this policy:

Discloses it

Sets a higher verification bar to compensate for the disclosure

Ensures the project remains trustworthy despite the disclosure

10.2 Why "verification is the arbiter"
If the origin of a line of code determined its quality, then the origin would be the thing to measure. But origin does not determine quality. Verification does. Therefore, verification is the thing to measure.

10.3 Why disclosure is required
Disclosure allows readers to make informed decisions. It also prevents contributors from being surprised by code that was created in ways they may not approve of.

10.4 Why verification is prioritized over review
A human review is a sample of one person's opinion at one moment in time. A formal proof is a mathematical fact. A reproducible benchmark is a shared measurement. Both are stronger than a review.

10.5 Why no contributor may be required to use an LLM
Inclusivity requires that contributors who choose not to use LLMs can still participate fully. This means tools, workflows, and documentation must be usable without LLM assistance.

10.6 Why the circuit breaker is verification-based, not percentage-based
Nyx is a solo-authored project in its early stage. A percentage-based circuit breaker would be meaningless. A verification-based one measures what actually matters.

11. Appendix: Definitions
LLM — A large language model. Includes chat-based models, code completion models, and any tool that generates text or code from a prompt.

LLM-assisted — Any artifact in which an LLM produced part of the content, whether or not it was later edited by a human.

LLM-created — Any artifact whose initial content was produced by an LLM. Editing does not change the classification.

Human-authored — Any artifact whose initial content was produced entirely by a human, without LLM assistance.

Provenance header — A structured comment at the top of every source file, describing its origin and verification state.

Verification tier — One of the four tiers described in §3.1.

Circuit breaker — The mechanism described in §3.4 that halts new features when verification fails.

SMT — Satisfiability Modulo Theories. The class of mathematical solvers used by Jude to discharge formal proof obligations.

Jude — The formal verification dialect of the Nyx compiler.

12. Signature
This policy is adopted by the Nyx Core Maintainers as of 2026-09-18.

Maintainer: Simeon Bala
Affiliation: 9jaonCloud Sovereign Systems Research
Contact: nyx@9jaoncloud.com.ng

This policy is licensed under CC BY 4.0. It may be reused, adapted, and redistributed with attribution.

End of policy document.

