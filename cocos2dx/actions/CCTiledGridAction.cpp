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
#include "CCTiledGridAction.h"
#include "CCDirector.h"
#include "ccMacros.h"
#include "CGPointExtension.h"
#include "effects/CCGrid.h"

#include <stdlib.h>

namespace cocos2d
{
	struct Tile
	{
		CGPoint	position;
		CGPoint	startPosition;
		ccGridSize	delta;
	};

	// implementation of ShakyTiles3D

	CCShakyTiles3D* CCShakyTiles3D::actionWithRange(int nRange, bool bShakeZ, cocos2d::ccGridSize gridSize, cocos2d::ccTime duration)
	{
		CCShakyTiles3D *pAction = new CCShakyTiles3D();

		if (pAction)
		{
			if (pAction->initWithRange(nRange, bShakeZ, gridSize, duration))
			{
				pAction->autorelease();
			}
			else
			{
				CCX_SAFE_RELEASE_NULL(pAction);
			}
		}

		return pAction;	
	}

	bool CCShakyTiles3D::initWithRange(int nRange, bool bShakeZ, cocos2d::ccGridSize gridSize, cocos2d::ccTime duration)
	{
		if (CCTiledGrid3DAction::initWithSize(gridSize, duration))
		{
			m_nRandrange = nRange;
			m_bShakeZ = bShakeZ;

			return true;
		}

		return false;
	}

	NSObject* CCShakyTiles3D::copyWithZone(cocos2d::NSZone *pZone)
	{
		NSZone* pNewZone = NULL;
		CCShakyTiles3D* pCopy = NULL;
		if(pZone && pZone->m_pCopyObject) 
		{
			//in case of being called at sub class
			pCopy = (CCShakyTiles3D*)(pZone->m_pCopyObject);
		}
		else
		{
			pCopy = new CCShakyTiles3D();
			pZone = pNewZone = new NSZone(pCopy);
		}
		
		CCTiledGrid3DAction::copyWithZone(pZone);

		pCopy->initWithRange(m_nRandrange, m_bShakeZ, m_sGridSize, m_fDuration);
		
		CCX_SAFE_DELETE(pNewZone);
		return pCopy;
	}

	void CCShakyTiles3D::update(cocos2d::ccTime time)
	{
		int i, j;
	
		for (i = 0; i < m_sGridSize.x; ++i)
		{
			for (j = 0; j < m_sGridSize.y; ++j)
			{
				ccQuad3 coords = originalTile(ccg(i, j));

				// X
				coords.bl.x += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
				coords.br.x += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
				coords.tl.x += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
				coords.tr.x += ( rand() % (m_nRandrange*2) ) - m_nRandrange;

				// Y
				coords.bl.y += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
				coords.br.y += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
				coords.tl.y += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
				coords.tr.y += ( rand() % (m_nRandrange*2) ) - m_nRandrange;

				if (m_bShakeZ)
				{
					coords.bl.z += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
					coords.br.z += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
					coords.tl.z += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
					coords.tr.z += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
				}
							
				setTile(ccg(i, j), coords);
			}
		}
	}

	// implementation of CCShatteredTiles3D

	CCShatteredTiles3D* CCShatteredTiles3D::actionWithRange(int nRange, bool bShatterZ, cocos2d::ccGridSize gridSize, cocos2d::ccTime duration)
	{
		CCShatteredTiles3D *pAction = new CCShatteredTiles3D();

		if (pAction)
		{
			if (pAction->initWithRange(nRange, bShatterZ, gridSize, duration))
			{
				pAction->autorelease();
			}
			else
			{
				CCX_SAFE_RELEASE_NULL(pAction);
			}
		}

		return pAction;	
	}

	bool CCShatteredTiles3D::initWithRange(int nRange, bool bShatterZ, cocos2d::ccGridSize gridSize, cocos2d::ccTime duration)
	{
		if (CCTiledGrid3DAction::initWithSize(gridSize, duration))
		{
			m_bOnce = false;
			m_nRandrange = nRange;
			m_bShatterZ = bShatterZ;

			return true;
		}

		return false;
	}

	NSObject* CCShatteredTiles3D::copyWithZone(cocos2d::NSZone *pZone)
	{
		NSZone* pNewZone = NULL;
		CCShatteredTiles3D* pCopy = NULL;
		if(pZone && pZone->m_pCopyObject)
		{
			pCopy = (CCShatteredTiles3D*)(pZone->m_pCopyObject);
		}
		else
		{
			pCopy = new CCShatteredTiles3D();
			pZone = pNewZone = new NSZone(pCopy);
		}

		//copy super class's member
		CCTiledGrid3DAction::copyWithZone(pZone);

		pCopy->initWithRange(m_nRandrange, m_bShatterZ, m_sGridSize, m_fDuration);

		CCX_SAFE_DELETE(pNewZone);
		return pCopy;
	}

	void CCShatteredTiles3D::update(cocos2d::ccTime time)
	{
		int i, j;
	
		if (m_bOnce == false)
		{
			for (i = 0; i < m_sGridSize.x; ++i)
			{
				for (j = 0; j < m_sGridSize.y; ++j)
				{
					ccQuad3 coords = originalTile(ccg(i ,j));
					
					// X
					coords.bl.x += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
					coords.br.x += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
					coords.tl.x += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
					coords.tr.x += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
					
					// Y
					coords.bl.y += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
					coords.br.y += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
					coords.tl.y += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
					coords.tr.y += ( rand() % (m_nRandrange*2) ) - m_nRandrange;

					if (m_bShatterZ) 
					{
						coords.bl.z += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
						coords.br.z += ( rand() % (m_nRandrange*2) ) - m_nRandrange;				
						coords.tl.z += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
						coords.tr.z += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
					}
					
					setTile(ccg(i, j), coords);
				}
			}
			
			m_bOnce = true;
		}
	}

	// implementation of CCShuffleTiles

	CCShuffleTiles* CCShuffleTiles::actionWithSeed(int s, cocos2d::ccGridSize gridSize, cocos2d::ccTime duration)
	{
		CCShuffleTiles *pAction = new CCShuffleTiles();

		if (pAction)
		{
			if (pAction->initWithSeed(s, gridSize, duration))
			{
				pAction->autorelease();
			}
			else
			{
				CCX_SAFE_RELEASE_NULL(pAction);
			}
		}

		return pAction;	
	}

	bool CCShuffleTiles::initWithSeed(int s, cocos2d::ccGridSize gridSize, cocos2d::ccTime duration)
	{
		if (CCTiledGrid3DAction::initWithSize(gridSize, duration))
		{
			m_nSeed = s;
			m_pTilesOrder = NULL;
			m_pTiles = NULL;

			return true;
		}

		return false;
	}

	NSObject* CCShuffleTiles::copyWithZone(cocos2d::NSZone *pZone)
	{
		NSZone* pNewZone = NULL;
		CCShuffleTiles* pCopy = NULL;
		if(pZone && pZone->m_pCopyObject)
		{
			pCopy = (CCShuffleTiles*)(pZone->m_pCopyObject);
		}
		else
		{
			pCopy = new CCShuffleTiles();
			pZone = pNewZone = new NSZone(pCopy);
		}

		CCTiledGrid3DAction::copyWithZone(pZone);

		pCopy->initWithSeed(m_nSeed, m_sGridSize, m_fDuration);

		CCX_SAFE_DELETE(pNewZone);
		return pCopy;
	}

	CCShuffleTiles::~CCShuffleTiles(void)
	{
		if (m_pTilesOrder)
		{
			delete[] m_pTilesOrder;
		}

		if (m_pTiles)
		{
			delete[] m_pTiles;
		}
	}

	void CCShuffleTiles::shuffle(int *pArray, int nLen)
	{
		int i;
		for( i = nLen - 1; i >= 0; i-- )
		{
			int j = rand() % (i+1);
			int v = pArray[i];
			pArray[i] = pArray[j];
			pArray[j] = v;
		}
	}

	ccGridSize CCShuffleTiles::getDelta(cocos2d::ccGridSize pos)
	{
		CGPoint	pos2;

		int idx = pos.x * m_sGridSize.y + pos.y;

		pos2.x = (float)(m_pTilesOrder[idx] / (int)m_sGridSize.y);
		pos2.y = (float)(m_pTilesOrder[idx] % (int)m_sGridSize.y);

		return ccg((int)(pos2.x - pos.x), (int)(pos2.y - pos.y));
	}

	void CCShuffleTiles::placeTile(ccGridSize pos, Tile *t)
	{
		ccQuad3 coords = originalTile(pos);

		CGPoint step = m_pTarget->getGrid()->getStep();
		coords.bl.x += (int)(t->position.x * step.x);
		coords.bl.y += (int)(t->position.y * step.y);

		coords.br.x += (int)(t->position.x * step.x);
		coords.br.y += (int)(t->position.y * step.y);

		coords.tl.x += (int)(t->position.x * step.x);
		coords.tl.y += (int)(t->position.y * step.y);

		coords.tr.x += (int)(t->position.x * step.x);
		coords.tr.y += (int)(t->position.y * step.y);

		setTile(pos, coords);
	}

	void CCShuffleTiles::startWithTarget(CCNode *pTarget)
	{
		CCTiledGrid3DAction::startWithTarget(pTarget);

		if (m_nSeed != -1)
		{
			srand(m_nSeed);
		}

		m_nTilesCount = m_sGridSize.x * m_sGridSize.y;
		m_pTilesOrder = new int[m_nTilesCount];
		int i, j;

		for (i = 0; i < m_nTilesCount; ++i)
		{
			m_pTilesOrder[i] = i;
		}

		shuffle(m_pTilesOrder, m_nTilesCount);

		m_pTiles = (struct Tile *)new Tile[m_nTilesCount];
		Tile *tileArray = (Tile*) m_pTiles;

		for (i = 0; i < m_sGridSize.x; ++i)
		{
			for (j = 0; j < m_sGridSize.y; ++j)
			{
				tileArray->position = ccp((float)i, (float)j);
				tileArray->startPosition = ccp((float)i, (float)j);
				tileArray->delta = getDelta(ccg(i, j));
				++tileArray;
			}
		}
	}

	void CCShuffleTiles::update(cocos2d::ccTime time)
	{
		int i, j;

		Tile *tileArray = (Tile*)m_pTiles;

		for (i = 0; i < m_sGridSize.x; ++i)
		{
			for (j = 0; j < m_sGridSize.y; ++j)
			{
				tileArray->position = ccpMult(ccp((float)tileArray->delta.x, (float)tileArray->delta.y), time);
				placeTile(ccg(i, j), tileArray);
				++tileArray;
			}
		}
	}

	// implementation of CCFadeOutTRTiles

	CCFadeOutTRTiles* CCFadeOutTRTiles::actionWithSize(cocos2d::ccGridSize gridSize, cocos2d::ccTime time)
	{
		CCFadeOutTRTiles *pAction = new CCFadeOutTRTiles();

		if (pAction)
		{
			if (pAction->initWithSize(gridSize, time))
			{
				pAction->autorelease();
			}
			else
			{
				CCX_SAFE_RELEASE_NULL(pAction);
			}
		}

		return pAction;	
	}

	float CCFadeOutTRTiles::testFunc(cocos2d::ccGridSize pos, cocos2d::ccTime time)
	{
		CGPoint n = ccpMult(ccp((float)m_sGridSize.x, (float)m_sGridSize.y), time);
		if ((n.x + n.y) == 0.0f)
		{
			return 1.0f;
		}

		return powf((pos.x + pos.y) / (n.x + n.y), 6);
	}

	void CCFadeOutTRTiles::turnOnTile(cocos2d::ccGridSize pos)
	{
		setTile(pos, originalTile(pos));
	}

	void CCFadeOutTRTiles::turnOffTile(cocos2d::ccGridSize pos)
	{
		ccQuad3 coords;
		memset(&coords, 0, sizeof(ccQuad3));
		setTile(pos, coords);
	}

	void CCFadeOutTRTiles::transformTile(cocos2d::ccGridSize pos, float distance)
	{
		ccQuad3 coords = originalTile(pos);
		CGPoint step = m_pTarget->getGrid()->getStep();

		coords.bl.x += (step.x / 2) * (1.0f - distance);
		coords.bl.y += (step.y / 2) * (1.0f - distance);

		coords.br.x -= (step.x / 2) * (1.0f - distance);
		coords.br.y += (step.y / 2) * (1.0f - distance);

		coords.tl.x += (step.x / 2) * (1.0f - distance);
		coords.tl.y -= (step.y / 2) * (1.0f - distance);

		coords.tr.x -= (step.x / 2) * (1.0f - distance);
		coords.tr.y -= (step.y / 2) * (1.0f - distance);

		setTile(pos, coords);
	}

	void CCFadeOutTRTiles::update(cocos2d::ccTime time)
	{
		int i, j;

		for (i = 0; i < m_sGridSize.x; ++i)
		{
			for (j = 0; j < m_sGridSize.y; ++j)
			{
				float distance = testFunc(ccg(i, j), time);
				if ( distance == 0 )
				{
					turnOffTile(ccg(i, j));
				} else 
				if (distance < 1)
				{
					transformTile(ccg(i, j), distance);
				}
				else
				{
					turnOnTile(ccg(i, j));
				}
			}
		}
	}

	// implementation of CCFadeOutBLTiles
	CCFadeOutBLTiles* CCFadeOutBLTiles::actionWithSize(cocos2d::ccGridSize gridSize, cocos2d::ccTime time)
	{
		CCFadeOutBLTiles *pAction = new CCFadeOutBLTiles();

		if (pAction)
		{
			if (pAction->initWithSize(gridSize, time))
			{
				pAction->autorelease();
			}
			else
			{
				CCX_SAFE_RELEASE_NULL(pAction);
			}
		}

		return pAction;
	}

	float CCFadeOutBLTiles::testFunc(cocos2d::ccGridSize pos, cocos2d::ccTime time)
	{
		CGPoint n = ccpMult(ccp((float)m_sGridSize.x, (float)m_sGridSize.y), (1.0f - time));
		if ((pos.x + pos.y) == 0)
		{
			return 1.0f;
		}

		return powf((n.x + n.y) / (pos.x + pos.y), 6);
	}

	// implementation of CCFadeOutUpTiles

	CCFadeOutUpTiles* CCFadeOutUpTiles::actionWithSize(cocos2d::ccGridSize gridSize, cocos2d::ccTime time)
	{
		CCFadeOutUpTiles *pAction = new CCFadeOutUpTiles();

		if (pAction)
		{
			if (pAction->initWithSize(gridSize, time))
			{
				pAction->autorelease();
			}
			else
			{
				CCX_SAFE_RELEASE_NULL(pAction);
			}
		}

		return pAction;
	}

	float CCFadeOutUpTiles::testFunc(cocos2d::ccGridSize pos, cocos2d::ccTime time)
	{
		CGPoint n = ccpMult(ccp((float)m_sGridSize.x, (float)m_sGridSize.y), time);
		if (n.y == 0.0f)
		{
			return 1.0f;
		}

		return powf(pos.y / n.y, 6);
	}

	void CCFadeOutUpTiles::transformTile(cocos2d::ccGridSize pos, float distance)
	{
		ccQuad3 coords = originalTile(pos);
		CGPoint step = m_pTarget->getGrid()->getStep();

		coords.bl.y += (step.y / 2) * (1.0f - distance);
		coords.br.y += (step.y / 2) * (1.0f - distance);
		coords.tl.y -= (step.y / 2) * (1.0f - distance);
		coords.tr.y -= (step.y / 2) * (1.0f - distance);

		setTile(pos, coords);
	}

	// implementation of CCFadeOutDownTiles
	CCFadeOutDownTiles* CCFadeOutDownTiles::actionWithSize(cocos2d::ccGridSize gridSize, cocos2d::ccTime time)
	{
		CCFadeOutDownTiles *pAction = new CCFadeOutDownTiles();

		if (pAction)
		{
			if (pAction->initWithSize(gridSize, time))
			{
				pAction->autorelease();
			}
			else
			{
				CCX_SAFE_RELEASE_NULL(pAction);
			}
		}

		return pAction;
	}

	float CCFadeOutDownTiles::testFunc(cocos2d::ccGridSize pos, cocos2d::ccTime time)
	{
		CGPoint n = ccpMult(ccp((float)m_sGridSize.x, (float)m_sGridSize.y), (1.0f - time));
		if (pos.y == 0)
		{
			return 1.0f;
		}

		return powf(n.y / pos.y, 6);
	}

	// implementation of TurnOffTiles
	CCTurnOffTiles* CCTurnOffTiles::actionWithSize(ccGridSize size, ccTime d)
	{
		CCTurnOffTiles* pAction = new CCTurnOffTiles();
		if (pAction->initWithSize(size, d))
		{
			pAction->autorelease();
		}
		else
		{
			CCX_SAFE_RELEASE_NULL(pAction);
		}
		return pAction;
	}

	CCTurnOffTiles* CCTurnOffTiles::actionWithSeed(int s, cocos2d::ccGridSize gridSize, cocos2d::ccTime duration)
	{
		CCTurnOffTiles *pAction = new CCTurnOffTiles();

		if (pAction)
		{
			if (pAction->initWithSeed(s, gridSize, duration))
			{
				pAction->autorelease();
			}
			else
			{
				CCX_SAFE_RELEASE_NULL(pAction);
			}
		}

		return pAction;
	}

	bool CCTurnOffTiles::initWithSeed(int s, cocos2d::ccGridSize gridSize, cocos2d::ccTime duration)
	{
		if (CCTiledGrid3DAction::initWithSize(gridSize, duration))
		{
			m_nSeed = s;
			m_pTilesOrder = NULL;

			return true;
		}

		return false;
	}

	NSObject* CCTurnOffTiles::copyWithZone(cocos2d::NSZone *pZone)
	{
		NSZone* pNewZone = NULL;
		CCTurnOffTiles* pCopy = NULL;
		if(pZone && pZone->m_pCopyObject) 
		{
			pCopy = (CCTurnOffTiles*)(pZone->m_pCopyObject);
		}
		else
		{
			pCopy = new CCTurnOffTiles();
			pZone = pNewZone = new NSZone(pCopy);
		}

		CCTiledGrid3DAction::copyWithZone(pZone);

		pCopy->initWithSeed(m_nSeed, m_sGridSize, m_fDuration);

		CCX_SAFE_DELETE(pNewZone);
		return pCopy;
	}

	CCTurnOffTiles::~CCTurnOffTiles(void)
	{
		if (m_pTilesOrder)
		{
			delete[] m_pTilesOrder;
		}
	}

	void CCTurnOffTiles::shuffle(int *pArray, int nLen)
	{
		int i;
		for (i = nLen - 1; i >= 0; i--)
		{
			int j = rand() % (i+1);
			int v = pArray[i];
			pArray[i] = pArray[j];
			pArray[j] = v;
		}
	}

	void CCTurnOffTiles::turnOnTile(cocos2d::ccGridSize pos)
	{
		setTile(pos, originalTile(pos));
	}

	void CCTurnOffTiles::turnOffTile(cocos2d::ccGridSize pos)
	{
		ccQuad3 coords;

		memset(&coords, 0, sizeof(ccQuad3));
		setTile(pos, coords);
	}

	void CCTurnOffTiles::startWithTarget(CCNode *pTarget)
	{
		int i;

		CCTiledGrid3DAction::startWithTarget(pTarget);

		if (m_nSeed != -1)
		{
			srand(m_nSeed);
		}

		m_nTilesCount = m_sGridSize.x * m_sGridSize.y;
		m_pTilesOrder = new int[m_nTilesCount];

		for ( i = 0; i < m_nTilesCount; ++i)
		{
			m_pTilesOrder[i] = i;
		}

		shuffle(m_pTilesOrder, m_nTilesCount);
	}

	void CCTurnOffTiles::update(cocos2d::ccTime time)
	{
		int i, l, t;

		l = (int)(time * (float)m_nTilesCount);

		for( i = 0; i < m_nTilesCount; i++ )
		{
			t = m_pTilesOrder[i];
			ccGridSize tilePos = ccg( t / m_sGridSize.y, t % m_sGridSize.y );

			if ( i < l )
			{
				turnOffTile(tilePos);
			}
			else
			{
				turnOnTile(tilePos);
			}
		}
	}

	// implementation of CCWavesTiles3D

	CCWavesTiles3D* CCWavesTiles3D::actionWithWaves(int wav, float amp, cocos2d::ccGridSize gridSize, cocos2d::ccTime duration)
	{
		CCWavesTiles3D *pAction = new CCWavesTiles3D();

		if (pAction)
		{
			if (pAction->initWithWaves(wav, amp, gridSize, duration))
			{
				pAction->autorelease();
			}
			else
			{
				CCX_SAFE_RELEASE_NULL(pAction);
			}
		}

		return pAction;
	}

	bool CCWavesTiles3D::initWithWaves(int wav, float amp, cocos2d::ccGridSize gridSize, cocos2d::ccTime duration)
	{
		if (CCTiledGrid3DAction::initWithSize(gridSize, duration))
		{
			m_nWaves = wav;
			m_fAmplitude = amp;
			m_fAmplitudeRate = 1.0f;

			return true;
		}

		return false;
	}

	NSObject* CCWavesTiles3D::copyWithZone(cocos2d::NSZone *pZone)
	{
		NSZone* pNewZone = NULL;
		CCWavesTiles3D* pCopy = NULL;
		if(pZone && pZone->m_pCopyObject)
		{
			pCopy = (CCWavesTiles3D*)(pZone->m_pCopyObject);
		}
		else
		{
			pCopy = new CCWavesTiles3D();
			pZone = pNewZone = new NSZone(pCopy);
		}

		CCTiledGrid3DAction::copyWithZone(pZone);

		pCopy->initWithWaves(m_nWaves, m_fAmplitude, m_sGridSize, m_fDuration);

		CCX_SAFE_DELETE(pNewZone);
		return pCopy;
	}

	void CCWavesTiles3D::update(cocos2d::ccTime time)
	{
		int i, j;

		for( i = 0; i < m_sGridSize.x; i++ )
		{
			for( j = 0; j < m_sGridSize.y; j++ )
			{
				ccQuad3 coords = originalTile(ccg(i, j));

				coords.bl.z = (sinf(time * (CGFloat)M_PI  *m_nWaves * 2 + 
					(coords.bl.y+coords.bl.x) * .01f) * m_fAmplitude * m_fAmplitudeRate );
				coords.br.z	= coords.bl.z;
				coords.tl.z = coords.bl.z;
				coords.tr.z = coords.bl.z;

				setTile(ccg(i, j), coords);
			}
		}
	}

	// implementation of CCJumpTiles3D

	CCJumpTiles3D* CCJumpTiles3D::actionWithJumps(int j, float amp, cocos2d::ccGridSize gridSize, cocos2d::ccTime duration)
	{
		CCJumpTiles3D *pAction = new CCJumpTiles3D();

		if (pAction)
		{
			if (pAction->initWithJumps(j, amp, gridSize, duration))
			{
				pAction->autorelease();
			}
			else
			{
				CCX_SAFE_RELEASE_NULL(pAction);
			}
		}

		return pAction;
	}

	bool CCJumpTiles3D::initWithJumps(int j, float amp, cocos2d::ccGridSize gridSize, cocos2d::ccTime duration)
	{
		if (CCTiledGrid3DAction::initWithSize(gridSize, duration))
		{
			m_nJumps = j;
			m_fAmplitude = amp;
			m_fAmplitudeRate = 1.0f;

			return true;
		}

		return false;
	}

	NSObject* CCJumpTiles3D::copyWithZone(cocos2d::NSZone *pZone)
	{
		NSZone* pNewZone = NULL;
		CCJumpTiles3D* pCopy = NULL;
		if(pZone && pZone->m_pCopyObject) 
		{
			pCopy = (CCJumpTiles3D*)(pZone->m_pCopyObject);
		}
		else
		{
			pCopy = new CCJumpTiles3D();
			pZone = pNewZone = new NSZone(pCopy);
		}

		CCTiledGrid3DAction::copyWithZone(pZone);
		pCopy->initWithJumps(m_nJumps, m_fAmplitude, m_sGridSize, m_fDuration);

		CCX_SAFE_DELETE(pNewZone);
		return pCopy;
	}

	void CCJumpTiles3D::update(cocos2d::ccTime time)
	{
		int i, j;

		float sinz =  (sinf((CGFloat)M_PI * time * m_nJumps * 2) * m_fAmplitude * m_fAmplitudeRate );
		float sinz2 = (sinf((CGFloat)M_PI * (time * m_nJumps * 2 + 1)) * m_fAmplitude * m_fAmplitudeRate );

		for( i = 0; i < m_sGridSize.x; i++ )
		{
			for( j = 0; j < m_sGridSize.y; j++ )
			{
				ccQuad3 coords = originalTile(ccg(i, j));

				if ( ((i+j) % 2) == 0 )
				{
					coords.bl.z += sinz;
					coords.br.z += sinz;
					coords.tl.z += sinz;
					coords.tr.z += sinz;
				}
				else
				{
					coords.bl.z += sinz2;
					coords.br.z += sinz2;
					coords.tl.z += sinz2;
					coords.tr.z += sinz2;
				}

				setTile(ccg(i, j), coords);
			}
		}
	}

	// implementation of CCSplitRows

	CCSplitRows* CCSplitRows::actionWithRows(int nRows, cocos2d::ccTime duration)
	{
		CCSplitRows *pAction = new CCSplitRows();

		if (pAction)
		{
			if (pAction->initWithRows(nRows, duration))
			{
				pAction->autorelease();
			}
			else
			{
				CCX_SAFE_RELEASE_NULL(pAction);
			}
		}

		return pAction;
	}

	bool CCSplitRows::initWithRows(int nRows, cocos2d::ccTime duration)
	{
		m_nRows = nRows;

		return CCTiledGrid3DAction::initWithSize(ccg(1, nRows), duration);
	}

	NSObject* CCSplitRows::copyWithZone(cocos2d::NSZone *pZone)
	{
		NSZone* pNewZone = NULL;
		CCSplitRows* pCopy = NULL;
		if(pZone && pZone->m_pCopyObject) 
		{
			pCopy = (CCSplitRows*)(pZone->m_pCopyObject);
		}
		else
		{
			pCopy = new CCSplitRows();
			pZone = pNewZone = new NSZone(pCopy);
		}

		CCTiledGrid3DAction::copyWithZone(pZone);

		pCopy->initWithRows(m_nRows, m_fDuration);

		CCX_SAFE_DELETE(pNewZone);
		return pCopy;
	}

	void CCSplitRows::startWithTarget(CCNode *pTarget)
	{
		CCTiledGrid3DAction::startWithTarget(pTarget);
		m_winSize = CCDirector::sharedDirector()->getWinSize();
	}

	void CCSplitRows::update(cocos2d::ccTime time)
	{
		int j;

		for (j = 0; j < m_sGridSize.y; ++j)
		{
			ccQuad3 coords = originalTile(ccg(0, j));
			float	direction = 1;

			if ( (j % 2 ) == 0 )
			{
				direction = -1;
			}

			coords.bl.x += direction * m_winSize.width * time;
			coords.br.x += direction * m_winSize.width * time;
			coords.tl.x += direction * m_winSize.width * time;
			coords.tr.x += direction * m_winSize.width * time;

			setTile(ccg(0, j), coords);
		}
	}

	// implementation of CCSplitCols

	CCSplitCols* CCSplitCols::actionWithCols(int nCols, cocos2d::ccTime duration)
	{
		CCSplitCols *pAction = new CCSplitCols();

		if (pAction)
		{
			if (pAction->initWithCols(nCols, duration))
			{
				pAction->autorelease();
			}
			else
			{
				CCX_SAFE_RELEASE_NULL(pAction);
			}
		}

		return pAction;
	}

	bool CCSplitCols::initWithCols(int nCols, cocos2d::ccTime duration)
	{
		m_nCols = nCols;
		return CCTiledGrid3DAction::initWithSize(ccg(nCols, 1), duration);
	}

	NSObject* CCSplitCols::copyWithZone(cocos2d::NSZone *pZone)
	{
		NSZone* pNewZone = NULL;
		CCSplitCols* pCopy = NULL;
		if(pZone && pZone->m_pCopyObject) 
		{
			pCopy = (CCSplitCols*)(pZone->m_pCopyObject);
		}
		else
		{
			pCopy = new CCSplitCols();
			pZone = pNewZone = new NSZone(pCopy);
		}

		CCTiledGrid3DAction::copyWithZone(pZone);
		pCopy->initWithCols(m_nCols, m_fDuration);

		CCX_SAFE_DELETE(pNewZone);
		return pCopy;
	}

	void CCSplitCols::startWithTarget(CCNode *pTarget)
	{
		CCTiledGrid3DAction::startWithTarget(pTarget);
		m_winSize = CCDirector::sharedDirector()->getWinSize();
	}

	void CCSplitCols::update(cocos2d::ccTime time)
	{
		int i;

		for (i = 0; i < m_sGridSize.x; ++i)
		{
			ccQuad3 coords = originalTile(ccg(i, 0));
			float	direction = 1;

			if ( (i % 2 ) == 0 )
			{
				direction = -1;
			}

			coords.bl.y += direction * m_winSize.height * time;
			coords.br.y += direction * m_winSize.height * time;
			coords.tl.y += direction * m_winSize.height * time;
			coords.tr.y += direction * m_winSize.height * time;

			setTile(ccg(i, 0), coords);
		}
	}


} // end of namespace cocos2d
