//
//  GuideManager.h
//  BotDota
//
//  Created by xyh on 15/8/10.
//
//

#ifndef __BotDota__GuideManager__
#define __BotDota__GuideManager__

#include "base/gcMacros.h"
#include "components/guide/GuideInfo.h"
#include "components/guide/GuideLayer.h"
#include "Data/SystemOpenInfo.h"

class GuideManager
{
private:
    ~GuideManager();
    GC_SINGLETON(GuideManager);
    
public:
    void beginGuide(int step = 0);
    void beginGuideIndependent(int flagId);
    void showDialog(SystemOpenInfo* info);
    
private:
    void endGuide();

    GuideLayer* getGuideLayer();
    void addListener();
    void removeListener();
    void initGuideInfo();
    void handleEvent(cocos2d::EventCustom* event);
    void initGuideLayey();
    
    void nextStep(int step);

    void checkDialog(const StepInfo& info);
    void checkTalk(const StepInfo& info);
    void checkHand(const StepInfo& info);
    void checkRect(const StepInfo& info);
    void checkEvent(const StepInfo& info);

    bool isLimit(const StepInfo& info);
    
    int searchStepId(int flagId);
    
private:
    std::vector<StepInfo> m_vecStep;
    std::vector<HandInfo> m_vecHand;
    std::vector<TalkInfo> m_vecTalk;
    std::vector<RectInfo> m_vecRect;
    std::vector<DialogInfo> m_vecDialog;
    std::vector<LimitInfo> m_vecLimit;
    std::vector<EventInfo> m_vecEvent;

    GC_SYNTHESIZE(int, m_nextStepId, NextStepId);
    GC_SYNTHESIZE(bool, m_wait, Wait);
    GC_SYNTHESIZE(cocos2d::Node*, m_parent, Parent);
    GC_SYNTHESIZE(bool, m_guideSleep, Sleep);

    GuideLayer* m_guideLayer;
};

#endif /* defined(__BotDota__GuideManager__) */
