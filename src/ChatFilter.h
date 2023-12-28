/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license,
 * you may redistribute it and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_CHATFILTER_H
#define _PLAYERBOT_CHATFILTER_H

#include "Common.h"
#include "PlayerbotAIAware.h"
#include <vector>
#include <memory> // Include for std::unique_ptr

class PlayerbotAI;

class ChatFilter : public PlayerbotAIAware
{
public:
    explicit ChatFilter(PlayerbotAI* botAI) : PlayerbotAIAware(botAI) {}
    virtual ~ChatFilter() {}

    // Changed return type to non-const for flexibility and use const reference for the message
    virtual std::string Filter(const std::string& message);
};

class CompositeChatFilter : public ChatFilter
{
public:
    explicit CompositeChatFilter(PlayerbotAI* botAI);

    // Using override keyword for clarity and modern C++ practice
    ~CompositeChatFilter() override;
    std::string Filter(const std::string& message) override;

private:
    // Using smart pointers for automatic memory management
    std::vector<std::unique_ptr<ChatFilter>> filters;
};

#endif
