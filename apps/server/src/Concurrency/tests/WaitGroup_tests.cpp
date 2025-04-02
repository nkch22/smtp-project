#include <Concurrency/WaitGroup.h>
#include <gtest/gtest.h>
#include <thread>

using namespace std::chrono_literals;

TEST(WaitGroup, JustWorks)
{
	concurrency::WaitGroup wg;

	wg.Add(1);
	wg.Done();
	wg.Wait();

	SUCCEED();
}

TEST(WaitGroup, InitZero)
{
	concurrency::WaitGroup wg;
	wg.Wait();
}

TEST(WaitGroup, AddCount)
{
	concurrency::WaitGroup wg;

	wg.Add(7);
	for (size_t i = 0; i < 7; ++i)
	{
		wg.Done();
	}

	wg.Wait();

	SUCCEED();
}

TEST(WaitGroup, Wait)
{
	concurrency::WaitGroup wg;
	bool ready = false;

	wg.Add(1);

	std::thread producer(
		[&]
		{
			std::this_thread::sleep_for(1s);
			ready = true;
			wg.Done();
		});

	wg.Wait();
	ASSERT_TRUE(ready);

	producer.join();
}

TEST(WaitGroup, MultiWait)
{
	std::vector<std::thread> threads;

	concurrency::WaitGroup wg;

	std::atomic<size_t> work{0};

	static constexpr size_t kWorkers = 3;
	static constexpr size_t kWaiters = 4;

	wg.Add(kWorkers);
	for (size_t i = 0; i < kWaiters; ++i)
	{
		threads.emplace_back(
			[&]
			{
				wg.Wait();
				ASSERT_EQ(work.load(), kWorkers);
			});
	}

	for (size_t i = 1; i <= kWorkers; ++i)
	{
		threads.emplace_back(
			[&, i]
			{
				std::this_thread::sleep_for(256ms * i);
				++work;
				wg.Done();
			});
	}

	for (auto&& t : threads)
	{
		t.join();
	}
}

TEST(WaitGroup, Cyclic)
{
	concurrency::WaitGroup wg;

	for (size_t i = 0; i < 4; ++i)
	{
		bool flag = false;

		wg.Add(1);

		std::thread worker(
			[&]
			{
				std::this_thread::sleep_for(1s);
				flag = true;
				wg.Done();
			});

		wg.Wait();

		ASSERT_TRUE(flag);

		worker.join();
	}
}