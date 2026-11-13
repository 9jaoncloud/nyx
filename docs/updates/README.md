# Update Issue Logs (OFFLINE — not deployed)

**These documents are internal engineering logs and are NOT served or uploaded.**
They capture, for every update, the issues we hit and exactly how we fixed them
(symptom → root cause → fix → files → verification), so the next engineer/AI never re-debugs them.

## Convention
- One Markdown file per update: `YYYY-MM-DD-<slug>.md`.
- Each file has: **Summary**, **Issues** (numbered), **Acceptance matrix**, **Compiler/runtime status**, **Traps**.
- Keep them here, in `docs/updates/`. They are covered by `.ftpignore` + `.no_upload` and must not
  be published to the site or any FTP target.

## Index
| Date | Update | File |
|---|---|---|
| 2026-11-07 | `std/net` hardening + compiler/runtime bug fixes | `2026-11-07-std-net-and-compiler-hardening.md` |
