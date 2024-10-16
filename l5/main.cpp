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

#include <memory>
#include <stdexcept>





// oop is a fucking joke
class SuffixTreeNode;
class SuffixTreeEdge;



class SuffixTreeNode {
public:
    // string chars to edges
    std::map<char, std::shared_ptr<SuffixTreeEdge>> next;
    std::shared_ptr<SuffixTreeNode> suffixLink = nullptr;

    SuffixTreeNode() {};
    SuffixTreeNode(const char &value, const size_t &start, const std::shared_ptr<size_t> &end) {
        this->next[value] = std::make_shared<SuffixTreeEdge>(start, end);
    }

    void addEdge(const char &value, const size_t &start, const std::shared_ptr<size_t> &end) {
        this->next[value] = std::make_shared<SuffixTreeEdge>(start, end);
    }

    friend std::ostream& operator<<(std::ostream &os, const SuffixTreeNode &node);
};


class SuffixTreeEdge {
public:
    size_t start = 0;
    std::shared_ptr<size_t> end = nullptr;

    // edge ends in this node
    std::shared_ptr<SuffixTreeNode> node = nullptr;

    SuffixTreeEdge(const size_t &start, const std::shared_ptr<size_t> &end) : start(start), end(end), node(std::make_shared<SuffixTreeNode>()) {}

    // split edge at this->start + length (0 < length < *this->end - this->start), returns shared pointer to a new internal node
    std::shared_ptr<SuffixTreeNode> split(const size_t &length, const char &newChar, const char &differentChar) {
        if (length == 0 || length >= this->getLength()) {
            throw std::invalid_argument("length is greater than the substring's length");
        }

        const size_t stringSplitIndex = this->start + length;

        std::shared_ptr<size_t> newEnd = std::make_shared<size_t>(stringSplitIndex);

        std::shared_ptr<SuffixTreeNode> internalNode = std::make_shared<SuffixTreeNode>(newChar, *this->end - 1, this->end); // node with the new char
        internalNode->addEdge(differentChar, stringSplitIndex, this->end); // old node

        this->node = internalNode;
        this->end = newEnd;

        return internalNode;
    }

    size_t getLength() { return *this->end - this->start; }

    friend std::ostream& operator<<(std::ostream &os, const SuffixTreeEdge &edge);
};



std::ostream& operator<<(std::ostream &os, const SuffixTreeNode &node) {
    os << "next:\n";
    for (const auto& [key, value] : node.next) {
        os << "\t" << key << " --- " << *value << '\n';
    }
    os << "suffixLink: " << node.suffixLink;
    return os;
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

    void ruleOneOrThree(const size_t &currentCharIndex) {
        char nextRemainingChar = '\0';

        if (this->remainder - 1 > this->activeLength) {
            nextRemainingChar = this->inputString[currentCharIndex - this->activeLength];
        } else {
            nextRemainingChar = this->inputString[currentCharIndex - this->remainder + 1];
        }

        if (this->activeNode == this->root) { //! rule 1
            this->activeLength--;
        } else if (this->activeNode->suffixLink != nullptr) { //! rule 3
            this->activeNode = this->activeNode->suffixLink;
        } else {
            this->activeNode = this->root;
        }

        //! applies to all rules
        const auto entry = this->activeNode->next.find(nextRemainingChar);
        if (entry != this->activeNode->next.end()) {
            this->activeEdge = entry->second;
        } else {
            this->activeEdge = nullptr;
        }


        if (activeEdge != nullptr) {
            this->canonicize(currentCharIndex);
        }
    }

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

    void buildSuffixLink(std::shared_ptr<SuffixTreeNode> &previousInternalNode, const size_t &currentCharIndex, const char &checkedChar) {
        const char currentChar = this->inputString[currentCharIndex];

        while (this->remainder > 0) {
            if (this->activeLength == 0) { // adding from a node

                this->activeNode->addEdge(currentChar, currentCharIndex, this->end);
                // if (this->activeNode != this->root) { //! rule 2
                //     previousInternalNode->suffixLink = this->activeNode;
                //     previousInternalNode = this->activeNode;
                // }
                this->remainder--;

            } else { // adding from an edge

                std::shared_ptr<SuffixTreeNode> currentInternalNode = this->activeEdge->split(this->activeLength, currentChar, checkedChar);
                this->remainder--;
                this->ruleOneOrThree(currentCharIndex); //! rule 1 or 3 goes after EACH split
                previousInternalNode->suffixLink = currentInternalNode; //! rule 2
                previousInternalNode = currentInternalNode;

            }
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
                    this->remainder--;
                }

            } else { // active point is on an edge

                const char checkedChar = this->inputString[this->activeEdge->start + this->activeLength];
                if (currentChar == checkedChar) { // char is in the edge's substring
                    this->activeLength++;
                    this->canonicize(currentCharIndex);
                } else { // char is not in the edge's substring
                    std::shared_ptr<SuffixTreeNode> previousInternalNode = this->activeEdge->split(this->activeLength, currentChar, checkedChar); // create a new node inside the edge
                    this->remainder--;
                    this->ruleOneOrThree(currentCharIndex); //! rule 1 or 3 goes after EACH split
                    this->buildSuffixLink(previousInternalNode, currentCharIndex, checkedChar);
                }

            }

            std::cout << "------------------------ AFTER PHASE " << phase << " ------------------------\n";
            std::cout << "Active Point -- node: " << this->activeNode << " edge: " << this->activeEdge << " length: " << this->activeLength << '\n';
            std::cout << "Remainder: " << this->remainder << '\n';
            std::cout << "root:\n" << *this->root << '\n';
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
    // SuffixTree st(s1 + '#' + s2);

    SuffixTree st("heyamama#yabobamaavmanvamiwavm");
    // SuffixTree st("abcdefabxybcdmnabcdx");

    // st.findMaxSubstring()

    return 0;
}