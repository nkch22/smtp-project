#include "Initializer.hpp"

class TestClass
{
private:
	LOGGER_GET_PRIVATE(TestClass)

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

class TestTest
{
protected:
	LOGGER_GET_PRIVATE(TestTest)

	TestClass t;
	int b;

public:
	TestTest() : t{}, b{5} {}
};
MAKE_LOGGABLE(TestTest, t, b)


class Test2 : public TestTest
{
private:
	LOGGER_GET_PRIVATE(Test2)

	double d;

	friend class TestTest;
	
public:
	Test2() : d{5.7} {}
};
MAKE_LOGGABLE(Test2, d, t)


void func(TestClass t)
{
	logger::Logger log;
	log.set_local_level(logger::LOG_LEVEL_TRACE);
	log.log_arguments(t);
}

void func2(TestTest t)
{
	logger::Logger log;
	log.set_local_level(logger::LOG_LEVEL_TRACE);
	log.log_arguments(t);
}

void func3(Test2 t)
{
	logger::Logger log;
	log.set_local_level(logger::LOG_LEVEL_TRACE);
	log.log_arguments(t);
}

int main()
{
	Initializer::Init("config.json");

	func(TestClass{});
	func2(TestTest{});
	func3(Test2{});
}