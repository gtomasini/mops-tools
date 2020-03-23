#pragma once
#include <string>
#include <vector>
#include <map>
#include <fstream>

class parseCsv{
	std::ifstream	_iStream;
	std::vector<std::string>  _headerV;
	const std::string SEPARATOR=",";
	
public:
	parseCsv(const std::string &ifname);//opens file fname

	bool readHeader();//readHeader and fills field2Col

	bool fileIsOpen() const {
		return _iStream.is_open();
	}

	//return false if EOF reached
	bool readRow(std::map<std::string, std::string> &row);
	
	bool endOfFile() const{
		return _iStream.eof();
	}

	std::vector<std::string>  getHeaderV() const {
		return _headerV;
	}

	virtual ~parseCsv();//close file
};

