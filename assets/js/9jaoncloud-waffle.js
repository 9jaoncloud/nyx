/**
 * 9jaOnCloud Universal App Switcher (Google 9-Dot Waffle Menu Widget)
 * Auto-injects 9-dot ecosystem app launcher with rich dark luxury styling
 */
(function() {
    window.initNineJaAppSwitcher = function(options) {
        options = options || {};
        const accountsUrl = (options.accountsUrl || 'https://accounts.9jaoncloud.com.ng').replace(/\/$/, '');
        const targetElement = document.querySelector(options.target || '#nineja-app-switcher');

        if (!targetElement) return;

        // Default Canonical Ecosystem Apps across 9jaonCloud
        const defaultApps = [
            { name: "Accounts IAM", url: "https://accounts.9jaoncloud.com.ng", icon: "🔑", color: "#3b82f6", desc: "Central SSO & Organization Identity" },
            { name: "Nyx Language", url: "https://nyx.9jaoncloud.com.ng", icon: "🌙", color: "#a855f7", desc: "Sovereign Systems & Zero-GC Engine" },
            { name: "Geo 9jaonCloud", url: "https://geo.com.ng", icon: "🌍", color: "#10b981", desc: "Geospatial & Earth Intelligence" },
            { name: "TrustGuard NG", url: "https://trustguard.9jaoncloud.com.ng", icon: "🛡️", color: "#6366f1", desc: "Governance, NDPA & GRC" },
            { name: "NowTrending", url: "https://nowtrending.com.ng", icon: "📈", color: "#ec4899", desc: "Real-Time News & Media Intel" },
            { name: "Tradea Platform", url: "https://tradea.com.ng", icon: "💳", color: "#f59e0b", desc: "Trade, Commerce & Settlements" },
            { name: "DxEnterprise", url: "https://dxenterprise.9jaoncloud.com.ng", icon: "🩺", color: "#06b6d4", desc: "Clinical Healthcare & Tele-Diagnostics (MedDiag)" },
            { name: "SafeHouse", url: "https://safehouse.9jaoncloud.com.ng", icon: "⛪", color: "#8b5cf6", desc: "Faith & Community Services" },
            { name: "AFNID Research", url: "https://afnid.org.ng", icon: "📚", color: "#14b8a6", desc: "Academic Research & Public Opinion Platform" }
        ];

        let appsHtml = '';
        defaultApps.forEach(app => {
            appsHtml += `
                <a href="${app.url}" class="nineja-waffle-app-card" target="_blank" title="${app.name}: ${app.desc}">
                    <div class="nineja-waffle-app-icon" style="background:${app.color}20; border: 1px solid ${app.color}50; color:${app.color};">
                        ${app.icon}
                    </div>
                    <span class="nineja-waffle-app-title">${app.name}</span>
                </a>
            `;
        });

        const widgetHtml = `
            <div class="nineja-waffle-container">
                <button type="button" class="nineja-waffle-btn" id="ninejaWaffleTrigger" title="9jaonCloud Ecosystem Apps">
                    <svg width="20" height="20" viewBox="0 0 24 24" fill="currentColor">
                        <circle cx="5" cy="5" r="2.5"/>
                        <circle cx="12" cy="5" r="2.5"/>
                        <circle cx="19" cy="5" r="2.5"/>
                        <circle cx="5" cy="12" r="2.5"/>
                        <circle cx="12" cy="12" r="2.5"/>
                        <circle cx="19" cy="12" r="2.5"/>
                        <circle cx="5" cy="19" r="2.5"/>
                        <circle cx="12" cy="19" r="2.5"/>
                        <circle cx="19" cy="19" r="2.5"/>
                    </svg>
                </button>
                <div class="nineja-waffle-dropdown" id="ninejaWaffleMenu">
                    <div class="nineja-waffle-header">
                        <div style="font-weight:700; color:#fff; font-size:13px;">9jaonCloud Ecosystem</div>
                        <div style="font-size:11px; color:#38bdf8;">Universal Suite</div>
                    </div>
                    <div class="nineja-waffle-grid">
                        ${appsHtml}
                    </div>
                    <div class="nineja-waffle-footer">
                        <a href="${accountsUrl}/" target="_blank">
                            Manage Central 9jaonCloud ID &rarr;
                        </a>
                    </div>
                </div>
            </div>
        `;

        targetElement.innerHTML = widgetHtml;

        const trigger = document.getElementById('ninejaWaffleTrigger');
        const menu = document.getElementById('ninejaWaffleMenu');

        if (trigger && menu) {
            trigger.addEventListener('click', function(e) {
                e.stopPropagation();
                menu.classList.toggle('active');
            });

            document.addEventListener('click', function(e) {
                if (!menu.contains(e.target) && e.target !== trigger) {
                    menu.classList.remove('active');
                }
            });
        }
    };

    // Auto-initialize on load
    document.addEventListener('DOMContentLoaded', function() {
        const target = document.querySelector('#nineja-app-switcher');
        if (target) {
            window.initNineJaAppSwitcher({ target: '#nineja-app-switcher' });
        }
    });
})();
