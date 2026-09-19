// Nyx Responsive Global Header & Footer Component
(function() {
    function injectGlobalHeaderAndFooter() {
        // Compute relative paths based on URL depth dynamically
        const pathname = window.location.pathname.replace(/\\/g, '/');
        const lastSlash = pathname.lastIndexOf('/');
        const dirPath = lastSlash !== -1 ? pathname.substring(0, lastSlash) : '';
        const segments = dirPath.split('/').filter(Boolean);
        
        const docsIdx = segments.indexOf('docs');
        let baseDocs = '';
        let baseRoot = '../';

        if (docsIdx !== -1) {
            // Currently within the docs directory or any subfolder of docs (e.g. /docs/extensions, /docs/paper)
            const depthAfterDocs = (segments.length - 1) - docsIdx;
            baseDocs = depthAfterDocs > 0 ? '../'.repeat(depthAfterDocs) : '';
            baseRoot = '../'.repeat(depthAfterDocs + 1);
        } else {
            // Not in docs (e.g. root /, /hausa, /projects/...)
            const depth = segments.length;
            baseDocs = (depth > 0 ? '../'.repeat(depth) : '') + 'docs/';
            baseRoot = depth > 0 ? '../'.repeat(depth) : '';
        }

        // --- 1. TOP NAVBAR ---
        if (!document.getElementById('nyx-global-nav')) {
            const nav = document.createElement('header');
            nav.id = 'nyx-global-nav';
            nav.innerHTML = `
                <style>
                    #nyx-global-nav {
                        position: sticky;
                        top: 0;
                        left: 0;
                        right: 0;
                        z-index: 1000;
                        background: rgba(6, 9, 19, 0.95);
                        backdrop-filter: blur(16px);
                        border-bottom: 1px solid rgba(255, 255, 255, 0.1);
                        padding: 0.75rem 1.5rem;
                        display: flex;
                        align-items: center;
                        justify-content: space-between;
                        font-family: 'Inter', sans-serif;
                    }
                    .nyx-nav-brand {
                        display: flex;
                        align-items: center;
                        gap: 0.6rem;
                        text-decoration: none;
                        color: #f8fafc;
                        font-family: 'Outfit', sans-serif;
                        font-weight: 800;
                        font-size: 1.15rem;
                    }
                    .nyx-nav-links {
                        display: flex;
                        align-items: center;
                        gap: 1.1rem;
                    }
                    .nyx-nav-link {
                        color: #cbd5e1;
                        text-decoration: none;
                        font-size: 0.88rem;
                        font-weight: 500;
                        transition: color 0.2s;
                    }
                    .nyx-nav-link:hover, .nyx-nav-link.active {
                        color: #38bdf8;
                    }
                    .nyx-btn-play {
                        background: linear-gradient(135deg, #10b981, #059669);
                        color: #fff !important;
                        padding: 0.4rem 0.9rem;
                        border-radius: 6px;
                        font-weight: 700;
                        font-size: 0.85rem;
                        display: inline-flex;
                        align-items: center;
                        gap: 0.4rem;
                        text-decoration: none;
                    }
                    .nyx-btn-side {
                        background: linear-gradient(135deg, #f43f5e, #be123c);
                        color: #fff !important;
                        padding: 0.4rem 0.85rem;
                        border-radius: 6px;
                        font-weight: 700;
                        font-size: 0.85rem;
                        display: inline-flex;
                        align-items: center;
                        gap: 0.4rem;
                        text-decoration: none;
                    }
                    .nyx-hamburger-btn {
                        display: none;
                        background: none;
                        border: 1px solid rgba(255,255,255,0.2);
                        border-radius: 6px;
                        color: #f8fafc;
                        padding: 0.4rem 0.65rem;
                        font-size: 1.1rem;
                        cursor: pointer;
                    }
                    .nyx-mobile-drawer {
                        display: none;
                        position: fixed;
                        top: 57px;
                        left: 0;
                        right: 0;
                        bottom: 0;
                        background: #060913;
                        z-index: 999;
                        overflow-y: auto;
                        padding: 1.5rem;
                        border-top: 1px solid rgba(255,255,255,0.1);
                    }
                    .nyx-mobile-drawer.open {
                        display: block;
                    }
                    .nyx-drawer-section {
                        margin-bottom: 1.5rem;
                    }
                    .nyx-drawer-heading {
                        font-family: 'Outfit', sans-serif;
                        font-size: 0.8rem;
                        font-weight: 700;
                        text-transform: uppercase;
                        letter-spacing: 0.08em;
                        color: #38bdf8;
                        margin-bottom: 0.6rem;
                        border-bottom: 1px solid rgba(255,255,255,0.06);
                        padding-bottom: 0.3rem;
                    }
                    .nyx-drawer-link {
                        display: block;
                        padding: 0.45rem 0;
                        color: #cbd5e1;
                        text-decoration: none;
                        font-size: 0.92rem;
                    }
                    .nyx-drawer-link:hover {
                        color: #38bdf8;
                    }
                    @media (max-width: 992px) {
                        .nyx-nav-links { display: none; }
                        .nyx-hamburger-btn { display: block; }
                    }
                </style>

                <a href="${baseRoot}index.html" class="nyx-nav-brand">
                    <span>🌌</span> Nyx Research &amp; Systems
                </a>

                <div class="nyx-nav-links">
                    <a href="${baseDocs}playground.html" class="nyx-btn-play"><i class="fas fa-play"></i> Playground</a>
                    <a href="${baseDocs}spatial-ui.html" class="nyx-nav-link" style="color:#00f0ff; font-weight:700;"><i class="fas fa-vr-cardboard"></i> Spatial UI</a>
                    <a href="${baseRoot}athena/spatial-desktop.html" class="nyx-nav-link" style="color:#00f0ff; font-weight:700;"><i class="fas fa-desktop"></i> Spatial OS</a>
                    <a href="${baseDocs}paper/papers.html" class="nyx-nav-link" style="color:#818cf8; font-weight:700;"><i class="fas fa-graduation-cap"></i> Papers</a>
                    <a href="${baseDocs}binary-armor.html" class="nyx-nav-link" style="color:#38bdf8; font-weight:700;"><i class="fas fa-shield-alt"></i> Binary Armor</a>
                    <a href="${baseDocs}benchmarks.html" class="nyx-nav-link">15-Lang Benchmarks</a>
                    <a href="${baseDocs}industry-benchmarks.html" class="nyx-nav-link">Industry Suites</a>
                    <a href="${baseDocs}side-ecosystem.html" class="nyx-btn-side"><i class="fas fa-shield-alt"></i> SIDE</a>
                </div>

                <button class="nyx-hamburger-btn" id="nyxHamburgerBtn" aria-label="Toggle Navigation Menu">
                    <i class="fas fa-bars"></i> Menu
                </button>
            `;

            const drawer = document.createElement('div');
            drawer.id = 'nyxMobileDrawer';
            drawer.className = 'nyx-mobile-drawer';
            drawer.innerHTML = `
                <div class="nyx-drawer-section">
                    <div class="nyx-drawer-heading">⚡ Quick Actions</div>
                    <a href="${baseRoot}athena/spatial-desktop.html" class="nyx-drawer-link" style="color:#00f0ff; font-weight:700;"><i class="fas fa-desktop"></i> Cybernetic Spatial AI OS (120 FPS Sandbox)</a>
                    <a href="${baseRoot}projects/openworm-sim/index.php" class="nyx-drawer-link" style="color:#00ff88; font-weight:700;"><i class="fas fa-worm"></i> OpenWorm C. elegans Simulator (120 FPS)</a>
                    <a href="${baseRoot}projects/hexapod-arena/index.php" class="nyx-drawer-link" style="color:#f59e0b; font-weight:700;"><i class="fas fa-spider"></i> Hexapod Bio-Robotics Arena (120 FPS)</a>
                    <a href="${baseRoot}projects/flybrain-pong/index.php" class="nyx-drawer-link" style="color:#34d399; font-weight:700;"><i class="fas fa-gamepad"></i> FlyBrain Connectome Pong (120 FPS Game)</a>
                    <a href="${baseDocs}bio-computing.html" class="nyx-drawer-link" style="color:#00ff88; font-weight:700;"><i class="fas fa-dna"></i> Open Bio-Computing &amp; Organisms Portal</a>
                    <a href="${baseDocs}connectome.html" class="nyx-drawer-link" style="color:#34d399; font-weight:700;"><i class="fas fa-brain"></i> Bio-Neuromorphic Connectome Spec</a>
                    <a href="${baseDocs}spatial-ui.html" class="nyx-drawer-link" style="color:#00f0ff; font-weight:700;"><i class="fas fa-vr-cardboard"></i> Spatial UI &amp; Holographic Shaders Spec</a>
                    <a href="${baseDocs}gpu-systems.html" class="nyx-drawer-link" style="color:#38bdf8; font-weight:700;"><i class="fas fa-microchip"></i> GPU Systems Subsystem &amp; Runtime</a>
                    <a href="${baseDocs}playground.html" class="nyx-drawer-link" style="color:#34d399; font-weight:700;"><i class="fas fa-play"></i> Interactive Compiler Playground</a>
                    <a href="${baseDocs}binary-armor.html" class="nyx-drawer-link" style="color:#38bdf8; font-weight:700;"><i class="fas fa-shield-alt"></i> Binary Armor &amp; Hardware PUF</a>
                    <a href="${baseDocs}side-ecosystem.html" class="nyx-drawer-link" style="color:#fb7185; font-weight:700;"><i class="fas fa-shield-alt"></i> SIDE Defense Ecosystem Whitepaper</a>
                    <a href="${baseRoot}downloads/nyx-academic-benchmarks.zip" class="nyx-drawer-link" style="color:#a855f7; font-weight:700;"><i class="fas fa-download"></i> Download Full Benchmark Suite (.zip)</a>
                </div>
                <div class="nyx-drawer-section">
                    <div class="nyx-drawer-heading">🔬 Research &amp; Papers Directory</div>
                    <a href="${baseDocs}paper/papers.html" class="nyx-drawer-link" style="color:#818cf8; font-weight:700;"><i class="fas fa-graduation-cap"></i> All Academic Papers &amp; Benchmarks Directory</a>
                    <a href="${baseDocs}paper/nyx_bio_neuromorphic_connectome_and_spiking_game_ai_2026.html" class="nyx-drawer-link" style="color:#34d399; font-weight:700;">🧠 Bio-Neuromorphic Connectome Simulation Paper</a>
                    <a href="${baseDocs}aero-mobility.html" class="nyx-drawer-link" style="color:#38bdf8; font-weight:700;">🚀 Hyperloop &amp; eVTOL 3D Mobility Specification</a>
                    <a href="${baseDocs}binary-armor.html" class="nyx-drawer-link">🛡️ Binary Armor &amp; Hardware PUF Specification</a>
                    <a href="${baseDocs}paper/index.html" class="nyx-drawer-link">📄 PLDI / POPL 2026 Primary Paper</a>
                    <a href="${baseDocs}safety-and-security.html" class="nyx-drawer-link">🛡️ Formal Region Soundness &amp; CWE Proofs</a>
                    <a href="${baseDocs}benchmarks.html" class="nyx-drawer-link">📊 15-Language Empirical Systems Paper</a>
                    <a href="${baseDocs}industry-benchmarks.html" class="nyx-drawer-link">🏆 5 Industry-Standard Suites (100M Concurrency, SIMD)</a>
                    <a href="${baseDocs}ai-benchmarks.html" class="nyx-drawer-link">🤖 Sovereign AI &amp; LLM Inference Paper (145.8 tok/s)</a>
                    <a href="${baseDocs}academic-benchmarks.html" class="nyx-drawer-link">🔬 Equal-Control Academic Suite (Bumpalo &amp; Arenas)</a>
                    <a href="${baseDocs}side-ecosystem.html" class="nyx-drawer-link">🛡️ Sovereign Intelligence &amp; Defense Ecosystem (SIDE)</a>
                </div>
                <div class="nyx-drawer-section">
                    <div class="nyx-drawer-heading">🌐 Documentation &amp; Sovereign Portals</div>
                    <a href="${baseDocs}index.html" class="nyx-drawer-link">📖 Documentation Home</a>
                    <a href="${baseDocs}connectome.html" class="nyx-drawer-link" style="color:#34d399; font-weight:700;">🧠 Bio-Neuromorphic Connectome Manual</a>
                    <a href="${baseRoot}hausa/index.html" class="nyx-drawer-link" style="color:#34d399; font-weight:700;">🇳🇬 Nyx a Harshen Hausa (Hausa Portal)</a>
                    <a href="${baseRoot}yoruba/index.html" class="nyx-drawer-link" style="color:#38bdf8; font-weight:700;">🇳🇬 Èdè Nyx ni Èdè Yorùbá (Yorùbá Portal)</a>
                    <a href="${baseRoot}igbo/index.html" class="nyx-drawer-link" style="color:#c084fc; font-weight:700;">🇳🇬 Asụsụ Nyx n'Asụsụ Igbo (Igbo Portal)</a>
                    <a href="${baseDocs}smart-board.html" class="nyx-drawer-link" style="color:#34d399; font-weight:700;">🖍️ Universal Smart Board &amp; AI Graphing</a>
                    <a href="${baseDocs}roadmap.html" class="nyx-drawer-link">🗺️ Engineering Roadmap (2026-2027)</a>
                    <a href="${baseDocs}learn/index.html" class="nyx-drawer-link">🎓 Interactive Academy (27 Modules)</a>
                    <a href="${baseRoot}mobile.html" class="nyx-drawer-link">📱 Sovereign Mobile SDK (120Hz)</a>
                    <a href="${baseRoot}projects/flybrain-pong/index.php" class="nyx-drawer-link" style="color:#34d399; font-weight:700;">🪰 FlyBrain Connectome Pong Simulator</a>
                    <a href="${baseRoot}projects/aero-simulator/index.php" class="nyx-drawer-link" style="color:#38bdf8; font-weight:700;">✈️ 3D Mobility &amp; Flight Studio</a>
                    <a href="${baseRoot}projects/binary-armor/index.php" class="nyx-drawer-link">🛡️ Binary Armor Live Studio</a>
                    <a href="${baseRoot}projects/smart-board/index.php" class="nyx-drawer-link">🖍️ Smart Board Live Blackboard App</a>
                    <a href="${baseRoot}projects/nyx-foundry/index.php" class="nyx-drawer-link">🏭 Foundry Sovereign Workforce</a>
                    <a href="${baseRoot}projects/nyx-ai-engine/index.php" class="nyx-drawer-link">🧠 Nyx AI Autonomous Engine</a>
                    <a href="${baseRoot}projects/nyx-side/index.php" class="nyx-drawer-link">🛡️ SIDE Tactical HUD Enclave</a>
                </div>
            `;

            document.body.insertBefore(nav, document.body.firstChild);
            document.body.insertBefore(drawer, nav.nextSibling);

            const btn = document.getElementById('nyxHamburgerBtn');
            btn.addEventListener('click', function() {
                drawer.classList.toggle('open');
                btn.innerHTML = drawer.classList.contains('open') ? '<i class="fas fa-times"></i> Close' : '<i class="fas fa-bars"></i> Menu';
            });
        }

        // --- 2. GLOBAL RICH FOOTER ---
        if (!document.getElementById('nyx-global-footer')) {
            const footer = document.createElement('footer');
            footer.id = 'nyx-global-footer';
            footer.innerHTML = `
                <style>
                    #nyx-global-footer {
                        background: #040711;
                        border-top: 1px solid rgba(255, 255, 255, 0.08);
                        padding: 3.5rem 1.5rem 2rem;
                        font-family: 'Inter', sans-serif;
                        color: #94a3b8;
                    }
                    .nyx-footer-container {
                        max-width: 1120px;
                        margin: 0 auto;
                        display: grid;
                        grid-template-columns: repeat(auto-fit, minmax(220px, 1fr));
                        gap: 2rem;
                    }
                    .nyx-footer-col h4 {
                        font-family: 'Outfit', sans-serif;
                        color: #f8fafc;
                        font-size: 0.95rem;
                        font-weight: 700;
                        margin: 0 0 1rem;
                        text-transform: uppercase;
                        letter-spacing: 0.05em;
                    }
                    .nyx-footer-col ul {
                        list-style: none;
                        padding: 0;
                        margin: 0;
                    }
                    .nyx-footer-col li {
                        margin-bottom: 0.55rem;
                    }
                    .nyx-footer-col a {
                        color: #cbd5e1;
                        text-decoration: none;
                        font-size: 0.88rem;
                        transition: color 0.2s;
                    }
                    .nyx-footer-col a:hover {
                        color: #38bdf8;
                    }
                    .nyx-footer-bottom {
                        max-width: 1120px;
                        margin: 2.5rem auto 0;
                        padding-top: 1.5rem;
                        border-top: 1px solid rgba(255, 255, 255, 0.06);
                        display: flex;
                        justify-content: space-between;
                        align-items: center;
                        flex-wrap: wrap;
                        gap: 1rem;
                        font-size: 0.82rem;
                    }
                </style>
                <div class="nyx-footer-container">
                    <div class="nyx-footer-col">
                        <h4>🌌 Core Ecosystem</h4>
                        <ul>
                            <li><a href="${baseRoot}index.html">Main Portal Home</a></li>
                            <li><a href="${baseDocs}index.html">Documentation Home</a></li>
                            <li><a href="${baseDocs}smart-board.html">Universal Smart Board Architecture</a></li>
                            <li><a href="${baseDocs}paper/papers.html">Academic Papers Directory</a></li>
                            <li><a href="${baseDocs}playground.html">Interactive Compiler Playground</a></li>
                            <li><a href="${baseDocs}learn/index.html">Interactive Academy (27 Modules)</a></li>
                            <li><a href="${baseRoot}mobile.html">Sovereign Mobile SDK (120Hz)</a></li>
                            <li><a href="${baseDocs}roadmap.html">Engineering Roadmap</a></li>
                        </ul>
                    </div>
                    <div class="nyx-footer-col">
                        <h4>📊 Empirical Research</h4>
                        <ul>
                            <li><a href="${baseDocs}paper/papers.html">All Papers &amp; Benchmarks</a></li>
                            <li><a href="${baseDocs}benchmarks.html">15-Language Systems Suite</a></li>
                            <li><a href="${baseDocs}industry-benchmarks.html">5 Industry-Standard Suites</a></li>
                            <li><a href="${baseDocs}ai-benchmarks.html">Sovereign AI &amp; LLM Inference</a></li>
                            <li><a href="${baseDocs}academic-benchmarks.html">Equal-Control Academic Suite</a></li>
                            <li><a href="${baseDocs}benchmark-code.html">Complete 15-Language Code</a></li>
                        </ul>
                    </div>
                    <div class="nyx-footer-col">
                        <h4>🛡️ Security &amp; Defense</h4>
                        <ul>
                            <li><a href="${baseDocs}aero-mobility.html">3D Mobility (Hyperloop/eVTOL)</a></li>
                            <li><a href="${baseRoot}projects/aero-simulator/index.php">Flight &amp; MagLev Studio</a></li>
                            <li><a href="${baseDocs}binary-armor.html">Binary Armor &amp; Hardware PUF</a></li>
                            <li><a href="${baseRoot}projects/binary-armor/index.php">Binary Armor Studio</a></li>
                            <li><a href="${baseDocs}side-ecosystem.html">SIDE Defense Whitepaper</a></li>
                            <li><a href="${baseDocs}safety-and-security.html">Formal Region Soundness</a></li>
                            <li><a href="${baseDocs}safety-and-security.html#section-cwe">CWE Top 25 Immunity Matrix</a></li>
                            <li><a href="${baseDocs}security.html">Threat Model &amp; Capabilities</a></li>
                        </ul>
                    </div>
                    <div class="nyx-footer-col">
                        <h4>📦 Public Verification</h4>
                        <ul>
                            <li><a href="${baseDocs}extensions/index.html" style="color:#c084fc; font-weight:700;"><i class="fas fa-cubes"></i> Sovereign Dialects &amp; Cybernetics</a></li>
                            <li><a href="${baseRoot}downloads/nyx-v0.42.0-windows-x64.zip" style="color:#34d399; font-weight:700;"><i class="fas fa-download"></i> Windows x64 Compiler v0.42 (.zip)</a></li>
                            <li><a href="${baseRoot}downloads/nyx-academic-benchmarks.zip"><i class="fas fa-download"></i> Academic Benchmark Suite (.zip)</a></li>
                            <li><a href="${baseRoot}downloads/nyx-v0.32.0-windows-x64.zip">Windows x64 Compiler v0.32 (.zip)</a></li>
                            <li><a href="${baseRoot}downloads/nyx-v0.32.0-linux-x64.tar.gz">Linux x86_64 Compiler v0.32 (.tar.gz)</a></li>
                            <li><a href="${baseRoot}rss.xml">RSS Technical Feed</a></li>
                        </ul>
                    </div>
                </div>
                <div class="nyx-footer-bottom">
                    <div>&copy; 2026 Nyx Programming Language &amp; Systems Architecture. All rights reserved.</div>
                    <div style="color:#64748b;">Bare-Metal Hardware Testbed: Intel Core i7-6700HQ @ 2.60GHz (16GB RAM)</div>
                </div>
            `;
            document.body.appendChild(footer);
        }
    }

    if (document.readyState === 'loading') {
        document.addEventListener('DOMContentLoaded', injectGlobalHeaderAndFooter);
    } else {
        injectGlobalHeaderAndFooter();
    }
})();
