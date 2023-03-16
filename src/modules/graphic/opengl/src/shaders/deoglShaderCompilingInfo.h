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

#include <dragengine/threading/deMutex.h>

class deoglRenderThread;


/**
 * Tracks if shaders are compiling to display information to user.
 */
class deoglShaderCompilingInfo{
private:
	enum eState{
		esInvisible,
		esDelayFadeIn,
		esFadeIn,
		esVisible,
		esDelayFadeOut,
		esFadeOut
	};
	
	
	
	deoglRenderThread &pRenderThread;
	
	eState pState;
	float pDelayFadeIn;
	float pTimeFadeIn;
	float pDelayFadeOut;
	float pTimeFadeOut;
	float pElapsed;
	float pTransparency;
	
	bool pHasLoadingShader;
	bool pHasCompilingShader;
	
	deMutex pMutex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shader compiling info. */
	deoglShaderCompilingInfo( deoglRenderThread &renderThread );
	
	/** Clean up shader compiling info. */
	~deoglShaderCompilingInfo();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** State. */
	inline eState GetState() const{ return pState; }
	
	/** Visible. */
	inline bool IsVisible() const{ return pState != esInvisible; }
	
	/** Transparency. */
	inline float GetTransparency() const{ return pTransparency; }
	
	/** Shaders are loading. */
	inline bool GetHasLoadingShader() const{ return pHasLoadingShader; }
	
	/** Shaders are compiling. */
	inline bool GetHasCompilingShader() const{ return pHasCompilingShader; }
	
	/** Update. */
	void Update( float elapsed );
	/*@}*/
	
	
	
private:
	void pUpdateChecks();
	bool pShouldBeVisible() const;
	void pUpdateState( float elapsed );
	void pUpdateTransparency();
};

#endif
