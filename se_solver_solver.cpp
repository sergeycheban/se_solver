#include "se_solver_solver.h"

#include <cassert>
#include <format>

std::string Solver::solve(double a, double b, double c)
{
    if (!a)
        return std::format("({} {} {}): Not a quadratic equation", a, b, c);

    double extremum = -b / (2 * a);

    double d = b * b - 4 * a * c;

    std::string roots;

    if (d < 0) {
        roots = "No roots";
    }
    else {
        double sqrtD = std::sqrt(d);
        double r1 = (-b - sqrtD) / (2 * a);
        double r2 = (-b + sqrtD) / (2 * a);
        roots = std::format("x1 = {}, x2 = {}", r1, r2);
    }

    std::string minmax;
    if (a < 0) {
        minmax = std::format("Xmax = {}", extremum);
    }
    else {
        minmax = std::format("Xmin = {}", extremum);
    }

    return std::format("({} {} {}): {}, {}", a, b, c, roots, minmax);
}

void Solver::loop()
{
    std::queue<Task> tasks;
    bool stopFlag = false;
    while (true) {
        {
            std::unique_lock lock(d_mutex);
            d_condvar.wait(lock, [this] { return !d_queue.empty() || d_stopFlag; });

            assert(tasks.empty());
            tasks.swap(d_queue);
            stopFlag = d_stopFlag;
        }

        while (!tasks.empty()) {
            const auto& task = tasks.front();
            *task.result_p = solve(task.a, task.b, task.c);
            tasks.pop();
        }

        if (stopFlag) {
            return;
        }
    }
}

Solver::Solver()
: d_stopFlag(false)
, d_thread([this] { loop(); })
{
}

Solver::~Solver()
{
    wait();
}

void Solver::wait()
{
    if (!d_thread.joinable())
        return;

    {
        std::lock_guard lock(d_mutex);
        d_stopFlag = true;
    }
    d_condvar.notify_one();
    d_thread.join();
}

void Solver::addTask(std::string* result_p, int a, int b, int c)
{
    {
        std::lock_guard lock(d_mutex);
        d_queue.push({ result_p, a, b, c });
    }
    d_condvar.notify_one();
}
