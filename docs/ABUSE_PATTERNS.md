# Living Abuse Patterns & Defender Blueprint

**Classification:** Blue Team Defense Catalog & Threat Telemetry Index  
**Maintained by:** Nyx Sovereign Security Working Group

This document catalogues real-world patterns in which modern systems programming features (across Rust, C, Go, and Nyx) are abused by threat actors, alongside concrete detection heuristics, network signatures, and Nyx-native mitigations.

---

## Pattern 1: Cloud & Git-as-C2 (e.g. APT36 / RUSTYSHADE)

### Description
A binary uses standard HTTPS clients (`std.http` / `reqwest`) to communicate with private GitHub/GitLab repositories or cloud storage endpoints, treating commit history, issue comments, or file uploads as a covert bidirectional Command & Control (C2) channel.

### Observed Indicators & Telemetry
- Outbound TLS beacons to `api.github.com/repos/*/contents/*` on fixed intervals (e.g., 30s to 120s jitter).
- Base64 or AES-GCM encrypted payload buffers stored within seemingly benign file names (`config.dat`, `telemetry.bin`).
- Specific custom HTTP User-Agent strings or missing browser standard headers.

### Defender Mitigations & Nyx Controls
1. **Network Layer:** Implement egress filtering and inspect API token scopes.
2. **SBOM Auditing:** Use `nyx-verify-sbom` to detect whether an offline binary imported `std.net` or `std.http`.
3. **Kernel Sandboxing:** Run untrusted worker processes under `nyx-sandbox` with:
   ```toml
   [network]
   allow_hosts = [] # Completely disables outbound socket creation
   ```

---

## Pattern 2: Removable Media & USB Auto-Propagation

### Description
A binary registers OS volume notifications or polls filesystem roots (`E:\`, `/media/*`) to copy self-contained executables and malicious LNK shortcut files onto newly connected USB drives.

### Observed Indicators & Telemetry
- Rapid succession of `FindFirstFile` / `read_dir` calls immediately following device arrival events.
- Generation of hidden `.lnk` or companion payloads in root volume paths.

### Defender Mitigations & Nyx Controls
1. **Host Policy:** Enforce GPO / udev rules disabling execution from removable media.
2. **Capability Sandbox:** Constrain filesystem access to explicit application working directories:
   ```toml
   [filesystem]
   read = ["/app/data/"]
   write = ["/app/logs/"]
   deny = ["/media/", "D:\\", "E:\\", "F:\\"]
   ```

---

## Pattern 3: Covert Screen Capture & Sensor Exfiltration

### Description
A stealth agent periodically captures desktop framebuffer data, compresses it, encrypts the buffer in memory, and posts the exfiltration payload to an external endpoint.

### Observed Indicators & Telemetry
- Periodic invocation of GDI / Wayland / Quartz screen scraping APIs without an interactive UI window.
- Spike in memory usage followed by immediate outbound network transmission.

### Defender Mitigations & Nyx Controls
1. **OS Permissions:** Enforce modern OS screen recording authorization gates (macOS TCC, Windows AppContainer capabilities).
2. **Havilah Linear Tokens:** In Nyx systems written in Havilah, screen capture requires an explicit non-forgeable `CAP_SENSORY` token passed from the trusted host supervisor.

---

## Pattern 4: Typo-Squatted Supply Chain Packages

### Description
An attacker publishes a malicious library with a name closely resembling a popular package (e.g., `@nyx-httpp` instead of `@nyx/http`), containing hidden post-install scripts or credential scrapers.

### Defender Mitigations & Nyx Controls
1. **Namespace Isolation:** The official `@nyx/` namespace is strictly signed and reserved for the sovereign core team.
2. **Deterministic Lockfiles:** `nyx.lock` records SHA-256 content hashes, preventing registry payload swapping.
3. **Formal Invariant Certificates:** Require third-party packages to provide `.jude.cert` proof certificates before allowing enterprise ingestion.
