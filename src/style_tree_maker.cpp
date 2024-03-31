#include <algorithm>
#include <vector>

#include "css_parser.h"
#include "dom.h"
#include "style_tree_maker.h"



bool StyleTreeMaker::matchSimpleSelector(const Element& element, const SimpleSelecter& selector) const
{
	if (selector.tagName() && selector.tagName() != element.GetTagName())
	{
		return false;
	}

	if (selector.id() && selector.id() != element.Id())
	{
		return false;
	} 

	for (const auto& className: selector.classes())
	{
		if (!element.classes().contains(className))
		{
			return false;
		}
	}

	return true;
}

std::vector<Rule> StyleTreeMaker::findRules(const Element &element, const Stylesheet& stylesheet) const
{
	std::vector<Rule> rules;
	rules.reserve(20); // TODO: 

	for (const auto& rule : stylesheet.rules())
	{
		if (!matchSimpleSelector(element, rule.selector()))
		{
			continue;
		}
		rules.emplace_back(rule);
	}

	std::sort(rules.begin(), rules.end(), [](const auto& lhs, const auto& rhs) {
		return lhs.selector().specificity() < rhs.selector().specificity();
	});

	return rules;
}


std::unordered_map<std::string, Value> StyleTreeMaker::matchSelector(DomNode* node, const Stylesheet &stylesheet) const
{
	std::unordered_map<std::string, Value> values;

	if (node->type() != ElementType::kElement)
	{
		return values;
	}

	std::vector<Rule> rules = findRules(*dynamic_cast<Element*>(node), stylesheet);
	for (const auto& rule : rules)
	{
		for (const auto& declaration : rule.declarations())
		{
			values[declaration.property_] = declaration.value_;
		}
	}

	return values;
}

StyleNode StyleTreeMaker::createStyleTree(DomNode* node, const Stylesheet& stylesheet)
{
	// TODO: apply smart pointer
	StyleNode styleNode;
	//
	//for (auto* childNode : node->childern())
	//{
	//	styleNode.addChild(std::move(createStyleTree(childNode, stylesheet)));
	//}

	//styleNode.setSpecifiedValues(std::move(matchSelector(node, stylesheet)));
	return styleNode;
}

void StyleNode::addChild(StyleNode&& styleNode)
{
	children_.emplace_back(std::move(styleNode));
}

void StyleNode::setSpecifiedValues(std::unordered_map<std::string, Value>&& specifiedValues)
{
	specifiedValues_ = std::move(specifiedValues);
}
