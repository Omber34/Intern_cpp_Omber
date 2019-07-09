#include <map>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <iterator>
#include <functional>
template<class _KTy,class _VT>
std::ostream & operator<<(std::ostream & os, const  std::pair<_KTy , _VT> & v)
{
	os << '[' << v.first.first << ']' << '[' << v.first.second << ']'  << v.second;
	return os;
}
namespace std
{
	template<>
	struct std::less< const std::pair<size_t, size_t> >
	{
		bool operator() (std::pair<size_t, size_t> const & l, std::pair<size_t, size_t> const & r)
		{
			return l.first >= r.first ? false : l.second < r.second;
		}
	};
}
//итератор на самом деле input_iterator_tag о для правильной логики std::distance он random_access_iterator_tag
template<class Cont>
class Iterator : public std::iterator<std::random_access_iterator_tag, typename std::map<std::pair<size_t,size_t>,typename Cont::value_type>::iterator >
{
private:

	Cont & _con;
	value_type p;
	using return_type = typename Cont::cont_type::value_type;
public:
	Iterator(Cont & _c, value_type  _p) : p(_p),_con(_c) {}
	Iterator(Iterator const & it) : p(it.p),_con(it._con) {}
	Iterator & operator=(Iterator const & it) { p = it.p; _con = it._con; return *this; }
	bool operator!=(Iterator const& other) const
	{
		return p != other.p;
	}
	bool operator==(Iterator const& other) const
	{
		return !(*this == other);
	}
	return_type & operator*()
	{
		return (*p);
	}
	return_type & operator*() const
	{
		return (*p);
	}
	Iterator& operator++()
	{
		p++;
		return *this;
	}
	
	typename difference_type operator-(Iterator const & _it)
	{
		if (&_con != &_it._con)
			return -1;
		if (_it.p == _con.end().p && p == _con.end().p)
			return 0;
		if (_it.p == _con.end().p)
			return (*_it.p).first.first * _con.size().first + (*_it.p).first.second - _con.real_size();
		if (p == _con.end().p)
			return _con.real_size() - (*_it.p).first.first * _con.size().first + (*_it.p).first.second;
	}


};

template<class _VTy>
class MyContainer
{
public:
	typedef std::map<std::pair<size_t, size_t>, _VTy> cont_type;
private:
	cont_type _data;
	std::pair<size_t, size_t> _size;
	

public:
	typedef Iterator<MyContainer> iterator;
	typedef _VTy value_type;

	MyContainer(size_t _size_x,size_t _size_y): _size(_size_y,_size_x){}

	value_type & at(size_t _y, size_t _x)
	{
		return _data.at(std::make_pair(_y, _x));
	}

	std::pair<iterator,bool> insert(size_t _y, size_t _x, _VTy const & _val)
	{
		if (_y > _size.first || _x > _size.second)
			return std::make_pair<iterator,bool>(iterator(*this,_data.end()), false);
		auto pos = std::make_pair(_y, _x);
		auto it = _data.emplace(pos, _val);
		return std::make_pair(iterator(*this,it.first), it.second);
	}
	void erase(iterator  pos)
	{
		_data.erase((*pos).first);
	}
	iterator begin()
	{
		return iterator(*this, _data.begin());
	}
	iterator end()
	{
		return iterator{ *this, _data.end() };
	}

	std::pair<size_t, size_t> size() const
	{
		return _size;
	}
	size_t real_size() const
	{
		return _size.first * _size.second;
	}
};


namespace std
{
	template <class C>
	struct iterator_traits < Iterator<C> >
	{
		typedef typename  Iterator<C>::iterator::difference_type  difference_type;
		typedef typename  Iterator<C>::iterator::iterator_category  iterator_category;
		typedef typename  Iterator<C>::iterator::pointer  pointer;
		typedef typename  Iterator<C>::iterator::reference reference;
		typedef typename  Iterator<C>::iterator::value_type value_type;


	};
}