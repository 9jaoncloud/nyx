/**
 * Nyx Standard Library — @nyx/neurotrader (Package #45)
 * Zero-GC Liquid State Machine (LSM) & Bio-Neuromorphic Market Analyser
 *
 * Author: Simeon Bala (9jaoncloud@gmail.com)
 * Architecture: 164K Drosophila Connectome Reservoir + Multi-Horizon Plasticity
 */

#ifndef NYX_NEUROTRADER_H
#define NYX_NEUROTRADER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Multi-Horizon Trading Actions */
typedef enum {
    SIGNAL_HOLD = 0,
    SIGNAL_STRONG_BUY = 1,
    SIGNAL_BUY = 2,
    SIGNAL_SELL = 3,
    SIGNAL_STRONG_SELL = 4
} NyxTradeAction;

/* Trading Horizons */
typedef enum {
    HORIZON_SCALPING = 0,    /* Sub-second to 1-minute microstructure */
    HORIZON_DAY_TRADING = 1, /* 15-minute to 4-hour trend momentum */
    HORIZON_LONG_TERM = 2    /* Fundamental intrinsic value & DCF */
} NyxTradeHorizon;

/* Market Tick Data Structure */
typedef struct {
    char symbol[16];
    double price;
    double bid;
    double ask;
    double volume;
    double high_24h;
    double low_24h;
    double rsi_14;
    double macd_hist;
    double vwap;
    uint64_t timestamp_ns;
} NyxMarketTick;

/* Fundamental Valuation Metrics */
typedef struct {
    double eps;                  /* Earnings Per Share */
    double expected_growth_rate; /* Projected 5-year growth rate (e.g., 12.5% = 12.5) */
    double aaa_bond_yield;       /* Corporate / Sovereign Bond Yield (e.g., 4.4%) */
    double free_cash_flow;       /* Annual FCF in millions */
    double total_debt;           /* Total Debt in millions */
    double cash_and_equiv;       /* Cash in millions */
    double shares_outstanding;   /* Shares in millions */
    double discount_rate;        /* WACC (e.g., 9.5% = 0.095) */
    double perpetual_growth;     /* Terminal growth rate (e.g., 2.5% = 0.025) */
    double book_value_per_share; /* Book value per share */
} NyxValuationInput;

typedef struct {
    double dcf_intrinsic_value;  /* Discounted Cash Flow Fair Value */
    double graham_number;        /* Benjamin Graham Intrinsic Value */
    double margin_of_safety_pct; /* (Intrinsic Value - Current Price) / Intrinsic Value */
    double pe_ratio;             /* Current P/E */
    double pb_ratio;             /* Current P/B */
    bool is_undervalued;         /* Margin of safety > 20% */
} NyxValuationResult;

/* Market Regimes */
typedef enum {
    REGIME_TREND_EXPANSION = 0,   /* High directional momentum & trend following */
    REGIME_MEAN_REVERSION = 1,    /* Range-bound compression & oscillator exhaustion */
    REGIME_LIQUIDITY_SWEEP = 2,    /* Killzone institutional order block sweeps */
    REGIME_MACRO_SHOCK = 3        /* High-volatility central bank / geopolitical event */
} NyxMarketRegime;

/* Individual Strategy Heuristics */
typedef struct {
    NyxTradeAction smm_matrix_signal;       /* SMM Matrix Sniper */
    float smm_confidence;
    NyxTradeAction gold_master_signal;      /* Gold Master Pro v9.1 Synergy / Topas */
    float gold_master_confidence;
    NyxTradeAction tdi_sharkfin_signal;     /* TDI Shark Fin Mean Reversion */
    float tdi_confidence;
    NyxTradeAction macro_carry_signal;      /* Central Bank Rate Differential Carry */
    float carry_confidence;
    NyxTradeAction real_yield_signal;       /* TIPS Real Yield & DXY Squeeze */
    float real_yield_confidence;
    NyxTradeAction dcf_graham_signal;       /* DCF & Graham Margin of Safety */
    float dcf_confidence;
} NyxStrategyHeuristics;

/* Meta-Ensemble Self-Optimized Output */
typedef struct {
    NyxTradeAction synthesized_action;
    float synthesized_confidence;
    NyxMarketRegime detected_regime;
    float regime_probabilities[4];
    float strategy_weights[6];            /* Dynamic weighting across heuristics */
    double recommended_kelly_fraction;   /* Optimal Half-Kelly position fraction */
    float meta_alpha_advantage_pct;       /* Estimated outperformance vs baseline */
} NyxMetaEnsembleOutput;

/* Connectome Reservoir State */
typedef struct {
    uint32_t num_neurons;         /* 164,587 biological neurons */
    uint32_t sensory_afferents;   /* 2,048 sensory injection points */
    uint32_t motor_readouts;      /* 882 descending output units */
    float* membrane_potentials;   /* V_m for all neurons (Zero-GC allocated) */
    uint8_t* spike_buffer;        /* Discrete spike emission flags */
    float* synaptic_weights;      /* Sparse recurrent reservoir weights */
    float* eligibility_traces;    /* Synaptic eligibility traces for R-STDP */
    float* readout_weights_scalp; /* Trained plastic weights for scalping */
    float* readout_weights_day;   /* Trained plastic weights for day trading */
    float* readout_weights_val;   /* Trained plastic weights for valuation */
    float dopamine_level;         /* Real-time reward modulator delta(t) */
    float homeostatic_target_rate;/* Homeostatic target firing rate (e.g. 0.05) */
    uint64_t total_spikes_fired;
    double last_eval_latency_ms;
} NyxMarketReservoir;

/* Multi-Horizon Evaluation Output */
typedef struct {
    NyxTradeAction action_scalp;
    float confidence_scalp;
    NyxTradeAction action_day;
    float confidence_day;
    NyxTradeAction action_value;
    float confidence_value;
    NyxValuationResult valuation;
    NyxMetaEnsembleOutput meta_ensemble;
    uint32_t active_neurons_fired;
    uint32_t total_spikes_burst;
    double inference_latency_ms;
} NyxMultiHorizonSignal;

/* API Functions */
NyxMarketReservoir* nyx_neurotrader_init_reservoir(void* memory_region, size_t region_size);
void nyx_neurotrader_encode_tick_spikes(NyxMarketReservoir* res, const NyxMarketTick* tick);
void nyx_neurotrader_step_reservoir_simd(NyxMarketReservoir* res, uint32_t steps);
NyxValuationResult nyx_neurotrader_calculate_valuation(const NyxValuationInput* input, double current_price);
NyxMarketRegime nyx_neurotrader_classify_regime(const NyxMarketTick* tick, double realized_volatility);
NyxMetaEnsembleOutput nyx_neurotrader_synthesize_strategies(
    NyxMarketReservoir* res,
    const NyxStrategyHeuristics* heuristics,
    NyxMarketRegime regime,
    double win_rate,
    double reward_to_risk
);
NyxMultiHorizonSignal nyx_neurotrader_analyze_market(
    NyxMarketReservoir* res,
    const NyxMarketTick* tick,
    const NyxValuationInput* val_input,
    const NyxStrategyHeuristics* heuristics
);
void nyx_neurotrader_apply_reward_stdp(
    NyxMarketReservoir* res,
    NyxTradeHorizon horizon,
    float trade_pnl_return,
    float sharpe_delta
);
double nyx_neurotrader_calculate_kelly_size(double win_rate, double win_loss_ratio, double confidence);

#ifdef __cplusplus
}
#endif

#endif /* NYX_NEUROTRADER_H */
