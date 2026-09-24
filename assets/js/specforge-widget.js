/**
 * SpecForge Embeddable Bug & Feedback Widget
 * Robust, resilient, red luxury popup button in bottom-right corner.
 * Usage: <script src="https://specforge.9jaoncloud.com.ng/assets/js/specforge-widget.js" data-token="YOUR_TOKEN"></script>
 */
(function () {
    function initWidget() {
        if (document.getElementById('sf-widget-root-btn')) return;

        // Extract script element and token safely
        const script = document.currentScript || 
                       document.querySelector('script[src*="specforge-widget.js"]') ||
                       document.querySelector('script[data-token]') ||
                       document.querySelector('script[data-project-token]');
        
        const token = (script ? (script.getAttribute('data-token') || script.getAttribute('data-project-token')) : '') || 
                      window.SPECFORGE_TOKEN || 
                      '85ef1ac50900f8bff142c95c85287db4'; // Fallback Nyx token

        const apiUrl = 'https://specforge.9jaoncloud.com.ng/api/router.php?action=public_submit_bug';

        // Capture Browser Console Errors Automatically
        const sfLogs = [];
        const origErr = console.error;
        console.error = function (...args) {
            sfLogs.push('[Console Error ' + new Date().toLocaleTimeString() + '] ' + args.join(' '));
            if (sfLogs.length > 10) sfLogs.shift();
            if (origErr) origErr.apply(console, args);
        };
        window.addEventListener('error', function (e) {
            sfLogs.push('[Uncaught Exception ' + new Date().toLocaleTimeString() + '] ' + (e.message || 'Error at ' + e.filename + ':' + e.lineno));
            if (sfLogs.length > 10) sfLogs.shift();
        });

        // Inject widget CSS
        const style = document.createElement('style');
        style.id = 'sf-widget-styles';
        style.innerHTML = `
            @keyframes sfPulseRed {
                0% { box-shadow: 0 0 0 0 rgba(239, 68, 68, 0.7); }
                70% { box-shadow: 0 0 0 14px rgba(239, 68, 68, 0); }
                100% { box-shadow: 0 0 0 0 rgba(239, 68, 68, 0); }
            }
            .sf-widget-btn {
                position: fixed !important;
                bottom: 24px !important;
                right: 24px !important;
                z-index: 2147483647 !important;
                background: linear-gradient(135deg, #ef4444, #dc2626) !important;
                color: #ffffff !important;
                border: 2px solid rgba(255, 255, 255, 0.25) !important;
                border-radius: 50px !important;
                padding: 12px 22px !important;
                font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif !important;
                font-size: 14px !important;
                font-weight: 700 !important;
                box-shadow: 0 10px 30px rgba(239, 68, 68, 0.45) !important;
                cursor: pointer !important;
                display: flex !important;
                align-items: center !important;
                gap: 8px !important;
                transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1) !important;
                animation: sfPulseRed 2.5s infinite !important;
                text-decoration: none !important;
            }
            .sf-widget-btn:hover {
                transform: translateY(-3px) scale(1.03) !important;
                background: linear-gradient(135deg, #dc2626, #b91c1c) !important;
                box-shadow: 0 15px 35px rgba(239, 68, 68, 0.6) !important;
            }
            .sf-widget-btn .sf-icon {
                font-size: 16px;
                display: inline-flex;
            }
            .sf-widget-btn .sf-badge-dot {
                width: 8px;
                height: 8px;
                background: #fef08a;
                border-radius: 50%;
                display: inline-block;
            }
            .sf-widget-overlay {
                position: fixed !important;
                top: 0 !important;
                left: 0 !important;
                right: 0 !important;
                bottom: 0 !important;
                background: rgba(15, 23, 42, 0.8) !important;
                backdrop-filter: blur(8px) !important;
                z-index: 2147483647 !important;
                display: flex !important;
                align-items: center !important;
                justify-content: center !important;
                padding: 20px !important;
                box-sizing: border-box !important;
                font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif !important;
            }
            .sf-widget-modal {
                background: #0f172a !important;
                color: #f8fafc !important;
                border: 1px solid rgba(255, 255, 255, 0.15) !important;
                border-radius: 18px !important;
                width: 100% !important;
                max-width: 580px !important;
                max-height: 90vh !important;
                overflow-y: auto !important;
                padding: 26px !important;
                box-shadow: 0 25px 60px rgba(0,0,0,0.7) !important;
                box-sizing: border-box !important;
            }
            .sf-widget-modal h3 {
                margin: 0 0 16px 0 !important;
                font-size: 20px !important;
                font-weight: 700 !important;
                display: flex !important;
                justify-content: space-between !important;
                align-items: center !important;
                color: #f8fafc !important;
            }
            .sf-widget-field { margin-bottom: 15px !important; }
            .sf-widget-field label {
                display: block !important;
                font-size: 13px !important;
                font-weight: 600 !important;
                margin-bottom: 6px !important;
                color: #cbd5e1 !important;
            }
            .sf-widget-field input, 
            .sf-widget-field select, 
            .sf-widget-field textarea {
                width: 100% !important;
                background: #1e293b !important;
                border: 1px solid #475569 !important;
                color: #ffffff !important;
                border-radius: 10px !important;
                padding: 10px 14px !important;
                font-size: 14px !important;
                box-sizing: border-box !important;
                outline: none !important;
            }
            .sf-widget-field input:focus, 
            .sf-widget-field select:focus, 
            .sf-widget-field textarea:focus {
                border-color: #ef4444 !important;
                box-shadow: 0 0 0 3px rgba(239, 68, 68, 0.25) !important;
            }
            .sf-widget-submit {
                background: linear-gradient(135deg, #ef4444, #dc2626) !important;
                color: #fff !important;
                border: none !important;
                padding: 13px 22px !important;
                border-radius: 10px !important;
                font-weight: 700 !important;
                width: 100% !important;
                cursor: pointer !important;
                font-size: 15px !important;
                transition: background 0.2s ease !important;
            }
            .sf-widget-submit:hover {
                background: linear-gradient(135deg, #dc2626, #b91c1c) !important;
            }
            .sf-close-btn {
                background: none !important;
                border: none !important;
                color: #94a3b8 !important;
                font-size: 24px !important;
                cursor: pointer !important;
                line-height: 1 !important;
            }
            .sf-close-btn:hover { color: #fff !important; }
        `;
        document.head.appendChild(style);

        // Create trigger button
        const btn = document.createElement('button');
        btn.id = 'sf-widget-root-btn';
        btn.className = 'sf-widget-btn';
        btn.setAttribute('type', 'button');
        btn.innerHTML = `<span class="sf-icon">&#128030;</span> <span>Report Bug</span> <span class="sf-badge-dot"></span>`;
        document.body.appendChild(btn);

        // Modal behavior
        btn.onclick = function () {
            const overlay = document.createElement('div');
            overlay.className = 'sf-widget-overlay';
            overlay.innerHTML = `
                <div class="sf-widget-modal">
                    <h3>
                        <span>&#128030; Submit Issue &amp; Feedback</span>
                        <button type="button" class="sf-close-btn" id="sfClose">&times;</button>
                    </h3>
                    <form id="sfForm">
                        <div class="sf-widget-field">
                            <label>Category</label>
                            <select name="feedback_type">
                                <option value="bug">&#128027; Bug / Compiler Regression</option>
                                <option value="ui">&#127912; UI / Layout Issue</option>
                                <option value="performance">&#9889; Performance / Latency</option>
                                <option value="feedback">&#128161; Feature Idea / RFC Suggestion</option>
                            </select>
                        </div>
                        <div class="sf-widget-field">
                            <label>1. Summary / Title</label>
                            <input type="text" name="title" placeholder="Brief summary of the issue or feature" required />
                        </div>
                        <div class="sf-widget-field">
                            <label>2. Page / Environment URL</label>
                            <input type="text" name="page_url" value="${window.location.href}" />
                        </div>
                        <div class="sf-widget-field">
                            <label>3. What actually happened / Error message?</label>
                            <textarea name="actual_behavior" rows="3" placeholder="Describe the error, crash or unexpected behavior" required></textarea>
                        </div>
                        <div class="sf-widget-field">
                            <label>4. What was expected?</label>
                            <textarea name="expected_behavior" rows="2" placeholder="Describe expected behavior"></textarea>
                        </div>
                        <div class="sf-widget-field">
                            <label>5. Steps to Reproduce / Code Snippet</label>
                            <textarea name="steps_to_reproduce" rows="3" placeholder="1. Run command or open page...&#10;2. Code snippet..."></textarea>
                        </div>
                        <div class="sf-widget-field">
                            <label>Your Email (Optional, for fix confirmation)</label>
                            <input type="email" name="reporter_email" placeholder="developer@example.com" />
                        </div>
                        <button type="submit" class="sf-widget-submit">Send to SpecForge Triage &rarr;</button>
                    </form>
                </div>
            `;
            document.body.appendChild(overlay);

            // Fetch dynamic categories for this specific project
            let catSelectHtml = `
                <option value="bug">&#128027; Bug / Defect</option>
                <option value="ui">&#127912; UI / Visual Issue</option>
                <option value="performance">&#9889; Performance Issue</option>
                <option value="feedback">&#128161; Feature Suggestion</option>
            `;

            fetch('https://specforge.9jaoncloud.com.ng/api/router.php?action=get_project_categories&token=' + encodeURIComponent(token))
                .then(r => r.json())
                .then(res => {
                    if (res && res.success && Array.isArray(res.categories) && res.categories.length > 0) {
                        const selectEl = overlay.querySelector('select[name="feedback_type"]');
                        if (selectEl) {
                            selectEl.innerHTML = res.categories.map(c => `<option value="${c.key}">${c.label}</option>`).join('');
                        }
                    }
                }).catch(() => {});


            document.getElementById('sfClose').onclick = () => document.body.removeChild(overlay);
            overlay.onclick = (e) => { if (e.target === overlay) document.body.removeChild(overlay); };

            document.getElementById('sfForm').onsubmit = function (e) {
                e.preventDefault();
                const submitBtn = overlay.querySelector('.sf-widget-submit');
                submitBtn.disabled = true;
                submitBtn.innerText = 'Submitting to SpecForge...';

                const formData = new FormData(this);
                const payload = {
                    token: token,
                    title: formData.get('title'),
                    feedback_type: formData.get('feedback_type'),
                    page_url: formData.get('page_url'),
                    actual_behavior: formData.get('actual_behavior'),
                    expected_behavior: formData.get('expected_behavior'),
                    steps_to_reproduce: formData.get('steps_to_reproduce'),
                    reporter_email: formData.get('reporter_email'),
                    browser_info: navigator.userAgent,
                    console_logs: sfLogs.join('\n')
                };

                fetch(apiUrl, {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify(payload)
                })
                .then(r => r.json())
                .then(data => {
                    if (data.success) {
                        const trackUrl = 'https://specforge.9jaoncloud.com.ng/bug_report.php?track=' + (data.access_token || '');
                        overlay.querySelector('.sf-widget-modal').innerHTML = `
                            <div style="text-align:center;padding:20px 10px;">
                                <h2 style="color:#10b981;margin-bottom:10px;">&#10004; Bug Report Received!</h2>
                                <p style="color:#cbd5e1;font-size:14px;">Your report has been logged with the product &amp; compiler triage team.</p>
                                ${data.access_token ? `
                                <div style="margin:18px 0;background:#1e293b;padding:14px;border-radius:10px;border:1px solid #334155;text-align:left;">
                                    <label style="font-size:12px;color:#94a3b8;display:block;margin-bottom:6px;font-weight:700;">Private Bug Tracking &amp; Fix Confirmation Link:</label>
                                    <a href="${trackUrl}" target="_blank" style="color:#38bdf8;font-size:13px;word-break:break-all;font-family:monospace;">${trackUrl}</a>
                                </div>
                                ` : ''}
                                <div style="display:flex;gap:10px;justify-content:center;margin-top:20px;">
                                    ${data.access_token ? `<a href="${trackUrl}" target="_blank" style="background:#ef4444;color:#fff;padding:10px 18px;border-radius:8px;text-decoration:none;font-weight:700;font-size:13px;">Track Status &rarr;</a>` : ''}
                                    <button type="button" class="sf-widget-submit" style="width:auto;padding:10px 18px;background:#475569;" onclick="document.body.removeChild(this.closest('.sf-widget-overlay'))">Close</button>
                                </div>
                            </div>
                        `;
                    } else {
                        alert(data.message || 'Failed to submit bug report.');
                        submitBtn.disabled = false;
                        submitBtn.innerText = 'Send to SpecForge Triage ->';
                    }
                })
                .catch(err => {
                    alert('Network error or CORS issue. Please try again.');
                    submitBtn.disabled = false;
                    submitBtn.innerText = 'Send to SpecForge Triage ->';
                });
            };
        };
    }

    if (document.readyState === 'loading') {
        document.addEventListener('DOMContentLoaded', initWidget);
    } else {
        initWidget();
    }
})();
