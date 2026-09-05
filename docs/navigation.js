// Nyx Docs - Sidebar Navigation (base-path aware & clean URL resolution)
(function() {
    const nav = document.getElementById('sidebar');
    if (!nav) return;

    // Detect the base path of the docs directory from script src
    var script = document.currentScript || (function() {
        var scripts = document.getElementsByTagName('script');
        return scripts[scripts.length - 1];
    })();
    var scriptPath = (script && script.src) ? script.src : '';
    var basePath = '.';
    
    if (scriptPath && scriptPath.indexOf('/') !== -1) {
        basePath = scriptPath.substring(0, scriptPath.lastIndexOf('/'));
    }

    var currentPath = window.location.pathname.replace(/\/+$/, '');

    function rel(href) {
        if (!basePath || basePath === '.') return href;
        var cleanBase = basePath.replace(/\/+$/, '');
        var cleanHref = href.replace(/^\/+/, '');
        return cleanBase + '/' + cleanHref;
    }

    var sections = [
        {
            heading: 'Getting Started',
            links: [
                { href: '../index.html', label: '🌙 Main Portal Home' },
                { href: 'index.html', label: 'Documentation Home' },
                { href: '../mobile.html', label: '📱 Sovereign Mobile SDK (120Hz)' },
                { href: 'features/mobile_frontier.html', label: '📱 Mobile Frontier Architecture & Roadmap' },
                { href: 'learn/index.html', label: '🎓 Interactive Academy (27 Modules)' },
                { href: 'roadmap.html#milestone-11', label: '🌐 v0.28 Web & Microservices' },
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
            heading: 'Tutorials & Interactive Academy',
            links: [
                { href: 'learn/index.html', label: '🎓 Interactive Academy (27 Modules)' },
                { href: 'tutorial/getting-started.html', label: 'Getting Started' },
                { href: 'tutorial/concurrency-and-safety-guide.html', label: '🚀 Concurrency & Safety Practical Guide' },
                { href: 'tutorial/building-calculator.html', label: '🧮 Building a Zero-GC Calculator' },
                { href: 'tutorial/building-snake-arcade.html', label: '🐍 Building a 60 FPS Snake Arcade' },
                { href: 'tutorial/building-whot-card-game.html', label: '🃏 Building Nigerian Whot Card Game' },
                { href: 'tutorial/building-sovereign-browser.html', label: '🌐 Building Sovereign Sandbox Browser' },
                { href: 'tutorial/building-a-web-app.html', label: 'Building a Web App' },
                { href: 'tutorial/building-a-desktop-app.html', label: 'Building a Desktop App' },
                { href: 'tutorial/building-a-mobile-app.html', label: 'Building a Mobile App' },
            ]
        },
        {
            heading: 'Standard Library',
            links: [
                { href: 'stdlib/index.html', label: 'Overview' },
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
            heading: 'About Nyx & Research',
            links: [
                { href: 'roadmap.html', label: '🗺️ Engineering Roadmap' },
                { href: '../rss.xml', label: '📡 RSS Technical Feed' },
                { href: 'what-can-i-build.html', label: '🚀 What Can I Build in Nyx?' },
                { href: 'why-nyx.html', label: '🏆 Why Nyx? (Comparison)' },
                { href: 'benchmarks.html', label: '📊 Empirical Benchmarks (15 Langs)' },
                { href: 'benchmark-code.html', label: '💻 Benchmark Source Code (15 Langs)' },
                { href: 'benchmark-methodology.html', label: '🔬 Benchmark Methodology (B1-B15)' },
                { href: 'memory-safety.html', label: '🛡️ Memory Safety Guarantees' },
                { href: 'paper/index.html', label: '📄 Academic Research Papers' },
                { href: 'security.html', label: 'Security & Threat Model' },
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
            heading: 'Developer Tools & Packages',
            links: [
                { href: 'nyxos-download.html', label: '📦 Download SDK (Win / Linux / Mac)' },
                { href: '../packages.html', label: '📚 @nyx/* Package Registry' },
                { href: '../projects/apk-inspector/index.php', label: '🤖 APK Inspector Tool' },
            ]
        },
        {
            heading: 'Governance & Policies',
            links: [
                { href: 'legal/terms-and-disclaimer.html', label: '⚖️ Terms & Disclaimer' },
                { href: 'security.html', label: '🛡️ Security Policy' },
            ]
        }
    ];

    var html = '<div class="logo"><a href="' + rel('index.html') + '">N<span class="accent">y</span>x <span class="accent">Docs</span></a></div>';

    for (var si = 0; si < sections.length; si++) {
        var section = sections[si];
        html += '<div class="nav-section">';
        html += '<h3>' + section.heading + '</h3>';
        for (var li = 0; li < section.links.length; li++) {
            var link = section.links[li];
            var href = rel(link.href);
            var isActive = currentPath.indexOf(link.href) !== -1;
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
