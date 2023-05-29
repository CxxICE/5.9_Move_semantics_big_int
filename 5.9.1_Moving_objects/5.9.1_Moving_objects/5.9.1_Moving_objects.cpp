#include <iostream>
#include <vector>


template<typename T>
void move_vectors(T &v1, T &v2)
{
	T tmp = std::move(v1);
	v1 = std::move(v2);
	v2 = std::move(tmp);
}

template<typename T>
void print_container(const T &container)
{
	for (const auto &el : container)
	{
		std::cout << el << " ";
	}
	std::cout << std::endl;
};

int main(int argc, char **argv)
{
	std::vector <std::string> one = { "test_string1", "test_string2" };
	std::vector <std::string> two;

	std::cout << "one: ";
	print_container(one);
	std::cout << "two: ";
	print_container(two);

	std::cout << "\nmove_vectors(one, two)\n\n";
	move_vectors(one, two);

	std::cout << "one: ";
	print_container(one);
	std::cout << "two: ";
	print_container(two);

	return 0;
}
