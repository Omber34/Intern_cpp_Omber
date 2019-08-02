#pragma once
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <boost/functional/hash.hpp>
#include <boost/accumulators/numeric/functional.hpp>
#include <thread>
#include <mutex>
#include <functional>
#include <memory>
const static float p = 0.3f;
const static float Q = 1.0f;
static std::mutex mx;
class Route
{
	size_t begin;
	size_t end;
	size_t dist;
	float ferom;
public:
	Route(size_t _b, size_t _e, size_t _d) :
		begin(_b),
		end(_e),
		dist(_d) 
	{
		ferom = 1.0f;
	}
	size_t getDist()const { return dist; }
	size_t getBegin() const{ return begin; }
	size_t getEnd()const { return end; }
	size_t getEndRoute(size_t _s)const ;
	float getFerom() const{ return ferom; }
	void deltaFerom(float _delThau) { ferom = ((1 - p)*ferom) + (_delThau); }
};

struct AntRes {
	size_t dist;
	std::vector<Route> _route;
};

class RouteCont
{
	friend class Ant;
	friend class RouteGen;	
	size_t num_of_point = 5;
	size_t min_dist = num_of_point * 100;
	std::vector<Route> min_route;
	std::unordered_map< std::pair<size_t, size_t>, Route, boost::hash<std::pair<size_t, size_t> > > _cont;
public:
	void writeResult(AntRes  ar);
	std::vector<Route> getPosibleRoutes(size_t start, std::vector<size_t> const & _bl);
	Route & operator[] (std::pair<size_t, size_t> const & _p);
	size_t getPointsCount() { return num_of_point; }
	void setMinDist(size_t _d) { if (_d < min_dist) min_dist = _d; }
	size_t getMinDist() { return min_dist; }
	void printState();
	void printMinRoute();
};

class RouteGen {
	size_t max_dist;
	size_t min_dist;
	size_t num_point;
	std::unordered_map<std::pair<size_t, size_t>, Route, boost::hash<std::pair<size_t,size_t>> > & _cont;
	size_t getRandDist() { return rand() % (max_dist - min_dist) + min_dist; };
public:
	RouteGen(size_t _np, size_t _md, size_t _Md, RouteCont * _c) :
		max_dist(_Md),
		min_dist(_md),
		num_point(_np),
		_cont(_c->_cont)
	{}
	void operator() ()
	{
		for (size_t i = 0; i < num_point; i++)
		{
			for (size_t j = i+1; j < num_point; j++)
			{	
				_cont.emplace( std::make_pair(i + 1, j + 1), Route(i+1, j+1, getRandDist()));
			}
		}
	}

};

class Ant {
	RouteCont * _routes;
	size_t start;
	std::vector<Route> _route;
	std::vector<size_t> _black_list;
	float processProbability(size_t _end, std::vector<Route> const & posRout);
	size_t getNextPos();
	size_t getResDist();
	void writeResult();
	size_t pos;
public:
	Ant(size_t _p, RouteCont * _r);
	void operator()();
	AntRes getResult();
};

class Graph
{
	RouteCont * _routs;
	std::vector<Ant *> _ants;
public:
	template<class _Gen = RouteGen>
	Graph(size_t _np, size_t _md, size_t _Md) {
		_routs = new RouteCont();
		_Gen(_np, _md, _Md, _routs)();
	}
	Graph()
	{
		_routs = new RouteCont();
		RouteGen(5, 40, 100, _routs)();
	}
	void goAnts();	
	void goAnt();
	size_t getMinDist() { return _routs->getMinDist(); }
};

