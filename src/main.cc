#include <fstream>

#include "dom.h"
#include "html_parser.h"
#include "tree_traval.h"

int main() {
    std::ifstream fin;
    fin.open("test.html");
    std::string html;
    getline(fin, html);

    HTMLParser html_parser(html);
    Node* dom = html_parser.parse_node();

    inorder(dom, "");
}