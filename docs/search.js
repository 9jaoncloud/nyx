// Nyx Docs - Client-side search (base-path aware)
(function() {
    var searchContainer = document.getElementById('search-container');
    if (!searchContainer) return;

    var searchIndex = [];
    var focusedIndex = -1;

    // Detect base path for search index
    var script = document.currentScript || (function() {
        var scripts = document.getElementsByTagName('script');
        return scripts[scripts.length - 1];
    })();
    var scriptPath = script.src;
    var basePath = scriptPath.substring(0, scriptPath.lastIndexOf('/') + 1);
    var searchUrl = basePath + 'search-index.json';

    var xhr = new XMLHttpRequest();
    xhr.open('GET', searchUrl, true);
    xhr.onload = function() {
        if (xhr.status === 200) {
            try { searchIndex = JSON.parse(xhr.responseText); } catch(e) {}
        }
    };
    xhr.send();

    var input = document.getElementById('search-input');
    var results = document.getElementById('search-results');

    input.addEventListener('input', function() {
        var query = input.value.trim().toLowerCase();
        if (query.length < 2) {
            results.style.display = 'none';
            results.innerHTML = '';
            return;
        }

        var matches = searchIndex.filter(function(item) {
            return item.title.toLowerCase().indexOf(query) !== -1 ||
                   item.content.toLowerCase().indexOf(query) !== -1;
        }).slice(0, 10);

        if (matches.length === 0) {
            results.innerHTML = '<a style="color:#64748b">No results found</a>';
        } else {
            results.innerHTML = matches.map(function(item, i) {
                return '<a href="' + basePath + item.url + '" data-index="' + i + '">' +
                       item.title + '<br><small>' + item.section + '</small></a>';
            }).join('');
        }

        results.style.display = 'block';
        focusedIndex = -1;
    });

    input.addEventListener('keydown', function(e) {
        var links = results.querySelectorAll('a[data-index]');
        if (e.key === 'ArrowDown') {
            e.preventDefault();
            focusedIndex = Math.min(focusedIndex + 1, links.length - 1);
            updateFocus(links);
        } else if (e.key === 'ArrowUp') {
            e.preventDefault();
            focusedIndex = Math.max(focusedIndex - 1, -1);
            updateFocus(links);
        } else if (e.key === 'Enter' && focusedIndex >= 0) {
            e.preventDefault();
            links[focusedIndex].click();
        } else if (e.key === 'Escape') {
            results.style.display = 'none';
        }
    });

    function updateFocus(links) {
        for (var i = 0; i < links.length; i++) {
            links[i].classList.toggle('focused', i === focusedIndex);
        }
    }

    document.addEventListener('click', function(e) {
        if (!searchContainer.contains(e.target)) {
            results.style.display = 'none';
        }
    });
})();
