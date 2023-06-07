#include <windows.h>

#include <iostream>
#include <fstream>

#include "Autocomplete.h"

COORD getCoords()
{
	CONSOLE_SCREEN_BUFFER_INFO info_x;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info_x);
	return info_x.dwCursorPosition;
}

// clears the console screen
void Autocomplete::cls(short X, short Y)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coordScreen = { X, Y };    // home for the cursor
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;

	// Get the number of character cells in the current buffer.
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		return;
	}

	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	// Fill the entire screen with blanks.
	if (!FillConsoleOutputCharacter(hConsole,        // Handle to console screen buffer
		                            (TCHAR)' ',      // Character to write to the buffer
		                            dwConSize,       // Number of cells to write
		                            coordScreen,     // Coordinates of first cell
		                            &cCharsWritten)) // Receive number of characters written
	{
		return;
	}

	// Get the current text attribute.
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		return;
	}

	// Set the buffer's attributes accordingly.
	if (!FillConsoleOutputAttribute(hConsole,         // Handle to console screen buffer
		                            csbi.wAttributes, // Character attributes to use
		                            dwConSize,        // Number of cells to set attribute
		                            coordScreen,      // Coordinates of first cell
		                            &cCharsWritten))  // Receive number of characters written
	{
		return;
	}

	// Put the cursor at its home coordinates.
	SetConsoleCursorPosition(hConsole, coordScreen);
}

// returns a new node with empty children
Autocomplete::TrieNode* Autocomplete::getNewNode(void)
{
	TrieNode* newNode = new TrieNode;
	newNode->isEndOfWord = false;

	for (auto i = 0; i < ALPHABET_SIZE; i++)
	{
		newNode->children[i] = nullptr;
	}
	return newNode;
}

// inserting a key into a prefix tree
void Autocomplete::insertKey(TrieNode* root, const std::string& key)
{
	TrieNode* newNode = root;

	for (auto& k : key)
	{
		int index = k - 'a';

		if (nullptr == newNode->children[index])
		{
			newNode->children[index] = getNewNode();
		}
		newNode = newNode->children[index];
	}
	newNode->isEndOfWord = true;
}

// returns true if node is a leaf, otherwise false
bool Autocomplete::isEmptyNode(TrieNode* node)
{
	for (auto& child : node->children)
	{
		if (nullptr != child)
		{
			return false;
		}
	}
	return true;
}

// dictionary add function
void Autocomplete::dataWords()
{
	root = getNewNode();
	std::string word;
	std::fstream fileWords;
	fileWords.open("engwords.txt");

	if (true == fileWords.is_open())
	{
		while (false == fileWords.eof())
		{
			fileWords >> word;
			insertKey(root, word);
		}
		fileWords.close();
	}
	else
	{
		std::cout << "File not found" << std::endl;
	}
}

// returns true if the string is empty or contains only spaces, false otherwise
bool Autocomplete::isEmptyText(const std::string& text)
{
	if ( 0 == text.size() || text.size() == std::count(text.begin(), text.end(), ' ') )
	{
		return true;
	}
	return false;
}

// returns true if the input consists of only letters
bool Autocomplete::isCheckPrefix(const std::string& text)
{
	std::string buffer = text;
	for (auto& b : buffer)
	{
		int i = static_cast<int>(b);
		if (97 > b || 122 < b)
		{
			return false;
		}
	}
	return true;
}


bool Autocomplete::isUint(const std::string& str)
{
	return str.find_first_not_of("0123456789") == std::string::npos;
}

// returns the number of suffixes
size_t Autocomplete::searchSuffix(TrieNode* root, std::string& result)
{
	size_t count = 0;

	if (nullptr == root)
	{
		std::cout << "The tree is empty" << std::endl;
		return count;
	}
	TrieNode* node = root;

	for (auto& k : result)
	{
		int index = k - 'a';

		if (nullptr == node->children[index])
		{
			return count;
		}
		node = node->children[index];
	}
	std::string buffer;
	result.clear();
	findSuffix(node, buffer, result, count);
	return count;
}

// finds all suffixes by prefix
void Autocomplete::findSuffix(TrieNode* node, std::string& buffer, std::string& result, size_t& count)
{
	if (true == isEmptyNode(node))
	{
		++count;
		result.append('(' + std::to_string(count) + ')' + buffer + ' ');
		return;
	}
	for (auto i = 0; i < ALPHABET_SIZE; i++)
	{
		if (nullptr != node->children[i])
		{
			buffer.push_back(i + 'a');

			if ( true == node->children[i]->isEndOfWord && false == isEmptyNode(node->children[i]) )
			{
				++count;
				result.append('(' + std::to_string(count) + ')' + buffer + ' ');
			}
			findSuffix(node->children[i], buffer, result, count);
			buffer.pop_back();
		}
	}
}

// word completion
void Autocomplete::setAutoWord(std::string& text, short& X, short& Y)
{
	std::string buffer;
	getline(std::cin, buffer);

	if (true == isEmptyText(buffer))
	{
		cls(X, Y);
		return;
	}
	else if (buffer.size() - 1 == buffer.find_last_of("0"))
	{
		text.append(buffer);
		startSwitch_ = false;
		return;
	}
	else if ('\t' == buffer.back())
	{
		buffer.erase(buffer.find_last_not_of("\t") + 1);
		std::string suffixesWord;

		if (buffer.find_last_of(" ") != std::string::npos)
		{
			suffixesWord = buffer.substr(buffer.find_last_of(" ") + 1);
		}
		else
		{
			suffixesWord = buffer;
		}
		std::string prefix = suffixesWord;

		if (0 == prefix.size())
		{
			buffer.erase(buffer.find_last_not_of(" ") + 1);
			text.append(buffer);
			X += short(buffer.size());
			cls(X, Y);
			return;
		}
		else if (false == isCheckPrefix(prefix))
		{
			text.append(buffer);
			X += short(buffer.size());
			cls(X, Y);
			return;
		}
		size_t count = searchSuffix(root, suffixesWord);

		if (0 == count)
		{
			text.append(buffer);
			X += short(buffer.size());
			cls(X, Y);
			return;
		}
		
		for (size_t i = 1; i <= count; i++)
		{
			size_t index = suffixesWord.find(std::to_string(i));
			std::cout << suffixesWord.substr(index - 1, suffixesWord.find(')', index) - index + 2)
				      << prefix
				      << suffixesWord.substr( suffixesWord.find(')', index) + 1,
						 suffixesWord.find(' ', index) - suffixesWord.find(')', index) );
		}
		std::cout << "\n";
		prefix.clear();
		int key = 0;
		do
		{
			std::cout << "Enter key (key (0) - exit): ";
			std::string str;
			std::cin >> str;

			if ("0" == str)
			{
				text.append(buffer);
				X += short(buffer.size());
				cls(X, Y);
				return;
			}

			if (true == isUint(str))
			{
				key = std::stoi(str);
			}

		} while (!(0 < key && count >= key));

		text.append(buffer);
		X += short(buffer.size());
		buffer.clear();
		size_t index = suffixesWord.find(std::to_string(key));
		buffer.append( suffixesWord.substr(suffixesWord.find(')', index) + 1,
			           suffixesWord.find(' ', index) - suffixesWord.find(')', index) ) );
		text.append(buffer);
		cls(X, Y);
		std::cout << buffer;
		X += short(buffer.size());
		cls(X, Y);
		return;
	}
	else
	{
		text.append(buffer);
		startSwitch_ = false;
	}
}

// program launch
std::string Autocomplete::getText()
{
	dataWords();
	std::string text;
	std::cout << "(key Tab, key Enter) - Autocomplete, (key 0, key Enter) - Exit"
		      << "\n" << "Enter text:" << std::endl;
	short X = getCoords().X;
	short Y = getCoords().Y;
	startSwitch_ = true;

	while (true == startSwitch_)
	{
		setAutoWord(text, X, Y);
	}
	if (text.size() - 1 == text.find_last_of("0"))
	{
		text.pop_back();
	}
	text.erase(0, text.find_first_not_of(" "));
	text.erase(text.find_last_not_of(" ") + 1);
	return text;
}