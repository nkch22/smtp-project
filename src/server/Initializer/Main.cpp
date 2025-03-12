#include "Initializer.hpp"

class TestClass
{
	GET_PRIVATE(TestClass)

	int a;
	double b;
	char c;
	std::string str;
	unsigned int ui;
	bool bo;

public:
	TestClass(int _a = 4, double _b = 2.5) : a{_a}, b{_b}, c{'h'}, str{"Hello world"}, ui{8}, bo{false} {}

	void print() { std::cout << a << " " << b; }
};

MAKE_LOGGABLE(TestClass, a, b, c, str, ui, bo)

void func(TestClass t)
{
	logger::Logger log;
	log.set_local_level(logger::LOG_LEVEL_TRACE);
	log.log_arguments(t);
}

int main()
{
	Initializer::Init("config.json");

	func(TestClass{});
}