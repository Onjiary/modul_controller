/* * Controller - Central High-Performance Framework
 * -----------------------------------------------
 */

#include "Controller.h"
#include "ScriptMgr.h"
#include "ScriptObject.h"
#include "Entities/Object/ObjectGuid.h"
#include <iostream>

 // 1. Globale Instanz
Controller* sController = new Controller();

// 2. Initialisierung der Worker-Threads
Controller::Controller() {
    for (int i = 0; i < 4; ++i) {
        _workers.emplace_back(&Controller::WorkerLoop, this);
    }
}

// 3. Destruktor zum sauberen Beenden
Controller::~Controller() {
    {
        std::lock_guard<std::mutex> lock(_queueMutex);
        _stop = true;
    }
    _cv.notify_all();
    for (auto& worker : _workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

// 4. Worker Loop
void Controller::WorkerLoop() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(_queueMutex);
            _cv.wait(lock, [this] { return _stop || !_taskQueue.empty(); });

            if (_stop && _taskQueue.empty()) return;

            task = std::move(_taskQueue.front());
            _taskQueue.pop();
        }
        task();
    }
}

// 5. Interface Funktionen
void Controller::DispatchAsync(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(_queueMutex);
        _taskQueue.push(std::move(task));
    }
    _cv.notify_one();
}

void Controller::SubmitResult(ResultData res) {
    std::lock_guard<std::mutex> lock(_resultMutex);
    _resultQueue.push(res);
}

bool Controller::HasResults() {
    std::lock_guard<std::mutex> lock(_resultMutex);
    return !_resultQueue.empty();
}

ResultData Controller::PopResult() {
    std::lock_guard<std::mutex> lock(_resultMutex);
    ResultData res = _resultQueue.front();
    _resultQueue.pop();
    return res;
}

// 6. Registrierung für den AzerothCore-Linker
class ControllerSystemScript : public ScriptObject
{
public:
    ControllerSystemScript() : ScriptObject("ControllerSystemScript") {
        std::cout << ">>> Controller-Framework geladen und bereit. <<<" << std::endl;
    }
};


void Controller::AddMainThreadTask(std::function<void()> task) {
    std::lock_guard<std::mutex> lock(_mainThreadMutex);
    _mainThreadTasks.push(std::move(task));
}

void Controller::UpdateMainThreadTasks() {
    std::lock_guard<std::mutex> lock(_mainThreadMutex);
    while (!_mainThreadTasks.empty()) {
        auto task = std::move(_mainThreadTasks.front());
        _mainThreadTasks.pop();
        task();
    }
}


class ControllerWorldScript : public WorldScript {
public:
    ControllerWorldScript() : WorldScript("ControllerWorldScript") {}

    void OnUpdate(uint32 /*diff*/) override {
        // Hier wird bei jedem Tick des Servers geprüft, ob Aufgaben für den Main-Thread anstehen
        sController->UpdateMainThreadTasks();
    }
};


void Addmod_Controller()
{
    new ControllerSystemScript();
}
