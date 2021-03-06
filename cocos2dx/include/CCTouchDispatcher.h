/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __TOUCH_DISPATCHER_CCTOUCH_DISPATCHER_H__
#define __TOUCH_DISPATCHER_CCTOUCH_DISPATCHER_H__

#include "CCTouchDelegateProtocol.h"
#include "NSObject.h"
#include "NSMutableArray.h"
namespace   cocos2d {

typedef enum
{
	ccTouchSelectorBeganBit = 1 << 0,
	ccTouchSelectorMovedBit = 1 << 1,
	ccTouchSelectorEndedBit = 1 << 2,
	ccTouchSelectorCancelledBit = 1 << 3,
	ccTouchSelectorAllBits = ( ccTouchSelectorBeganBit | ccTouchSelectorMovedBit | ccTouchSelectorEndedBit | ccTouchSelectorCancelledBit),
} ccTouchSelectorFlag;


enum {
	ccTouchBegan,
	ccTouchMoved,
	ccTouchEnded,
	ccTouchCancelled,
	
	ccTouchMax,
};

class NSSet;
class UIEvent;

struct ccTouchHandlerHelperData {
	// we only use the type
//	void (StandardTouchDelegate::*touchesSel)(NSSet*, UIEvent*);
//	void (TargetedTouchDelegate::*touchSel)(NSTouch*, UIEvent*);
	int  m_type;
};


class CCX_DLL EGLTouchDelegate
{
public:
	virtual void touchesBegan(NSSet* touches, UIEvent* pEvent) = 0;
	virtual void touchesMoved(NSSet* touches, UIEvent* pEvent) = 0;
	virtual void touchesEnded(NSSet* touches, UIEvent* pEvent) = 0;
	virtual void touchesCancelled(NSSet* touches, UIEvent* pEvent) = 0;

    virtual ~EGLTouchDelegate() {}
};

class CCTouchHandler;
struct _ccCArray;
/** @brief CCTouchDispatcher.
 Singleton that handles all the touch events.
 The dispatcher dispatches events to the registered TouchHandlers.
 There are 2 different type of touch handlers:
   - Standard Touch Handlers
   - Targeted Touch Handlers
 
 The Standard Touch Handlers work like the CocoaTouch touch handler: a set of touches is passed to the delegate.
 On the other hand, the Targeted Touch Handlers only receive 1 touch at the time, and they can "swallow" touches (avoid the propagation of the event).
 
 Firstly, the dispatcher sends the received touches to the targeted touches.
 These touches can be swallowed by the Targeted Touch Handlers. If there are still remaining touches, then the remaining touches will be sent
 to the Standard Touch Handlers.

 @since v0.8.0
 */
class CCX_DLL CCTouchDispatcher : public NSObject, public EGLTouchDelegate
{
public:
	~CCTouchDispatcher();
	bool init(void);
	CCTouchDispatcher() {}

public:
	/** Whether or not the events are going to be dispatched. Default: true */
    bool isDispatchEvents(void);
	void setDispatchEvents(bool bDispatchEvents);

	/** Adds a standard touch delegate to the dispatcher's list.
	 See StandardTouchDelegate description.
	 IMPORTANT: The delegate will be retained.
	 */
	void addStandardDelegate(CCTouchDelegate *pDelegate, int nPriority);

	/** Adds a targeted touch delegate to the dispatcher's list.
	 See TargetedTouchDelegate description.
	 IMPORTANT: The delegate will be retained.
	 */
	void addTargetedDelegate(CCTouchDelegate *pDelegate, int nPriority, bool bSwallowsTouches);

	/** Removes a touch delegate.
	 The delegate will be released
	 */
	void removeDelegate(CCTouchDelegate *pDelegate);

	/** Removes all touch delegates, releasing all the delegates */
	void removeAllDelegates(void);

	/** Changes the priority of a previously added delegate. The lower the number,
    the higher the priority */
	void setPriority(int nPriority, CCTouchDelegate *pDelegate);

	void touches(NSSet *pTouches, UIEvent *pEvent, unsigned int uIndex);

	virtual void touchesBegan(NSSet* touches, UIEvent* pEvent);
	virtual void touchesMoved(NSSet* touches, UIEvent* pEvent);
	virtual void touchesEnded(NSSet* touches, UIEvent* pEvent);
	virtual void touchesCancelled(NSSet* touches, UIEvent* pEvent);

public:
	/** singleton of the CCTouchDispatcher */
	static CCTouchDispatcher* sharedDispatcher();

protected:
	void forceRemoveDelegate(CCTouchDelegate *pDelegate);
	void forceAddHandler(CCTouchHandler *pHandler, NSMutableArray<CCTouchHandler*> *pArray);
	void forceRemoveAllDelegates(void);

protected:
 	NSMutableArray<CCTouchHandler*> *m_pTargetedHandlers;
 	NSMutableArray<CCTouchHandler*> *m_pStandardHandlers;

	bool m_bLocked;
	bool m_bToAdd;
	bool m_bToRemove;
 	NSMutableArray<CCTouchHandler*> *m_pHandlersToAdd;
	struct _ccCArray *m_pHandlersToRemove;
	bool m_bToQuit;
	bool m_bDispatchEvents;

	// 4, 1 for each type of event
	struct ccTouchHandlerHelperData m_sHandlerHelperData[ccTouchMax];
};
}//namespace   cocos2d 

#endif // __TOUCH_DISPATCHER_CCTOUCH_DISPATCHER_H__
