#include "apireq.h"
#include "cconnect.h"
#include <iostream>
#include "json.hpp"
#include <map>
#include <regex>
#include <string>
#include "utils.h"

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

	BINANCE_PAIR_NUMBER = j.size();

	// Create a map with symbol-price pairs
	for (int i = 0; i < BINANCE_PAIR_NUMBER; i++) {
		string sym = j[i]["symbol"];
		string sPrice = j[i]["price"];
		dPrice = std::stod(sPrice);

		pairMap.insert(std::make_pair(sym, dPrice));
	}

	return 0;
}

//bid[0](green)=latest asks=red ~ threads could be useful
int apireq::getPairBids() {

	getBinaAll();

	utils u;
	cconnect getAll;

	string requestData;
	double dp = 0;
	int cnt = 0;

	for (const auto &elem : pairMap) {
		// skip buggy entries
		if (elem.first == "123456" || elem.first == "BTMETH" 
			|| elem.first == "ELCBTC" || elem.first == "ETC"
			|| elem.first == "HCCBTC" || elem.first == "LLTBTC") {
			continue;
		}

		requestData = getAll.connector(BINANCE_HOST + "api/v1/depth?symbol=" + elem.first + "&limit=5");

		std::stringstream ss;
		ss << requestData;
		json j = json::parse(ss);
		
		string sBid = j["bids"][0][0];
		string sAsk = j["asks"][0][0];
		double bid = std::stod(sBid);
		double ask = std::stod(sAsk);
		
		double variance = ask - bid;
	
		dp = u.percDif(ask, bid);
		
		varianceMap.insert(std::make_pair(elem.first, dp));
		
		if (dp > 2) {
			cout << std::fixed << elem.first << "::" << std::setprecision(2) << dp << "%::";
			cout << std::setprecision(8) << "(" << variance << ")" << endl;
		}
		//cout << "\r>Processing " << elem.first << "(" << cnt << "/" << BINANCE_PAIR_NUMBER << ")\r";

		cnt++;
	}

	cout << endl;

//	for (const auto &elem : varianceMap) {
//		cout << std::fixed << std::setprecision(2) << elem.first << "::" << elem.second << "%" << endl;
//	}

	//todo: list by biggest variance, check volume smh
	
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