#pragma once
#include <string>
#include <map>
#include <vector>
class apireq {
public:
	apireq();
	int getBinaAll();
	double getBinaPair(std::string const &pair);
	int getPairBids();
	void listBinaSymbols();
private:
	std::string BINANCE_HOST = "https://www.binance.com/";
	int BINANCE_PAIR_NUMBER = 0;
	
	std::map<std::string, double> pairMap;

	std::map<std::string, double> varianceMap;

	double dPrice = 0;
};