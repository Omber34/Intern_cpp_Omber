#include "container.hpp"

int main()
{
	srand(time(NULL));
	MyContainer<int> mc(20,20);
	auto test_lambda = [](MyContainer<int>::cont_type::value_type const & i) {return !(i.second % 2); };
	size_t str = 0;
	for (size_t i = 0; i < 20; i++)
	{
		mc.insert(rand()%mc.size().first, rand() % mc.size().second, rand() % 50);
	}
	std::cout << "random filled container: " << std::endl;
	auto out = mc.begin();
	while (out != mc.end())
	{
		if (str == 5) {
			str = 0;
			std::cout << std::endl;
		}
		else
			str++;
		std::cout << *out << '\t';
		++out;
	}
	std::cout << std::endl;	
	std::cout << "delete next element \'!(i%2)\':" << std::endl;
	auto it = std::find_if(mc.begin(), mc.end(), test_lambda);
	str = 0;
	while (it != mc.end())
	{
		if (str == 5) {
			str = 0;
			std::cout << std::endl;
		}
		else
			str++;
		std::cout << *it << '\t';
		mc.erase(it);
		it = std::find_if(mc.begin(), mc.end(), test_lambda);
	}
	std::cout << std::endl;
	std::cout << "result container with inserted 0,0 elem" << std::endl;
	mc.insert(0, 0, rand()%20);
	out = mc.begin();
	str = 0;
	while (out != mc.end())
	{
		if (str == 5) {
			str = 0;
			std::cout << std::endl;
		}
		else
			str++;
		std::cout << *out << '\t';
		++out;
	}
	std::cout << std::endl;
	std::cout << "size (std::distance(begin,end))= " << std::distance(mc.begin(), mc.end()) << std::endl;
	std::cout << "size (real_size)= " << mc.real_size() << std::endl;
	system("pause");
	return 0;
}