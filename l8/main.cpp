#include <iostream>
#include <vector>


template <typename T>
T power(T base, T exponent) {
    T result = 1;
    for (T _ = 1; _ < exponent; ++_) {
        result *= base;
    }
    return result;
}


std::vector<uint64_t> dynamic(const std::vector<uint64_t> &coins, const uint64_t amount) {
    const size_t coinsSize = coins.size();

    // vector of pairs: count of all coins and a vector of count of each coin
    std::vector<std::pair<uint64_t, std::vector<uint64_t>>> dp(amount + 1, std::pair<uint64_t, std::vector<uint64_t>>(0, std::vector<uint64_t>(coinsSize, 0)));

    for (size_t i = 0; i < amount + 1; ++i) {

    }

    return dp[amount].second;
}

std::vector<uint64_t> greedy(const std::vector<uint64_t> &coins, uint64_t amount) {
    const size_t coinsSize = coins.size();
    size_t currentCoinIndex = coinsSize - 1;

    std::vector<uint64_t> result(coinsSize, 0);

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
    uint64_t exponent, base, amount;
    std::cin >> exponent >> base >> amount;
    // exponent = 3, base = 5, amount = 71;

    std::vector<std::uint64_t> coins(exponent);
    for (size_t currentExponent = 1; currentExponent <= exponent; ++currentExponent) {
        coins[currentExponent - 1] = power(base, currentExponent);
    }

    std::vector<uint64_t> result = dynamic(coins, amount);
    // std::vector<uint64_t> result = greedy(coins, amount);

    for (size_t i = 0; i < exponent; ++i) {
        std::cout << result[i] << '\n';
    }

    return 0;
}