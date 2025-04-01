#pragma once

#include <functional>

namespace concurrency
{

/**
 * @brief Represents a unit of work to be executed by a worker thread.
 *
 * A `Task` is a callable function with no arguments and no return value.
 * It encapsulates the work that will be scheduled and executed asynchronously
 * by the thread pool.
 */
using Task = std::function<void()>;

} // namespace concurrency
