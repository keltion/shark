#include <array>
#include <memory>
#include <vector>

#include "dom.h"
#include "html_parser.h"
#include "utils.h"

bool HTMLParser::PumpTokenizer() {
    while (true) {
        std::unique_ptr<HTMLToken> token = tokenizer_.NextToken();
        if (!token) {
            break;
        }

        treeBuilder_.ConstructTreeFromHTMLToken(std::move(token));
    }
    return true;
}

const DomNode& HTMLParser::DomTree() const {
    return treeBuilder_.domTree();
}

