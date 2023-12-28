#include "ChatFilter.h"
#include "Group.h"
#include "Playerbots.h"
#include "RtiTargetValue.h"
#include <string>
#include <vector>
#include <map>
#include <cstdlib> // for std::atoi

std::string ChatFilter::Filter(const std::string& message)
{
    size_t atPos = message.find('@');
    if (atPos == std::string::npos)
        return message;

    size_t spacePos = message.find(' ', atPos);
    return spacePos != std::string::npos ? message.substr(spacePos + 1) : message;
}

class StrategyChatFilter : public ChatFilter
{
public:
    explicit StrategyChatFilter(PlayerbotAI* botAI) : ChatFilter(botAI) {}

    std::string Filter(const std::string& message) override
    {
        Player* bot = botAI->GetBot();

        if (message.rfind("@tank", 0) == 0 && !botAI->IsTank(bot))
            return "";

        if (message.rfind("@dps", 0) == 0 && (botAI->IsTank(bot) || botAI->IsHeal(bot)))
            return "";

        if (message.rfind("@heal", 0) == 0 && !botAI->IsHeal(bot))
            return "";

        if (message.rfind("@ranged", 0) == 0 && !botAI->IsRanged(bot))
            return "";

        if (message.rfind("@melee", 0) == 0 && botAI->IsRanged(bot))
            return "";

        return ChatFilter::Filter(message);
    }
};

class LevelChatFilter : public ChatFilter
{
public:
    explicit LevelChatFilter(PlayerbotAI* botAI) : ChatFilter(botAI) {}

    std::string Filter(const std::string& message) override
    {
        Player* bot = botAI->GetBot();

        if (message[0] != '@')
            return message;

        size_t dashPos = message.find('-');
        size_t spacePos = message.find(' ');
        uint32 fromLevel = std::atoi(message.substr(1, dashPos - 1).c_str());
        uint32 toLevel = dashPos != std::string::npos ? std::atoi(message.substr(dashPos + 1, spacePos - dashPos - 1).c_str()) : fromLevel;

        if (bot->getLevel() >= fromLevel && bot->getLevel() <= toLevel)
            return ChatFilter::Filter(message);

        return message;
    }
};

class CombatTypeChatFilter : public ChatFilter
{
public:
    explicit CombatTypeChatFilter(PlayerbotAI* botAI) : ChatFilter(botAI) {}

    std::string Filter(const std::string& message) override
    {
        Player* bot = botAI->GetBot();

        bool melee = message.rfind("@melee", 0) == 0;
        bool ranged = message.rfind("@ranged", 0) == 0;

        if (!melee && !ranged)
            return message;

        switch (bot->getClass())
        {
            case CLASS_WARRIOR:
            case CLASS_PALADIN:
            case CLASS_ROGUE:
            case CLASS_DEATH_KNIGHT:
                if (ranged)
                    return "";
                break;
            case CLASS_HUNTER:
            case CLASS_PRIEST:
            case CLASS_MAGE:
            case CLASS_WARLOCK:
                if (melee)
                    return "";
                break;
            case CLASS_DRUID:
                if (ranged && botAI->IsTank(bot))
                    return "";
                if (melee && !botAI->IsTank(bot))
                    return "";
                break;
            case CLASS_SHAMAN:
                if (melee && botAI->IsHeal(bot))
                    return "";
                if (ranged && !botAI->IsHeal(bot))
                    return "";
                break;
        }

        return ChatFilter::Filter(message);
    }
};

class RtiChatFilter : public ChatFilter
{
public:
    explicit RtiChatFilter(PlayerbotAI* botAI) : ChatFilter(botAI)
    {
        rtis = {"@star", "@circle", "@diamond", "@triangle", "@moon", "@square", "@cross", "@skull"};
    }

    std::string Filter(const std
