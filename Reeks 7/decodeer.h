#include<string>
#include<fstream>
#include "bincode.h"
#include "binairetrie.h"
#include "bitstream.h"
using std::string;
using std::cout;
void Decodeer(string gecomp, string plain, string codefile){
	
	Bincode code(codefile.c_str());
	
	BinaireTrie trie(code);
		
	BinaireTrie *afdalen = &trie;
	
	ibitstream inv(gecomp.c_str());
 	ofstream output(plain);
 	
	bool bit = inv.leesbit();
	while(!inv.fail()){
		if((*afdalen)->isBlad()){
			output << (char)static_cast<TrieBlad*>(afdalen->get())->letter;
			afdalen = &trie;
		}
		afdalen = &static_cast<TrieNietblad*>(afdalen->get())->geefKind(bit);
		//vb.voegToe(bit);
		
		bit = inv.leesbit();
	}
	
}
