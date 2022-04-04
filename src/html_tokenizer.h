#pragma once

#include "ascii.h"
#include "html_token.h"

class HTMLTokenizer {
public:
    HTMLTokenizer() = delete;
    HTMLTokenizer(const std::string& input)
        : state_(State::kDataState), input_(input), pos(0) {}
    enum class State{
        kDataState,
        kTagOpenState,
        kEndTagOpenState,
        kTagNameState,
        kBeforeAttributeNameState,
        kAttributeNameState,
        kAfterAttributeNameState,
        kBeforeAttributeValueState,
        kAttributeValuedState,
        kAfterAttributeValueQuotedState
    };
    bool NextToken(HTMLToken& token);
    char GetNextChar() const {
        return input_[pos];
    }
    void ConsumeChar(char& cc) {
        //TODO : exception
        cc = input_[++pos];
    }
private:
    const std::string& input_;
    std::size_t pos;
    State state_;
};