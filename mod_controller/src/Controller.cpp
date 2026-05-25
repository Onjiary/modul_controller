/* * Controller - Central Communication Hub
 * --------------------------------------
 * Responsible for the Module Postman System.
 * Coordinates inter-module messaging and event dispatching.
 */

#include "Controller.h"
#include "Entities/Object/ObjectGuid.h"
#include <iostream>
#include "ScriptMgr.h"
#include "ScriptObject.h"

 /** * 1. Postman (The Dispatcher)
  * Receives a packet and distributes it to all modules
  * that are registered for this specific event.
  */
void Controller::Postman(uint32 postsender, uint32 eventId, ObjectGuid targetGuid)
{
    // Security check: Is the packet valid?
    if (targetGuid.IsEmpty())
    {
        return;
    }

    // Check if any modules are registered for this event
    auto it = _subscribers.find(eventId);
    if (it != _subscribers.end())
    {
        // Iterate through all registered recipient modules
        for (uint32 recipientId : it->second)
        {
            std::cout << "[Postman] Event " << eventId
                << " from module " << postsender
                << " delivered to recipient " << recipientId
                << " (Target: " << targetGuid.GetRawValue() << ")"
                << std::endl;
        }
    }
}

/** * 2. PostmanRecipient (The Registrar)
 * A module registers itself to listen for a specific event.
 */
void Controller::PostmanRecipient(uint32 postrecipient, uint32 runpostrecipient)
{
    // Register module ID in the list for this event
    _subscribers[runpostrecipient].push_back(postrecipient);

    std::cout << "[Recipient] Module " << postrecipient
        << " registered for event " << runpostrecipient
        << std::endl;
}

/** * 3. PostmanExaminer (The Security Guard)
 * Checks incoming mail for authorization.
 */
void Controller::PostmanExaminer(uint32 postexaminer, uint32 runpostexaminer)
{
    // Implement blacklists or permission checks here.
    if (postexaminer == 0)
    {
        std::cerr << "[Examiner] ERROR: Unknown sender (ID 0)!" << std::endl;
        return;
    }

    std::cout << "[Examiner] Event " << runpostexaminer
        << " from module " << postexaminer
        << " checked and approved."
        << std::endl;
}

// Class to hook the module into the AzerothCore script system
class MyControllerScript : public ScriptObject {
public:
    MyControllerScript() : ScriptObject("MyControllerScript") {
        // Module registration message upon server startup.
        std::cout << ">>> Hello! The Post-System Controller is active and ready to handle events. <<<" << std::endl;
    }
};

// Function to initialize the script
void Addmod_Controller()
{
    new MyControllerScript();
}
