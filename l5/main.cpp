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





// oop is a fucking joke
class SuffixTreeNode;
class SuffixTreeEdge;





class SuffixTreeNode {
public:
    // chars to edges
    std::map<char, std::shared_ptr<SuffixTreeEdge>> next;
    SuffixTreeNode *suffixLink = nullptr;

    SuffixTreeNode() {};
    SuffixTreeNode(const char &value, const int &start, const std::shared_ptr<int> &end);

    friend std::ostream& operator<<(std::ostream &os, const SuffixTreeNode &node);
};


class SuffixTreeEdge {
public:
    int start = 0;
    std::shared_ptr<int> end = nullptr;

    // edge ends in this node
    std::shared_ptr<SuffixTreeNode> node = nullptr;

    SuffixTreeEdge(const int &start, const std::shared_ptr<int> &end);

    friend std::ostream& operator<<(std::ostream &os, const SuffixTreeEdge &edge);
};





SuffixTreeEdge::SuffixTreeEdge(const int &start, const std::shared_ptr<int> &end) : start(start), end(end), node(std::make_shared<SuffixTreeNode>()) {}

std::ostream& operator<<(std::ostream &os, const SuffixTreeEdge &edge) {
    os << "s: " << edge.start << " e: " << *edge.end << " (" << edge.end << ") node: " << edge.node;
    return os;
}



SuffixTreeNode::SuffixTreeNode(const char &value, const int &start, const std::shared_ptr<int> &end) {
    next[value] = std::make_shared<SuffixTreeEdge>(start, end);
}

std::ostream& operator<<(std::ostream &os, const SuffixTreeNode &node) {
    os << "next:\n";
    for (const auto& [key, value] : node.next) {
        os << "\t" << key << " --- " << *value << '\n';
    }
    os << "suffixLink: " << node.suffixLink;
    return os;
}





class SuffixTree {
private:
    std::shared_ptr<SuffixTreeNode> root = nullptr;

public:
    SuffixTree(std::string inputString) {
        inputString += '$';
        size_t inputStringLength = inputString.length();


        std::shared_ptr<int> end = std::make_shared<int>(1);
        this->root = std::make_shared<SuffixTreeNode>(inputString[0], 0, end);

        std::shared_ptr<SuffixTreeNode> activeNode = this->root;
        std::shared_ptr<SuffixTreeEdge> activeEdge = nullptr;
        size_t activeLength = 0;

        size_t remainder = 0;


        for (size_t phase = 1; phase < inputStringLength; ++phase) {
            remainder++;

            std::cout << "------------------------ PHASE " << phase << " ------------------------\n";
            std::cout << "Active Point: node: " << activeNode << " edge: " << activeEdge << " length: " << activeLength << '\n';
            std::cout << "Remainder: " << remainder << '\n';
            std::cout << "root:\n" << *root << '\n';

            ++*end; // postfix ++ is not implemented (???)

            for (size_t currentCharIndex = phase - remainder + 1; currentCharIndex <= phase; ++currentCharIndex) {
                char currentChar = inputString[currentCharIndex];
                auto entry = activeNode->next.find(currentChar);
                if (entry != activeNode->next.end()) { // char is present
                    activeEdge = entry->second;
                    activeLength++;
                } else { // char is not present
                    activeNode->next[currentChar] = std::make_shared<SuffixTreeEdge>(phase, end);
                    remainder--;
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