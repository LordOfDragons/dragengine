/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DEOGLSHADERCOMPILINGINFO_H_
#define _DEOGLSHADERCOMPILINGINFO_H_

#include <dragengine/deObject.h>
#include <dragengine/threading/deMutex.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasText.h>
#include <dragengine/resources/canvas/deCanvasVideoPlayer.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/video/deVideo.h>
#include <dragengine/resources/video/deVideoPlayer.h>

class deGraphicOpenGl;


/**
 * Tracks if shaders are compiling to display information to user.
 */
class deoglShaderCompilingInfo : public deObject{
public:
	typedef deTObjectReference<deoglShaderCompilingInfo> Ref;
	
	
private:
	enum eState{
		esInvisible,
		esDelayFadeIn,
		esFadeIn,
		esVisible,
		esDelayFadeOut,
		esFadeOut
	};
	
	enum eMode{
		emLoading,
		emCompiling
	};
	
	
	
	deGraphicOpenGl &pOgl;
	
	eState pState;
	eMode pMode;
	float pDelayFadeIn;
	float pTimeFadeIn;
	float pDelayFadeOut;
	float pTimeFadeOut;
	float pElapsed;
	int pFrames;
	float pTransparency;
	float pMaxTransparency;
	
	bool pHasLoadingShader;
	bool pHasCompilingShader;
	
	deCanvasView::Ref pCanvasView;
	deCanvasVideoPlayer::Ref pCanvasVideo;
	deCanvasText::Ref pCanvasText1;
	deCanvasText::Ref pCanvasText2;
	
	deFont::Ref pFontText;
	
	deVideo::Ref pVideoCompile;
	deVideoPlayer::Ref pVideoPlayerCompile;
	float pVideoCompilePlayPosition;
	
	deMutex pMutex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shader compiling info. */
	deoglShaderCompilingInfo(deGraphicOpenGl &ogl);
	
protected:
	/** Clean up shader compiling info. */
	virtual ~deoglShaderCompilingInfo();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** State. */
	inline eState GetState() const{return pState;}
	
	/** Visible. */
	bool IsVisible() const;
	
	/** Transparency. */
	inline float GetTransparency() const{return pTransparency;}
	
	/** Shaders are loading. */
	inline bool GetHasLoadingShader() const{return pHasLoadingShader;}
	
	/** Shaders are compiling. */
	inline bool GetHasCompilingShader() const{return pHasCompilingShader;}
	
	/** Update. Called from main thread. */
	void Update(float elapsed);
	
	/** Prepare for render. Called from render thread. */
	void PrepareForRender(float elapsed);
	/*@}*/
	
	
	
private:
	void pCreateCanvas();
	void pUpdateChecks();
	bool pShouldBeVisible() const;
	void pUpdateState(float elapsed);
	void pUpdateTransparency();
	void pUpdateCanvas(float elapsed);
	void pUpdateText();
};

#endif
