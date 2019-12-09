#ifndef __DECODEER_H__
#define __DECODEER_H__

#include "bincode.h"
#include "binairetrie.h"
#include "bitstream.h"
#include <fstream>

void Decodeer(const char * in_gecodeerd, const char * out_gedecodeerd, const char * code) {
    Bincode bincode(code);
    BinaireTrie trie(bincode);
    ibitstream bitstream(in_gecodeerd);
    bool huidigeBit;
    ofstream output(out_gedecodeerd);
    BinaireTrie * zoekTrie = &trie;
    while(!bitstream.fail()) {
        if((*zoekTrie)->isBlad()) {
            output << (char)static_cast<TrieBlad *>((*zoekTrie).get())->letter;
            zoekTrie = &trie;
        }
        zoekTrie = &static_cast<TrieNietblad *>((*zoekTrie).get())->geefKind(bitstream.leesbit());

    }
    bitstream.close();
    output.close();
}

#endif