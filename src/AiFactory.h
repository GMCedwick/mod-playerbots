// AiFactory.h

/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license.
 * You may redistribute it and/or modify it under version 2 of the License, or (at your option),
 * any later version.
 */

#ifndef _PLAYERBOT_AIFACTORY_H
#define _PLAYERBOT_AIFACTORY_H

#include "Common.h"
#include <map>
#include <string>

class AiObjectContext;
class Engine;
class Player;
class PlayerbotAI;

enum class BotRoles : uint8_t;

class AiFactory
{
public:
    static AiObjectContext* CreateAiObjectContext(Player* player, PlayerbotAI* botAI);
    static Engine* CreateCombatEngine(Player* player, PlayerbotAI* botAI, AiObjectContext* aiObjectContext);
    static Engine* CreateNonCombatEngine(Player* player, PlayerbotAI* botAI, AiObjectContext* aiObjectContext);
    static Engine* CreateDeadEngine(Player* player, PlayerbotAI* botAI, AiObjectContext* aiObjectContext);

    static void AddDefaultNonCombatStrategies(Player* player, PlayerbotAI* botAI, Engine* nonCombatEngine);
    static void AddDefaultDeadStrategies(Player* player, PlayerbotAI* botAI, Engine* deadEngine);
    static void AddDefaultCombatStrategies(Player* player, PlayerbotAI* botAI, Engine* engine);

    static uint8_t GetPlayerSpecTab(Player* player);
    static std::map<uint8_t, uint32_t> GetPlayerSpecTabs(Player* player);
    static BotRoles GetPlayerRoles(Player* player);
    static std::string GetPlayerSpecName(Player* player);
};

#endif // _PLAYERBOT_AIFACTORY_H
