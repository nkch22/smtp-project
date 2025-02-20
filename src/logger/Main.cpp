#include "Logger.h"



int main() {
	MainLogger logger{LOG_LEVEL_TRACE};

	std::thread th1{[] { LOG_START();
						LOG_SAVE_MESSAGE("Hello from th1!");
						LOG_RETURN_NOTHING();
		}};
	std::thread th2{[]
					{
						LOG_START();
						LOG_SAVE_MESSAGE("Hello from th2!");
						LOG_RETURN_NOTHING();
					}};
	std::thread th3{[]
					{
						LOG_START();
						LOG_SAVE_MESSAGE("Hello from th3!");
						LOG_RETURN_NOTHING();
					}};
	std::thread th4{[]
					{
						LOG_START();
						LOG_SAVE_MESSAGE("Hello from th4!");
						LOG_RETURN_NOTHING();
					}};



	th1.join();
	th2.join();
	th3.join();
	th4.join();
}