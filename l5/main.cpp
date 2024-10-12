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



class SuffixTreeNode;

class SuffixTreeEdge {
public:
    int start = 0;
    std::shared_ptr<int> end = nullptr;
    SuffixTreeNode *node;

    SuffixTreeEdge(const int &start, const std::shared_ptr<int> &end) : start(start), end(end) {}

    friend std::ostream& operator<<(std::ostream &os, const SuffixTreeEdge &edge) {
        os << "s: " << edge.start << " e: " << *edge.end << " (" << edge.end << ')';
        return os;
    }
};


class SuffixTreeNode {
public:
    // chars to edges
    std::map<char, SuffixTreeEdge*> next;
    SuffixTreeNode *suffixLink = nullptr;

    SuffixTreeNode(const char &value, const int &start, const std::shared_ptr<int> &end) {
        next[value] = new SuffixTreeEdge(start, end);
    }

    friend std::ostream& operator<<(std::ostream &os, const SuffixTreeNode &node) {
        os << "next:\n";
        for (auto [key, value] : node.next) {
            os << "\t" << key << " --- " << *value << '\n';
        }
        os << "suffixLink: " << node.suffixLink;
        return os;
    }
};


class SuffixTree {
private:
    SuffixTreeNode *root = nullptr;

public:
    SuffixTree(std::string inputString) {
        inputString += '$';
        size_t inputStringLength = inputString.length();


        std::shared_ptr<int> end = std::make_shared<int>(1);
        this->root = new SuffixTreeNode(inputString[0], 0, end);

        SuffixTreeNode *activeNode = this->root;
        SuffixTreeEdge *activeEdge = nullptr;
        int activeLength = 0;

        int remainder = 0;


        for (size_t phase = 1; phase < inputStringLength; ++phase) {
            std::cout << "------------------------ PHASE " << phase << " ------------------------\n";
            std::cout << "ROOT:\n" << *root << '\n';
        }
    }

    ~SuffixTree() {
    }
};


int main() {
    std::string s1("lol"), s2("alob");

    // std::cin >> s1 >> s2;

    SuffixTree st(s1 + '#' + s2);

    return 0;
}