#include "apireq.h"
#include "cconnect.h"
#include <iostream>
#include "json.hpp"
#include <regex>
#include <string>

using std::cout;
using std::endl;
using namespace nlohmann;

apireq::apireq() {}

double apireq::getBinaPrice(std::string pair) {

	std::regex validPair(R"([A-Z]{4,10})");
	std::smatch match;

	if (regex_search(pair, match, validPair)) {
		cconnect getPrice;
		
		std::string requestData = getPrice.connector(BINANCE_HOST + "api/v1/ticker/allPrices");

		std::stringstream ss;
		ss << requestData;
		json j = json::parse(ss);

		int n = j.size();

		for (int i = 0; i < n; i++) {

			if (j[i]["symbol"] == pair) {
				std::string sPrice = j[i]["price"];
				dPrice = std::stod(sPrice);
				cout << "Price: " << dPrice << endl; //remove?
			}
		}
	}

	else {
		cout << "Invalid Pair" << endl;
	}

	if (dPrice != 0) {
		return dPrice;
	}

	return 0;
}