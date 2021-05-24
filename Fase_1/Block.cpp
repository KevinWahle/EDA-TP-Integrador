#include "Block.h"
#include "Transaction.h"
#include <iostream>
#include <string>
#include <cmath>
#include <nlohmann/json.hpp>

using namespace std;
using namespace nlohmann;

Block::Block(const std::string& jsonString) {
	json fromJSON =  nlohmann::json::parse(jsonString);

	//Para cada transacci�n, creamos la estructura y la appendeamos.
	for (int i = 0; i < fromJSON["tx"].size(); i++) {
		//string aux = 
		txs.push_back(Transaction((fromJSON["tx"][i]).dump()));
	}

	nonce = fromJSON["nonce"];
	blockId = fromJSON["blockid"];
	prevBlockId = fromJSON["previousblockid"];
	merkleRoot = fromJSON["merkleroot"];
}


FullCompleteTree<string> Block::getMerkleTree() {

	FullCompleteTree<string> merkleTree(2);	// Siempre por lo menos hay dos hojas

	// Creo arbol con cantidad de hojas igual a la potencia de dos mas proxima a txs.size (redondeado para arriba)
	if (txs.size() > 1) {
		merkleTree = FullCompleteTree<string>(exp2(ceil(log2(txs.size()))));
	}

	vector<string> txHashes;
	for (Transaction& tx : txs) {	// Por cada transacci�n
		string id;
		for (TransactionEntry& entry : tx.getEntries()) {	// Por cada entry en la transacci�n
			id += entry.getTxId();								// Concateno los txid
		}
		txHashes.push_back(hexCode(generateID((const unsigned char*)id.c_str())));		// Genero ID en HexCodeASCII
	}

	unsigned int lastRow = merkleTree.getRows() - 1;
	for (int j = 0; j < merkleTree[lastRow].size(); j++) {		// Agrego los nidstr como hojas
		if (j < txHashes.size()) {
			merkleTree[lastRow][j] = txHashes[j];
		}
		else {		// Si no hay mas transacciones, repito la ultima
			merkleTree[lastRow][j] = txHashes[txHashes.size() - 1];
		}
	}

	for (int i = lastRow - 1; i >= 0; i--) {		// Por cada profundidad
		for (int j = 0; j < merkleTree[i].size(); j++) {		// Por cada nodo en esa profundidad
			
			string concat = merkleTree[i + 1][2 * j] + merkleTree[i + 1][2 * j + 1];
			
			merkleTree[i][j] = hexCode(generateID((const unsigned char*)concat.c_str()));
		}
	}

	return merkleTree;
}

unsigned int Block::generateID(const unsigned char* str) {
	unsigned int ID = 0;
	int c;
	while (c = *str++) {
		ID = c + (ID << 6) + (ID << 16) - ID;
	}
	return ID;
}

string Block::hexCode(const unsigned int id) {
	string output;
	const char convTable[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	for (int i = 2*sizeof(id)-1; i >= 0; i--) {
		int temp = (id & (0xF << (4 * i))) >> (4 * i);
		output += convTable[temp];
	}
	return output;
}

string Block::getBlockId(){
	return blockId;
}
string Block::getPrevBlockId(){
	return prevBlockId;
}
string Block::getMerkleRoot(){
	return merkleRoot;
}
unsigned int Block::getNonce(){
	return nonce;
}

vector<Transaction> Block::getTxs(){
	return txs;
}