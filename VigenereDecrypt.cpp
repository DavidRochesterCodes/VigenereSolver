/*
 *
 * VigenereDecrypt source code
 * created by David Rochester
 *
 */

#include "VigenereDecrypt.h"
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <unordered_map>
using namespace std;



// This struct creates the skeleton for my Trie structure
// creates a map var to store characters and Trie nodes
// Insert method allows us to insert characters into nodes
// Search method allows us to traverse through the given string
// checking if it is contained in our dictionary of words
struct VigenereDecrypt::Trie {
  public:
    unordered_map<char, Trie *> child;
    bool isEnd = false;

    void insert(string word) {
        Trie *node = this;
        for (char c : word) {
            if (node->child.find(c) == node->child.end()) {
                node->child[c] = new Trie();
            }
            node = node->child[c];
        }
        node->isEnd = true;
    }

    bool search(string word) {
        Trie *node = this;
        for (char c : word) {
            if (node->child.find(c) == node->child.end()) {
                return false;
            }
            node = node->child[c];
        }
        return node->isEnd;
		}
};

// unknownMultiWordScramble
// If you're reading this, this function made me want to jump off the football
// stadium This function takes in a scramble of words, encoded with a Vigenere
// cipher The key is unknown, but this workhorse tries every key in the english
// dictionary and finds all valid key-message combinations. This function
// implements the above created Trie data structure, fills the structure with
// all words from the dictionary, creating a search tree for us to use. The Trie
// is sometimes referred to as the "prefix tree", allowing us to traverse
// directly through the prefix and througout the word if it exists. If valid
// key-message combinations are found, this function will print them. Otherwise,
// NO POSSIBILITIES FOUND is returned.
void VigenereDecrypt::breakVigenere(string scramble) {

    vector<string> dictionary;
    bool keyFound;
    Trie trie;

    fillDictAndTree(dictionary, trie);

    string plain;
    cout << "message"
         << "        "
         << "key" << endl
         << endl;
    for (auto &key : dictionary) {
        plain = decrypt(scramble, key);
        if (findWords(plain, trie)) {
            cout << plain << "     " << left << key << endl;
            keyFound = true;
        }
    }
    if (!keyFound)
        cout << "NO POSSIBILITIES EXIST" << endl;
}

// This is a similar function to decodeMessage from Cipher class
// Takes in a string to decode and the key to use when decoding
// Instead of wasting time allocating space for the linked list,
// we utilize an array sized to the key.
// // we utilize an array sized to the key.
string VigenereDecrypt::decrypt(string &in, string &key) {

    int stringInLength = in.length();
    int newKeyLength = key.length();
    string encodedWord;

    int *keyArr = new int[newKeyLength];
    for (int i = 0; i < newKeyLength; ++i) {
        keyArr[i] = static_cast<int>(key[i]);
    }

    int next = 0;
    int j = 0;

    for (int i = 0; i < stringInLength; ++i) {
        next = keyArr[j];
        encodedWord += letterShiftFast(static_cast<int>(in.at(i)), next);
        j++;
        if (j > newKeyLength - 1)
            j = 0;
    }

    delete[] keyArr;
    return encodedWord;
}

// letterShift fast
// this is a modified version of our previous letterShift from Cipher class
// There is no boolean to evaluate, and it uses the global alphabet string
// to index and shift the letters passed in
char VigenereDecrypt::letterShiftFast(const int &letter1, const int &letter2) {

    int diff = letter1 - letter2;
    if (diff <= 0)
        diff = 'z' - abs(diff);
    else
        diff += 96;

    return diff;
}

// fillDict function
// This function takes in a vector to be used as our dictionary,
// and a trie structure to be used as our prefix search tree
// Using an ifstream, we grab the dictionary from /usr/share/dict
// and populate the vector and tree with words that are greater than len 3
// and have no special characters in them.
void VigenereDecrypt::fillDictAndTree(vector<string> &dict, Trie &trie) {
    ifstream inFile("/usr/share/dict/words");
    string word;
    int len = 0;
    bool special;

    while (inFile >> word) {
        special = false;
        len = word.length();

        for (int i = 0; i < len; ++i) {
            word[i] = tolower(word[i]);

            if (ispunct(word[i]))
                special = true;
        }

        if (len > 2 && !special) {
            dict.push_back(word);
            trie.insert(word);
        }
    }
}

// findWords function
// This function utilizes our Trie data structure we created above
// Using a vector to store our boolean values, this function allows us to
// traverse through our trie structure, searching for each substring of the
// given plaintext. We mark the corresponding index of the vector true if our
// word is found, else it remains false.
bool VigenereDecrypt::findWords(const string &word, Trie &trie) {
    vector<bool> find(word.size() + 1, false); // vector to track if word exists
    find[0] = true;
    int wordLen = word.length();

    for (int len = 3; len <= wordLen; ++len) {
        for (int i = 0; i < len; ++i) {
            if (find[i] && trie.search(word.substr(i, len - i))) {
                find[len] = true;
            }
        }
    }
    return find[wordLen];
}
