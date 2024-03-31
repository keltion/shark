#pragma once

#include "ascii.h"
#include "html_token.h"

enum class State {
    kDataState,
    kTagOpenState,
    kEndTagOpenState,
    kTagNameState,
    kBeforeAttributeNameState,
    kAttributeNameState,
    kAfterAttributeNameState,
    kBeforeAttributeValueState,
    kAttributeValuedState,
    kAfterAttributeValueQuotedState,
    kCharacterState,
    kInvalidState,
};

class HTMLTokenizer {
public:
    HTMLTokenizer() = delete;
    HTMLTokenizer(std::string&& input)
        : state_(State::kDataState), input_(std::move(input)), pos_(0) {}

    std::unique_ptr<HTMLToken> NextToken();
    bool HandleTagOpenState(HTMLToken& token);
    bool HandleTagNameState(HTMLToken& token);
    bool HandleEndTagOpenState(HTMLToken& token);
    bool HandleDataState(HTMLToken &token);
    bool HandleCharacterState(HTMLToken& token);
    bool HandleBeforeAttributeNameState(HTMLToken& token);

    bool HandleAttributeNameState(HTMLToken& token);
    bool HandleAfterAttributeNameState(HTMLToken& token);

    bool HandleBeforeAttributeValueState(HTMLToken& token);
    bool HandleAttributeValuedState(HTMLToken& token);
    bool HandleAfterAttributeValueQuotedState(HTMLToken& token);

    char GetNextChar() const {
        if (pos_ >= input_.size()) {
            return '\0';
        }
        return input_[pos_];
    }

    void ConsumeChar() {
        if (pos_ >= input_.size())
        {
            return;
        }
        ++pos_;
    }

    char CurrentChar() {
        if (pos_ >= input_.size())
        {
            return '\0';
        }
        return input_[pos_];
    }
private:
    const std::string input_;
    std::size_t pos_;
    State state_;
};