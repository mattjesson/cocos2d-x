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
#include "CCParallaxNode.h"
#include "CGPointExtension.h"
#include "support/data_support/ccCArray.h"

namespace cocos2d {

	class CGPointObject : NSObject
	{
		CCX_SYNTHESIZE(CGPoint, m_tRatio, Ratio)
		CCX_SYNTHESIZE(CGPoint, m_tOffset, Offset)
		CCX_SYNTHESIZE(CCNode *,m_pChild, Child)	// weak ref

		static CGPointObject * pointWithCGPoint(CGPoint ratio, CGPoint offset)
		{
			CGPointObject *pRet = new CGPointObject();
			pRet->initWithCGPoint(ratio, offset);
			pRet->autorelease();
			return pRet;
		}
		bool initWithCGPoint(CGPoint ratio, CGPoint offset)
		{
			m_tRatio = ratio;
			m_tOffset = offset;
			m_pChild = NULL;
			return true;
		}
	};

	CCParallaxNode::CCParallaxNode()
	{
		m_pParallaxArray = ccArrayNew(5);		
		m_tLastPosition = CGPointMake(-100,-100);
	}
	CCParallaxNode::~CCParallaxNode()
	{
		if( m_pParallaxArray )
		{
			ccArrayFree(m_pParallaxArray);
			m_pParallaxArray = NULL;
		}
	}
	CCParallaxNode * CCParallaxNode::node()
	{
		CCParallaxNode *pRet = new CCParallaxNode();
		pRet->autorelease();
		return pRet;
	}
	CCNode * CCParallaxNode::addChild(CCNode * child, int zOrder, int tag)
	{
		NSAssert(0,"ParallaxNode: use addChild:z:parallaxRatio:positionOffset instead");
		return NULL; 
	}
	CCParallaxNode * CCParallaxNode::addChild(CCNode *child, int z, CGPoint ratio, CGPoint offset)
	{
		NSAssert( child != NULL, "Argument must be non-nil");
		CGPointObject *obj = CGPointObject::pointWithCGPoint(ratio, offset);
		obj->setChild(child);
		ccArrayAppendObjectWithResize(m_pParallaxArray, (NSObject*)obj);

		CGPoint pos = m_tPosition;
		float x = pos.x * ratio.x + offset.x;
		float y = pos.y * ratio.y + offset.y;
		child->setPosition(ccp(x,y));

		return (CCParallaxNode*)CCNode::addChild(child, z, child->getTag());
	}
	void CCParallaxNode::removeChild(CCNode* child, bool cleanup)
	{
		for( unsigned int i=0;i < m_pParallaxArray->num;i++)
		{
			CGPointObject *point = (CGPointObject*)m_pParallaxArray->arr[i];
			if( point->getChild()->isEqual(child)) 
			{
				ccArrayRemoveObjectAtIndex(m_pParallaxArray, i);
				break;
			}
		}
		CCNode::removeChild(child, cleanup);
	}
	void CCParallaxNode::removeAllChildrenWithCleanup(bool cleanup)
	{
		ccArrayRemoveAllObjects(m_pParallaxArray);
		CCNode::removeAllChildrenWithCleanup(cleanup);
	}
	CGPoint CCParallaxNode::absolutePosition()
	{
		CGPoint ret = m_tPosition;
		CCNode *cn = this;
		while (cn->getParent() != NULL)
		{
			cn = cn->getParent();
			ret = ccpAdd( ret,  cn->getPosition());
		}
		return ret;
	}

	/*
	The positions are updated at visit because:
	- using a timer is not guaranteed that it will called after all the positions were updated
	- overriding "draw" will only precise if the children have a z > 0
	*/
	void CCParallaxNode::visit()
	{
		//	CGPoint pos = position_;
		//	CGPoint	pos = [self convertToWorldSpace:CGPointZero];
		CGPoint pos = this->absolutePosition();
		if( ! CGPoint::CGPointEqualToPoint(pos, m_tLastPosition) )
		{
			for(unsigned int i=0; i < m_pParallaxArray->num; i++ ) 
			{
				CGPointObject *point = (CGPointObject*)m_pParallaxArray->arr[i];
				float x = -pos.x + pos.x * point->getRatio().x + point->getOffset().x;
				float y = -pos.y + pos.y * point->getRatio().y + point->getOffset().y;			
				point->getChild()->setPosition(ccp(x,y));
			}
			m_tLastPosition = pos;
		}
		CCNode::visit();
	}

}// namespace cocos2d
