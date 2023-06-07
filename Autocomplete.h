#pragma once

#include <string>

constexpr auto ALPHABET_SIZE = 26;

class Autocomplete
{
public:
	Autocomplete() = default;

	std::string getText();

	~Autocomplete() = default;

private:
	class TrieNode // tree node structure
	{
	public:
		TrieNode* children[ALPHABET_SIZE];

		// true if the element is the end of the key
		bool isEndOfWord = true;
	};

	// the value of this variable is used to track the operation of the program
	bool startSwitch_ = false; 

	// tree root
	TrieNode* root = nullptr;

	// returns a new node with empty children
	TrieNode* getNewNode(void);

	// inserting a key into a prefix tree
	void insertKey(TrieNode* root, const std::string& key);

	// returns true if node is a leaf, otherwise false
	bool isEmptyNode(TrieNode* root);

	// word completion
	void setAutoWord(std::string& text, short& X, short& Y);

	// dictionary add function
	void dataWords();

	// returns true if the string is empty or contains only spaces, false otherwise
	bool isEmptyText(const std::string& text);

	// returns true if the input consists of only letters
	bool isCheckPrefix(const std::string& text);


	bool isUint(const std::string& s);

	// returns the number of suffixes
	size_t searchSuffix(TrieNode* root, std::string& result);

	// finds all suffixes by prefix
	void findSuffix(TrieNode* node, std::string& buffer, std::string& result, size_t& count);
	
	// clears the console screen
	void cls(short X, short Y);
};