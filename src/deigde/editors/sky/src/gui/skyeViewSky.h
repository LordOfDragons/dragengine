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

#ifndef _SKYEVIEWSKY_H_
#define _SKYEVIEWSKY_H_

#include "../sky/skyeSky.h"

#include <deigde/gui/igdeViewRenderWindow.h>
#include <deigde/gui/event/igdeMouseCameraListener.h>

class skyeWindowMain;



/**
 * \brief View of the sky.
 */
class skyeViewSky : public igdeViewRenderWindow{
public:
	typedef deTObjectReference<skyeViewSky> Ref;
	
private:
	skyeWindowMain &pWindowMain;
	
	skyeSky::Ref pSky;
	
	igdeMouseCameraListener::Ref pCameraInteraction;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new sky view. */
	skyeViewSky(skyeWindowMain &windowMain);
	
protected:
	/** \brief Clean up the sky view. */
	virtual ~skyeViewSky();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline skyeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brie Reset view. */
	void ResetView();
	
	/** \brief Sky. */
	inline const skyeSky::Ref &GetSky() const{ return pSky; }
	
	/** \brief Set sky. */
	void SetSky(skyeSky *sky);
	
	/** \brief Game like frame update. */
	virtual void OnFrameUpdate(float elapsed);
	
	/** \brief Create canvas. */
	virtual void CreateCanvas();
	/*@}*/
};

#endif
