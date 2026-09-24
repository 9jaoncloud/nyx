<?php
// Nyx Language Documentation Bridge
// Seamlessly serves docs/index.html
if (file_exists(__DIR__ . '/index.html')) {
    include __DIR__ . '/index.html';
} else {
    header("Location: index.html");
    exit();
}
