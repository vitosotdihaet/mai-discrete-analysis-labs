/*
Найти самую длинную общую подстроку двух строк с использованием суфф. дерева.

Формат ввода
    Две строки.

Формат вывода
    На первой строке нужно распечатать длину максимальной общей подстроки,
затем перечислить все возможные варианты общих подстрок этой длины
в порядке лексикографического возрастания без повторов. 

Пример
    Ввод
        xabay
        xabcbay

        
    Вывод
        3
        bay
        xab
*/


#include <iostream>

#include <string>
#include <map>
#include <vector>

#include <algorithm>

#include <memory>
#include <stdexcept>

// c++ is a fucking joke

struct SuffixTreeNode;
struct SuffixTreeEdge;


struct SuffixTreeNode {
    static size_t currentMaxSuffixIndex;
    // chars to edges
    std::map<char, std::shared_ptr<SuffixTreeEdge>> next;
    std::shared_ptr<SuffixTreeNode> suffixLink = nullptr;
    size_t id = 0;

    SuffixTreeNode();
    SuffixTreeNode(const char &value, const size_t &start, const std::shared_ptr<size_t> &end);
    void addEdge(const char &value, const size_t &start, const std::shared_ptr<size_t> &end);
    void addEdge(const std::shared_ptr<SuffixTreeNode> &node, const char &value, const size_t &start, const std::shared_ptr<size_t> &end);
    std::shared_ptr<SuffixTreeNode> setSuffixLink(const std::shared_ptr<SuffixTreeNode> &node);

    friend std::ostream& operator<<(std::ostream &os, const SuffixTreeNode &node);
};

size_t SuffixTreeNode::currentMaxSuffixIndex = 0;



struct SuffixTreeEdge {
    size_t start = 0;
    std::shared_ptr<size_t> end = nullptr;
    // edge ends in this node
    std::shared_ptr<SuffixTreeNode> node = nullptr;

    SuffixTreeEdge(const size_t &start, const std::shared_ptr<size_t> &end) : start(start), end(end), node(std::make_shared<SuffixTreeNode>()) {}
    // split the edge at this->start + length (0 < length < *this->end - this->start), returns shared pointer to a new internal node
    std::shared_ptr<SuffixTreeNode> split(const size_t &length, const char &newChar, const char &differentChar);
    size_t getLength();

    friend std::ostream& operator<<(std::ostream &os, const SuffixTreeEdge &edge);
};





// implementation of SuffixTreeNode
SuffixTreeNode::SuffixTreeNode() {}

SuffixTreeNode::SuffixTreeNode(const char &value, const size_t &start, const std::shared_ptr<size_t> &end) {
    this->next[value] = std::make_shared<SuffixTreeEdge>(start, end);
}

void SuffixTreeNode::addEdge(const char &value, const size_t &start, const std::shared_ptr<size_t> &end) {
    this->next[value] = std::make_shared<SuffixTreeEdge>(start, end);
}

void SuffixTreeNode::addEdge(const std::shared_ptr<SuffixTreeNode> &node, const char &value, const size_t &start, const std::shared_ptr<size_t> &end) {
    std::shared_ptr<SuffixTreeEdge> newEdge = std::make_shared<SuffixTreeEdge>(start, end);
    newEdge->node = node;
    this->next[value] = newEdge;
}

std::shared_ptr<SuffixTreeNode> SuffixTreeNode::setSuffixLink(const std::shared_ptr<SuffixTreeNode> &node) {
    this->suffixLink = node;
    return node;
}

std::ostream& operator<<(std::ostream &os, const SuffixTreeNode &node) {
    os << "next:\n";
    for (const auto& [key, value] : node.next) {
        os << "\t" << key << " --- " << *value << '\n';
    }
    os << "suffixLink: " << node.suffixLink;
    return os;
}




// implementation of SuffixTreeEdge
std::shared_ptr<SuffixTreeNode> SuffixTreeEdge::split(const size_t &length, const char &newChar, const char &differentChar) {
    if (length == 0 || length >= this->getLength()) {
        throw std::invalid_argument("length is greater than the substring's length");
    }

    const size_t stringSplitIndex = this->start + length;

    std::shared_ptr<size_t> newEnd = std::make_shared<size_t>(stringSplitIndex);

    std::shared_ptr<SuffixTreeNode> internalNode = std::make_shared<SuffixTreeNode>(newChar, *this->end - 1, this->end); // node with the new char
    internalNode->next[newChar]->node->id = SuffixTreeNode::currentMaxSuffixIndex++;
    internalNode->addEdge(this->node, differentChar, stringSplitIndex, this->end); // old node

    this->node = internalNode;
    this->end = newEnd;

    return internalNode;
}

size_t SuffixTreeEdge::getLength() {
    return *this->end - this->start;
}

std::ostream& operator<<(std::ostream &os, const SuffixTreeEdge &edge) {
    os << ' ' << edge.start << ":" << *edge.end << " (" << edge.end << ") node: " << edge.node;
    return os;
}


class SuffixTree {
private:
    std::string inputString;
    std::shared_ptr<SuffixTreeNode> root;

    std::shared_ptr<size_t> end = std::make_shared<size_t>(0);


private:
    void buildRuleOne() {
        (*this->end)++;
    }

    void build() {
        const size_t inputStringLength = this->inputString.length();

        // build I_0
        this->root->addEdge(inputString[0], 0, this->end);
        this->buildRuleOne();

        // build I_i
        for (size_t phase = 1; phase < inputStringLength; ++phase) {
            this->buildRuleOne();
        }
    }

public:
    SuffixTree(const std::string &inputString) {
        this->inputString = inputString + '$';
        this->build();
    }

    std::pair<std::vector<size_t>, size_t> findMaxSubstrings(const std::string &pattern) {
        const size_t patternLength = pattern.length();

        std::vector<size_t> indecies;
        size_t maxSubstringLength = 0;

        // TODO: implement the algorithm
        for (size_t suffixIndex = 0; suffixIndex < patternLength; ++suffixIndex) { // search for ith pattern's suffix via dfs from root
            size_t index;
            for (index = suffixIndex; index < patternLength; ++index) {
                const size_t substringLength = index - suffixIndex;
                const char currentChar = pattern[index];
            }
        }

        return std::make_pair(indecies, maxSubstringLength);
    }
};


int main() {
    std::string text, pattern;
    std::cin >> text >> pattern;

    if (text.length() < pattern.length()) {
        std::string temp = std::move(text);
        text = std::move(pattern);
        pattern = std::move(temp);
    }

    SuffixTree st(text);

    std::pair<std::vector<size_t>, size_t> foundStrings = st.findMaxSubstrings(pattern);
    size_t length = foundStrings.second;
    if (length == 0) { 
        return 0;
    }

    std::vector<size_t> &starts = foundStrings.first;

    std::vector<std::string> substrings;
    substrings.reserve(starts.size());
    for (size_t start : starts) {
        substrings.push_back(pattern.substr(start, length));
    }

    std::sort(substrings.begin(), substrings.end());    
    substrings.erase(std::unique(substrings.begin(), substrings.end()), substrings.end());

    std::cout << length << '\n';
    for (std::string &substring : substrings) {
        std::cout << substring << '\n';
    }
}