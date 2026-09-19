// Nyx Docs - Sidebar Navigation (base-path aware & clean URL resolution)
(function() {
    const nav = document.getElementById('sidebar');
    if (!nav) return;

    // Compute relative paths based on URL depth dynamically
    const pathname = window.location.pathname.replace(/\\/g, '/');
    const lastSlash = pathname.lastIndexOf('/');
    const dirPath = lastSlash !== -1 ? pathname.substring(0, lastSlash) : '';
    const segments = dirPath.split('/').filter(Boolean);
    
    const docsIdx = segments.indexOf('docs');
    let baseDocs = '';
    let baseRoot = '../';
    
    if (docsIdx !== -1) {
        const depthAfterDocs = (segments.length - 1) - docsIdx;
        baseDocs = depthAfterDocs > 0 ? '../'.repeat(depthAfterDocs) : '';
        baseRoot = '../'.repeat(depthAfterDocs + 1);
    } else {
        const depth = segments.length;
        baseDocs = (depth > 0 ? '../'.repeat(depth) : '') + 'docs/';
        baseRoot = depth > 0 ? '../'.repeat(depth) : '';
    }

    function rel(href) {
        if (!href) return '#';
        if (href.startsWith('http://') || href.startsWith('https://') || href.startsWith('mailto:') || href.startsWith('//')) {
            return href;
        }
        if (href.startsWith('../')) {
            return baseRoot + href.substring(3);
        }
        return baseDocs + href;
    }

    var sections = [
        {
            heading: 'Getting Started',
            links: [
                { href: '../index.html', label: '🌙 Main Portal Home' },
                { href: 'playground.html', label: '⚡ Interactive Compiler Playground' },
                { href: 'index.html', label: 'Documentation Home' },
                { href: '../mobile.html', label: '📱 Sovereign Mobile SDK (120Hz)' },
                { href: 'features/mobile_frontier.html', label: '📱 Mobile Frontier Architecture & Roadmap' },
                { href: 'learn/index.html', label: '🎓 Interactive Academy (27 Modules)' },
                { href: 'roadmap.html#milestone-16', label: '🚀 v0.30 3D Mobility & Sovereign i18n' },
                { href: 'roadmap.html#milestone-13', label: '🌐 v0.29 Ecosystem & Packages Archive' },
                { href: '../comparison.html', label: '🏆 2026 Comparison Matrix' },
                { href: 'ecosystem.html', label: '🌐 Nyx Ecosystem Directory' },
                { href: 'language/introduction.html', label: 'Introduction' },
                { href: 'setup/installation.html', label: 'Installation' },
                { href: 'setup/cli-guide.html', label: '💻 CLI Setup & Tutorial' },
                { href: 'setup/quickstart.html', label: 'Quick Start' },
                { href: 'setup/build-from-source.html', label: 'Build from Source' },
            ]
        },
        {
            heading: 'Language Guide',
            links: [
                { href: 'language/types.html', label: 'Types & Data' },
                { href: 'language/functions.html', label: 'Functions' },
                { href: 'language/control-flow.html', label: 'Control Flow' },
                { href: 'language/modules.html', label: 'Modules & Imports' },
                { href: 'language/memory.html', label: 'Memory Model' },
                { href: 'language/concurrency.html', label: 'Concurrency' },
                { href: 'language/grammar.html', label: 'Grammar Reference' },
            ]
        },
        {
            heading: 'Multi-Dialect Extensions & AGI (Upgrade 2)',
            links: [
                { href: 'extensions/index.html', label: '🌐 Multi-Dialect Overview' },
                { href: 'extensions/zephaniah.html', label: '⚡ Zephaniah (.zeph - Gradual ARC)' },
                { href: 'extensions/havilah.html', label: '🛡️ Havilah (.hav - Linear Caps)' },
                { href: 'extensions/jude.html', label: '📐 Jude (.jude - SMT Proofs)' },
                { href: 'extensions/seraphim.html', label: '🤖 SERAPHIM (1 kHz Robotics)' },
                { href: 'extensions/sophia-agi.html', label: '🧠 Nyx SOPHIA (7-Layer AGI)' },
            ]
        },
        {
            heading: 'Tutorials & Interactive Academy',
            links: [
                { href: 'learn/index.html', label: '🎓 Interactive Academy (27 Modules)' },
                { href: '../hausa/index.html', label: '🇳🇬 Nyx a Harshen Hausa' },
                { href: '../yoruba/index.html', label: '🇳🇬 Èdè Nyx ni Yorùbá' },
                { href: '../igbo/index.html', label: '🇳🇬 Asụsụ Nyx n\'Igbo' },
                { href: 'gaming.html', label: '🎮 Gaming Academy (120 FPS)' },
                { href: 'tutorial/getting-started.html', label: 'Getting Started' },
                { href: 'tutorial/concurrency-and-safety-guide.html', label: '🚀 Concurrency & Safety Practical Guide' },
                { href: 'tutorial/building-calculator.html', label: '🧮 Building a Zero-GC Calculator' },
                { href: 'tutorial/building-snake-arcade.html', label: '🐍 Building a 60 FPS Snake Arcade' },
                { href: 'tutorial/building-whot-card-game.html', label: '🃏 Building Nigerian Whot Card Game' },
                { href: 'tutorial/building-sovereign-browser.html', label: '🌐 Building Sovereign Sandbox Browser' },
                { href: '../projects/material-ui/index.php', label: '🎨 Material 3 & QML Live Studio' },
                { href: 'tutorial/building-a-web-app.html', label: 'Building a Web App' },
                { href: 'tutorial/building-a-desktop-app.html', label: 'Building a Desktop App' },
                { href: 'tutorial/building-a-mobile-app.html', label: 'Building a Mobile App' },
            ]
        },
        {
            heading: 'Standard Library',
            links: [
                { href: 'stdlib/index.html', label: 'Overview' },
                { href: 'stdlib/search.html', label: '🔍 @nyx/search (Full-Text)' },
                { href: 'stdlib/serde.html', label: '📦 @nyx/serde (Serialization)' },
                { href: 'stdlib/ffi.html', label: '🌉 @nyx/ffi (Python/Node Bridge)' },
                { href: 'stdlib/embassy.html', label: '🔌 @nyx/embassy (Embedded HAL)' },
                { href: 'stdlib/mesh.html', label: '☁️ @nyx/mesh (K8s & Mesh)' },
                { href: 'stdlib/fuzz.html', label: '🧪 @nyx/fuzz (Property Fuzzing)' },
                { href: 'stdlib/crew.html', label: '🤖 @nyx/crew (Multi-Agent Swarm)' },
                { href: 'stdlib/calc.html', label: '📊 std.office.calc (Spreadsheet & AI)' },
                { href: 'stdlib/ai.html', label: '🧠 Nyx AI (FreeToken 700B MoE)' },
                { href: 'stdlib/trade.html', label: '📈 Nyx Trade (Pine Script v5 & HFT)' },
                { href: 'stdlib/sec.html', label: 'std.sec (Security)' },
                { href: 'stdlib/gis.html', label: 'std.gis (Geospatial)' },
                { href: 'stdlib/cloud.html', label: 'std.cloud (Cloud S3/RPC)' },
                { href: 'stdlib/ui.html', label: 'std.ui (Material UI)' },
                { href: 'stdlib/ml.html', label: 'std.ml (AI & Tensors)' },
                { href: 'stdlib/robotics.html', label: 'std.robotics (Kinematics)' },
                { href: 'stdlib/blockchain.html', label: 'std.blockchain (Ledger)' },
                { href: 'stdlib/audio.html', label: 'std.audio (DSP & Whisper)' },
                { href: 'ecosystem.html#domain-identity', label: 'std.identity (SD-JWT / Passports)' },
                { href: 'ecosystem.html#domain-industrial', label: 'std.industrial (SCADA / GOOSE)' },
                { href: 'ecosystem.html#domain-telecom', label: 'std.telecom (AIS / ADS-B / CAP)' },
                { href: 'ecosystem.html#domain-commerce', label: 'std.commerce (Beckn / UPI)' },
            ]
        },
        {
            heading: 'Language Comparisons & Deep-Dives',
            links: [
                { href: 'comparisons/index.html', label: '📐 Memory Trilemma & Hub' },
                { href: 'comparisons/nyx-vs-verona.html', label: '🔬 Nyx vs Microsoft Verona' },
                { href: 'comparisons/nyx-vs-rust.html', label: '🦀 Nyx vs Rust' },
                { href: 'comparisons/nyx-vs-cyclone.html', label: '🌀 Nyx vs Cyclone' },
                { href: 'comparisons/nyx-vs-c-cpp.html', label: '⚡ Nyx vs C & C++' },
                { href: 'comparisons/nyx-vs-go-java.html', label: '☕ Nyx vs Go & Java' },
            ]
        },
        {
            heading: 'About Nyx & Research',
            links: [
                { href: 'roadmap.html', label: '🗺️ Engineering Roadmap' },
                { href: 'connectome.html', label: '🧠 Bio-Neuromorphic Connectome Architecture' },
                { href: 'aero-mobility.html', label: '🚀 Hyperloop & eVTOL 3D Mobility' },
                { href: 'binary-armor.html', label: '🛡️ Binary Armor & Hardware PUF' },
                { href: 'side-ecosystem.html', label: '🛡️ SIDE Defense Ecosystem (GhostMesh & Vanguard)' },
                { href: 'safety-and-security.html', label: '🛡️ Formal Safety & Security Spec' },
                { href: 'industry-benchmarks.html', label: '🏆 5 Industry-Standard Suites' },
                { href: 'ai-benchmarks.html', label: '🤖 Sovereign AI & LLM Benchmarks' },
                { href: 'academic-benchmarks.html', label: '🔬 Academic Benchmarks (Peer Review)' },
                { href: 'benchmarks.html', label: '📊 Empirical Benchmarks (15 Langs)' },
                { href: 'benchmark-code.html', label: '💻 Benchmark Source Code (15 Langs)' },
                { href: 'benchmark-methodology.html', label: '🔬 Benchmark Methodology (B1-B15)' },
                { href: 'what-can-i-build.html', label: '🚀 What Can I Build in Nyx?' },
                { href: 'game-engine.html', label: '🎮 Universal Game Engine (Zero-GC)' },
                { href: 'spatial-vr.html', label: '🥽 Spatial VR & AR Engine (OpenXR)' },
                { href: 'nyxos-vs-omarchy.html', label: '⚔️ NyxOS vs. Omarchy (Comparison)' },
                { href: 'smart-board.html', label: '📊 Smart Board AI Engine' },
                { href: 'side-ecosystem.html', label: '🏛️ Sovereign SIDE Ecosystem' },
                { href: 'industry-benchmarks.html', label: '🏢 Industry AI Benchmarks' },
                { href: 'why-nyx.html', label: '🏆 Why Nyx? (Comparison)' },
                { href: 'memory-safety.html', label: '🛡️ Memory Safety Guarantees' },
                { href: 'paper/nyx-provenance-and-verification.html', label: '📜 LLM Provenance & Verification Paper' },
                { href: 'paper/index.html', label: '📄 Academic Research Papers' },
                { href: 'security.html', label: 'Security & Threat Model' },
                { href: 'security-audit.html', label: '🛡️ Adversarial Security & Penetration Audit' },
                { href: 'features/algebraic_math.html', label: 'Algebraic Math & Systems' },
                { href: 'features/formal_verification.html', label: '📐 Formal Verification & Registry' },
                { href: 'assessment.html', label: 'Honest Assessment' },
                { href: 'response-to-assessment.html', label: 'Designers\' Response' },
            ]
        },
        {
            heading: 'Compiler Internals',
            links: [
                { href: 'internals/compiler-pipeline.html', label: 'Compiler Pipeline' },
                { href: 'internals/region-inference.html', label: 'Region Inference' },
                { href: 'internals/mlir-dialects.html', label: 'MLIR Dialects' },
                { href: 'internals/llvm-lto-pgo.html', label: 'LLVM 18 LTO & PGO' },
                { href: 'internals/runtime-engine.html', label: 'Runtime Engine' },
                { href: 'internals/dap-profiler.html', label: 'DAP & Time-Travel Profiler' },
                { href: 'internals/tooling-debugging.html', label: 'Tooling & Debugging' },
                { href: 'escape-playground.html', label: '⚡ Escape Analysis Playground' },
            ]
        },
        {
            heading: 'Developer Tools & Simulations',
            links: [
                { href: 'nyxos-download.html', label: '📦 Download SDK (Win / Linux / Mac)' },
                { href: '../athena/spatial-desktop.html', label: '🌌 Cybernetic Spatial AI OS (120 FPS)' },
                { href: '../athena/spatial-hud.html', label: '🌌 Athena 3D Spatial HUD Sandbox' },
                { href: 'spatial-ui.html', label: '🌌 Spatial UI Architecture Spec' },
                { href: 'gpu-systems.html', label: '⚡ GPU Systems Subsystem & Runtime' },
                { href: 'connectome.html', label: '🧠 Bio-Neuromorphic Connectome Spec' },
                { href: 'bio-computing.html', label: '🧬 Bio-Computing & Organisms Portal' },
                { href: 'paper/papers.html', label: '📚 Master Research Papers (10 Papers)' },
                { href: '../projects/openworm-sim/index.php', label: '🐛 OpenWorm C. elegans Sim (120 FPS)' },
                { href: '../projects/hexapod-arena/index.php', label: '🕷️ Hexapod Bio-Robotics Arena (120 FPS)' },
                { href: '../projects/flybrain-chess/index.php', label: '♟️ FlyBrain Chess Grandmaster (164K)' },
                { href: '../projects/flybrain-pong/index.php', label: '🪰 FlyBrain Connectome Pong (120 FPS)' },
                { href: '../projects/ios-vm-studio/index.php', label: '🍎 iOS Virtualization Studio (@nyx/vz)' },
                { href: '../projects/neurotrader/index.php', label: '📈 Sovereign NeuroTrader LSM Studio' },
                { href: '../projects/nyx-game-engine/index.php', label: '🎮 Nyx Game Engine & Simulator' },
                { href: '../projects/nyx-ai-engine/index.php', label: '🤖 Sovereign AI Engine Console' },
                { href: '../athena/jarvis.html', label: '🎙️ Athena Jarvis Voice & Vision' },
                { href: '../packages.html', label: '📚 @nyx/* Package Registry (45 Packages)' },
                { href: '../projects/aero-simulator/index.php', label: '✈️ 3D Mobility & Flight Studio' },
                { href: '../projects/fusion-simulator/index.php', label: '⚛️ Fusion Plasma Simulator' },
                { href: '../projects/spatial-vr/index.php', label: '🥽 Spatial 3D / WebXR Studio' },
                { href: '../projects/dna-codec/index.php', label: '🧬 DNA Codec & Bio Engine' },
                { href: '../projects/binary-armor/index.php', label: '🛡️ Binary Armor Live Studio' },
                { href: '../projects/agentic-os/index.php', label: '🖥️ NyxOS Agentic Simulator' },
                { href: '../projects/smart-board/index.php', label: '📐 Universal Smart Board Demo' },
                { href: '../projects/index.php', label: '🌐 All 40+ Interactive Simulations' },
            ]
        },
        {
            heading: 'Governance & Policies',
            links: [
                { href: 'llm-policy.html', label: '🤖 LLM Usage & Provenance Policy' },
                { href: 'legal/terms-and-disclaimer.html', label: '⚖️ Terms & Disclaimer' },
                { href: 'security.html', label: '🛡️ Security Policy' },
                { href: 'security-audit.html', label: '🛡️ Penetration & Security Audit (2026)' },
            ]
        }
    ];

    var html = '<div class="logo"><a href="' + rel('index.html') + '">N<span class="accent">y</span>x <span class="accent">Docs</span></a></div>';

    var cleanCurrent = pathname.split('#')[0].split('?')[0];
    var isCurrentDocHome = (cleanCurrent.endsWith('/docs/') || cleanCurrent.endsWith('/docs/index.html') || cleanCurrent.endsWith('/docs'));

    for (var si = 0; si < sections.length; si++) {
        var section = sections[si];
        html += '<div class="nav-section">';
        html += '<h3>' + section.heading + '</h3>';
        for (var li = 0; li < section.links.length; li++) {
            var link = section.links[li];
            var href = rel(link.href);
            var isActive = false;
            if (link.href === 'index.html') {
                isActive = isCurrentDocHome;
            } else if (!link.href.startsWith('../')) {
                isActive = cleanCurrent.endsWith('/' + link.href) || cleanCurrent.endsWith(link.href);
            }
            html += '<a href="' + href + '" class="' + (isActive ? 'active' : '') + '">' + link.label + '</a>';
        }
        html += '</div>';
    }

    nav.innerHTML = html;

    // Ensure mobile overlay exists
    var overlay = document.querySelector('.mobile-overlay');
    if (!overlay) {
        overlay = document.createElement('div');
        overlay.className = 'mobile-overlay';
        document.body.appendChild(overlay);
    }

    // Ensure mobile toggle button exists
    var toggleBtn = document.getElementById('mobile-toggle');
    if (!toggleBtn) {
        toggleBtn = document.createElement('button');
        toggleBtn.id = 'mobile-toggle';
        toggleBtn.className = 'mobile-toggle';
        toggleBtn.innerHTML = '&#9776; Menu';
        toggleBtn.setAttribute('aria-label', 'Toggle Navigation Menu');
        document.body.insertBefore(toggleBtn, document.body.firstChild);
    }

    function toggleMenu(open) {
        if (typeof open === 'boolean') {
            if (open) {
                nav.classList.add('open');
                overlay.classList.add('active');
            } else {
                nav.classList.remove('open');
                overlay.classList.remove('active');
            }
        } else {
            nav.classList.toggle('open');
            overlay.classList.toggle('active');
        }
    }

    toggleBtn.addEventListener('click', function(e) {
        e.stopPropagation();
        toggleMenu();
    });

    overlay.addEventListener('click', function() {
        toggleMenu(false);
    });

    nav.querySelectorAll('a').forEach(function(link) {
        link.addEventListener('click', function() {
            if (window.innerWidth <= 860) {
                toggleMenu(false);
            }
        });
    });
})();
