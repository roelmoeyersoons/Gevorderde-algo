#include<iostream>
#include "PRquad.h"

using namespace std;

int main(){
	PRQuadtree tree(500);
	tree.voegToe(200,200);
	tree.voegToe(400,400);
	tree.voegToe(-250,-250);
	return 0;
}
