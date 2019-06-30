#pragma once
#include <boost/fusion/tuple.hpp>
#include <boost/fusion/include/tuple.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/fusion/sequence/hash.hpp>
#include <boost/functional/hash.hpp>
#include <boost/fusion/tuple/tuple.hpp>
#include <boost/fusion/sequence/comparison/equal_to.hpp>
#include <boost/fusion/tuple/make_tuple.hpp>
#include <boost/fusion/include/make_tuple.hpp>
#include <boost/fusion/adapted/boost_tuple.hpp>
#include <boost/tuple/tuple.hpp>

#include <unordered_map>
#include <deque>
#include <algorithm>

namespace std {
	template<class ... Args>
	struct hash< boost::tuple<Args...> > {
		size_t operator()(const boost::tuple<Args...> &p) const {
			return boost::fusion::hash_value(p);
		}
	};
	template<class ... Args>
	struct equal_to <boost::tuple<Args...>>
	{
		constexpr bool operator() (const boost::tuple<Args...> & left, const boost::tuple<Args...> & right) const
		{
			return boost::fusion::equal_to(left, right);
		}
	};	
}

template< template <class, class ...> class CP ,class Result, class ... Args>
class Cache
{
	typedef Result(*func)(Args...);
	CP<Result, Args...> cp;	
	std::unordered_map<boost::tuple<Args...>, Result> _cache;

public:
	Cache(func f) : Func(f), cp(10, f, _cache)
	{}
	Result find(Args && ... args)
	{
		return cp.find(std::forward<Args...>(args)...);
	}
};

template<class Result, class ... Args>
class FIFOClearPolicy 
{
	typedef Result(*func)(Args...);
	typedef boost::tuple<Args...> _KTy;
	typedef std::unordered_map<_KTy, Result> _ConTy;
	func Func;
	_ConTy & _con;
	std::deque<_KTy> list;
	size_t size;
public:
	FIFOClearPolicy(size_t _s, func f_ ,_ConTy & _c) :
		 Func(f_),  size(_s), _con(_c)
	{}
	Result find(Args && ... args)
	{
		auto t = boost::make_tuple<Args ...>(std::forward<Args...>(args)...);
		if (_con.find(t) != _con.end())
		{
			auto el = std::find_if(list.begin(), list.end(), [&t](const _KTy & _val) {return boost::fusion::equal_to(t,_val); });
			if (el != list.end())
			{
				std::swap(*el, *(list.end()-1));
			}
			else
			{
				list.push_back(t);
			}
			std::cout << "*using cache* ";
			return _con[t];
		}
		Result r = Func(std::forward<Args...>(args...));
		_con.emplace(t, r);
		list.push_back(t);
		std::cout << "*add to cache* ";
		if (list.size() > size)
		{
			auto del = list.front();
			_con.erase(del);
			list.pop_front();
			std::cout << "*clean up cache* ";
		}
		return r;
	}
};
