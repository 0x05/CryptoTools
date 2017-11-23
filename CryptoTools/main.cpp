#include "apireq.h"
#include "curl\curl.h"
#include <iostream>
#include <string>
#include "utils.h"

using std::cin;
using std::cout;
using std::endl;

//int percDif(double x, double y);

int main() {

	curl_global_init(CURL_GLOBAL_DEFAULT);

	apireq a;
	utils u;

	bool done = false;
	char option;
	std::string pair;
	double amount;

	cout << "::CryptoTools:: (v0.1.1)" << endl;

	while (!done) {	
		cout << "1. Request pair price" << endl;
		cout << "2. Crypto to USD\n3. Quit" << endl;
		cout << ">";
		cin >> option;
		cout << ">";

		switch (option)	{
		case '1':
			cin >> pair;
			a.getBinaPrice(pair);
			break;
		case '2':
			cout << "Pair: ";
			cin >> pair;
			cout << ">Balance: ";
			cin >> amount;
			u.cryptoValue(amount, a.getBinaPrice(pair));

			break;
		case '3':
			done = true;
			break;
		default:
			cout << "Say Again?" << endl;
			break;
		}

	}
	
	curl_global_cleanup();
	return 0;

}




