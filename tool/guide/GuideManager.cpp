//
//  GuideManager.cpp
//  BotDota
//
//  Created by xyh on 15/8/10.
//
//

#include "GuideManager.h"
#include "components/guide/GuideEvent.h"
#include "Common/GCAssistant.h"
#include "Common/Facade.h"
#include "components/guide/GuideHeader.h"



USING_NS_CC;
USING_NS_GC;


GuideManager::GuideManager()
:m_vecStep()
,m_vecHand()
,m_vecTalk()
,m_vecRect()
,m_vecDialog()
,m_vecLimit()
,m_vecEvent()
,m_nextStepId(0)
,m_wait(false)
,m_parent(NULL)
,m_guideSleep(false)
,m_guideLayer(NULL)
{
    initGuideInfo();
}

GuideManager::~GuideManager()
{

}

void GuideManager::beginGuide(int step)
{
    Assistant::sharedAssistant()->loadResourceFromFile(P_Guide);
    m_guideLayer = GuideLayer::create(Rect());
    m_guideLayer->retain();
    addListener();
    setNextStepId(step);
}

void GuideManager::endGuide()
{
    Assistant::sharedAssistant()->removeResourceFromFile(P_Guide, T_Guide);
    m_guideLayer->removeFromParent();
    m_guideLayer->release();
    removeListener();
    GuideHelper::getInstance()->setInGuide(false);
}

void GuideManager::addListener()
{
    auto func = [this](EventCustom* event)
    {
        this->handleEvent(event);
    };
    
    ADD_LISTENER(GUIDE_EVENT_TOUCH_IN_RECT, func);
    ADD_LISTENER(GUIDE_EVENT_OUT_OF_RECT, func);
    ADD_LISTENER(GUIDE_EVENT_BEGIN_GUIDE, func);
    ADD_LISTENER(GUIDE_EVENT_CONTENUE, func);
    ADD_LISTENER(GUIDE_EVENT_WAIT, func);
}

void GuideManager::removeListener()
{
    REMOVE_LISTENER(GUIDE_EVENT_TOUCH_IN_RECT);
    REMOVE_LISTENER(GUIDE_EVENT_OUT_OF_RECT);
    REMOVE_LISTENER(GUIDE_EVENT_BEGIN_GUIDE);
    REMOVE_LISTENER(GUIDE_EVENT_CONTENUE);
    REMOVE_LISTENER(GUIDE_EVENT_WAIT);
}

void GuideManager::initGuideInfo()
{
    GuideInfoParser parser;
    m_vecHand.clear();
    m_vecTalk.clear();
    m_vecStep.clear();
    m_vecRect.clear();
    m_vecDialog.clear();
    m_vecLimit.clear();
    m_vecEvent.clear();
    m_vecStep = parser.getSteps();
    m_vecHand = parser.getHands();
    m_vecTalk = parser.getTalks();
    m_vecRect = parser.getRects();
    m_vecDialog = parser.getDialogs();
    m_vecLimit = parser.getLimits();
    m_vecEvent = parser.getEvents();
}

void GuideManager::handleEvent(cocos2d::EventCustom* event)
{
    void* userdata = event->getUserData();
    if (userdata)
    {
        setParent((Node*)userdata);
    }
    
    if (event->getEventName() == GUIDE_EVENT_TOUCH_IN_RECT)
    {
        nextStep(getNextStepId());
    }
    else if (event->getEventName() == GUIDE_EVENT_BEGIN_GUIDE)
    {
        nextStep(0); // test here
    }
    else if (event->getEventName() == GUIDE_EVENT_CONTENUE)
    {
        setWait(false);
        nextStep(getNextStepId());
    }
    else
    {}
}

void GuideManager::nextStep(int step)
{
    CCLOG("step = %d",step);

    const StepInfo& stepInfo = m_vecStep.at(step);
    
    if (getWait())
    {
        return;
    }
    
    if (isLimit(stepInfo))
    {
        return;
    }
    
    m_guideLayer->removeFromParent();
    getParent()->addChild(m_guideLayer, GUIDE_LAYER_ZORDER);
    
    if (stepInfo.getNextStepId() == END)
    {
        endGuide();
        return;
    }
    else if (stepInfo.getNextStepId() == WAIT)
    {
        setWait(true);
    }
    else if (stepInfo.getNextStepId() == SLEEP)
    {
        getGuideLayer()->sleep();
    }
    else if (stepInfo.getNextStepId() == WAKE)
    {
        getGuideLayer()->wake();
    }
    
    
    // by order
    checkDialog(stepInfo);
    checkRect(stepInfo);
    checkTalk(stepInfo);
    checkHand(stepInfo);
    checkEvent(stepInfo);
    
    setNextStepId(step + 1);
}

GuideLayer* GuideManager::getGuideLayer()
{
    return m_guideLayer;
}

void GuideManager::checkHand(const StepInfo& info)
{
    if (info.getHandId() != INVALID_IDX)
    {
        const HandInfo& handInfo = m_vecHand.at(info.getHandId());
        
        if (handInfo.getHandType() == CLICK)
        {
            getGuideLayer()->showHandClick(handInfo);
        }
        else
        {
            getGuideLayer()->showHandMove(handInfo);
        }
    }
}

void GuideManager::checkTalk(const StepInfo& info)
{
    if (info.getTalkId() != INVALID_IDX)
    {
        const TalkInfo& talkInfo = m_vecTalk.at(info.getTalkId());
        getGuideLayer()->showTalk(talkInfo);
    }
}

void GuideManager::checkDialog(const StepInfo& info)
{
    if (info.getDialogId() != INVALID_IDX)
    {
        const DialogInfo& dialogInfo = m_vecDialog.at(info.getDialogId());
        getGuideLayer()->showDialog(dialogInfo);
    }
}

void GuideManager::checkRect(const StepInfo& info)
{
    if (info.getRectId() != INVALID_IDX)
    {
        if (info.getRectId() == PAUSE)
        {
            getGuideLayer()->wait(); // white mask full size of window, swallow all touch
        }
        else
        {
            const RectInfo& rectInfo = m_vecRect.at(info.getRectId());
            getGuideLayer()->resetCliping(rectInfo);
        }
    }
    else
    {
        getGuideLayer()->reset(); // win size mask
    }
}

void GuideManager::checkEvent(const StepInfo& info)
{
    if (info.getEventId() != INVALID_IDX)
    {
        const EventInfo& eventInfo = m_vecEvent.at(info.getEventId());
        getGuideLayer()->sentEvent(eventInfo);
    }
}

bool GuideManager::isLimit(const StepInfo& info)
{
    if (info.getLimitId() != INVALID_IDX)
    {
        const LimitInfo& limitInfo = m_vecLimit.at(info.getLimitId());
        bool player_level_Limit = true;
        bool map_level_Limit = true;
        bool map_event_Limit = true;

        if (limitInfo.getLimitPlayerLevel() != INVALID_IDX)
        {
            player_level_Limit = Facade::getPlayerInfo()->getUserlv() < limitInfo.getLimitPlayerLevel();
        }
        
        if (limitInfo.getLimitMapLevel() != INVALID_IDX)
        {
            map_level_Limit = Facade::isMapLevelLocked(limitInfo.getLimitMapLevel());
            
            if (limitInfo.getLimitMapLevel() != INVALID_IDX)
            {
                map_event_Limit = Facade::isMapHandEventFinished(limitInfo.getLimitMapLevel(), limitInfo.getLimitMapEventId());
            }
        }
        
        return player_level_Limit && map_level_Limit && map_event_Limit;
    }
    
    return false;
}

void GuideManager::beginGuideIndependent(int flagId)
{
    beginGuide(searchStepId(flagId));
    GuideHelper::getInstance()->setInGuide(true);
}

void GuideManager::showDialog(SystemOpenInfo* info)
{
    DialogInfo dialogInfo;
    dialogInfo.setContent(info->getDesc());
    dialogInfo.setId(0);
    dialogInfo.setFirstTime(1);
    dialogInfo.setType(1);
    
    getGuideLayer()->showDialog(dialogInfo);
}

int GuideManager::searchStepId(int flagId)
{
    for (int i = 0; i < m_vecStep.size(); i++)
    {
        const StepInfo& info = m_vecStep.at(i);
        if (info.getId() == flagId)
        {
            return i;
        }
    }
    
    return (int)END;
}













