/*
Вариант алгоритма: Поиск большого количества образцов при помощи алгоритма Ахо-Корасик.
Вариант алфавита: Числа в диапазоне от 0 до 2^32 - 1.

Формат ввода

Образцы задаются по одному на строку вплоть до пустой строки.
Затем следует текст, состоящий из слов или чисел, в котором нужно найти заданные образцы.
Никаких ограничений на длину строк, равно как и на количество слов или чисел в них, не накладывается.

Формат вывода

В выходной файл нужно вывести информацию о всех вхождениях искомых образцов в обрабатываемый текст: по одному вхождению на строку.
На каждое вхождение нужно вывести три числа через запятую: номер строки; номер слова в строке, с которого начинается найденный образец; порядковый номер образца.
Нумерация начинается с единицы. Номер строки в тексте должен отсчитываться от его реального начала (без учёта строк, занятых образцами).
Порядок следования вхождений образцов несущественен.

Пример

Ввод
1 0002 001 2
1 2 1
2 02 2 2

01 2 1 2   1 2 1 3
2 2 2 2 02

Вывод
1, 1, 1
1, 1, 2
1, 3, 1
1, 3, 2
1, 5, 2
2, 1, 3
2, 2, 3
*/



#include <iostream>
#include <sstream>

#include <string>
#include <vector>
#include <queue>
#include <map>
#include <tuple>

#include <algorithm>

#include <cstdint>

static size_t global_id = 0;

template <typename T>
class TrieNode {
    private:
        T value = 0;
        bool wordEnded = false;
        std::map<T, TrieNode<T>*> children;
        TrieNode<T>* parent = nullptr;
        TrieNode<T>* failureLink = nullptr;
        TrieNode<T>* endingLink = nullptr;
        size_t id = 0;

        void print(std::ostream& os, const std::string& prefix) const {
            os << prefix << this << ": " << value;
            if (wordEnded) os << "$";
            os << " fail: " << failureLink << " end: " << endingLink << '\n';
            for (const auto& [symbol, child] : children) {
                child->print(os, prefix + " ");
            }
        }

    public:
        TrieNode(T value) : value(value) {};
        ~TrieNode() {
            for (auto [symbol, child] : children) {
                delete child;
            }
        }

        void add(const std::vector<T>& word) {
            TrieNode<T> *currentNode = this;

            // iterate over all symbols in a word and insert them
            for (size_t i = 0; i < word.size(); ++i) {
                T currentValue = word[i];

                auto it = currentNode->children.find(currentValue);
                if (it == currentNode->children.end()) { // no such path, need to create a new node
                    TrieNode<T> *newNode = new TrieNode<T>(currentValue);
                    newNode->parent = currentNode;

                    currentNode->children.insert({ currentValue, newNode });
                    currentNode = newNode;
                } else { // found a path to this symbol
                    currentNode = it->second;
                }
            }

            currentNode->wordEnded = true;
            currentNode->id = global_id++;
        }

        std::vector<std::tuple<size_t, size_t, size_t>> search(const std::vector<std::vector<T>> &text) {
            std::vector<std::tuple<size_t, size_t, size_t>> results;

            for (size_t lineIndex = 0; lineIndex < text.size(); ++lineIndex) {
                std::vector<T> line = text[lineIndex];
                // reset trie
                TrieNode<T> *currentNode = this;

                // traverse the current line
                for (size_t onLineIndex = 0; onLineIndex < line.size(); ++onLineIndex) {
                    T currentValue = line[onLineIndex];

                    #ifdef DEBUG
                    std::cout << "at " << lineIndex << " " << onLineIndex << ": " << currentValue << " currentNode =\n" << *currentNode;
                    #endif

                    auto it = currentNode->children.find(currentValue);

                    if (it != currentNode->children.end()) {
                        #ifdef DEBUG
                        std::cout << "DAM I FOUND SMTH!\n\n";
                        #endif

                        currentNode = it->second;

                        // check if any words end here
                        TrieNode<T> *tempNode = currentNode;
                        while (tempNode->wordEnded) {
                            size_t charsCounter = 0;
                            TrieNode<T> *parent = tempNode;
                            while (parent->parent) {
                                parent = parent->parent;
                                charsCounter++;
                            }

                            results.push_back({lineIndex, onLineIndex - charsCounter + 1, tempNode->id});
                            if (tempNode->endingLink == tempNode) break;
                            tempNode = tempNode->endingLink;
                        }
                    } else {
                        #ifdef DEBUG
                        std::cout << "EPIC FAIL!\n\n";
                        #endif

                        if (currentNode == this) continue;

                        // follow the failure link
                        currentNode = currentNode->failureLink;
                        // go back once, otherwise you skip this shit
                        onLineIndex = std::max<size_t>(0, onLineIndex - 1);

                        // // check if any words end here
                        // TrieNode<T> *tempNode = currentNode;
                        // while (tempNode->wordEnded) {
                        //     size_t charsCounter = 0;
                        //     TrieNode<T> *parent = tempNode;
                        //     while (parent->parent) {
                        //         parent = parent->parent;
                        //         charsCounter++;
                        //     }

                        //     results.push_back({lineIndex, onLineIndex - charsCounter + 1, tempNode});
                        //     if (tempNode->endingLink == tempNode) break;
                        //     tempNode = tempNode->endingLink;
                        // }
                    }
                }
            }

            return results;
        }

        void buildFailureLinks() {
            std::queue<TrieNode<T>*> q;

            for (auto& [symbol, child] : children) {
                child->failureLink = this;
                q.push(child);
            }

            // traverse the trie in BFS manner to set failure links
            while (!q.empty()) {
                TrieNode<T> *currentNode = q.front();
                q.pop();

                for (auto& [symbol, child] : currentNode->children) {
                    TrieNode<T> *failureTarget = currentNode->failureLink;

                    // find the deepest node which is a suffix of the current node's path
                    while (failureTarget != nullptr && failureTarget->children.find(symbol) == failureTarget->children.end()) {
                        failureTarget = failureTarget->failureLink;
                    }

                    if (failureTarget == nullptr) { // if no such node is found, set failure link to root
                        child->failureLink = this;
                    } else { // otherwise, set failure link to the child node found
                        child->failureLink = failureTarget->children[symbol];
                    }

                    q.push(child);
                }
            }
        }

        void buildEndingLinks() {
            std::queue<TrieNode<T>*> q;

            for (auto& [symbol, child] : children) {
                child->endingLink = nullptr;
                q.push(child);
            }

            // traverse the trie in BFS manner to set ending links
            while (!q.empty()) {
                TrieNode<T> *currentNode = q.front();
                q.pop();

                if (currentNode->failureLink->wordEnded) { // word end in a failure link
                    currentNode->endingLink = currentNode->failureLink;
                } else if (currentNode->failureLink->endingLink) { // there is an ending link in a failure link
                    currentNode->endingLink = currentNode->failureLink->endingLink;
                } else if (currentNode->wordEnded) { // current node is a word end, set its ending link to itself
                    currentNode->endingLink = currentNode;
                }

                for (auto& [symbol, child] : currentNode->children) {
                    q.push(child);
                }
            }
        }

        friend std::ostream& operator<<(std::ostream& os, const TrieNode& node) {
            node.print(os, "");
            return os;
        }
};

template <typename T>
class Trie : public TrieNode<T> {
    public:
        Trie() : TrieNode<T>(0) {};
};


template <typename T>
class AhoCorasick {
    private:
        Trie<T> trie;

    public:
        AhoCorasick() {};
        AhoCorasick(const std::vector<std::vector<T>> &patterns) {
            for (std::vector<T> pattern : patterns) {
                trie.add(pattern);
            }
            trie.buildFailureLinks();
            trie.buildEndingLinks();
        }

        std::vector<std::tuple<size_t, size_t, size_t>> search(const std::vector<std::vector<T>> &text) {
            std::vector<std::tuple<size_t, size_t, size_t>> results = trie.search(text);
            std::vector<std::tuple<size_t, size_t, size_t>> mapped_results;

            for (auto e : results) {
                mapped_results.push_back({std::get<0>(e) + 1, std::get<1>(e) + 1, std::get<2>(e) + 1});
            }

            return mapped_results;
        }

        friend std::ostream& operator<<(std::ostream& os, const AhoCorasick& ahoCorasick) {
            os << ahoCorasick.trie;
            return os;
        }
};


template <typename T>
std::vector<T> parseLine(const std::string& line) {
    std::vector<T> numbers;
    std::stringstream ss(line);
    std::string numberStr;

    while (ss >> numberStr) {
        T number = std::stoull(numberStr);
        numbers.push_back(number);
    }

    return numbers;
}


int main() {
    std::string line;

    // read patterns
    std::vector<std::vector<uint32_t>> patterns;
    while (getline(std::cin, line) && !line.empty()) {
        patterns.push_back(parseLine<uint32_t>(line));
    }

    // read text
    std::vector<std::vector<uint32_t>> text;
    while (getline(std::cin, line)) {
        text.push_back(parseLine<uint32_t>(line));
    }

    AhoCorasick<uint32_t> ahoCorasick(patterns);
    #ifdef DEBUG
    std::cout << ahoCorasick << '\n';
    #endif
    auto results = ahoCorasick.search(text);

    for (const auto& result : results) {
        std::cout << std::get<0>(result) << ", " << std::get<1>(result) << ", " << std::get<2>(result) << '\n';
    }

    return 0;
}