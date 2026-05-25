#pragma once
#include "Common.h"
#include <map>
#include <vector>
#include "Entities/Object/ObjectGuid.h"

class Controller
{
public:
    static Controller* instance() {
        static Controller instance;
        return &instance;
    }

    void Postman(uint32 postsender, uint32 eventId, ObjectGuid targetGuid);
    void PostmanRecipient(uint32 postrecipient, uint32 runpostrecipient);
    void PostmanExaminer(uint32 postexaminer, uint32 runpostexaminer);

private:
    // The "mailbox": EventID -> List of module IDs
    std::map<uint32, std::vector<uint32>> _subscribers;
};

#define sController Controller::instance()
