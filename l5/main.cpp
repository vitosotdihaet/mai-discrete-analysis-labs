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


* source: https://stackoverflow.com/questions/9452701/ukkonens-suffix-tree-algorithm-in-plain-english
*/


#include <iostream>
#include <string>
#include <map>

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

    std::shared_ptr<SuffixTreeNode> root = std::make_shared<SuffixTreeNode>();


    std::shared_ptr<SuffixTreeNode> activeNode = this->root;
    std::shared_ptr<SuffixTreeEdge> activeEdge = nullptr;
    size_t activeLength = 0;

    size_t remainder = 0;

    std::shared_ptr<size_t> end = std::make_shared<size_t>(0);


private:
    // rule 1
    void ruleOne(const size_t &currentCharIndex) {
        if (this->activeNode != this->root || this->activeLength == 0) {
            throw std::logic_error("activeNode is not root or activeLength is zero");
        }

        char nextRemainingChar = '\0';

        if (this->remainder - 1 > this->activeLength) {
            nextRemainingChar = this->inputString[currentCharIndex - this->activeLength];
        } else {
            nextRemainingChar = this->inputString[currentCharIndex - this->remainder + 1];
        }


        // apply the rule
        this->activeLength--;

        const auto entry = this->activeNode->next.find(nextRemainingChar);
        if (entry != this->activeNode->next.end()) {
            this->activeEdge = entry->second;
            this->canonicize(currentCharIndex);
        } else {
            this->activeEdge = nullptr;
        }
    }

    // rule 3
    void ruleThree(const size_t &currentCharIndex) {
        if (this->activeNode == this->root) {
            throw std::logic_error("activeNode is root");
        }

        char nextRemainingChar = '\0';

        if (this->remainder - 1 > this->activeLength) {
            nextRemainingChar = this->inputString[currentCharIndex - this->activeLength];
        } else {
            nextRemainingChar = this->inputString[currentCharIndex - this->remainder + 1];
        }


        // apply the rule
        if (this->activeNode->suffixLink != nullptr) {
            this->activeNode = this->activeNode->suffixLink;
        } else {
            this->activeNode = this->root;
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
    void ruleOneOrThree(const size_t &currentCharIndex) {
        if (this->activeNode == this->root && this->activeLength != 0) { //! rule 1
            this->ruleOne(currentCharIndex);
        } else if (this->activeNode != this->root) { //! rule 3
            this->ruleThree(currentCharIndex);
        }
    }

    //! observation 2
    void canonicize(const size_t &currentCharIndex) {
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
    void buildSuffixLinks(std::shared_ptr<SuffixTreeNode> &previousInternalNode, const size_t &currentCharIndex, const char &checkedChar) {
        const char currentChar = this->inputString[currentCharIndex];

        while (this->remainder > 0) {
            if (this->activeLength == 0) { // inserting new edge to the active node

                bool lastAndInTree = false;
                if (this->remainder == 1) { //! observation 1
                    const auto entry = this->activeNode->next.find(currentChar); // i love performance
                    if (entry != this->activeNode->next.end()) {
                        lastAndInTree = true;
                        this->activeEdge = entry->second;
                        this->activeLength++;
                        this->canonicize(currentCharIndex);
                    }
                }

                if (!lastAndInTree) {
                    this->activeNode->addEdge(currentChar, currentCharIndex, this->end);
                    this->activeNode->next[currentChar]->node->id = SuffixTreeNode::currentMaxSuffixIndex++;
                } else {
                    break;
                }

                if (this->activeNode != this->root) { //! rule 2
                    previousInternalNode = previousInternalNode->setSuffixLink(this->activeNode);
                }

                this->remainder--;                

            } else { // splitting active edge

                std::shared_ptr<SuffixTreeNode> currentInternalNode = this->activeEdge->split(this->activeLength, currentChar, checkedChar);
                previousInternalNode = previousInternalNode->setSuffixLink(currentInternalNode); //! rule 2 
                this->remainder--;

            }

            ruleOneOrThree(currentCharIndex); //! rule 1 or 3 (or none)
        }
    }

    void buildTree() {
        const size_t inputStringLength = this->inputString.length();

        for (size_t phase = 0; phase < inputStringLength; ++phase) {
            (*this->end)++;
            this->remainder++;

            size_t currentCharIndex = phase;
            const char currentChar = this->inputString[currentCharIndex];


            if (this->activeLength == 0) { // active point is on a node

                const auto entry = this->activeNode->next.find(currentChar);
                if (entry != this->activeNode->next.end()) { // there is an edge starting with this char
                    this->activeEdge = entry->second; // enter the edge
                    this->activeLength++;
                    this->canonicize(currentCharIndex);
                } else { // there is no edge starting with this char
                    this->activeNode->addEdge(currentChar, phase, this->end); // create an edge
                    this->activeNode->next[currentChar]->node->id = SuffixTreeNode::currentMaxSuffixIndex++;
                    this->remainder--;

                    if (this->activeNode != this->root) {
                        std::shared_ptr<SuffixTreeNode> activeNodePreRule = this->activeNode;
                        this->ruleThree(currentCharIndex); //! rule 3. rule 1 can't be applied because activeLength is 0
                        this->buildSuffixLinks(activeNodePreRule, currentCharIndex, this->inputString[currentCharIndex - 1]);
                    }
                }

            } else { // active point is on an edge

                const char checkedChar = this->inputString[this->activeEdge->start + this->activeLength];
                if (currentChar == checkedChar) { // char is in the edge's substring
                    this->activeLength++;
                    this->canonicize(currentCharIndex);
                } else { // char is not in the edge's substring, should split and create a new internal node
                    std::shared_ptr<SuffixTreeNode> previousInternalNode = this->activeEdge->split(this->activeLength, currentChar, checkedChar); // create a new node inside the edge
                    this->remainder--;
                    this->ruleOneOrThree(currentCharIndex); //! rule 1 or 3
                    this->buildSuffixLinks(previousInternalNode, currentCharIndex, checkedChar);
                }

            }
        }
    }

public:
    SuffixTree(const std::string &inputString) {
        this->inputString = inputString + '$';
        this->buildTree();
    }
};





int main() {
    // std::string s1, s2;
    // std::cin >> s1 >> s2;

    // if (s1.length() < s2.length()) {
    //     std::string temp = std::move(s1);
    //     s1 = std::move(s2);
    //     s2 = std::move(temp);
    // }

    // SuffixTree st(s1 + '#' + s2);

    // SuffixTree st("amama"); //* working
    // SuffixTree st("heyamama#"); //* working
    // SuffixTree st("ayaboba"); //* working
    // SuffixTree st("ayabac"); //* working
    SuffixTree st("heyamama#yabobamaavmanvamiwavm"); //* working
    // SuffixTree st("abcdefabxybcdmnabcdx"); //* working

    // st.findMaxSubstring();
}