#pragma once
#include <string>

class apireq {
public:
	apireq();
	double getBinaPrice(std::string pair);
private:
	std::string BINANCE_HOST = "https://www.binance.com/";
	double dPrice = 0;
};