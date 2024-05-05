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

#ifndef _DEOGLSKY_H_
#define _DEOGLSKY_H_

#include <dragengine/systems/modules/graphic/deBaseGraphicSky.h>

class deoglRSky;
class deoglSkyLayer;
class deGraphicOpenGl;

class deSky;



/**
 * Sky peer.
 */
class deoglSky : public deBaseGraphicSky{
private:
	deGraphicOpenGl &pOgl;
	const deSky &pSky;
	
	deoglRSky *pRSky;
	
	bool pDirtyParameters;
	unsigned int pUpdateTracker;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create sky. */
	deoglSky( deGraphicOpenGl &ogl, const deSky &sky );
	
	/** Clean up sky. */
	virtual ~deoglSky();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Opengl object. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** Sky. */
	inline const deSky &GetSky() const{ return pSky; }
	
	/** Render sky. */
	inline deoglRSky *GetRSky() const{ return pRSky; }
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	
	/** Update tracker state. */
	inline unsigned int GetUpdateTracker() const{ return pUpdateTracker; }
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Parameters changed. */
	virtual void ParametersChanged();
	/*@}*/
};

#endif
