//
//  GCEventHandler.hpp
//  BotDota
//
//  Created by xyh on 15/12/24.
//
//

#ifndef GCEventHandler_h
#define GCEventHandler_h

#include "cocos2d.h"

class EventHandler
{
protected:
    virtual void handleEvent(cocos2d::EventCustom* event){}
    
public:
    virtual void notice(const std::string &eventName, void *optionalUserData = nullptr);
    void setEventHandler(EventHandler* handler);
    EventHandler* getEventHandler();
    
private:
    EventHandler*  m_eventHandler;
};


#endif /* GCEventHandler_h */
