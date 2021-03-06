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

#ifndef __CCCONFIGURATION_H__
#define __CCCONFIGURATION_H__

#include "NSObject.h"
#include <string>
#include <GLES/gl.h>

namespace   cocos2d {

/**
 @brief CCConfiguration contains some openGL variables
 @since v0.99.0
 */
class CCConfiguration : public NSObject
{
protected:
	GLint	m_nMaxTextureSize;
	GLint	m_nMaxModelviewStackDepth;
	bool	m_bSupportsPVRTC;
	bool	m_bSupportsNPOT;
	bool	m_bSupportsBGRA8888;
	bool	m_bSupportsDiscardFramebuffer;

public:
	CCConfiguration(void);
	
	/** OpenGL Max texture size. */
	inline int getMaxTextureSize(void)
	{
		return m_nMaxTextureSize;
	}

	/** OpenGL Max Modelview Stack Depth */
	inline int getMaxModelviewStackDepth(void)
	{
		return m_nMaxModelviewStackDepth;
	}

	/** Whether or not the GPU supports NPOT (Non Power Of Two) textures.
	 NPOT textures have the following limitations:
	 - They can't have mipmaps
	 - They only accept GL_CLAMP_TO_EDGE in GL_TEXTURE_WRAP_{S,T}
	 
	 @since v0.99.2
	 */
	inline bool isSupportsNPOT(void)
	{
		return m_bSupportsNPOT;
	}

	/** Whether or not PVR Texture Compressed is supported */
	inline bool isSupportsPVRTC(void)
	{
		return m_bSupportsPVRTC;
	}

	/** Whether or not BGRA8888 textures are supported.
	 @since v0.99.2
	 */
	inline bool isSupportsBGRA8888(void)
	{
		return m_bSupportsBGRA8888;
	}

	/** Whether or not glDiscardFramebufferEXT is supported
	 @since v0.99.2
	 */
	inline bool isSupportsDiscardFramebuffer(void)
	{
		return m_bSupportsDiscardFramebuffer;
	}

	/** returns whether or not an OpenGL is supported */
	bool checkForGLExtension(const std::string &searchName);

	bool init(void);

public:
	/** returns a shared instance of the CCConfiguration */
	static CCConfiguration* sharedConfiguration(void);
};
}//namespace   cocos2d 

#endif // __CCCONFIGURATION_H__
