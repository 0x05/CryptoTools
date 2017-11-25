#include "apireq.h"
#include "cconnect.h"
#include <iostream>
#include "json.hpp"
#include <map>
#include <regex>
#include <string>

using std::cout;
using std::endl;
using std::string;
using namespace nlohmann;

apireq::apireq() {}

int apireq::getBinaAll(){
	cconnect getAll;
	string requestData = getAll.connector(BINANCE_HOST + "api/v1/ticker/allPrices");

	// cURL request to json object
	std::stringstream ss;
	ss << requestData;
	json j = json::parse(ss);

	int n = j.size();

	// Create a map with symbol-price pairs
	for (int i = 0; i < n; i++) {
		string sym = j[i]["symbol"];
		string sPrice = j[i]["price"];
		dPrice = std::stod(sPrice);

		pairMap.insert(std::make_pair(sym, dPrice));
	}

	return 0;
}

// Get price of a single pair
double apireq::getBinaPair(string const &pair) {
	// Validate input
	std::regex validPair(R"([A-Z]{4,10})");
	std::smatch match;

	double targetPrice = 0;

	if (regex_search(pair, match, validPair)) {
		
		getBinaAll();

		for (const auto &elem : pairMap) {
			if (elem.first == pair) {
				targetPrice = elem.second;
				cout << elem.second;
			}
		}
	} else {
		cout << "Invalid Pair" << endl;
	}
	
	return targetPrice;
}

// Print symbol names
void apireq::listBinaSymbols() {
	
	getBinaAll();

	for (const auto &elem : pairMap) {
		cout << elem.first << ",";
	}
}