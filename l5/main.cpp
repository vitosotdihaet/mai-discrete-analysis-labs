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
    // chars to edges
    std::map<char, std::shared_ptr<SuffixTreeEdge>> next;
    SuffixTreeNode *suffixLink = nullptr;

    SuffixTreeNode() {};
    SuffixTreeNode(const char &value, const size_t &start, const std::shared_ptr<size_t> &end);

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

    SuffixTreeEdge(const size_t &start, const std::shared_ptr<size_t> &end);

    // split edge at this->start + length (0 < length < *this->end - this->start), returns shared pointer to a new internal node
    const std::shared_ptr<SuffixTreeNode> SuffixTreeEdge::split(const size_t &length, const char &newChar, const char &differentChar);
    size_t getLength() noexcept { return *this->end - this->start; }

    friend std::ostream& operator<<(std::ostream &os, const SuffixTreeEdge &edge);
};





SuffixTreeNode::SuffixTreeNode(const char &value, const size_t &start, const std::shared_ptr<size_t> &end) {
    this->next[value] = std::make_shared<SuffixTreeEdge>(start, end);
}

std::ostream& operator<<(std::ostream &os, const SuffixTreeNode &node) {
    os << "next:\n";
    for (const auto& [key, value] : node.next) {
        os << "\t" << key << " --- " << *value << '\n';
    }
    os << "suffixLink: " << node.suffixLink;
    return os;
}



SuffixTreeEdge::SuffixTreeEdge(const size_t &start, const std::shared_ptr<size_t> &end) : start(start), end(end), node(std::make_shared<SuffixTreeNode>()) {}

const std::shared_ptr<SuffixTreeNode> SuffixTreeEdge::split(const size_t &length, const char &newChar, const char &differentChar) {
    if (length >= this->getLength()) {
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

std::ostream& operator<<(std::ostream &os, const SuffixTreeEdge &edge) {
    os << "s: " << edge.start << " e: " << *edge.end << " (" << edge.end << ") node: " << edge.node;
    return os;
}





class SuffixTree {
private:
    std::shared_ptr<SuffixTreeNode> root = std::make_shared<SuffixTreeNode>();

public:
    SuffixTree(std::string inputString) {
        inputString += '$';
        size_t inputStringLength = inputString.length();


        std::shared_ptr<size_t> end = std::make_shared<size_t>(0);

        std::shared_ptr<SuffixTreeNode> activeNode = this->root;
        std::shared_ptr<SuffixTreeEdge> activeEdge = nullptr;
        size_t activeLength = 0;

        size_t remainder = 0;


        for (size_t phase = 0; phase < inputStringLength; ++phase) {
            (*end)++;
            remainder++;


            std::cout << "------------------------ PHASE " << phase << " ------------------------\n";
            std::cout << "Active Point -- node: " << activeNode << " edge: " << activeEdge << " length: " << activeLength << '\n';
            std::cout << "Remainder: " << remainder << '\n';
            std::cout << "root:\n" << *this->root << '\n';


            size_t currentCharIndex = phase;
            char currentChar = inputString[currentCharIndex];

            if (activeLength == 0) { // active point is on a node

                auto entry = activeNode->next.find(currentChar);
                if (entry != activeNode->next.end()) { // there is an edge starting with this char
                    activeEdge = entry->second; // we entered the edge
                    activeLength++;
                } else { // there is no edge starting with this char
                    activeNode->addEdge(currentChar, phase, end);
                    remainder--;
                }

            } else { // active point is on an edge

                char checkedChar = inputString[activeEdge->start + activeLength];
                if (currentChar == checkedChar) { // char is in the edge's substring
                    activeLength++;
                    if (activeLength < activeEdge->getLength()) { // char is not out of bounds of current substring

                    } else { // substring is too smol, go further (to the node)
                        activeLength = 0;
                        activeNode = activeEdge->node;
                        activeEdge = nullptr;
                    }
                } else { // char is not in the edge's substring
                    // create node inside the edge
                    const std::shared_ptr<SuffixTreeNode> internalNode = activeEdge->split(activeLength, currentChar, checkedChar);
                    remainder--;

                    if (activeNode == this->root) { //! rule 1
                        activeEdge = this->root->next[inputString[currentCharIndex - 1]];
                        activeLength--;
                    } else { //! rule 3

                    }

                    for (; remainder > 0; remainder--) {
                        
                    }
                }

            }
        }
    }
};





int main() {
    // std::string s1("lol"), s2("alob");
    // std::cin >> s1 >> s2;

    SuffixTree st("abcabxabcd");

    return 0;
}