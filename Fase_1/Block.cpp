#include "Block.h"
#include "Transaction.h"

Block::Block(const nlohmann::json& fromJSON) {

	// M�todo A:
	for (auto& el : fromJSON["tx"].items()) {
		//txs.push_back(Transaction(el));
	}
	// M�todo B: 
	/*for (int i = 0; i < fromJSON["nTx"]; i++){
		txs.push_back(Transaction(fromJSON["tx"](i)));
	}*/
	
	nonce = fromJSON["nonce"];
	blockId = fromJSON["blockid"];
	prevBlockId = fromJSON["previousblockid"];
	merkleRoot = fromJSON["merkleroot"];
	
}