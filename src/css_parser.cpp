#include <algorithm>
#include <iostream>
#include <tuple>

#include "css_parser.h"

std::unordered_map<std::string, std::pair<kProperty, kValueType>> propertyPool{
		{"font-weight", std::make_pair(kProperty::FontWeight,kValueType::Length)},
		{"color",  std::make_pair(kProperty::Color,kValueType::Color)},
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
	while (nextCharacter() != '{')
	{
		switch (nextCharacter())
		{
		case '.':
		{
			consumeCharacter();
			std::string className;
			while (nextCharacter() != '{' && nextCharacter() != '#' && nextCharacter() != '.')
			{
				className += nextCharacter();
				consumeCharacter();
			}
#ifdef TEST
			std::cout << "className= " << className << std::endl;
#endif // TEST
			selector.pushClass(className);
		}
		break;
		case '#':
		{
			consumeCharacter();
			std::string idName;
			while (nextCharacter() != '{' && nextCharacter() != '.')
			{
				idName += nextCharacter();
				consumeCharacter();
			}
			
#ifdef TEST
			std::cout << "id= " << idName << std::endl;
#endif // TEST
			selector.setId(idName);
		}
		break;
		default:
		{
			std::string tagName;
			while (nextCharacter() != '{' && nextCharacter() != '.' && nextCharacter() != '#')
			{
				tagName += nextCharacter();
				consumeCharacter();
			}
#ifdef TEST
			std::cout << "tagName= " << tagName << std::endl;
#endif // TEST
			selector.setTagName(tagName);
		}
		}
	}
	return selector;

}

Value CSSParser::parseValue(const std::string& property)
{
	Value value;

	if (!propertyPool.contains(property))
	{
		// ASSERT(false);
	}

	const auto& [propertyType, valueType] = propertyPool[property];

	std::string valueString;

	while (nextCharacter() != ';')
	{
		valueString += nextCharacter();
		consumeCharacter();
	}

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
	while (nextCharacter() != ':')
	{
		propertyName += nextCharacter();
		consumeCharacter();
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
			if (nextCharacter() == '{')
			{
				state_ = State::kDeclarationProperty;
				consumeCharacter();
				continue;
			}
#ifdef TEST
			SimpleSelecter selector = parseSelectors();
			rule.setSelector(std::move(selector));
#else // TEST
			rule.setSelector(std::move(parseSelectors()));

#endif // TEST
			break;
		}
		case State::kDeclarationProperty: {
			if (nextCharacter() == '}')
			{
				stylesheet_.emplaceRule(rule);
				consumeCharacter();
				state_ = State::kSelectorNameState;

				return;
			}
			if (nextCharacter() == ':')
			{
				state_ = State::kDeclarationValue;
				consumeCharacter();
				continue;
			}
#ifdef TEST
			std::string property = parseProperty();
			declaration.property_ = std::move(property);
#else // TEST
			declaration.property_ = std::move(parseProperty());
#endif // TEST
			break;
		}
		case State::kDeclarationValue: {
			if (nextCharacter() == ';')
			{
				rule.pushDelaration(declaration);
				state_ = State::kDeclarationProperty;
				consumeCharacter();
				continue;
			}
#ifdef TEST
			Value value = parseValue(declaration.property_);
			declaration.value_ = std::move(value);
#else // TEST
			declaration.value_ = std::move(parseValue(declaration.property_));
#endif // TEST
			break;
		}
									 //default:
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

