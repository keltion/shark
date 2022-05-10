#include "html_tokenizer.h"

bool HTMLTokenizer::NextToken(HTMLToken& token) {
    char cc = GetNextChar();

    switch(state_) {
    case State::kDataState:
    kDataState:
        if (cc == '<') {
            state_ = State::kTagOpenState;
            ConsumeChar(cc);
            goto kTagOpenState;
        } else if (IsASCIIAlpha(cc) || cc == ' ') { // Text Token
            token.AppendToCharater(cc);
            ConsumeChar(cc);
            token.SetType(HTMLToken::TokenType::kCharacter);
        } else if (cc == '\0') {
            token.SetType(HTMLToken::TokenType::kEndOfFile);
            return false;
        } else {
            exit(0); // error
        }
        break;

    case State::kTagOpenState :
    kTagOpenState :
        if (cc == '/') {
            ConsumeChar(cc);
            state_ = State::kEndTagOpenState;
            goto kEndTagOpenState;
        } else if (IsASCIIAlpha(cc)) {
            state_ = State::kTagNameState;
            token.SetType(HTMLToken::TokenType::kStartTag);
            goto kTagNameState;
        } else {
            exit(0); //error
        }
        break;

    case State::kTagNameState : // TODO : 속성판단해야함
    kTagNameState :
        if (cc == '>') {
            state_ = State::kDataState;
            ConsumeChar(cc);
            // Emit token
        } else if (IsASCIIAlpha(cc)) {
            token.AppendToCharater(cc);
            ConsumeChar(cc);
            goto kTagNameState;
        } else if (cc == ' ') {
            state_ = State::kBeforeAttributeNameState;
            ConsumeChar(cc);
            goto kBeforeAttributeNameState;
        } else {
            exit(0); //error
        }
        break;

    case State::kEndTagOpenState :
    kEndTagOpenState :
        if (IsASCIIAlpha(cc)) {
            state_ = State::kTagNameState;
            token.SetType(HTMLToken::TokenType::kEndTag);
            goto kTagNameState;
        } else {
            exit(0); //error
        }
        break;

    case State::kBeforeAttributeNameState :
    kBeforeAttributeNameState :
        if (IsASCIIAlpha(cc)) {
            state_ = State::kAttributeNameState;
            goto kAttributeNameState;
        } else {
            exit(0); //error
        }
        break;
        
    case State::kAttributeNameState :
    kAttributeNameState :
        if (IsASCIIAlpha(cc)) {
            token.AppendToAttributeName(cc);
            ConsumeChar(cc);
            goto kAttributeNameState;
        } else if (cc == ' ') {
            state_ = State::kAfterAttributeNameState;
            ConsumeChar(cc);
            goto kAfterAttributeNameState;
        } else if (cc == '=') {
            state_ = State::kBeforeAttributeValueState;
            ConsumeChar(cc);
            goto kBeforeAttributeValueState;
        } else {
            exit(0); //error
        }
        break;

    case State::kAfterAttributeNameState :
    kAfterAttributeNameState :
        if (GetNextChar() == '>') { // Emit the current tag token
            ConsumeChar(cc);
            state_ = State::kDataState;
        } else {
            exit(0); //error
        }
        break;

    case State::kBeforeAttributeValueState :
    kBeforeAttributeValueState :
        if (GetNextChar() == '\"') {
            state_ = State::kAttributeValuedState;
            ConsumeChar(cc);
            goto kAttributeValuedState;
        } else {
            exit(0); //error
        }
        break;
    
    case State::kAttributeValuedState :
    kAttributeValuedState :
        if (IsASCIIAlpha(cc) || IsASCIIDigit(cc)) {
            token.AppendToAttributeValue(cc);
            ConsumeChar(cc);
            goto kAttributeValuedState;
        } else if (cc == '\"') {
            state_ = State::kAfterAttributeValueQuotedState;
            ConsumeChar(cc);
            goto kAfterAttributeValueQuotedState;
        } else {
            exit(0); //error
        }
        break;

    case State::kAfterAttributeValueQuotedState :
    kAfterAttributeValueQuotedState :
        if (cc == '>') { // Emit the current tag token.
            ConsumeChar(cc);
            token.InputAttributeData();
            state_ = State::kDataState;
        } else {
            exit(0); //error
        }
        break;
    }
    return true;
}