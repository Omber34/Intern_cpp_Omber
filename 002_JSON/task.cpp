#include "task.h"


Task::Task(const std::string & fname)
{
    std::ifstream json(fname);
    rapidjson::Document doc;
    std::string line;
	std::getline(json, line);
	doc.Parse(line.c_str());
	if (doc.IsArray())
		for (size_t i{ 0 }; i < doc.Size(); i++)
		{
			_values.push_back(doc[i].GetInt());
		}
}

void Task::insert_back(int i)
{
	_values.push_back(i);
}

void Task::insert(int i, size_t pos)
{
	_values.insert(_values.begin() + pos, i);
}



size_t Task::find(int _i)
{
    for (size_t i{0};i<_values.size();i++)
        if (_values[i] == _i)
	        return i;
	return -1;
}

std::vector<int> Task::find(bool(*f)(int))
{
	std::vector<int> res;
	auto ins = std::back_inserter(res);
	std::copy_if(_values.begin(), _values.end(), ins, f);
	return res;
}

void Task::erase(int _i)
{
	_values.erase(std::find(_values.begin(),_values.end(),_i));
}

void Task::erase(size_t _i)
{
	 _values.erase(_values.begin() + _i);
}

void Task::erase(bool(*f)(int))
{
	_values.resize(std::remove_if(_values.begin(), _values.end(), f) - _values.begin());
}

void Task::print()
{
    for (auto i : _values)
        std::cout << i << ' '; 
}

void Task::writeResult()
{
	rapidjson::Document d;
	std::ofstream ofs("output.json");
	rapidjson::OStreamWrapper osw(ofs);
	rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
	std::stringstream ss;
	writer.StartObject();
	writer.Key("current array");
	writer.StartArray();
	for (int i : _values)
		writer.Int(i);
	writer.EndArray();
	writer.Key("min value");
	writer.Int(getMin());
	writer.Key("max value");
	writer.Int(getMax());
	ss << "find al elements btw " << -10 << ',' << 5;
	writer.Key(ss.str().c_str());
	writer.StartArray();
	for (int i : find([](int i ) {return -10 < i && i < 5;}))
		writer.Int(i);
	writer.EndArray();
	writer.Key("delete all elements less than 0");
	erase([](int i) {return i < 0; });
	writer.StartArray();
	for (int i : _values)
		writer.Int(i);
	writer.EndArray();
	writer.Key("insert rands at front");
	for (size_t i = 0; i < 3; i++)
	{
		insert(rand() % 10, 0);
	}
	writer.StartArray();
	for (int i : _values)
		writer.Int(i);
	writer.EndArray();
	writer.Key("insert rands at back");
	for (size_t i = 0; i < 3; i++)
	{
		insert_back(rand() % 10);
	}
	writer.StartArray();
	for (int i : _values)
		writer.Int(i);
	writer.EndArray();
	writer.EndObject();
}

int Task::getMax()
{	
	return *std::max_element(_values.begin(), _values.end());
}

int Task::getMin()
{
	return *std::min_element(_values.begin(), _values.end());
}