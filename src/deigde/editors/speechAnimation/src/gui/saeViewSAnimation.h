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

#ifndef _SAEVIEWSANIMATION_H_
#define _SAEVIEWSANIMATION_H_

#include <deigde/gui/igdeViewRenderWindow.h>
#include <deigde/gui/event/igdeMouseCameraListener.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/font/deFont.h>
#include "saeSAnimation.h"

class saeWindowMain;



/**
 * Speech Animation View.
 */
class saeViewSAnimation : public igdeViewRenderWindow{
private:
	saeWindowMain &pWindowMain;
	deFont::Ref pFontStats;
	
	saeSAnimation::Ref pSAnimation;
	
	igdeMouseCameraListener::Ref pCameraInteraction;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create speech animation view. */
	saeViewSAnimation(saeWindowMain &windowMain);
	
protected:
	/** Clean up speech animation view. */
	virtual ~saeViewSAnimation();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Reset view. */
	void ResetView();
	
	/** Speech animation. */
	inline const saeSAnimation::Ref &GetSAnimation() const{ return pSAnimation; }
	
	/** Set speech animation. */
	void SetSAnimation(saeSAnimation *sanimation);
	
	/** Game like frame update. */
	virtual void OnFrameUpdate(float elapsed);
	
	/** Create canvas. */
	virtual void CreateCanvas();
	/*@}*/
};

#endif
