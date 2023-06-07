#include <iostream>

#include "Chat.h"
#include "ChatException.h"
#include "Autocomplete.h"

void Chat::startChat()
{
	chatSwitch_ = true;
	userAdminRegistration();
	while (true == isChatSwitch())
	{
		showMainMenu();
		while (nullptr != getActiveUser())
		{
			showChatMenu();
		}
	}
}


void Chat::userAdminRegistration()
{
	std::string login = "All";
	std::string password = "password";
	std::string name = "all";
	userData.emplace_back(login, password, name);
}

void Chat::setChatSwitch()
{
	chatSwitch_ = false;
}

bool Chat::isChatSwitch() const
{
	return chatSwitch_;
}

void Chat::setActiveUser()
{
	activeUser_ = nullptr;
}

std::shared_ptr<User> Chat::getActiveUser() const
{
	return activeUser_;
}


void Chat::showMainMenu()
{
	std::string key;
	do
	{   // displays the first menu with a choice: SignIn, Registration, Exit
		std::cout << "Menu (key): 1 - Sign In  /  2 - Create an account  /  0 - Exit"
			      << std::endl;
		std::cin >> key;

		// calls appropriate methods
		if ("0" == key)
		{
			setChatSwitch();
		}
		else if ("1" == key)
		{
			userSignIn();
		}
		else if ("2" == key)
		{
			try
			{
				// displays invalid values for login and nam arguments
				std::cout << "\nInvalid login and name: Admin, admin, All, all, 0\n";
				userRegistration();
			}
			catch (const std::exception& exp)
			{
				std::cout << exp.what() << std::endl;
			}
		}
		else
		{
			// message, input error (valid values 1, 2, 0)
			std::cout << "Input error! Repeat please:\n";
		}
	} while (true == chatSwitch_ && nullptr == activeUser_);
}

void Chat::userSignIn()
{
	std::string login;
	std::string password;
	do
	{
		std::cout << "\nLogin: ";
		std::cin >> login;
		std::cout << "Password: ";
		std::cin >> password;
		activeUser_ = checkUserLogin(login);

		// executed if the login is not in the database or the password is incorrect
		if (nullptr == activeUser_ || password != activeUser_->getUserPassword())
		{
			std::string key;
			do
			{
				// runs until an available value is entered (0 or 1)
				std::cout << "\nLogin failed! Enter: 1 - repeat input  /  0 - exit: ";
				std::cin >> key;
			} while ( !("0" == key || "1" == key) );

			if ("0" == key)
			{
				setActiveUser();
				return;
			}
			setActiveUser();
		}
	} while (nullptr == activeUser_);
}

void Chat::userRegistration()
{
	std::cout << "\nLogin: ";
	std::string login;
	std::cin >> login;

	//checks if the user exists in the database and
	// if the new login meets the requirements
	if (nullptr != checkUserLogin(login) || "admin" == login ||
		"Admin" == login || "all" == login || "0" == login)
	{
		throw ExceptionUserLogin();
	}
	std::cout << "Password: ";
	std::string password;
	std::cin >> password;

	std::cout << "Name: ";
	std::string name;
	std::cin >> name;

	//checks if the user exists in the database and
	// if the new name meets the requirements
	if (checkUserName(name) != nullptr || "admin" == name ||
		"Admin" == name || "All" == name || "0" == name)
	{
		throw ExceptionUserName();
	}

	// if the data matches the requirements, a new user is created
	User user = User(login, password, name);

	// new user added to database
	userData.push_back(user);
	activeUser_ = std::make_shared<User>(user);
}

std::shared_ptr<User> Chat::checkUserLogin(const std::string& login) const
{
	for (auto& data : userData)
	{
		if (login == data.getUserLogin())
		{
			return std::make_shared<User>(data);
		}
	}
	return nullptr;
}

std::shared_ptr<User> Chat::checkUserName(const std::string& name) const
{
	for (auto& data : userData)
	{
		if (name == data.getUserName())
		{
			return std::make_shared<User>(data);
		}
	}
	return nullptr;
}


void Chat::showChatMenu()
{
	// displays the names of all registered users, active user not showing
	std::cout << "\n\tWelcome to chat, " << activeUser_->getUserName() << std::endl;
	showAllUsersNames(activeUser_->getUserName());
	std::string name;
	do
	{
		std::cout << "Select a user and enter a name or enter 0 - exit: ";
		std::cin >> name;

		if ("0" == name)
		{
			setActiveUser();
			return;
		}
	} while (nullptr == checkUserName(name) ||
		     !(activeUser_->getUserName() != name || "all" == name));

	while (nullptr != activeUser_)
	{
		showAllChatMessages(name);
		std::string key;
		do
		{
			// runs until an available value is entered (0 or 1)
			std::cout << "Menu chat (key): 1 - Send a message  /  0 - Exit" << std::endl;
			std::cin >> key;
		} while ( !("0" == key || "1" == key) );

		if ("0" == key)
		{
			return;
		}
		sendMessage(name);
	}
}

void Chat::showAllUsersNames(const std::string& name) const
{
	std::cout << "\nUsernames:\n--------------------" << std::endl;

	for (auto& data : userData)
	{
		if (name != data.getUserName() || "all" == name)
		{
			std::cout << data.getUserName() << "\n";
		}
	}
	std::cout << "--------------------" << std::endl;
}

void Chat::showAllChatMessages(const std::string& name) const
{
	std::cout << "\nChat with user " << name << ":\n--------------------" << std::endl;

	if ("all" == name) // display messages for general chat
	{
		for (auto& data : messageData)
		{
			if (activeUser_->getUserLogin() == data.getMassageFrom() &&
				"all" == data.getMassageTo())
			{
				std::cout << "\t\t" << data.getMassageText() << "\n" << std::endl;
			}
			else if ("all" == data.getMassageTo())
			{
				std::cout << data.getMassageFrom() << ": "
					      << data.getMassageText() << "\n" << std::endl;
			}
		}
	}
	else // display messages for personal chat
	{
		for (auto& data : messageData)
		{
			if (activeUser_->getUserLogin() == data.getMassageTo() &&
				name == data.getMassageFrom())
			{
				std::cout << data.getMassageText() << "\n" << std::endl;
			}
			else if (activeUser_->getUserLogin() == data.getMassageFrom() &&
				     name == data.getMassageTo())
			{
				std::cout << "\t\t" << data.getMassageText() << "\n" << std::endl;
			}
		}
	}
	std::cout << "--------------------" << std::endl;
}

void Chat::sendMessage(const std::string& name)
{
	Autocomplete textMessage;
	std::string text = textMessage.getText();
	messageData.emplace_back(activeUser_->getUserLogin(), name, text);
}