//
//  GuideNode.h
//  BotDota
//
//  Created by xyh on 15/8/10.
//
//

#ifndef __BotDota__GuideNode__
#define __BotDota__GuideNode__

#include "cocos2d.h"
#include "base/gcMacros.h"
#include "components/guide/GuideInfo.h"


class GuideNode
{
public:
    static void showHand(cocos2d::Node* parent, const HandInfo& info);
    static void showHandMove(cocos2d::Node* parent, const HandInfo& info);
    static void showTalk(cocos2d::Node* parent, const TalkInfo& info);
    static void showDialog(cocos2d::Node* parent, const DialogInfo& info);
};


class TalkNode : public cocos2d::Node
{
protected:
    TalkNode();
    ~TalkNode();
    
public:
    CREATE_FUNC(TalkNode);
    
    void initData(const TalkInfo& info);
    
private:
    std::string getContentByLength(int length);
    void updateContent(float dt);
    void initContentLength();
    void initUI();
    void initTimer();
    void addListener();
    void removeListener();
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

private:
    GC_SYNTHESIZE(std::string, m_content, Content);
    GC_SYNTHESIZE(int, m_contentLength, ContentLength);
    GC_SYNTHESIZE(int, m_type, Type);
    GC_SYNTHESIZE(int, m_showType, ShowType);

    int m_wordCount;
    cocos2d::Label* m_label;
    cocos2d::EventListenerTouchOneByOne* m_listener;
    cocos2d::Sprite* m_arrow;
};

class DialogNode : public cocos2d::Node
{
private:
    enum
    {
        SEE = 0,
        KNOW = 1,
    };
    
protected:
    DialogNode();
    ~DialogNode();
    
public:
    CREATE_FUNC(DialogNode);
    
    void initData(const DialogInfo& info);
    
private:
    void initUI();
    void btnCloseClick(cocos2d::Ref* target);
    void btnSeeClick(cocos2d::Ref* target);
    
private:
    cocos2d::Label* m_label;
    std::string m_content;
    std::string m_theme;
    bool m_firstTime;
    int m_type;
};

#endif /* defined(__BotDota__GuideNode__) */
