#pragma once

#include <memory>
#include <string>

#include "html_token.h"
#include "html_tokenizer.h"

class HTMLParser {
public:
    HTMLParser(const std::string& html)
        : pos(0),token_(std::make_unique<HTMLToken>()),
        tokenizer_(std::make_unique<HTMLTokenizer>(html)) {}
    bool PumpTokenizer();
    HTMLTokenizer* Tokenizer() { return tokenizer_.get(); }

private:
    std::size_t pos;
    std::unique_ptr<HTMLToken> token_;
    std::unique_ptr<HTMLTokenizer> tokenizer_;
};