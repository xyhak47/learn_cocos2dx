//
//  GCEventHandler.cpp
//  BotDota
//
//  Created by xyh on 15/12/24.
//
//

#include "EventHandler.h"


void EventHandler::notice(const std::string &eventName, void *optionalUserData)
{
    if (getEventHandler())
    {
        cocos2d::EventCustom event(eventName);
        event.setUserData(optionalUserData);
        getEventHandler()->handleEvent(&event);
    }
}

void EventHandler::setEventHandler(EventHandler* handler)
{
    m_eventHandler = handler;
}

EventHandler* EventHandler::getEventHandler()
{
    return m_eventHandler;
}
