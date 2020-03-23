#include <sstream>
#include <iostream>
#include <fstream>
#include <regex>
#include <Poco/StringTokenizer.h>
#include "parseCsv.h"

using Poco::StringTokenizer;

parseCsv::parseCsv(const std::string &ifname) {
	_iStream.open(ifname);
}

//only reads header, first line 
bool parseCsv::readHeader() {
	std::vector<std::string> fields;
	if (_iStream.is_open() == false) return false;
	_iStream.seekg(0, std::ios::beg);

	std::string header;
	std::getline(_iStream, header);

	auto rs = std::regex_replace(header, std::regex("\\s+"), "");
	StringTokenizer tokenizer(rs, SEPARATOR, 1);

	for (const auto &t : tokenizer) {
		fields.push_back(t);
	}
	std::cout << std::endl;

	_headerV = fields;
	return true;
}

bool parseCsv::readRow(std::map<std::string, std::string> &rowMap) {
	std::string row;
	std::getline(_iStream, row);

	if (_iStream.eof()) return false;

	StringTokenizer tokenizer(row, SEPARATOR, 1);
	
	//TODO put a log err here
	if (tokenizer.count() != _headerV.size()){
		std::cerr << "WARNING: #cols: "<<tokenizer.count()
			<<", #header keys: "<<_headerV.size()<< std::endl;
		std::cerr << row << std::endl;
	}

	for (int i = 0; i < _headerV.size(); ++i)
	{
		if (i >= tokenizer.count()) continue;
		if (tokenizer[i].empty()) tokenizer[i] = "0";
		rowMap[_headerV[i]] = tokenizer[i];
	}

	return true;
}


parseCsv::~parseCsv(){
	_iStream.close();
}
