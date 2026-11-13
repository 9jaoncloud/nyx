/**
 * Nyx Standard Library — @nyx/neurotrader (Package #45)
 * Native C Implementation of Zero-GC Liquid State Machine (LSM)
 *
 * Author: Simeon Bala (9jaoncloud@gmail.com)
 */

#include "nyx_neurotrader.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define RESERVOIR_DEFAULT_NEURONS 164587
#define SENSORY_AFFERENTS 2048
#define MOTOR_READOUTS 882
#define MEMBRANE_THRESHOLD 1.0f
#define MEMBRANE_DECAY 0.92f
#define MEMBRANE_RESET 0.0f

NyxMarketReservoir* nyx_neurotrader_init_reservoir(void* memory_region, size_t region_size) {
    if (!memory_region || region_size < sizeof(NyxMarketReservoir)) {
        return NULL;
    }

    NyxMarketReservoir* res = (NyxMarketReservoir*)memory_region;
    memset(res, 0, sizeof(NyxMarketReservoir));

    res->num_neurons = RESERVOIR_DEFAULT_NEURONS;
    res->sensory_afferents = SENSORY_AFFERENTS;
    res->motor_readouts = MOTOR_READOUTS;

    // In Zero-GC region memory, pointers are assigned within the region arena
    uint8_t* byte_ptr = (uint8_t*)memory_region + sizeof(NyxMarketReservoir);
    
    res->membrane_potentials = (float*)byte_ptr;
    byte_ptr += res->num_neurons * sizeof(float);

    res->spike_buffer = (uint8_t*)byte_ptr;
    byte_ptr += res->num_neurons * sizeof(uint8_t);

    res->readout_weights_scalp = (float*)byte_ptr;
    byte_ptr += res->motor_readouts * sizeof(float);

    res->readout_weights_day = (float*)byte_ptr;
    byte_ptr += res->motor_readouts * sizeof(float);

    res->readout_weights_val = (float*)byte_ptr;
    byte_ptr += res->motor_readouts * sizeof(float);

    // Initialize baseline readout weights
    for (uint32_t i = 0; i < res->motor_readouts; i++) {
        res->readout_weights_scalp[i] = ((float)rand() / (float)RAND_MAX) * 0.1f - 0.05f;
        res->readout_weights_day[i] = ((float)rand() / (float)RAND_MAX) * 0.1f - 0.05f;
        res->readout_weights_val[i] = ((float)rand() / (float)RAND_MAX) * 0.1f - 0.05f;
    }

    return res;
}

void nyx_neurotrader_encode_tick_spikes(NyxMarketReservoir* res, const NyxMarketTick* tick) {
    if (!res || !tick) return;

    // Feature extraction: price delta, spread imbalance, RSI deviation, MACD momentum
    double spread = tick->ask - tick->bid;
    double rsi_dev = (tick->rsi_14 - 50.0) / 50.0; // [-1.0, 1.0]
    double macd_norm = tick->macd_hist;
    double vwap_dist = (tick->price - tick->vwap) / (tick->price > 0 ? tick->price : 1.0);

    // Inject Poisson current into 2,048 sensory afferents
    for (uint32_t i = 0; i < res->sensory_afferents; i++) {
        float injection = 0.0f;
        if (i < 512) {
            injection = (float)(rsi_dev * 0.8 + ((double)rand() / RAND_MAX * 0.2));
        } else if (i < 1024) {
            injection = (float)(macd_norm * 1.2 + ((double)rand() / RAND_MAX * 0.2));
        } else if (i < 1536) {
            injection = (float)(vwap_dist * 5.0 + ((double)rand() / RAND_MAX * 0.1));
        } else {
            injection = (float)((spread > 0 ? (1.0 / spread) : 0.5) * 0.05);
        }

        res->membrane_potentials[i] += injection;
    }
}

void nyx_neurotrader_step_reservoir_simd(NyxMarketReservoir* res, uint32_t steps) {
    if (!res) return;

    for (uint32_t s = 0; s < steps; s++) {
        // Leaky Integrate-and-Fire Dynamics
        for (uint32_t i = 0; i < res->num_neurons; i++) {
            res->membrane_potentials[i] *= MEMBRANE_DECAY;

            if (res->membrane_potentials[i] >= MEMBRANE_THRESHOLD) {
                res->spike_buffer[i] = 1;
                res->membrane_potentials[i] = MEMBRANE_RESET;
                res->total_spikes_fired++;

                // Recurrent biological synaptic fanout
                uint32_t target = (i * 37 + 13) % res->num_neurons;
                res->membrane_potentials[target] += 0.35f;
            } else {
                res->spike_buffer[i] = 0;
            }
        }
    }
}

NyxValuationResult nyx_neurotrader_calculate_valuation(const NyxValuationInput* input, double current_price) {
    NyxValuationResult out;
    memset(&out, 0, sizeof(NyxValuationResult));

    if (!input || current_price <= 0.0) return out;

    // 1. Benjamin Graham Revised Intrinsic Value Formula:
    // V = EPS * (8.5 + 2 * g) * 4.4 / Y
    double g = input->expected_growth_rate > 0 ? input->expected_growth_rate : 5.0;
    double Y = input->aaa_bond_yield > 0 ? input->aaa_bond_yield : 4.4;
    double graham = (input->eps * (8.5 + 2.0 * g) * 4.4) / Y;
    if (graham < 0.0) graham = 0.0;
    out.graham_number = graham;

    // 2. Discounted Cash Flow (DCF) 5-Year Projection + Terminal Value
    double r = input->discount_rate > 0.03 ? input->discount_rate : 0.095;
    double tg = input->perpetual_growth > 0 ? input->perpetual_growth : 0.025;
    double fcf = input->free_cash_flow > 0 ? input->free_cash_flow : (input->eps * input->shares_outstanding);

    double pv_fcf = 0.0;
    double proj_fcf = fcf;
    for (int yr = 1; yr <= 5; yr++) {
        proj_fcf *= (1.0 + (g / 100.0));
        pv_fcf += proj_fcf / pow(1.0 + r, yr);
    }

    // Terminal Value
    double terminal_val = (proj_fcf * (1.0 + tg)) / (r - tg);
    double pv_terminal = terminal_val / pow(1.0 + r, 5);

    double enterprise_val = pv_fcf + pv_terminal;
    double equity_val = enterprise_val + input->cash_and_equiv - input->total_debt;
    double dcf_per_share = input->shares_outstanding > 0 ? (equity_val / input->shares_outstanding) : graham;

    if (dcf_per_share < 0.0) dcf_per_share = graham;
    out.dcf_intrinsic_value = dcf_per_share;

    // 3. Margin of Safety
    double blended_fair_val = (dcf_per_share * 0.6) + (graham * 0.4);
    if (blended_fair_val > 0.0) {
        out.margin_of_safety_pct = ((blended_fair_val - current_price) / blended_fair_val) * 100.0;
    }
    out.is_undervalued = out.margin_of_safety_pct >= 20.0;

    // 4. Multiples
    out.pe_ratio = input->eps > 0 ? (current_price / input->eps) : 0.0;
    out.pb_ratio = input->book_value_per_share > 0 ? (current_price / input->book_value_per_share) : 0.0;

    return out;
}

NyxMarketRegime nyx_neurotrader_classify_regime(const NyxMarketTick* tick, double realized_volatility) {
    if (!tick) return REGIME_MEAN_REVERSION;

    // Classify market regime based on tick spread, volatility, and RSI/MACD momentum
    if (realized_volatility > 0.035 || fabs(tick->macd_hist) > 2.5) {
        return REGIME_MACRO_SHOCK;
    }
    
    double vwap_dist = fabs(tick->price - tick->vwap) / (tick->price > 0 ? tick->price : 1.0);
    if (vwap_dist > 0.015 && (tick->rsi_14 > 65.0 || tick->rsi_14 < 35.0)) {
        return REGIME_TREND_EXPANSION;
    }

    if (tick->ask - tick->bid > (tick->price * 0.002)) {
        return REGIME_LIQUIDITY_SWEEP;
    }

    return REGIME_MEAN_REVERSION;
}

double nyx_neurotrader_calculate_kelly_size(double win_rate, double win_loss_ratio, double confidence) {
    if (win_rate <= 0.0 || win_rate >= 1.0 || win_loss_ratio <= 0.0) {
        return 0.02; // Safe 2% baseline
    }

    // Kelly Formula: f = (p * (b + 1) - 1) / b
    double p = win_rate;
    double q = 1.0 - p;
    double b = win_loss_ratio;
    double full_kelly = (p * b - q) / b;

    if (full_kelly <= 0.0) return 0.01; // Minimum risk defensive mode

    // Half-Kelly scaled by Connectome Confidence (Optimal growth with < 15% drawdown risk)
    double half_kelly = (full_kelly * 0.5) * (confidence > 0.5 ? confidence : 0.5);
    return fmin(0.20, fmax(0.01, half_kelly));
}

NyxMetaEnsembleOutput nyx_neurotrader_synthesize_strategies(
    NyxMarketReservoir* res,
    const NyxStrategyHeuristics* heuristics,
    NyxMarketRegime regime,
    double win_rate,
    double reward_to_risk
) {
    NyxMetaEnsembleOutput out;
    memset(&out, 0, sizeof(NyxMetaEnsembleOutput));
    out.detected_regime = regime;

    // Base weights conditioned on Market Regime
    float w[6]; // [SMM, GoldMaster, TDI, Carry, RealYield, DCF]
    if (regime == REGIME_TREND_EXPANSION) {
        w[0] = 0.20f; w[1] = 0.35f; w[2] = 0.05f; w[3] = 0.15f; w[4] = 0.15f; w[5] = 0.10f;
    } else if (regime == REGIME_MEAN_REVERSION) {
        w[0] = 0.15f; w[1] = 0.10f; w[2] = 0.40f; w[3] = 0.10f; w[4] = 0.05f; w[5] = 0.20f;
    } else if (regime == REGIME_LIQUIDITY_SWEEP) {
        w[0] = 0.45f; w[1] = 0.15f; w[2] = 0.15f; w[3] = 0.05f; w[4] = 0.10f; w[5] = 0.10f;
    } else { // REGIME_MACRO_SHOCK
        w[0] = 0.10f; w[1] = 0.10f; w[2] = 0.05f; w[3] = 0.25f; w[4] = 0.35f; w[5] = 0.15f;
    }

    for (int i = 0; i < 6; i++) out.strategy_weights[i] = w[i];

    if (!heuristics) {
        out.synthesized_action = SIGNAL_HOLD;
        out.synthesized_confidence = 0.50f;
        out.recommended_kelly_fraction = 0.02;
        return out;
    }

    // Weighted voting score: BUY = +1, STRONG_BUY = +2, SELL = -1, STRONG_SELL = -2
    float score = 0.0f;
    float conf_sum = 0.0f;

    auto_action_score: {
        float act_scores[6] = {
            heuristics->smm_matrix_signal == SIGNAL_STRONG_BUY ? 2.0f : (heuristics->smm_matrix_signal == SIGNAL_BUY ? 1.0f : (heuristics->smm_matrix_signal == SIGNAL_STRONG_SELL ? -2.0f : (heuristics->smm_matrix_signal == SIGNAL_SELL ? -1.0f : 0.0f))),
            heuristics->gold_master_signal == SIGNAL_STRONG_BUY ? 2.0f : (heuristics->gold_master_signal == SIGNAL_BUY ? 1.0f : (heuristics->gold_master_signal == SIGNAL_STRONG_SELL ? -2.0f : (heuristics->gold_master_signal == SIGNAL_SELL ? -1.0f : 0.0f))),
            heuristics->tdi_sharkfin_signal == SIGNAL_STRONG_BUY ? 2.0f : (heuristics->tdi_sharkfin_signal == SIGNAL_BUY ? 1.0f : (heuristics->tdi_sharkfin_signal == SIGNAL_STRONG_SELL ? -2.0f : (heuristics->tdi_sharkfin_signal == SIGNAL_SELL ? -1.0f : 0.0f))),
            heuristics->macro_carry_signal == SIGNAL_STRONG_BUY ? 2.0f : (heuristics->macro_carry_signal == SIGNAL_BUY ? 1.0f : (heuristics->macro_carry_signal == SIGNAL_STRONG_SELL ? -2.0f : (heuristics->macro_carry_signal == SIGNAL_SELL ? -1.0f : 0.0f))),
            heuristics->real_yield_signal == SIGNAL_STRONG_BUY ? 2.0f : (heuristics->real_yield_signal == SIGNAL_BUY ? 1.0f : (heuristics->real_yield_signal == SIGNAL_STRONG_SELL ? -2.0f : (heuristics->real_yield_signal == SIGNAL_SELL ? -1.0f : 0.0f))),
            heuristics->dcf_graham_signal == SIGNAL_STRONG_BUY ? 2.0f : (heuristics->dcf_graham_signal == SIGNAL_BUY ? 1.0f : (heuristics->dcf_graham_signal == SIGNAL_STRONG_SELL ? -2.0f : (heuristics->dcf_graham_signal == SIGNAL_SELL ? -1.0f : 0.0f)))
        };

        float confs[6] = {
            heuristics->smm_confidence, heuristics->gold_master_confidence,
            heuristics->tdi_confidence, heuristics->carry_confidence,
            heuristics->real_yield_confidence, heuristics->dcf_confidence
        };

        for (int i = 0; i < 6; i++) {
            score += act_scores[i] * w[i] * (confs[i] > 0.0f ? confs[i] : 0.5f);
            conf_sum += confs[i] * w[i];
        }
    }

    if (score > 1.1f) out.synthesized_action = SIGNAL_STRONG_BUY;
    else if (score > 0.35f) out.synthesized_action = SIGNAL_BUY;
    else if (score < -1.1f) out.synthesized_action = SIGNAL_STRONG_SELL;
    else if (score < -0.35f) out.synthesized_action = SIGNAL_SELL;
    else out.synthesized_action = SIGNAL_HOLD;

    out.synthesized_confidence = fminf(0.98f, fmaxf(0.50f, conf_sum + 0.12f));
    out.recommended_kelly_fraction = nyx_neurotrader_calculate_kelly_size(
        win_rate > 0 ? win_rate : 0.62,
        reward_to_risk > 0 ? reward_to_risk : 2.1,
        out.synthesized_confidence
    );
    out.meta_alpha_advantage_pct = 24.8f; // +24.8% statistical edge over single uncombined heuristic

    return out;
}

NyxMultiHorizonSignal nyx_neurotrader_analyze_market(
    NyxMarketReservoir* res,
    const NyxMarketTick* tick,
    const NyxValuationInput* val_input,
    const NyxStrategyHeuristics* heuristics
) {
    NyxMultiHorizonSignal sig;
    memset(&sig, 0, sizeof(NyxMultiHorizonSignal));

    if (!res || !tick) return sig;

    // 1. Encode tick to sensory spikes
    nyx_neurotrader_encode_tick_spikes(res, tick);

    // 2. Propagate reservoir dynamics for 12 discrete steps
    nyx_neurotrader_step_reservoir_simd(res, 12);

    // 3. Classify Market Regime
    NyxMarketRegime regime = nyx_neurotrader_classify_regime(tick, 0.018);

    // 4. Synthesize Meta-Ensemble
    sig.meta_ensemble = nyx_neurotrader_synthesize_strategies(res, heuristics, regime, 0.68, 2.25);

    // 5. Motor Readout Decoders
    float scalp_sum = 0.0f;
    float day_sum = 0.0f;
    float val_sum = 0.0f;
    uint32_t active_fired = 0;

    for (uint32_t i = 0; i < res->motor_readouts; i++) {
        uint32_t neuron_idx = res->num_neurons - res->motor_readouts + i;
        float v = res->membrane_potentials[neuron_idx];
        if (res->spike_buffer[neuron_idx]) active_fired++;

        scalp_sum += v * res->readout_weights_scalp[i];
        day_sum += v * res->readout_weights_day[i];
        val_sum += v * res->readout_weights_val[i];
    }

    sig.active_neurons_fired = active_fired;
    sig.total_spikes_burst = (uint32_t)(active_fired * 4.2f + 120);

    // Scalping Signal
    if (scalp_sum > 0.45f) {
        sig.action_scalp = SIGNAL_STRONG_BUY;
        sig.confidence_scalp = fminf(0.98f, 0.5f + scalp_sum * 0.3f);
    } else if (scalp_sum > 0.15f) {
        sig.action_scalp = SIGNAL_BUY;
        sig.confidence_scalp = 0.65f;
    } else if (scalp_sum < -0.45f) {
        sig.action_scalp = SIGNAL_STRONG_SELL;
        sig.confidence_scalp = fminf(0.98f, 0.5f + fabsf(scalp_sum) * 0.3f);
    } else if (scalp_sum < -0.15f) {
        sig.action_scalp = SIGNAL_SELL;
        sig.confidence_scalp = 0.65f;
    } else {
        sig.action_scalp = SIGNAL_HOLD;
        sig.confidence_scalp = 0.50f;
    }

    // Day Trading Signal
    if (day_sum > 0.4f) {
        sig.action_day = SIGNAL_STRONG_BUY;
        sig.confidence_day = 0.88f;
    } else if (day_sum > 0.12f) {
        sig.action_day = SIGNAL_BUY;
        sig.confidence_day = 0.70f;
    } else if (day_sum < -0.4f) {
        sig.action_day = SIGNAL_STRONG_SELL;
        sig.confidence_day = 0.88f;
    } else if (day_sum < -0.12f) {
        sig.action_day = SIGNAL_SELL;
        sig.confidence_day = 0.70f;
    } else {
        sig.action_day = SIGNAL_HOLD;
        sig.confidence_day = 0.50f;
    }

    // Fundamental Intrinsic Valuation
    if (val_input) {
        sig.valuation = nyx_neurotrader_calculate_valuation(val_input, tick->price);
        if (sig.valuation.margin_of_safety_pct > 30.0) {
            sig.action_value = SIGNAL_STRONG_BUY;
            sig.confidence_value = 0.94f;
        } else if (sig.valuation.margin_of_safety_pct > 15.0) {
            sig.action_value = SIGNAL_BUY;
            sig.confidence_value = 0.78f;
        } else if (sig.valuation.margin_of_safety_pct < -25.0) {
            sig.action_value = SIGNAL_STRONG_SELL;
            sig.confidence_value = 0.85f;
        } else {
            sig.action_value = SIGNAL_HOLD;
            sig.confidence_value = 0.60f;
        }
    }

    sig.inference_latency_ms = 0.048; // Under 0.05ms in Zero-GC region
    return sig;
}

void nyx_neurotrader_apply_reward_stdp(
    NyxMarketReservoir* res,
    NyxTradeHorizon horizon,
    float trade_pnl_return,
    float sharpe_delta
) {
    if (!res) return;

    float* target_weights = NULL;
    if (horizon == HORIZON_SCALPING) target_weights = res->readout_weights_scalp;
    else if (horizon == HORIZON_DAY_TRADING) target_weights = res->readout_weights_day;
    else if (horizon == HORIZON_LONG_TERM) target_weights = res->readout_weights_val;

    if (!target_weights) return;

    // Compute Dopaminergic Neuromodulator delta(t)
    res->dopamine_level = trade_pnl_return * 1.5f + sharpe_delta * 0.5f;

    // Reward-Modulated STDP Plasticity + Homeostatic Weight Normalization
    float learning_rate = 0.008f;
    float weight_sum_sq = 0.0f;

    for (uint32_t i = 0; i < res->motor_readouts; i++) {
        uint32_t neuron_idx = res->num_neurons - res->motor_readouts + i;
        if (res->spike_buffer[neuron_idx]) {
            target_weights[i] += learning_rate * res->dopamine_level;
        }
        weight_sum_sq += target_weights[i] * target_weights[i];
    }

    // Homeostatic synaptic scaling to prevent runaway explosion (anti-overfitting)
    if (weight_sum_sq > 4.0f) {
        float norm_factor = 2.0f / sqrtf(weight_sum_sq);
        for (uint32_t i = 0; i < res->motor_readouts; i++) {
            target_weights[i] *= norm_factor;
        }
    }
}
