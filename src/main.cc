#include <fstream>

#include "dom.h"
#include "parser.h"
#include "tree_traval.h"

int main() {
    std::ifstream fin;
    fin.open("test.html");
    std::string html;
    getline(fin, html);

    Parser parser(html);
    Node* dom = parser.parse_node();

    inorder(dom, "");
}