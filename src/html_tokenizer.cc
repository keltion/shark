#include <format>
#include <memory>
#include <optional>

#include "html_tokenizer.h"
#include "css_parser.h"
constexpr char kLeftAngleBracket = '<';
constexpr char kRightAngleBracket = '>';
constexpr char kSpace = ' ';
constexpr char kSlash = '/';
constexpr char kQuote = '"';
constexpr char kEqual = '=';
constexpr char kEndOfFile = '\0';



std::unique_ptr<HTMLToken> HTMLTokenizer::NextToken() {
    std::unique_ptr<HTMLToken> token = std::make_unique<HTMLToken>();

    bool tokenCompleted = false;
    while (!tokenCompleted) {
        if (pos_ >= input_.size()) {
            // token->SetType(HTMLToken::TokenType::kEndOfFile);
            return nullptr;
        }

        switch (state_) {
        case State::kDataState:
            tokenCompleted = HandleDataState(*token);
            break;
        case State::kCharacterState:
            tokenCompleted = HandleCharacterState(*token);
            break;
        case State::kTagOpenState:
            tokenCompleted = HandleTagOpenState(*token);
            break;
        case State::kTagNameState:
            tokenCompleted = HandleTagNameState(*token);
            break;
        case State::kEndTagOpenState:
            tokenCompleted = HandleEndTagOpenState(*token);
            break;
        case State::kBeforeAttributeNameState:
            tokenCompleted = HandleBeforeAttributeNameState(*token);
            break;
        case State::kAttributeNameState:
            tokenCompleted = HandleAttributeNameState(*token);
            break;
        case State::kAfterAttributeNameState:
            tokenCompleted = HandleAfterAttributeNameState(*token);
            break;
        case State::kBeforeAttributeValueState:
            tokenCompleted = HandleBeforeAttributeValueState(*token);
            break;
        case State::kAttributeValuedState:
            tokenCompleted = HandleAttributeValuedState(*token);
            break;
        case State::kAfterAttributeValueQuotedState:
            tokenCompleted = HandleAfterAttributeValueQuotedState(*token);
            break;
        default:
            std::cout << std::format("! Error Invalid State[{}].", static_cast<int32_t>(state_));
            exit(0);
            break;
        }
    }

    return token;
}

bool HTMLTokenizer::HandleDataState(HTMLToken &token) {
    switch (CurrentChar()) {
    case kLeftAngleBracket:
        state_ = State::kTagOpenState;
        ConsumeChar();
        break;
    default:
        if (IsASCIIAlpha(CurrentChar()) || CurrentChar() == kSpace) {
            token.SetType(HTMLToken::TokenType::kCharacter);
            state_ = State::kCharacterState;
        }
        else {
            std::cout << std::format("! Error [HandleDataState] Invalid Character[{}].", CurrentChar());
            state_ = State::kInvalidState;
        }
    }
    return false;
}

bool HTMLTokenizer::HandleCharacterState(HTMLToken& token) {
    if (CurrentChar() == kLeftAngleBracket) {
        state_ = State::kDataState;
        return true;
    }

    token.AppendToCharater(CurrentChar());
    ConsumeChar();
    state_ = State::kCharacterState;
    return false;
}

bool HTMLTokenizer::HandleTagOpenState(HTMLToken& token) {
    switch (CurrentChar()) {
    case kSlash:
        ConsumeChar();
        state_ = State::kEndTagOpenState;
        break;

    default:
        if (IsASCIIAlpha(CurrentChar())) {
            state_ = State::kTagNameState;
            token.SetType(HTMLToken::TokenType::kStartTag);
        }
        else {
            std::cout << std::format("! Error [HandleTagOpenState] Invalid Character[{}].", CurrentChar());
            state_ = State::kInvalidState;
        }
    }
    return false;
}

bool HTMLTokenizer::HandleTagNameState(HTMLToken& token) {

    switch (CurrentChar()) {
    case kRightAngleBracket :
        state_ = State::kDataState;
        ConsumeChar();
        return true;
        break;
    case kSpace:
         state_ = State::kBeforeAttributeNameState;
         ConsumeChar();
        break;
    default:
        if (IsASCIIAlpha(CurrentChar())) {
            token.AppendToCharater(CurrentChar());
            ConsumeChar();
        }
        else {
            std::cout << std::format("! Error [HandleTagNameState] Invalid Character[{}].", CurrentChar());
            state_ = State::kInvalidState;
        }
    }
    return false;
}

bool HTMLTokenizer::HandleEndTagOpenState(HTMLToken& token) {
    if (IsASCIIAlpha(CurrentChar())) {
        state_ = State::kTagNameState;
        token.SetType(HTMLToken::TokenType::kEndTag);
    } else {
        std::cout << std::format("! Error [HandleEndTagOpenState] Invalid Character[{}].", CurrentChar());
        state_ = State::kInvalidState;
    }
    return false;
}

bool HTMLTokenizer::HandleBeforeAttributeNameState(HTMLToken &token) {
    if (IsASCIIAlpha(CurrentChar())) {
        state_ = State::kAttributeNameState;
    }
    else {
        std::cout << std::format("! Error [HandleBeforeAttributeNameState] Invalid Character[{}].", CurrentChar());
        state_ = State::kInvalidState;
    }
    return false;
}

bool HTMLTokenizer::HandleAttributeNameState(HTMLToken& token) {

    switch (CurrentChar()) {
    case kEqual:
        state_ = State::kBeforeAttributeValueState;
        ConsumeChar();
        break;
    case kSpace:
        state_ = State::kAfterAttributeNameState;
        ConsumeChar();
        break;
    default:
        if (IsASCIIAlpha(CurrentChar())) {
            token.AppendToAttributeName(CurrentChar());
            ConsumeChar();
        }
        else {
            std::cout << std::format("! Error [HandleTagNameState] Invalid Character[{}].", CurrentChar());
            state_ = State::kInvalidState;
        }
    }
    return false;
}

bool HTMLTokenizer::HandleAfterAttributeNameState(HTMLToken& token) {
    if (GetNextChar() == kRightAngleBracket) {
        state_ = State::kDataState;
        ConsumeChar();
        return true;
    } else {
        std::cout << std::format("! Error [HandleAfterAttributeNameState] Invalid Character[{}].", CurrentChar());
        state_ = State::kInvalidState;
    }

    return false;
}

bool HTMLTokenizer::HandleBeforeAttributeValueState(HTMLToken& token) {
    if (GetNextChar() == kQuote) {
        state_ = State::kAttributeValuedState;
        ConsumeChar();
    } else {
        std::cout << std::format("! Error [HandleBeforeAttributeValueState] Invalid Character[{}].", CurrentChar());
        state_ = State::kInvalidState;
    }

    return false;
}

bool HTMLTokenizer::HandleAttributeValuedState(HTMLToken& token) {
    switch (CurrentChar()) {
    case kQuote:
        state_ = State::kAfterAttributeValueQuotedState;
        ConsumeChar();
        break;
    default:
        if (IsASCIIAlpha(CurrentChar()) || IsASCIIDigit(CurrentChar()) || CurrentChar() == kSpace) {
            token.AppendToAttributeValue(CurrentChar());
            ConsumeChar();
        } else {
            std::cout << std::format("! Error [HandleAttributeValuedState] Invalid Character[{}].", CurrentChar());
            state_ = State::kInvalidState;
        }
    }
    return false;
}

bool HTMLTokenizer::HandleAfterAttributeValueQuotedState(HTMLToken& token) {
    if (CurrentChar() == kRightAngleBracket) {
        state_ = State::kDataState;
        ConsumeChar();
        token.InputAttributeData();
        return true;
    }
    else {
        std::cout << std::format("! Error [HandleAfterAttributeValueQuotedState] Invalid Character[{}].", CurrentChar());
        state_ = State::kInvalidState;
    }
    return false;
}
