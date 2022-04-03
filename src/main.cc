#include <fstream>

#include "html_parser.h"

int main() {
    std::ifstream fin;
    fin.open("test.html");
    std::string html;
    getline(fin, html);

    HTMLParser parser(html);
    parser.PumpTokenizer();
}