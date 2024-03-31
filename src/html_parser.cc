#include <array>
#include <memory>
#include <vector>

#include "dom.h"
#include "html_parser.h"

bool HTMLParser::PumpTokenizer() {
    while (true) {
        std::unique_ptr<HTMLToken> token = tokenizer_.NextToken();
        if (!token) {
            break;
        }
    }
    return true;
}



