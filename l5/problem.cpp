// leetcode 208


#include <iostream>

#include <string>
#include <unordered_map>
#include <set>
#include <vector>

#include <algorithm>

#include <memory>
#include <stdexcept>





struct SuffixTreeNode;
struct SuffixTreeEdge;



struct SuffixTreeNode {
    static int64_t currentMaxSuffixIndex;
    // chars to edges
    std::unordered_map<uint64_t, std::shared_ptr<SuffixTreeEdge>> next;
    std::shared_ptr<SuffixTreeNode> suffixLink = nullptr;
    size_t id = 0;

    SuffixTreeNode();
    SuffixTreeNode(const uint64_t value, const size_t start, const std::shared_ptr<size_t> &end);
    void addEdge(const uint64_t value, const size_t start, const std::shared_ptr<size_t> &end);
    void addEdge(const std::shared_ptr<SuffixTreeNode> &node, const uint64_t value, const size_t start, const std::shared_ptr<size_t> &end);
    std::shared_ptr<SuffixTreeNode> setSuffixLink(const std::shared_ptr<SuffixTreeNode> &node);

    bool empty();

    friend std::ostream& operator<<(std::ostream &os, const SuffixTreeNode &node);
};

int64_t SuffixTreeNode::currentMaxSuffixIndex = 0;



struct SuffixTreeEdge {
    size_t start = 0;
    std::shared_ptr<size_t> end = nullptr;
    // edge ends in this node
    std::shared_ptr<SuffixTreeNode> node = nullptr;

    SuffixTreeEdge(const size_t start, const std::shared_ptr<size_t> &end);
    // split the edge at this->start + length (0 < length < *this->end - this->start), returns shared pointer to a new internal node
    std::shared_ptr<SuffixTreeNode> split(const std::shared_ptr<size_t> &currentEnd, const size_t length, const uint64_t newChar, const uint64_t differentChar);
    size_t getLength();

    friend std::ostream& operator<<(std::ostream &os, const SuffixTreeEdge &edge);
};






// implementation of SuffixTreeNode
SuffixTreeNode::SuffixTreeNode() {}

SuffixTreeNode::SuffixTreeNode(const uint64_t value, const size_t start, const std::shared_ptr<size_t> &end) {
    this->next[value] = std::make_shared<SuffixTreeEdge>(start, end);
}

void SuffixTreeNode::addEdge(const uint64_t value, const size_t start, const std::shared_ptr<size_t> &end) {
    this->next[value] = std::make_shared<SuffixTreeEdge>(start, end);
    this->next[value]->node->id = SuffixTreeNode::currentMaxSuffixIndex++;
}

void SuffixTreeNode::addEdge(const std::shared_ptr<SuffixTreeNode> &node, const uint64_t value, const size_t start, const std::shared_ptr<size_t> &end) {
    std::shared_ptr<SuffixTreeEdge> newEdge = std::make_shared<SuffixTreeEdge>(start, end);
    newEdge->node = node;
    this->next[value] = newEdge;
}

std::shared_ptr<SuffixTreeNode> SuffixTreeNode::setSuffixLink(const std::shared_ptr<SuffixTreeNode> &node) {
    this->suffixLink = node;
    return node;
}

bool SuffixTreeNode::empty() {
    return this->next.empty();
}

std::ostream& operator<<(std::ostream &os, const SuffixTreeNode &node) {
    os << "id: " << node.id << '\n';
    os << "suffixLink: " << node.suffixLink << '\n';
    os << "next:\n";
    for (const auto& [key, value] : node.next) {
        os << "\t" << key << " --- " << *value << '\n';
    }
    return os;
}


// implementation of SuffixTreeEdge
SuffixTreeEdge::SuffixTreeEdge(const size_t start, const std::shared_ptr<size_t> &end) : start(start), end(end), node(std::make_shared<SuffixTreeNode>()) {}

std::shared_ptr<SuffixTreeNode> SuffixTreeEdge::split(const std::shared_ptr<size_t> &currentEnd, const size_t length, const uint64_t newChar, const uint64_t differentChar) {
    if (length == 0 || length >= this->getLength()) {
        throw std::invalid_argument("length is greater than the substring's length");
    }

    std::shared_ptr<SuffixTreeNode> internalNode = std::make_shared<SuffixTreeNode>(newChar, *currentEnd - 1, currentEnd); // node with the new uint64_t
    internalNode->next[newChar]->node->id = SuffixTreeNode::currentMaxSuffixIndex++;
    internalNode->addEdge(this->node, differentChar, this->start + length, this->end); // old node

    this->node = internalNode;
    this->end = std::make_shared<size_t>(this->start + length);

    return internalNode;
}

size_t SuffixTreeEdge::getLength() {
    return *this->end - this->start;
}

std::ostream& operator<<(std::ostream &os, const SuffixTreeEdge &edge) {
    os << ' ' << edge.start << ":" << *edge.end << " (" << edge.end << ") node: " << *edge.node;
    return os;
}




class Trie {
private:
    std::vector<uint64_t> inputString;
    size_t inputStringLength = 0;
    
    std::vector<size_t> stringStarts;

    std::shared_ptr<SuffixTreeNode> root = std::make_shared<SuffixTreeNode>();

    std::shared_ptr<SuffixTreeNode> activeNode = this->root;
    std::shared_ptr<SuffixTreeEdge> activeEdge = nullptr;
    size_t activeLength = 0;
    size_t depth = 0;

    size_t remainder = 0;

    uint64_t sentinel = 'z' + 1;

    std::shared_ptr<size_t> end = std::make_shared<size_t>(0);

private:
    //! rule 1
    void ruleOne(const size_t currentCharIndex) {
        if (this->activeNode != this->root || this->depth == 0) {
            throw std::logic_error("activeNode is not root or activeLength is zero");
        }

        uint64_t nextRemainingChar = this->inputString[currentCharIndex - this->remainder + 1];

        // apply the rule
        this->depth--;

        const auto entry = this->activeNode->next.find(nextRemainingChar);
        if (entry != this->activeNode->next.end()) {
            this->activeEdge = entry->second;
            this->activeLength = this->depth;
            this->canonicize(currentCharIndex);
        } else {
            this->activeLength = 0;
            this->activeEdge = nullptr;
        }
    }

    //! rule 3
    void ruleThree(const size_t currentCharIndex) {
        if (this->activeNode == this->root) {
            throw std::logic_error("activeNode is root");
        }

        uint64_t nextRemainingChar = '\0';

        if (this->remainder - 1 > this->activeLength) {
            nextRemainingChar = this->inputString[currentCharIndex - this->activeLength];
        } else {
            nextRemainingChar = this->inputString[currentCharIndex - this->remainder + 1];
        }


        // apply the rule
        if (this->activeNode->suffixLink != nullptr) {
            this->activeNode = this->activeNode->suffixLink;
            this->depth--;
        } else {
            this->activeNode = this->root;
            this->ruleOne(currentCharIndex);
            return;
        }

        const auto entry = this->activeNode->next.find(nextRemainingChar);
        if (entry != this->activeNode->next.end()) {
            this->activeEdge = entry->second;
            this->canonicize(currentCharIndex);
        } else {
            this->activeEdge = nullptr;
        }
    }

    // rule 1 or 3
    void ruleOneOrThree(const size_t currentCharIndex) {
        if (this->activeNode == this->root && this->activeLength != 0) { //! rule 1
            this->ruleOne(currentCharIndex);
        } else if (this->activeNode != this->root) { //! rule 3
            this->ruleThree(currentCharIndex);
        }
    }

    //! observation 2
    void canonicize(const size_t currentCharIndex) {
        size_t activeEdgeLength = this->activeEdge->getLength();

        while (this->activeLength >= activeEdgeLength) { // substring is too smol, go further through the edges
            this->activeNode = this->activeEdge->node;
            this->activeLength = this->activeLength - activeEdgeLength;

            if (this->activeLength == 0) {
                this->activeEdge = nullptr;
                return;
            } else {
                this->activeEdge = this->activeNode->next[inputString[currentCharIndex - this->activeLength]];
                activeEdgeLength = this->activeEdge->getLength();
            }
        }
    }

    // explicitly add all suffixes
    void buildSuffixLinks(std::shared_ptr<SuffixTreeNode> &previousInternalNode, const size_t currentCharIndex) {
        const uint64_t currentChar = this->inputString[currentCharIndex];

        while (this->remainder > 0) {
            if (this->activeLength == 0) { // inserting new edge to the active node

                const auto entry = this->activeNode->next.find(currentChar); //* i HATE performance
                if (entry != this->activeNode->next.end()) {
                    this->activeEdge = entry->second;
                    this->activeLength++;
                    this->depth++;
                    this->canonicize(currentCharIndex);
                    break;
                }

                this->activeNode->addEdge(currentChar, currentCharIndex, this->end);

                if (this->activeNode != this->root) { //! rule 2
                    previousInternalNode = previousInternalNode->setSuffixLink(this->activeNode);
                }

                this->remainder--;

            } else { // splitting active edge

                const uint64_t checkedChar = this->inputString[this->activeEdge->start + this->activeLength];
                if (checkedChar == currentChar) {
                    break;
                }

                std::shared_ptr<SuffixTreeNode> currentInternalNode = this->activeEdge->split(this->end, this->activeLength, currentChar, checkedChar);
                previousInternalNode = previousInternalNode->setSuffixLink(currentInternalNode); //! rule 2 
                this->remainder--;

            }

            ruleOneOrThree(currentCharIndex); //! rule 1 or 3 (or none)
        }
    }


    bool idInStringStarts(const uint64_t id) {
        size_t l = 0, r = this->stringStarts.size(), m = (l + r) / 2;

        while (m != l) {
            if (this->stringStarts[m] == id) {
                return true;
            } else if (this->stringStarts[m] < id) {
                l = m;
            } else {
                r = m;
            }

            m = (l + r) / 2;
        }

        return this->stringStarts[m] == id;
    }


    bool findString(const std::string &pattern) {
        // std::cout << "find initialized\n\n";
        const size_t patternLength = pattern.size();

        std::shared_ptr<SuffixTreeNode> currentActiveNode = this->root;
        std::shared_ptr<SuffixTreeEdge> currentActiveEdge = nullptr;
        size_t currentActiveLength = 0;

        for (size_t index = 0; index < patternLength; ++index) {
            // std::cout << "index = " << index << "\n";
            const uint64_t currentChar = pattern[index];

            if (currentActiveLength == 0) {
                const auto entry = currentActiveNode->next.find(currentChar);
                if (entry != currentActiveNode->next.end()) { // there is an edge starting with this uint64_t
                    currentActiveEdge = entry->second; // enter the edge
                    currentActiveLength++;
                    if (currentActiveLength == currentActiveEdge->getLength()) {
                        currentActiveNode = currentActiveEdge->node;
                        currentActiveEdge = nullptr;
                        currentActiveLength = 0;
                    }
                } else {
                    return false;
                }
            } else {
                const uint64_t checkedChar = this->inputString[currentActiveEdge->start + currentActiveLength];
                if (currentChar == checkedChar) { // uint64_t is in the edge's substring
                    currentActiveLength++;
                    if (currentActiveLength == currentActiveEdge->getLength()) {
                        currentActiveNode = currentActiveEdge->node;
                        currentActiveEdge = nullptr;
                        currentActiveLength = 0;
                    }
                } else {
                    return false;
                }
            }
        }

        if (
            ( // on edge
                currentActiveLength != 0 &&
                this->inputString[currentActiveEdge->start + currentActiveLength] > 'z' &&
                idInStringStarts(currentActiveEdge->node->id)
            ) || ( // in node
                currentActiveLength == 0 &&
                (
                    (
                        !currentActiveNode->empty() && // internal node
                        std::find_if( // check for an edge with a sentinel
                            currentActiveNode->next.begin(),
                            currentActiveNode->next.end(),
                            [&] (const auto &entry) {
                                return entry.first > 'z' && idInStringStarts(entry.second->node->id);
                            }
                        ) != currentActiveNode->next.end()
                    ) || ( // leaf
                        currentActiveNode->empty() &&
                        idInStringStarts(currentActiveNode->id)
                    )
                )
            )
        ) {
            return true;
        }

        return false;
    }

    bool findPrefix(const std::string &pattern) {
        const size_t patternLength = pattern.size();

        std::shared_ptr<SuffixTreeNode> currentActiveNode = this->root;
        std::shared_ptr<SuffixTreeEdge> currentActiveEdge = nullptr;
        size_t currentActiveLength = 0;

        for (size_t index = 0; index < patternLength; ++index) {
            const uint64_t currentChar = pattern[index];

            if (currentActiveLength == 0) {
                const auto entry = currentActiveNode->next.find(currentChar);
                if (entry != currentActiveNode->next.end()) { // there is an edge starting with this uint64_t
                    currentActiveEdge = entry->second; // enter the edge
                    currentActiveLength++;
                    if (currentActiveLength == currentActiveEdge->getLength()) {
                        currentActiveNode = currentActiveEdge->node;
                        currentActiveEdge = nullptr;
                        currentActiveLength = 0;
                    }
                } else {
                    return false;
                }
            } else {
                const uint64_t checkedChar = this->inputString[currentActiveEdge->start + currentActiveLength];
                if (currentChar == checkedChar) { // uint64_t is in the edge's substring
                    currentActiveLength++;
                    if (currentActiveLength == currentActiveEdge->getLength()) {
                        currentActiveNode = currentActiveEdge->node;
                        currentActiveEdge = nullptr;
                        currentActiveLength = 0;
                    }
                } else {
                    return false;
                }
            }
        }

        if (
            (currentActiveLength != 0 && this->hasLeafNodeInStringStarts(currentActiveEdge->node)) ||
            (currentActiveLength == 0 && this->hasLeafNodeInStringStarts(currentActiveNode))
        ) {
            return true;
        }

        return false;
    }

    bool hasLeafNodeInStringStarts(std::shared_ptr<SuffixTreeNode> node) {
        if (node->empty() && idInStringStarts(node->id)) { return true; }

        bool answ = false;
        for (const auto &[c, edge] : node->next) {
            answ = answ || this->hasLeafNodeInStringStarts(edge->node);
            if (answ) {
                return true;
            }
        }

        return false;
    }

public:
    Trie() {
        SuffixTreeNode::currentMaxSuffixIndex = 0;
    };

    void insert(const std::string &inputString) {
        const size_t lastLength = this->inputStringLength;
        this->stringStarts.push_back(lastLength);

        const size_t inputStringLength = inputString.size();

        this->inputString.reserve(this->inputString.size() + inputStringLength + 1);
        for (size_t i = 0; i < inputStringLength; ++i) {
            this->inputString.push_back(inputString[i]);
        }

        this->inputString.push_back(sentinel++);
        this->inputStringLength = this->inputString.size();

        for (size_t phase = lastLength; phase < this->inputStringLength; ++phase) {
            (*this->end)++;
            this->remainder++;

            size_t currentCharIndex = phase;
            const uint64_t currentChar = this->inputString[currentCharIndex];


            if (this->activeLength == 0) { // active point is on a node

                const auto entry = this->activeNode->next.find(currentChar);
                if (entry != this->activeNode->next.end()) { // there is an edge starting with this uint64_t
                    this->activeEdge = entry->second; // enter the edge
                    this->activeLength++;
                    this->depth++;
                    this->canonicize(currentCharIndex);
                } else { // there is no edge starting with this uint64_t
                    this->activeNode->addEdge(currentChar, phase, this->end); // create an edge
                    this->remainder--;

                    if (this->activeNode != this->root) {
                        std::shared_ptr<SuffixTreeNode> activeNodePreRule = this->activeNode;
                        this->ruleThree(currentCharIndex); //! rule 3. rule 1 can't be applied because activeLength is 0
                        this->buildSuffixLinks(activeNodePreRule, currentCharIndex);
                    }
                }

            } else { // active point is on an edge

                const uint64_t checkedChar = this->inputString[this->activeEdge->start + this->activeLength];
                if (currentChar == checkedChar) { // uint64_t is in the edge's substring
                    this->activeLength++;
                    this->depth++;
                    this->canonicize(currentCharIndex);
                } else { // uint64_t is not in the edge's substring, should split and create a new internal node
                    std::shared_ptr<SuffixTreeNode> previousInternalNode = this->activeEdge->split(this->end, this->activeLength, currentChar, checkedChar); // create a new node inside the edge
                    this->remainder--;
                    this->ruleOneOrThree(currentCharIndex); //! rule 1 or 3
                    this->buildSuffixLinks(previousInternalNode, currentCharIndex);
                }

            }
        }
    }

    std::vector<uint64_t> vectoricize(const std::string &s) {
        std::vector<uint64_t> v(s.length(), 0);
        for (size_t i = 0; i < s.length(); ++i) {
            v[i] = s[i];
        }
        return v;
    }

    bool search(const std::string &word) {
        return this->findString(word);
    }

    bool startsWith(const std::string &prefix) {
        return this->findPrefix(prefix);
    }
};
