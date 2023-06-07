#include "User.h"

const std::string& User::getUserLogin() const
{
	return login_;
}

const std::string& User::getUserPassword() const
{
	return password_;
}

const std::string& User::getUserName() const
{
	return name_;
}