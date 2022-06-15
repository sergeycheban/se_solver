#ifndef SE_SOLVER_SOLVER_H
#define SE_SOLVER_SOLVER_H

#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

class Solver
{
    struct Task {
        std::string* result_p;
        int a;
        int b;
        int c;
    };

    std::queue<Task>        d_queue;
    std::mutex              d_mutex; // protects d_queue
    std::condition_variable d_condvar; // notifies about new tasks
    bool                    d_stopFlag;
    std::thread             d_thread;

    void loop();
    // Get quadratic equations from the queue and solve them.

    static std::string solve(double a, double b, double c);
    // Solve a quadratic equation with the specified 'a', 'b' and 'c'.
    // Return the result.

public:
    Solver();
    // Create a new Solver object.

    ~Solver();
    // Destroy this object.

    void wait();
    // Wait until all tasks are solved and stop the solver thread.

    void addTask(std::string* result_p, int a, int b, int c);
    // Add a task to be solved.
};

#endif
