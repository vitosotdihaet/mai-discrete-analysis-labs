#include <iostream>
#include <vector>
#include <limits>



template <typename T>
T power(const T base, const T exponent) {
    T result = 1;
    for (T _ = 1; _ < exponent; ++_) {
        result *= base;
    }
    return result;
}



std::vector<uint32_t> dynamicOptimizedForTime(const std::vector<uint64_t> &coins, const uint32_t amount) {
    const size_t coinsSize = coins.size();

    // vector of pairs: count of coins and the last added coin for current amount
    std::vector<std::pair<uint32_t, uint64_t>> dp(amount + 1, std::pair<uint32_t, uint64_t>(amount, 0));
    dp[0].first = 0;

    for (uint32_t currentAmount = 0; currentAmount <= amount; ++currentAmount) {
        for (size_t currentCoinIndex = 0; currentCoinIndex < coinsSize; ++currentCoinIndex) {
            const uint64_t currentCoin = coins[currentCoinIndex];

            if (currentAmount + currentCoin > amount) break;

            if (dp[currentAmount].first + 1 < dp[currentAmount + currentCoin].first) {
                dp[currentAmount + currentCoin].first = dp[currentAmount].first + 1;
                dp[currentAmount + currentCoin].second = currentCoinIndex;
            }
        }
    }

    std::vector<uint32_t> result(coinsSize, 0);

    size_t currentAmount = amount;
    while (currentAmount != 0) {
        const size_t currentCoinIndex = dp[currentAmount].second;
        const uint64_t currentCoin = coins[currentCoinIndex];
        result[currentCoinIndex]++;
        currentAmount -= currentCoin;
    }

    return result;
}


std::vector<uint32_t> dynamicOptimizedForStorage(const std::vector<uint64_t> &coins, const uint32_t amount) {
    const size_t coinsSize = coins.size();

    // vector of count of coins for current amount
    std::vector<uint32_t> dp(amount + 1, amount);
    dp[0] = 0;

    for (uint32_t currentAmount = 0; currentAmount <= amount; ++currentAmount) {
        for (size_t currentCoinIndex = 0; currentCoinIndex < coinsSize; ++currentCoinIndex) {
            const uint64_t currentCoin = coins[currentCoinIndex];

            if (currentAmount + currentCoin > amount) break;

            dp[currentAmount + currentCoin] = std::min(dp[currentAmount] + 1, dp[currentAmount + currentCoin]);
        }
    }

    std::vector<uint32_t> result(coinsSize, 0);

    size_t currentAmount = amount;
    while (currentAmount != 0) {
        for (size_t currentCoinIndex = 0; currentCoinIndex < coinsSize; ++currentCoinIndex) {
            const uint64_t currentCoin = coins[currentCoinIndex];

            if (currentAmount < currentCoin) break;

            if (dp[currentAmount - currentCoin] + 1 == dp[currentAmount]) {
                currentAmount -= currentCoin;
                result[currentCoinIndex]++;
            }
        }
    }

    return result;
}


std::vector<uint32_t> greedy(const std::vector<uint64_t> &coins, uint32_t amount) {
    const size_t coinsSize = coins.size();
    size_t currentCoinIndex = coinsSize - 1;

    std::vector<uint32_t> result(coinsSize, 0);

    while (amount != 0) {
        uint64_t currentCoin = coins[currentCoinIndex];
        while (amount < currentCoin) {
            currentCoin = coins[--currentCoinIndex];
        }

        result[currentCoinIndex] = amount / currentCoin;
        amount = amount % currentCoin;
    }

    return result;
}



int main() {
    uint64_t exponent, base;
    uint32_t amount;

    std::cin >> exponent >> base >> amount;

    // populate coins
    std::vector<std::uint64_t> coins(exponent);
    for (size_t currentExponent = 1; currentExponent <= exponent; ++currentExponent) {
        coins[currentExponent - 1] = power(base, currentExponent);
    }

    std::vector<uint32_t> result = dynamicOptimizedForTime(coins, amount);
    // std::vector<uint32_t> result = dynamicOptimizedForStorage(coins, amount);
    // std::vector<uint32_t> result = greedy(coins, amount);

    for (uint32_t count : result) {
        std::cout << count << '\n';
    }

    return 0;
}