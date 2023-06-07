#include "Message.h"

const std::string& Message::getMassageFrom() const
{
	return from_;
}

const std::string& Message::getMassageTo() const
{
	return to_;
}

const std::string& Message::getMassageText() const
{
	return text_;
}