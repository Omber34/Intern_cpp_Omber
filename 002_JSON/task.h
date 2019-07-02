#include "rapidjson/document.h"
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
#include <sstream>
#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
class Task
{
private:
    std::vector<int> _values;
public:
    Task(const std::string & _fname);
    void insert_back(int i);
	void insert(int i, size_t pos);
    size_t find(int i);
	std::vector<int> find(bool(*)(int));
    void erase(int i);
    void erase(size_t _pos);
	void erase(bool(*)(int));
    int getMin();
    int getMax(); 
    void print();
	void writeResult();
};
