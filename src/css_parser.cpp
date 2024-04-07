#include <algorithm>
#include <format>
#include <iostream>
#include <tuple>

#include "css_parser.h"
#include "utils.h"

// TODO: Integrate with CssProperties & CssPropertyValues
std::unordered_map<std::string, std::pair<kProperty, kValueType>> propertyPool{
		{"font-weight", std::make_pair(kProperty::FontWeight,kValueType::Length)},
		{"color",  std::make_pair(kProperty::Color,kValueType::Color)},
		{"display",  std::make_pair(kProperty::Display, kValueType::Keyword)},

};

std::unordered_map<std::string, Color> colorToRGB{
	{"forestgreen", {0x22, 0x8b, 0x22, 0xFF}},
	{"red", {0xFF, 0x00, 0x00, 0xFF}},
};

static void removeWhitespace(std::string& str) {
	str.erase(
		std::remove_if(str.begin(), str.end(), [](unsigned char x) { return std::isspace(x); }),
		str.end()
	);
}

void CSSParser::parseCss()
{
	removeWhitespace(input_);
	while (input_.size() - 1 >= position_)
	{
		pumpRule();
	}
	stylesheet_.sortRule();
}

char CSSParser::nextCharacter()
{
	// TODO: size exception
	return input_[position_];
}


char CSSParser::peekCharacter() const
{
	if (input_.size() <= position_ + 1)
	{
		return 0;
	}
	return input_[position_ + 1];
}

void CSSParser::consumeCharacter()
{
	++position_;
}

SimpleSelecter CSSParser::parseSelectors()
{
	Rule rule;
	SimpleSelecter selector;
	while (nextCharacter() != Symbols::kLeftSquareBracket)
	{
		switch (nextCharacter())
		{
		case Symbols::kDot:
		{
			consumeCharacter();
			std::string className;
			while (nextCharacter() != Symbols::kLeftSquareBracket && nextCharacter() != Symbols::kSharp && nextCharacter() != Symbols::kDot)
			{
				className += nextCharacter();
				consumeCharacter();
			}
			selector.pushClass(className);
		}
		break;
		case Symbols::kSharp:
		{
			consumeCharacter();
			std::string idName;
			while (nextCharacter() != Symbols::kLeftSquareBracket && nextCharacter() != Symbols::kDot)
			{
				idName += nextCharacter();
				consumeCharacter();
			}
			
			selector.setId(idName);
		}
		break;
		default:
		{
			std::string tagName;
			while (nextCharacter() != Symbols::kLeftSquareBracket && nextCharacter() != Symbols::kDot && nextCharacter() != Symbols::kSharp)
			{
				tagName += nextCharacter();
				consumeCharacter();
			}
			selector.setTagName(tagName);
		}
		}
	}
	return selector;

}

Value CSSParser::parseValue(const std::string& property)
{
	if (!propertyPool.contains(property))
	{
		std::cout << std::format("! Error Invalid Property[{}].\n", property);
		exit(0);
	}
	const auto& [propertyType, valueType] = propertyPool.at(property);

	std::string valueString;

	while (nextCharacter() != Symbols::kSemicolon)
	{
		valueString += nextCharacter();
		consumeCharacter();
	}

	Value value;
	switch (valueType)
	{
	case kValueType::Keyword:
		break;
	case kValueType::Length: {
		float floatValue = std::stof(valueString);
		value = Length(std::make_pair(floatValue, Unit::Px));
		break;
	}
	case kValueType::Color: {
		// TODO : exception
		value = colorToRGB[valueString];
		break;
	}
	default:
		break;
	}

	return value;
}

std::string CSSParser::parseProperty()
{
	std::string propertyName;
	while (nextCharacter() != Symbols::kColon)
	{
		propertyName += nextCharacter();
		consumeCharacter();
	}

	if (!propertyPool.contains(propertyName))
	{
		std::cout << std::format("! Error Invalid Property[{}].\n", propertyName);
		exit(0);
	}
	return propertyName;
}

void CSSParser::pumpRule()
{
	Rule rule;
	Declaration declaration;
	while (true)
	{
		switch (state_)
		{
		case State::kSelectorNameState: {
			if (nextCharacter() == Symbols::kLeftSquareBracket)
			{
				state_ = State::kDeclarationProperty;
				consumeCharacter();
				continue;
			}

			rule.setSelector(std::move(parseSelectors()));
			break;
		}
		case State::kDeclarationProperty: {
			if (nextCharacter() == Symbols::kRightSquareBracket)
			{
				stylesheet_.emplaceRule(rule);
				consumeCharacter();
				state_ = State::kSelectorNameState;

				return;
			}
			if (nextCharacter() == Symbols::kColon)
			{
				state_ = State::kDeclarationValue;
				consumeCharacter();
				continue;
			}

			declaration.property_ = std::move(parseProperty());
			break;
		}
		case State::kDeclarationValue: {
			if (nextCharacter() == Symbols::kSemicolon)
			{
				rule.pushDelaration(declaration);
				state_ = State::kDeclarationProperty;
				consumeCharacter();
				continue;
			}

			declaration.value_ = std::move(parseValue(declaration.property_));
			break;
		}
		default:
			std::cout << std::format("! Error Invalid CCSParser State[{}].\n", static_cast<int32_t>(state_));
			exit(0);
		}
	}
}

const Stylesheet& CSSParser::stylesheet()
{
	return stylesheet_;
}

void Stylesheet::emplaceRule(Rule rule)
{
	rules_.emplace_back(rule);
}

void Stylesheet::sortRule()
{
	std::sort(rules_.begin(), rules_.end(), [](const Rule& rhs, const Rule& lhs) {
		return rhs.selector().specificity() > lhs.selector().specificity();
		});
	for (int i = 0; i < rules_.size(); i++)
		std::cout << std::get<0>(rules_[i].selector().specificity()) << "\t" << std::get<1>(rules_[i].selector().specificity()) << "\t" << std::get<2>(rules_[i].selector().specificity()) << std::endl;
}

Rule& Stylesheet::back()
{
	return rules_.back();
}

const std::vector<Rule>& Stylesheet::rules() const
{
	return rules_;
}

void SimpleSelecter::pushClass(std::string className)
{
	classes_.emplace_back(std::move(className));
}

void SimpleSelecter::setId(std::string id)
{
	id_ = std::move(id);
}

void SimpleSelecter::setTagName(std::string tagName)
{
	tagName_ = std::move(tagName);
}

const std::optional<std::string>& SimpleSelecter::tagName() const
{
	return tagName_;
}

const std::optional<std::string>& SimpleSelecter::id() const
{
	return id_;
}

const std::vector<std::string>& SimpleSelecter::classes() const
{
	return classes_;
}

void Rule::pushDelaration(Declaration declaration)
{
	declarations_.emplace_back(std::move(declaration));
}

void Rule::setSelector(SimpleSelecter selector)
{
	selector_ = std::move(selector);
}

const SimpleSelecter& Rule::selector() const
{
	return selector_;
}

const std::vector<Declaration>& Rule::declarations() const
{
	return declarations_;
}

