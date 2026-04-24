# Stock Market Analytics with C — AI Enhanced

A command-line stock market analytics system written in C, with an **AI prediction engine** using Linear Regression and a Multi-Factor Scoring Model.

## Features

| # | Feature | Type |
|---|---------|------|
| 1 | Short-Term Risk Analysis | Original |
| 2 | Long-Term Fundamental Evaluation | Original |
| 3 | Stock Correlation Comparison | Original |
| 4 | Rule-Based Investment Advice | Original |
| 5 | AI Price Prediction & Smart Advice|


### Linear Regression — OLS Price Prediction
Fits a least-squares trend line to the 5 historical prices and predicts the next day's price, computing the expected return.

### Weighted Multi-Factor Scoring Model
Normalises five financial signals to a [-1, +1] range and combines them using empirically chosen weights:

| Signal | Weight | Description |
|--------|--------|-------------|
| Trend (OLS) | 35% | Predicted price momentum |
| Volatility | 25% | Risk-adjusted penalty |
| Avg Return | 20% | Historical performance |
| Debt Ratio | 15% | Balance-sheet health |
| Sentiment | 5% | User-provided market mood |

The weighted AI Score drives a BUY / HOLD / SELL recommendation with a confidence percentage.

## Tech Stack
- **Language:** C
- **AI/ML:** Linear Regression (OLS), Weighted Scoring Model
- **Math:** Standard C `<math.h>`

## Purpose
To help users make more informed investment decisions using automated statistical analysis and AI-driven recommendations.
