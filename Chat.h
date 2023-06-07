#pragma once

#include <vector>
#include <memory>

#include "User.h"
#include "Message.h"

class Chat
{
public:
	Chat() = default;

	void startChat(); // program start method

	~Chat() = default;

private:
	// the value of this variable is used to track the operation of the program
	bool chatSwitch_ = false;

	// the value of this pointer is used to track user activity
	std::shared_ptr<User> activeUser_ = nullptr;

	// stores the data of all registered users (arguments: login, password, name)
	std::vector <User> userData;

	// stores the data of all written messages (arguments: from, to, text)
	std::vector <Message> messageData;


	// user registration: login - "All", name "all"
	void userAdminRegistration();

	// changes the value of a variable (chatSwitch_)
	void setChatSwitch();

	// returns the value of a variable (chatSwitch_)
	bool isChatSwitch() const;

	// changes the value of a pointer (activeUser_)
	void setActiveUser();

	// returns the pointer value (activeUser_)
	std::shared_ptr<User> getActiveUser() const;


	// displays the first menu with a choice: SignIn, Registration, Exit.
	// Calls appropriate methods
	void showMainMenu();

	// enter the chat, checks if the user exists in the database
	// and matches the arguments
	void userSignIn();

	// chat registration, checks if the user exists in the database
	// and registers the new user
	void userRegistration();

	//checks if the login attribute matches in the database
	std::shared_ptr<User> checkUserLogin(const std::string& login) const;

	//checks if the name attribute matches in the database
	std::shared_ptr<User> checkUserName(const std::string& name) const;


	// displays a chat menu with a choice of username (showAllUsersNames)
	// calls the message display method (showAllChatMessages)
	// calls the send message method (sendMessage)
	void showChatMenu();
	void showAllUsersNames(const std::string& name) const;
	void showAllChatMessages(const std::string& name) const;
	void sendMessage(const std::string& name);
};