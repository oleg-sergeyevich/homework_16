# Project Chat
Сode author Shlychkov Oleg Sergeyevich

Files main.cpp: creating a Chat class object, calling a method to run the program.

Files ChatException.h: contains exceptions for validating User class arguments.

Files User.h and User.cpp: contain the definition and implementation of a User class. Instantiating an object with arguments (login, password, name).

Files Message.h and Message.cpp: contain the definition and implementation of a Message class. Instantiating an object with arguments (from, to, text).

Files Autocomplete.h and Autocomplete.cpp: contain the definition and implementation of a Autocomplete class. Writing message text with autocomplete feature.

Files Chat.h and Chat.cpp (main methods of the program):

public:

void start(); // program start method

private:

bool chatSwitch_ = false; // the value of this variable is used to track the operation of the program

std::shared_ptr<User> activeUser_ = nullptr; // the value of this pointer is used to track user activity
  
std::vector <User> userData; // stores the data of all registered users (arguments: login, password, name)
  
std::vector <Message> messageData; // stores the data of all written messages (arguments: from, to, text)

void userAdminRegistration(); // user registration: login - "All", name "all"
  
void setChatSwitch(); // changes the value of a variable (chatSwitch_)
  
bool getChatSwitch() const; // returns the value of a variable (chatSwitch_)
  
void setActiveUser(); // changes the value of a pointer (activeUser_)
  
std::shared_ptr<User> getActiveUser() const; // returns the pointer value (activeUser_)

void showMainMenu(); // displays the first menu with a choice: SignIn, Registration, Exit. Calls appropriate methods
  
void userSignIn(); // enter the chat, checks if the user exists in the database and matches the arguments
  
void userRegistration(); // chat registration, checks if the user exists in the database and registers the new user
  
std::shared_ptr<User> chekUserLogin(const std::string& login) const; //checks if the login attribute matches in the database
  
std::shared_ptr<User> chekUserName(const std::string& name) const; //checks if the name attribute matches in the database
  
void showChatMenu(); // displays the chat menu, calls the appropriate methods written below
  
void showAllUsersNames(const std::string& name) const; // displays the names of all registered users
  
void showAllChatMessages(const std::string& name) const; // display private or public messages
  
void sendMessage(const std::string& name); // send message method
