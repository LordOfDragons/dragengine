/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	deoglShaderCompilingInfo( deGraphicOpenGl &ogl );
	
protected:
	/** Clean up shader compiling info. */
	virtual ~deoglShaderCompilingInfo();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** State. */
	inline eState GetState() const{ return pState; }
	
	/** Visible. */
	bool IsVisible() const;
	
	/** Transparency. */
	inline float GetTransparency() const{ return pTransparency; }
	
	/** Shaders are loading. */
	inline bool GetHasLoadingShader() const{ return pHasLoadingShader; }
	
	/** Shaders are compiling. */
	inline bool GetHasCompilingShader() const{ return pHasCompilingShader; }
	
	/** Update. Called from main thread. */
	void Update( float elapsed );
	
	/** Prepare for render. Called from render thread. */
	void PrepareForRender( float elapsed );
	/*@}*/
	
	
	
private:
	void pCreateCanvas();
	void pUpdateChecks();
	bool pShouldBeVisible() const;
	void pUpdateState( float elapsed );
	void pUpdateTransparency();
	void pUpdateCanvas( float elapsed );
	void pUpdateText();
};

#endif
