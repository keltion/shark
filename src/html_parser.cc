#include <array>
#include <memory>
#include <vector>

#include "html_parser.h"

bool HTMLParser::PumpTokenizer() {
    while(1) {
        if (!tokenizer_->NextToken(*token_)) {
            break;
        }
        // TODO : Implement ConstructTreeFromHTMLToken();
        // TEST 
        std::array<std::string, 4> matching {"", "Start Tag", "E n d Tag", "Character"};
        std::cout << "["<< matching[token_->GetType()] << "] "
        << token_->TagName() << " ";
        
        if (!token_->Attributes().empty()) {
            for (auto& attr : token_->Attributes()) {
                std::cout << "[attr name: " << attr.first << " "
                << "attr value: " << attr.second << "]";
            }
        }
        std::cout << std::endl;

        token_.reset();
        token_ = std::make_unique<HTMLToken>();
    }
    return true;
}
