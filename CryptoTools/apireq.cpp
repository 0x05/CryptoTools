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

//bid[0](green)=latest asks=red
int apireq::getPairBids(string const &pair) {

	std::regex validPair(R"([A-Z]{4,10})");
	std::smatch match;

	if (regex_search(pair, match, validPair)) {

		cconnect getAll;
		string requestData = getAll.connector(BINANCE_HOST + "api/v1/depth?symbol=" + pair + "&limit=5");

		// cURL request to json object
		std::stringstream ss;
		ss << requestData;
		json j = json::parse(ss);

		//cout << j << endl;
		//cout << "asks:" << j["asks"][0][0] << endl;
		//cout << "bids:" << j["bids"][0][0] << endl;
		//todo: fix var names, loop all pairs, list by biggest variance, check volume smh
		string s2 = j["asks"][0][0];
		string s1 = j["bids"][0][0];
		double d1 = std::stod(s1);
		double d2 = std::stod(s2);

		double variance = d2 - d1;

		cout << std::fixed << std::setprecision(8) << variance << endl;
	}
	else {
		cout << "Invalid Pair" << endl;
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
				cout << std::fixed << std::setprecision(8) << elem.second;
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