/*
 *
 * VigenereDecrypt header file
 * created by David Rochester
 * */


#ifndef VIGENEREDECRYPT_H
#define VIGENEREDECRYPT_H

#include <vector>
#include <string>
using namespace std;

class VigenereDecrypt {
	public:
		struct Trie;
		VigenereDecrypt() {};
		void breakVigenere(string);
		string decrypt(string&, string&);
		char letterShiftFast(const int &,const int &);
		void fillDictAndTree(vector<string> &, Trie &);
		bool findWords(const string&, Trie &);

};
#endif

