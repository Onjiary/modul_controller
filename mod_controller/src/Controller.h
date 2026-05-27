#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>
#include <vector>
#include "Define.h" // AzerothCore Basis-Header

// Struktur für den Datenaustausch
struct ResultData {
    uint32 npcId;
    float posX,posY, posZ;
    uint32 playerGuidLow; // Benenne es zur Klarheit um
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

public:
    Controller();
    ~Controller();

    void WorkerLoop();
    void DispatchAsync(std::function<void()> task);
    void SubmitResult(ResultData res);
    bool HasResults();
    ResultData PopResult();
};

// Globaler Zugriffspunkt (extern deklariert)
extern Controller* sController;

#endif
