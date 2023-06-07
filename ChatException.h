#pragma once
#include <exception>

// registration exception, login not available
class ExceptionUserLogin : public std::exception
{
public:
	const char* what() const noexcept override
	{
		return "\nError: Invalid login";
	}
};

//registration exception, name not available
class ExceptionUserName : public std::exception
{
public:
	const char* what() const noexcept override
	{
		return "\nError: Invalid name. Enter 0 - exit";
	}
};