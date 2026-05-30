#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>
#include <vector>
#include "Define.h"

struct ResultData {
    uint32 npcId;
    float posX, posY, posZ;
    uint32 playerGuidLow;
};

class Controller {
private:
    std::queue<std::function<void()>> _taskQueue;
    std::queue<ResultData> _resultQueue;
    std::mutex _queueMutex;
    std::mutex _resultMutex;
    std::condition_variable _cv;
    std::vector<std::thread> _workers;
    bool _stop = false;

    // Neue Main-Thread Queue innerhalb der Klasse!
    std::queue<std::function<void()>> _mainThreadTasks;
    std::mutex _mainThreadMutex;

public:
    Controller();
    ~Controller();

    void WorkerLoop();
    void DispatchAsync(std::function<void()> task);
    void SubmitResult(ResultData res);
    bool HasResults();
    ResultData PopResult();

    // Neue Methoden innerhalb der Klasse
    void AddMainThreadTask(std::function<void() > task);
    void UpdateMainThreadTasks();
};

extern Controller* sController;

#endif // CONTROLLER_H
