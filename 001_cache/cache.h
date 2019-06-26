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


template<class Result, class ... Args>
class Cache
{

	typedef Result(*func)(Args...);
	std::unordered_map<boost::tuple<Args...>, Result> _cache;
	func Func;
public:
	Cache(func f) : Func(f) 
	{}
	Result find(Args && ... args)
	{		
		auto t = boost::make_tuple<Args ...>(std::forward<Args...>(args)...);
		auto ft = _cache.find(t);
		if (ft != _cache.end())
		{
			std::cout << "*using cache* " ;
			return _cache[t];
		}
		Result r = Func(std::forward<Args...>(args...));
		_cache.emplace(t, r);
		return r;
	}

};
