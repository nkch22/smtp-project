#include <Concurrency/UnboundedBlockingMPMCQueue.h>
#include <gtest/gtest.h>
#include <string>
#include <thread>

template<typename T>
using Queue = concurrency::UnboundedBlockingMPMCQueue<T>;

using namespace std::chrono_literals;

TEST(BlockingQueue, JustWorks)
{
	Queue<int> queue;

	queue.Push(7);

	auto value = queue.Pop();
	ASSERT_TRUE(value);
	ASSERT_EQ(*value, 7);

	queue.Close();
	ASSERT_FALSE(queue.Pop());
}

TEST(BlockingQueue, Fifo)
{
	Queue<int> queue;
	queue.Push(1);
	queue.Push(2);
	queue.Push(3);

	ASSERT_EQ(*queue.Pop(), 1);
	ASSERT_EQ(*queue.Pop(), 2);
	ASSERT_EQ(*queue.Pop(), 3);
}

TEST(BlockingQueue, Close)
{
	Queue<std::string> queue;

	queue.Push("Hello");
	queue.Push(",");
	queue.Push("World");

	queue.Close();

	ASSERT_EQ(*queue.Pop(), "Hello");
	ASSERT_EQ(*queue.Pop(), ",");
	ASSERT_EQ(*queue.Pop(), "World");
	ASSERT_FALSE(queue.Pop());
}

struct MoveOnly
{
	MoveOnly() = default;

	MoveOnly(const MoveOnly& that) = delete;
	MoveOnly& operator=(const MoveOnly& that) = delete;

	MoveOnly(MoveOnly&& that) = default;
	MoveOnly& operator=(MoveOnly&& that) = default;
};

TEST(BlockingQueue, MoveOnly)
{
	Queue<MoveOnly> queue;

	queue.Push(MoveOnly{});
	ASSERT_TRUE(queue.Pop().has_value());
}

TEST(BlockingQueue, BlockingPop)
{
	Queue<int> queue;

	std::thread producer(
		[&]()
		{
			std::this_thread::sleep_for(1s);
			queue.Push(7);
		});

	auto value = queue.Pop();

	ASSERT_TRUE(value);
	ASSERT_EQ(*value, 7);

	producer.join();
}

TEST(BlockingQueue, BlockingPop2)
{
	Queue<int> queue;

	std::thread producer(
		[&]()
		{
			std::this_thread::sleep_for(1s);
			queue.Close();
		});

	auto value = queue.Pop();

	ASSERT_FALSE(value);

	producer.join();
}

TEST(BlockingQueue, UnblockConsumers)
{
	Queue<int> queue;

	// Consumers

	std::thread consumer1([&]() { queue.Pop(); });

	std::thread consumer2([&]() { queue.Pop(); });

	// Producer
	std::this_thread::sleep_for(100ms);
	queue.Close();

	consumer1.join();
	consumer2.join();
}

TEST(BlockingQueue, ProducerConsumer)
{
	Queue<int> queue;

	std::thread producer(
		[&]()
		{
			// Producer
			for (int i = 0; i < 10; ++i)
			{
				queue.Push(i);
				std::this_thread::sleep_for(100ms);
			}
			queue.Close();
		});

	// Consumer

	for (int i = 0; i < 10; ++i)
	{
		auto value = queue.Pop();
		ASSERT_TRUE(value);
		ASSERT_EQ(*value, i);
	}

	ASSERT_FALSE(queue.Pop());

	producer.join();
}
