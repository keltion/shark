#include "dom_tree_builder.h"
#include "dom.h"
#include "html_token.h"


void DomTreeBuilder::SetAttribute(Element &element, const std::vector<std::pair<std::string, std::string>> &attributes) {
	if (attributes.empty()) {
		return;
	}

	std::unordered_map<std::string, std::string> attributeMap;
	for (const auto& attribute : attributes) {
		attributeMap[attribute.first] = attribute.second;
	}
	element.setAttributeMap(std::move(attributeMap));
}

void DomTreeBuilder::HandleNoneState(std::unique_ptr<HTMLToken> token) {
	if (token->type() != HTMLToken::TokenType::kStartTag) {
		return;
	}

	domTree_ = std::make_unique<Element>(token->TagName());
	SetAttribute(*domTree_, token->Attributes());

	header_ = domTree_.get();
	state_ = State::kTagOpen;
}

void DomTreeBuilder::HandleTagOpen(std::unique_ptr<HTMLToken> token) {
	std::unique_ptr<DomNode> currentNode = nullptr;
	
	switch (token->type()) {
	case HTMLToken::TokenType::kStartTag: {
		currentNode = std::make_unique<Element>(token->TagName());
		SetAttribute(*static_cast<Element *>(currentNode.get()), token->Attributes());
		currentNode->setParent(header_);
	}
	break;
	case HTMLToken::TokenType::kCharacter: {
		auto currentNode = std::make_unique<TextNode>(token->TagName());
		currentNode->setParent(header_);
	}
	break;
	case HTMLToken::TokenType::kEndTag: {
		header_ = header_->parent();
		state_ = State::kTagClose;
	}
	break;
	}

	if (currentNode) {
		DomNode* newHeader = currentNode.get();
		header_->addChild(std::move(currentNode));
		header_ = newHeader;
	}
}

void DomTreeBuilder::ConstructTreeFromHTMLToken(std::unique_ptr<HTMLToken> token) {
	switch (state_) {
	case State::kNone:
		HandleNoneState(std::move(token));
		break;
	case State::kTagOpen:
		HandleTagOpen(std::move(token));
		break;
	//case State::kTagClose:
	//	if (token->type() == HTMLToken::TokenType::kStartTag)
	//	{
	//		currentNode = new Element(token->TagName());
	//		if (!token->Attributes().empty())
	//		{
	//			std::unordered_map<std::string, std::string> attributeMap;
	//			for (const auto& attribute : token->Attributes())
	//			{
	//				attributeMap[attribute.first] = attributeMap[attribute.second];
	//			}
	//			static_cast<Element*>(currentNode)->setAttributeMap(std::move(attributeMap));
	//		}
	//		currentNode->setParent(header_);
	//		header_->addChild(currentNode);
	//		header_ = currentNode;
	//		state_ = State::kTagOpen;
	//		
	//		break;
	//	}
	//	if (token->type() == HTMLToken::TokenType::kCharacter)
	//	{
	//		currentNode = new TextNode(token->TagName());
	//		currentNode->setParent(header_);
	//		header_->addChild(currentNode);
	//		header_ = currentNode;
	//		state_ = State::kTagOpen;

	//		break;
	//	}
	//	break;
	}
}


Element* DomTreeBuilder::domTree() const
{ 
	return domTree_.get();
}
