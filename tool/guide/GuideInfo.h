//
//  GuideInfo.h
//  BotDota
//
//  Created by xyh on 15/8/10.
//
//

#ifndef __BotDota__GuideInfo__
#define __BotDota__GuideInfo__

#include "cocos2d.h"
#include "base/gcMacros.h"
#include "utils/GCCsvHelper.h"

enum
{
    INVALID_IDX = -1,
    WAIT = -1,
    END = -2,
    PAUSE = -2,
    LIMIT = -3,
    SLEEP = -3,
    WAKE = -4,

    CLICK = 0,
    MOVE = 1,
    
    EGG = 0,
    HERO = 1,
    BOSS = 2,
    
    LEFT = 0,
    RIGHT = 1,
    
    RECT = 0,
    ROUND = 1,
};


class StepInfo;
class HandInfo;
class TalkInfo;
class RectInfo;
class DialogInfo;
class LimitInfo;
class EventInfo;

class GuideInfoParser
{
public:
    GuideInfoParser();
    virtual ~GuideInfoParser(){}

public:
    const std::vector<StepInfo> getSteps() const;
    const std::vector<HandInfo> getHands() const;
    const std::vector<TalkInfo> getTalks() const;
    const std::vector<RectInfo> getRects() const;
    const std::vector<DialogInfo> getDialogs() const;
    const std::vector<LimitInfo> getLimits() const;
    const std::vector<EventInfo> getEvents() const;

private:
    void readFromCsv();
    void parseStepInfo(int idx);
    void parseHandInfo(int idx);
    void parseTalkInfo(int idx);
    void parseRectInfo(int idx);
    void parseDialogInfo(int idx);
    void parseLimitInfo(int idx);
    void parseEventInfo(int idx);

    cocos2d::Point parsePoint(std::string pointStr);
    cocos2d::Rect parseRect(std::string pointStr);
    cocos2d::Size parseSize(std::string pointStr);

private:
    gc::utils::CsvHelper m_csvHelper;
    
    std::vector<StepInfo> m_vecStep;
    std::vector<HandInfo> m_vecHand;
    std::vector<TalkInfo> m_vecTalk;
    std::vector<RectInfo> m_vecRect;
    std::vector<DialogInfo> m_vecDialog;
    std::vector<LimitInfo> m_vecLimit;
    std::vector<EventInfo> m_vecEvent;
};

class BaseGuideInfo
{
protected:
    BaseGuideInfo()
    :m_id(0)
    {}
    
    GC_SYNTHESIZE(int, m_id, Id);
};

class StepInfo : public BaseGuideInfo
{
public:
    StepInfo()
    :m_nextstepId(0)
    ,m_talkId(0)
    ,m_rectId(0)
    ,m_dialogId(0)
    ,m_limitId(0)
    ,m_eventId(0)
    {}
    
    GC_SYNTHESIZE(int, m_nextstepId, NextStepId);
    GC_SYNTHESIZE(int, m_talkId, TalkId);
    GC_SYNTHESIZE(int, m_rectId, RectId);
    GC_SYNTHESIZE(int, m_handId, HandId);
    GC_SYNTHESIZE(int, m_dialogId, DialogId);
    GC_SYNTHESIZE(int, m_limitId, LimitId);
    GC_SYNTHESIZE(int, m_eventId, EventId);
};

class HandInfo : public BaseGuideInfo
{
public:
    HandInfo()
    :m_handType(0)
    ,m_handFlipType(0)
    ,m_HandFrom()
    ,m_HandTo()
    ,m_HandPos()
    ,m_visiableRectSize()
    {}
    
    GC_SYNTHESIZE(int, m_handType, HandType);
    GC_SYNTHESIZE(int, m_handFlipType, HandFlipType);
    GC_SYNTHESIZE(cocos2d::Point, m_HandFrom, HandFrom);
    GC_SYNTHESIZE(cocos2d::Point, m_HandTo, HandTo);
    GC_SYNTHESIZE(cocos2d::Point, m_HandPos, HandPos);
    GC_SYNTHESIZE(cocos2d::Size, m_visiableRectSize, VisiableRectSize);
};

class TalkInfo : public BaseGuideInfo
{
public:
    TalkInfo()
    :m_talkType(0)
    ,m_talkShowType(0)
    ,m_talkContent("")
    {}
    
    GC_SYNTHESIZE(int, m_talkType, TalkType);
    GC_SYNTHESIZE(int, m_talkShowType, TalkShowType);
    GC_SYNTHESIZE(std::string, m_talkContent, TalkContent);
};

class RectInfo : public BaseGuideInfo
{
public:
    RectInfo()
    :m_name("")
    {}
    
    GC_SYNTHESIZE(std::string, m_name, Name);
};


class DialogInfo : public BaseGuideInfo
{
public:
    DialogInfo()
    :m_firstTime(0)
    ,m_content("")
    ,m_name("")
    ,m_type(0)
    {}
    
    GC_SYNTHESIZE(int, m_firstTime, FirstTime);
    GC_SYNTHESIZE(std::string, m_content, Content);
    GC_SYNTHESIZE(std::string, m_name, Name);
    GC_SYNTHESIZE(int, m_type, Type);
};


class LimitInfo : public BaseGuideInfo
{
public:
    LimitInfo()
    :m_limitPlayerLevel(0)
    ,m_limitMapLevel(0)
    ,m_limitMapEventId(0)
    {}
    
    GC_SYNTHESIZE(int, m_limitPlayerLevel, LimitPlayerLevel);
    GC_SYNTHESIZE(int, m_limitMapLevel, LimitMapLevel);
    GC_SYNTHESIZE(int, m_limitMapEventId, LimitMapEventId);
};

class EventInfo : public BaseGuideInfo
{
public:
    EventInfo()
    :m_EventName("")
    ,m_EventUserData(0)
    {}
    
    GC_SYNTHESIZE(std::string, m_EventName, EventName);
    GC_SYNTHESIZE(int, m_EventUserData, EventUserData);
};

#endif /* defined(__BotDota__GuideInfo__) */
