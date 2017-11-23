#include "apireq.h"
#include "curl\curl.h"
#include <iostream>
#include <string>
#include "utils.h"

using std::cin;
using std::cout;
using std::endl;

int main() {

	curl_global_init(CURL_GLOBAL_DEFAULT);

	apireq api;
	utils util;

	bool done = false;
	char option;
	std::string pair;
	double amount;

	cout << "::CryptoTools:: (v0.1.3)" << endl;

	while (!done) {	
		cout << "\n1. Request pair price" << endl;
		cout << "2. Crypto to USD" << endl;
		cout << "3. List symbols\n4. Quit" << endl;
		cout << ">";
		cin >> option;
		cout << ">";

		switch (option)	{
		case '1':
			cin >> pair;
			api.getBinaPair(pair);
			break;
		case '2':
			cout << "Pair: ";
			cin >> pair;
			cout << ">Balance: ";
			cin >> amount;
			util.cryptoValue(amount, api.getBinaPair(pair));
			break;
		case '3':
			api.listBinaSymbols();
			break;
		case '4':
			done = true;
			break;
		default:
			cout << "hmm?" << endl;
			break;
		}
	}
	
	curl_global_cleanup();
	return 0;

}




