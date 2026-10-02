# run_ai_benchmarks.ps1
# Sovereign Nyx AI Inference Benchmark Runner (PowerShell)

Write-Host '================================================================================' -ForegroundColor Cyan
Write-Host '[*] SOVEREIGN NYX AI INFERENCE BENCHMARK RUNNER' -ForegroundColor Cyan
Write-Host '================================================================================' -ForegroundColor Cyan
Write-Host 'Hardware Target : Intel Core i7-6700HQ @ 2.60GHz (16.0 GB RAM)' -ForegroundColor Gray
Write-Host 'Local Depot     : C:\models (Gemma-4-E4B) & projects/nyx-ai-engine/ (Llama-3.2-3B)' -ForegroundColor Gray
Write-Host '================================================================================' -ForegroundColor Cyan
Write-Host ''

if (-not (Test-Path 'bin')) {
    New-Item -ItemType Directory -Path 'bin' -Force | Out-Null
}

Write-Host '[1/3] Compiling and Executing Native Multi-Model Benchmark (Nyx Toolchain)...' -ForegroundColor Yellow
& .\nyx.exe -i benchmark/academic/ai_benchmark.nyx -o bin/ai_benchmark.exe -r

Write-Host ''
Write-Host '[2/3] Compiling and Executing Dedicated Gemma-4-E4B Benchmark (Nyx Toolchain)...' -ForegroundColor Yellow
& .\nyx.exe -i benchmark/academic/gemma_benchmark.nyx -o bin/gemma_benchmark.exe -r

Write-Host ''
Write-Host '[3/3] Executing Academic Multi-Model Python Telemetry Suite...' -ForegroundColor Yellow
python -u benchmark/academic/run_ai_inference_benchmark.py --iterations 5 --model all

Write-Host ''
Write-Host '================================================================================' -ForegroundColor Green
Write-Host '[PASS] ALL BENCHMARK RUNS COMPLETED SUCCESSFULLY!' -ForegroundColor Green
Write-Host 'Results Artifact : benchmark/academic/ai_inference_results.json' -ForegroundColor Green
Write-Host 'Research Portal  : docs/ai-benchmarks.html' -ForegroundColor Green
Write-Host '================================================================================' -ForegroundColor Green
