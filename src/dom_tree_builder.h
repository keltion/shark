#pragma once
#include <memory>

#include "dom.h"
#include "html_token.h"


// singleton
class DomTreeBuilder {
public:
    enum class State {
        kNone,
        kTagOpen,
        kTagClose,
    };
    DomTreeBuilder() : state_{State::kNone}, domTree_{nullptr}, header_{nullptr} {}
    //DomTreeBuilder(const DomTreeBuilder& other) = delete;
    //DomTreeBuilder(DomTreeBuilder&& other) noexcept = delete;
    //DomTreeBuilder& operator=(const DomTreeBuilder& other) = delete;
    //DomTreeBuilder& operator=(DomTreeBuilder&& other) noexcept = delete;

    void SetAttribute(Element& element, const std::vector<std::pair<std::string, std::string>>& attributes);
    void HandleNoneState(std::unique_ptr<HTMLToken> token);

    void HandleTagOpenState(std::unique_ptr<HTMLToken> token);
    void HandleTagCloseState(std::unique_ptr<HTMLToken> token);

    void ConstructTreeFromHTMLToken(std::unique_ptr<HTMLToken> token);
    const Element& domTree() const;

private:
    State state_;
    std::unique_ptr<Element> domTree_;
    DomNode* header_;
};