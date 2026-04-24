#include <stdio.h>
#include <math.h>
#include <string.h>

struct Stock {
    char name[50];
    double past_price[5];
    int history_count;

    double avgReturn;
    double volatility;
    double debtRatio;

    char volRiskLevel[10];
    char debtRiskLevel[10];
    char combinedRiskLevel[10];

    double revenuePast;
    double revenueNow;
    double profit;
    double totalDebt;
    double totalAssets;
    double marketPrice;
    double marketCap;

    double peRatio;
    double revenueGrowth;
    double profitMargin;
    double returnOnEquity;
    char scope[50];
};


double calculate_return(double current, double previous) {
    return (current - previous) / previous;
}

double calculate_mean(double arr[], int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) sum += arr[i];
    return sum / n;
}

double calculate_std_dev(double arr[], int n) {
    double mean = calculate_mean(arr, n);
    double sum_squared_diff = 0.0;
    for (int i = 0; i < n; i++) {
        double diff = arr[i] - mean;
        sum_squared_diff += diff * diff;
    }
    return sqrt(sum_squared_diff / (n - 1));
}

int risk_level_to_int(const char *risk) {
    if (strcmp(risk, "Low") == 0)    return 1;
    if (strcmp(risk, "Medium") == 0) return 2;
    if (strcmp(risk, "High") == 0)   return 3;
    return 0;
}

void assess_volatility_risk(double volatility, char *riskLevel) {
    if (volatility < 0.01)      strcpy(riskLevel, "Low");
    else if (volatility < 0.02) strcpy(riskLevel, "Medium");
    else                        strcpy(riskLevel, "High");
}

void assess_debt_risk(double debtRatio, char *riskLevel) {
    if (debtRatio < 0.3)      strcpy(riskLevel, "Low");
    else if (debtRatio < 0.6) strcpy(riskLevel, "Medium");
    else                      strcpy(riskLevel, "High");
}

void assess_combined_risk(const char *volRisk, const char *debtRisk, char *combinedRisk) {
    int vol  = risk_level_to_int(volRisk);
    int debt = risk_level_to_int(debtRisk);
    int maxRisk = vol > debt ? vol : debt;
    strcpy(combinedRisk, (maxRisk == 1) ? "Low" : (maxRisk == 2) ? "Medium" : "High");
}


void compute_common_metrics(struct Stock *s) {
    double returns[4];
    for (int i = 1; i < 5; i++)
        returns[i - 1] = calculate_return(s->past_price[i], s->past_price[i - 1]);

    s->avgReturn  = calculate_mean(returns, 4);
    s->volatility = calculate_std_dev(returns, 4);
    s->debtRatio  = s->totalDebt / s->totalAssets;

    assess_volatility_risk(s->volatility, s->volRiskLevel);
    assess_debt_risk(s->debtRatio, s->debtRiskLevel);
    assess_combined_risk(s->volRiskLevel, s->debtRiskLevel, s->combinedRiskLevel);
}


void short_term_analysis(struct Stock *s) {
    compute_common_metrics(s);
    printf("\n===== Short-Term Analysis for %s =====\n", s->name);
    printf("Average Return: %.2f%%\n",  s->avgReturn  * 100);
    printf("Volatility    : %.2f%% -- Risk: %s\n", s->volatility * 100, s->volRiskLevel);
    printf("Debt Ratio    : %.2f -- Risk: %s\n",   s->debtRatio,        s->debtRiskLevel);
    printf("Combined Risk : %s\n", s->combinedRiskLevel);
}

void long_term_fundamentals(struct Stock *s) {
    compute_common_metrics(s);

    printf("Enter revenue 5 years ago (crores): "); scanf("%lf", &s->revenuePast);
    printf("Enter current revenue (crores): ");      scanf("%lf", &s->revenueNow);
    printf("Enter net profit (crores): ");           scanf("%lf", &s->profit);
    printf("Enter market price per share: ");        scanf("%lf", &s->marketPrice);
    printf("Enter market cap (crores): ");           scanf("%lf", &s->marketCap);

    s->peRatio       = s->marketCap / s->profit;
    s->revenueGrowth = calculate_return(s->revenueNow, s->revenuePast);
    s->profitMargin  = s->profit / s->revenueNow;
    double equity    = s->totalAssets - s->totalDebt;
    s->returnOnEquity = (equity > 0) ? s->profit / equity : 0.0;

    int score = 0;
    if (s->peRatio >= 10 && s->peRatio <= 25) score++;
    if (s->revenueGrowth >= 0.30)             score++;
    if (s->profitMargin  >= 0.10)             score++;
    if (s->debtRatio < 0.5)                   score++;
    if (s->returnOnEquity >= 0.15)            score++;

    if      (score >= 4) strcpy(s->scope, "Good for long-term hold");
    else if (score >= 2) strcpy(s->scope, "Moderate potential, consider carefully");
    else                 strcpy(s->scope, "Not recommended for long-term hold");

    printf("\n=== Long-Term Fundamental Analysis: %s ===\n", s->name);
    printf("P/E Ratio          : %.2f\n",   s->peRatio);
    printf("Revenue Growth     : %.2f%%\n", s->revenueGrowth * 100);
    printf("Profit Margin      : %.2f%%\n", s->profitMargin  * 100);
    printf("Return on Equity   : %.2f%%\n", s->returnOnEquity * 100);
    printf("Debt Ratio         : %.2f\n",   s->debtRatio);
    printf("Investment Advice  : %s\n",     s->scope);
}

void compare_stocks(struct Stock *s1) {
    compute_common_metrics(s1);
    char   name2[50];
    double perf2[5];

    printf("Enter second stock name: "); scanf("%s", name2);
    printf("Enter 5 past returns for %s:\n", name2);
    for (int i = 0; i < 5; i++) {
        printf("Return %d (as decimal): ", i + 1);
        scanf("%lf", &perf2[i]);
    }

    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0, sumY2 = 0;
    for (int i = 0; i < 5; i++) {
        sumX  += s1->past_price[i];
        sumY  += perf2[i];
        sumXY += s1->past_price[i] * perf2[i];
        sumX2 += s1->past_price[i] * s1->past_price[i];
        sumY2 += perf2[i] * perf2[i];
    }

    double den = sqrt((5 * sumX2 - sumX * sumX) * (5 * sumY2 - sumY * sumY));
    double r   = (den == 0) ? 0 : (5 * sumXY - sumX * sumY) / den;

    printf("\nStock Comparison\n");
    printf("Correlation Coefficient: %.4f\n", r);

    if      (r > 0.7)        printf("Positive correlation - stocks tend to move together\n");
    else if (r < -0.7)       printf("Negative correlation - stocks tend to move in opposite directions\n");
    else if (fabs(r) < 0.3)  printf("Weak / no correlation\n");
    else                     printf("Moderate correlation\n");

    printf("\nRisk Profiles:\n");
    printf("%-15s %-25s %-15s\n", "Stock", "Volatility", "Debt Risk");
    printf("%-15s %.2f%% (%s)               %-15s\n",
           s1->name, s1->volatility * 100, s1->volRiskLevel, s1->debtRiskLevel);
    printf("%-15s %-25s %-15s\n", name2, "N/A", "N/A");
}

void investment_advice(struct Stock s) {
    compute_common_metrics(&s);

    float latest_change = calculate_return(s.past_price[0], s.past_price[1]);
    float sum = 0.0;
    for (int i = 2; i < 5; i++)
        sum += calculate_return(s.past_price[i], s.past_price[i - 1]);
    float avg_change = sum / 3.0;

    printf("\nANALYSIS RESULT for %s:\n", s.name);
    printf("Latest Change: %.2f%%\n",               latest_change * 100);
    printf("Average of Previous Changes: %.2f%%\n", avg_change    * 100);

    if (latest_change > avg_change * 1.5) {
        printf("Recommendation: BUY\n");
        printf("Reason: The latest price increase is significantly above the average momentum.\n");
    } else if (latest_change < avg_change * 0.5) {
        printf("Recommendation: SELL\n");
        printf("Reason: The latest drop is much worse than the average performance.\n");
    } else {
        printf("Recommendation: HOLD\n");
        printf("Reason: The latest change is within a normal range compared to historical changes.\n");
    }

    printf("\nRisk Profile:\n");
    printf("- Volatility Risk: %s\n", s.volRiskLevel);
    printf("- Debt Risk      : %s\n", s.debtRiskLevel);
    printf("- Combined Risk  : %s\n", s.combinedRiskLevel);
}


double linear_regression_predict(double prices[], int n) {
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;

    for (int i = 0; i < n; i++) {
        double x = (double)(i + 1);
        sumX  += x;
        sumY  += prices[i];
        sumXY += x * prices[i];
        sumX2 += x * x;
    }

    double denom = (double)n * sumX2 - sumX * sumX;
    if (fabs(denom) < 1e-9) return prices[n - 1];

    double m = ((double)n * sumXY - sumX * sumY) / denom;
    double b = (sumY - m * sumX) / (double)n;

    return m * (double)(n + 1) + b;
}


void ai_smart_advice(struct Stock *s) {
    compute_common_metrics(s);

    // 1. OLS prediction
    double predicted_price  = linear_regression_predict(s->past_price, 5);
    double current_price    = s->past_price[4];
    double predicted_return = (predicted_price - current_price) / current_price;

    // 2. Sentiment input
    double sentiment;
    printf("\n[AI Module] Enter market sentiment score for %s\n", s->name);
    printf("  (-1.0 = very bearish, 0.0 = neutral, +1.0 = very bullish): ");
    scanf("%lf", &sentiment);
    if (sentiment >  1.0) sentiment =  1.0;
    if (sentiment < -1.0) sentiment = -1.0;

    // 3. Normalise signals to [-1, +1]
    double trend_signal  = predicted_return / 0.10;
    if (trend_signal  >  1.0) trend_signal  =  1.0;
    if (trend_signal  < -1.0) trend_signal  = -1.0;

    double return_signal = s->avgReturn / 0.05;
    if (return_signal >  1.0) return_signal =  1.0;
    if (return_signal < -1.0) return_signal = -1.0;

    double vol_signal    = 1.0 - (s->volatility / 0.02);
    if (vol_signal    >  1.0) vol_signal    =  1.0;
    if (vol_signal    < -1.0) vol_signal    = -1.0;

    double debt_signal   = 1.0 - 2.0 * s->debtRatio;
    if (debt_signal   >  1.0) debt_signal   =  1.0;
    if (debt_signal   < -1.0) debt_signal   = -1.0;

    // 4. Weighted combination
    double ai_score = 0.35 * trend_signal
                    + 0.25 * vol_signal
                    + 0.20 * return_signal
                    + 0.15 * debt_signal
                    + 0.05 * sentiment;

    double confidence = fabs(ai_score) * 100.0;

    // 5. Decision
    const char *recommendation;
    const char *reason;

    if (ai_score > 0.15) {
        recommendation = "BUY";
        reason = "Positive trend prediction, healthy risk metrics, and favourable "
                 "sentiment together produce a strong bullish signal.";
    } else if (ai_score < -0.15) {
        recommendation = "SELL";
        reason = "Negative trend prediction or elevated risk factors outweigh "
                 "positive signals — the model flags downside risk.";
    } else {
        recommendation = "HOLD";
        reason = "Signals are mixed or near-neutral. Insufficient evidence "
                 "to justify a directional move right now.";
    }

    // 6. Display
    printf("  AI PREDICTION ENGINE FOR %s\n", s->name);

    printf("\n[1] Linear Regression Price Prediction (OLS)\n");
    printf("    Current Price (Day 5)  : %.2f\n", current_price);
    printf("    Predicted Price (Day 6): %.2f\n", predicted_price);
    printf("    Expected Return        : %+.2f%%\n", predicted_return * 100);

    printf("\n[2] Multi-Factor AI Scoring Model\n");
    printf("    %-24s %+.3f  (weight 35%%)\n", "Trend Signal:",      trend_signal);
    printf("    %-24s %+.3f  (weight 25%%)\n", "Volatility Signal:", vol_signal);
    printf("    %-24s %+.3f  (weight 20%%)\n", "Return Signal:",     return_signal);
    printf("    %-24s %+.3f  (weight 15%%)\n", "Debt Signal:",       debt_signal);
    printf("    %-24s %+.3f  (weight  5%%)\n", "Sentiment Signal:",  sentiment);
    printf("    ------------------------------------------\n");
    printf("    %-24s %+.4f\n", "AI Score:", ai_score);
    printf("    %-24s %.1f%%\n", "Confidence:", confidence);

    printf("\n>>> AI Recommendation : %s\n", recommendation);
    printf("    Reason: %s\n\n", reason);
}

int main() {
    struct Stock stock1;
    stock1.history_count = 5;

    printf("Enter stock name: ");
    scanf("%s", stock1.name);

    printf("Enter 5 daily prices for %s:\n", stock1.name);
    for (int i = 0; i < 5; i++) {
        printf("Price on Day %d: ", i + 1);
        scanf("%lf", &stock1.past_price[i]);
    }

    printf("Enter total debt (in crores): ");
    scanf("%lf", &stock1.totalDebt);
    printf("Enter total assets (in crores): ");
    scanf("%lf", &stock1.totalAssets);

    int choice;
    do {
        printf("\n====== STOCK ANALYSIS MENU ======\n");
        printf("1. Short-Term Risk Analysis\n");
        printf("2. Long-Term Fundamental Evaluation\n");
        printf("3. Compare with Another Stock (Correlation)\n");
        printf("4. Investment Advice\n");
        printf("5. AI Price Prediction & Smart Advice  [AI]\n");
        printf("6. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: short_term_analysis(&stock1);   break;
            case 2: long_term_fundamentals(&stock1); break;
            case 3: compare_stocks(&stock1);         break;
            case 4: investment_advice(stock1);       break;
            case 5: ai_smart_advice(&stock1);        break;
            case 6: printf("Exiting...\n");          break;
            default: printf("Invalid option. Try again.\n");
        }
    } while (choice != 6);

    return 0;
}
