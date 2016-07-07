//
//  GuideInfo.cpp
//  BotDota
//
//  Created by xyh on 15/8/10.
//
//

#include "GuideInfo.h"
#include "utils/gcstring.h"

USING_NS_CC;


GuideInfoParser::GuideInfoParser()
:m_vecStep()
,m_vecHand()
,m_vecTalk()
,m_vecRect()
,m_vecDialog()
,m_vecLimit()
,m_vecEvent()
{
    readFromCsv();
}

void GuideInfoParser::parseStepInfo(int idx)
{
    StepInfo info;
    info.setId(atoi(m_csvHelper.getData(idx, 1)));
    info.setNextStepId(atoi(m_csvHelper.getData(idx, 2)));
    info.setTalkId(atoi(m_csvHelper.getData(idx, 3)));
    info.setRectId(atoi(m_csvHelper.getData(idx, 4)));
    info.setHandId(atoi(m_csvHelper.getData(idx, 5)));
    info.setDialogId(atoi(m_csvHelper.getData(idx, 6)));
    info.setLimitId(atoi(m_csvHelper.getData(idx, 7)));
    info.setEventId(atoi(m_csvHelper.getData(idx, 8)));
    m_vecStep.push_back(info);
}

void GuideInfoParser::parseHandInfo(int idx)
{
    HandInfo info;
    info.setId(atoi(m_csvHelper.getData(idx, 1)));
    info.setHandType(atoi(m_csvHelper.getData(idx, 2)));
    info.setHandFlipType(atoi(m_csvHelper.getData(idx, 3)));
    info.setHandFrom(parsePoint(m_csvHelper.getData(idx, 4)));
    info.setHandTo(parsePoint(m_csvHelper.getData(idx, 5)));
    m_vecHand.push_back(info);
}

void GuideInfoParser::parseTalkInfo(int idx)
{
    TalkInfo info;
    info.setId(atoi(m_csvHelper.getData(idx, 1)));
    info.setTalkType(atoi(m_csvHelper.getData(idx, 2)));
    info.setTalkShowType(atoi(m_csvHelper.getData(idx, 3)));
    info.setTalkContent(m_csvHelper.getData(idx, 4));
    m_vecTalk.push_back(info);
}

void GuideInfoParser::parseRectInfo(int idx)
{
    RectInfo info;
    info.setId(atoi(m_csvHelper.getData(idx, 1)));
    info.setName(m_csvHelper.getData(idx, 2));
    m_vecRect.push_back(info);
}

void GuideInfoParser::parseDialogInfo(int idx)
{
    DialogInfo info;
    info.setId(atoi(m_csvHelper.getData(idx, 1)));
    info.setFirstTime(atoi(m_csvHelper.getData(idx, 2)));
    info.setType(atoi(m_csvHelper.getData(idx, 3)));
    info.setName(m_csvHelper.getData(idx, 4));
    info.setContent(m_csvHelper.getData(idx, 5));
    m_vecDialog.push_back(info);
}

void GuideInfoParser::parseLimitInfo(int idx)
{
    LimitInfo info;
    info.setId(atoi(m_csvHelper.getData(idx, 1)));
    info.setLimitPlayerLevel(atoi(m_csvHelper.getData(idx, 2)));
    info.setLimitMapLevel(atoi(m_csvHelper.getData(idx, 3)));
    info.setLimitMapEventId(atoi(m_csvHelper.getData(idx, 4)));
    m_vecLimit.push_back(info);
}

void GuideInfoParser::parseEventInfo(int idx)
{
    EventInfo info;
    info.setId(atoi(m_csvHelper.getData(idx, 1)));
    info.setEventName(m_csvHelper.getData(idx, 2));
    info.setEventUserData(atoi(m_csvHelper.getData(idx, 3)));
    m_vecEvent.push_back(info);
}

void GuideInfoParser::readFromCsv()
{
    //read csv config file
    if (m_csvHelper.openAndResolveFile("guide.csv") == false)
    {
        return;
    }
    
    for (int i = 0; i < m_csvHelper.getRowLength(); ++i)
    {
        const char *type = m_csvHelper.getData(i, 0);
        
        if (strcmp(type, "STEP") == 0)
        {
            parseStepInfo(i);
        }

        if (strcmp(type, "HAND") == 0)
        {
            parseHandInfo(i);
        }

        if (strcmp(type, "TALK") == 0)
        {
            parseTalkInfo(i);
        }
        
        if (strcmp(type, "DIALOG") == 0)
        {
            parseDialogInfo(i);
        }
        
        if (strcmp(type, "RECT") == 0)
        {
            parseRectInfo(i);
        }
        
        if (strcmp(type, "LIMIT") == 0)
        {
            parseLimitInfo(i);
        }
        
        if (strcmp(type, "EVENT") == 0)
        {
            parseEventInfo(i);
        }
    }
}

Point GuideInfoParser::parsePoint(std::string pointStr)
{
    std::vector<int> vecInt = gc::utils::stringToVecInt(pointStr,";");
    return Point(vecInt.at(0), vecInt.at(1));
}

Rect GuideInfoParser::parseRect(std::string pointStr)
{
    std::vector<int> vecInt = gc::utils::stringToVecInt(pointStr,";");
    return Rect(vecInt.at(0), vecInt.at(1), vecInt.at(2), vecInt.at(3));
}

Size GuideInfoParser::parseSize(std::string pointStr)
{
    std::vector<int> vecInt = gc::utils::stringToVecInt(pointStr,";");
    return Size(vecInt.at(0), vecInt.at(1));
}

const std::vector<StepInfo> GuideInfoParser::getSteps() const
{
    return m_vecStep;
}

const std::vector<HandInfo> GuideInfoParser::getHands() const
{
    return m_vecHand;
}

const std::vector<TalkInfo> GuideInfoParser::getTalks() const
{
    return m_vecTalk;
}

const std::vector<RectInfo> GuideInfoParser::getRects() const
{
    return m_vecRect;
}

const std::vector<DialogInfo> GuideInfoParser::getDialogs() const
{
    return m_vecDialog;
}

const std::vector<LimitInfo> GuideInfoParser::getLimits() const
{
    return m_vecLimit;
}

const std::vector<EventInfo> GuideInfoParser::getEvents() const
{
    return m_vecEvent;
}
