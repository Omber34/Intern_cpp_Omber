#include "ant_alh.h"

void goAnt1(Ant * a)
{
	(*a)();
}
void Graph::goAnts()
{
	
	for (size_t i = 1; i < _routs->getPointsCount()+1; i++)
	{
		_ants.push_back(new Ant(i, _routs));
		std::thread(goAnt1, _ants[_ants.size()-1]).detach();
	}
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

void Graph::goAnt()
{
	Ant(1, _routs)();
}

float Ant::processProbability(size_t _end, std::vector<Route> const & posRout = std::vector<Route>())
{
	float num = 0.0f;
	float div = 0.0f;
	Route &r = (*_routes)[std::make_pair(pos, _end)];
	num = (1.0f / static_cast<float>(r.getDist())) * r.getFerom();
	std::vector<Route> prob;
	if (posRout.size() == 0)
		prob = _routes->getPosibleRoutes(pos, _black_list);
	else
		prob = posRout;
	for (auto it : prob)
		div += (1.0f / static_cast<float>(it.getDist())) * it.getFerom();
	return 100.0f * (num / div);
}

size_t Ant::getNextPos()
{
	
	std::vector< std::pair<size_t, float> > prob;
	auto probRouts = _routes->getPosibleRoutes(pos, _black_list);
	for (auto it : probRouts)
		prob.push_back(std::make_pair(it.getEndRoute(pos), processProbability(it.getEndRoute(pos),probRouts)));
	int rr = rand() % 100;
	float sum = 0;
	for (size_t i = 0; i < prob.size(); i++)
	{
		if ((sum += prob[i].second) >= rr)
			return prob[i].first;
	}
	return 0;
}

Ant::Ant(size_t _p, RouteCont * _r) :
	pos(_p),
	_routes(_r),
	start(_p)
{
	_black_list.push_back(_p);
}

void Ant::operator()()
{
	for (int i = 0; i < 20; i++) {
		
		size_t nextPos = getNextPos();
		while (nextPos != 0)
		{
			_black_list.push_back(nextPos);
			_route.push_back((*_routes)[std::make_pair(pos, nextPos)]);
			pos = nextPos;
			nextPos = getNextPos();
		}
		_route.push_back((*_routes)[std::make_pair(pos, start)]);
		writeResult();
		(*_routes).writeResult(getResult());
		pos = start;
		_black_list.clear();
		_black_list.push_back(start);
		_route.clear();
	}
}

AntRes Ant::getResult()
{
	return AntRes{ getResDist(), _route };
}

size_t Ant::getResDist()
{
	size_t res = 0;
	for (auto it : _route)
		res += it.getDist();
	return res;
}

void Ant::writeResult()
{
	AntRes ar = getResult();
	std::lock_guard<std::mutex> lock(mx);
	if (ar.dist < (*_routes).min_dist) {
		(*_routes).min_dist = ar.dist;
		(*_routes).min_route = ar._route;
	}
	for (auto it : ar._route)
	{
		float delta = Q / static_cast<float>(it.getDist());
		auto f = (*_routes)._cont.find(std::make_pair(it.getBegin(), it.getEnd()));
		if (f != (*_routes)._cont.end())
			(*f).second.deltaFerom(delta);
	}
	(*_routes).printMinRoute();
	std::cout << "MIN DIST: " << (*_routes).getMinDist() << std::endl;
}

void RouteCont::writeResult(AntRes  ar)
{
	std::lock_guard<std::mutex> lock(mx);
	if (ar.dist < min_dist) {
		min_dist = ar.dist;
		min_route = ar._route;
	}
	for (auto it : ar._route)
	{
		float delta = Q / static_cast<float>(it.getDist());
		auto f = _cont.find(std::make_pair(it.getBegin(), it.getEnd()));
		if (f != _cont.end())
			(*f).second.deltaFerom(delta);
	}
	printMinRoute();
	std::cout << "MIN DIST: " << getMinDist() << std::endl;
}

std::vector<Route> RouteCont::getPosibleRoutes(size_t start, std::vector<size_t>const & _bl = std::vector<size_t>())
{
	std::vector<Route> res;
	for (auto it : _cont)
	{
		if (it.first.first == start)
		{
			if (std::find(_bl.begin(), _bl.end(), (it).second.getEnd()) != _bl.end())
				continue;
			res.push_back((it).second);
		}
		if (it.first.second == start)
		{
			if (std::find(_bl.begin(), _bl.end(), (it).second.getBegin()) != _bl.end())
				continue;
			res.push_back((it).second);
		}
	}
	return res;
}

Route & RouteCont::operator[](std::pair<size_t, size_t> const & _p)
{
	auto R = _cont.find(_p);
	if (R == _cont.end()) {
		R = _cont.find(std::make_pair(_p.second, _p.first));
		if (R == _cont.end()) {
		
			throw std::logic_error("can't find route");
		}
		else {
			return (*R).second;
		}
	}
	else {
		return (*R).second;
	}
}

void RouteCont::printState()
{
	std::cout << "----------------------------------------------------" << std::endl;

	for (auto it : _cont)
		std::cout << "from: " << it.first.first << " to: " << it.first.second << " dist: " << it.second.getDist() << " ferom: " << it.second.getFerom() << std::endl;
	std::cout << "----------------------------------------------------" << std::endl;

}

void RouteCont::printMinRoute()
{
	std::cout << "----------------------------------------------------" << std::endl;
	for (auto it : min_route)
		std::cout << "from: " << it.getBegin() << " to: " << it.getEnd() << " dist: " << it.getDist() << " ferom: " << it.getFerom() << std::endl;
	std::cout << "----------------------------------------------------" << std::endl;

}

size_t Route::getEndRoute(size_t _s) const 
{
	if (_s == begin)
		return end;
	else if (_s == end)
		return begin;
	else 
		throw std::logic_error("wrong start arg");
}
