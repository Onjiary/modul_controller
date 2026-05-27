#ifndef EVENT_CONSTANTS_H
#define EVENT_CONSTANTS_H


// Liste der Module-IDs (für den PostmanExaminer)

enum ModuleIds {
    MODULE_CONTROLLER_ID     = 0,
    MODULE_GUILD_HOUSING     = 100,
    MOUDLE_ANNOUNCER         = 101,
    MOUDLE_LOGGIN            =102
};


// Liste der Event-IDs (für die Kommunikation)
enum EventIds {
    EVENT_GUILD_HOUSING_PURCHASED      = 1000,
    EVENT_GUILD_HOUSING_UPGRADE        = 10001,
    EVENT_GUILD_HOUSING_ENTERED        = 10002,


    // Allgemeine System Events (ab 2000)
    EVENT_SERVER_STARTUP = 2000,
    EVENT_PLAYER_LOGGOUT = 2001
};

#endif
