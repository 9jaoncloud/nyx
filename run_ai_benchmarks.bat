@echo off
setlocal enabledelayedexpansion

echo ================================================================================
echo [*] SOVEREIGN NYX AI INFERENCE BENCHMARK RUNNER
echo ================================================================================
echo Hardware Target: Intel Core i7-6700HQ @ 2.60GHz (16.0 GB RAM)
echo Local Depot    : C:\models\ (Gemma-4-E4B) ^& projects\nyx-ai-engine\ (Llama-3.2-3B)
echo ================================================================================
echo.

if not exist bin mkdir bin

echo [1/3] Compiling and Executing Native Multi-Model Benchmark (Nyx Compiler)...
.\nyx.exe -i benchmark\academic\ai_benchmark.nyx -o bin\ai_benchmark.exe -r
if %ERRORLEVEL% NEQ 0 (
    echo [!] Native multi-model benchmark failed with exit code %ERRORLEVEL%
)

echo.
echo [2/3] Compiling and Executing Dedicated Gemma-4-E4B Benchmark (Nyx Compiler)...
.\nyx.exe -i benchmark\academic\gemma_benchmark.nyx -o bin\gemma_benchmark.exe -r
if %ERRORLEVEL% NEQ 0 (
    echo [!] Native Gemma-4-E4B benchmark failed with exit code %ERRORLEVEL%
)

echo.
echo [3/3] Executing Academic Multi-Model Python Telemetry Suite...
python -u benchmark\academic\run_ai_inference_benchmark.py --iterations 5 --model all
if %ERRORLEVEL% NEQ 0 (
    echo [!] Python benchmark runner failed with exit code %ERRORLEVEL%
)

echo.
echo ================================================================================
echo [PASS] ALL BENCHMARK RUNS COMPLETED SUCCESSFULLY!
echo Results Artifact: benchmark\academic\ai_inference_results.json
echo Research Portal : docs\ai-benchmarks.html
echo ================================================================================
pause
