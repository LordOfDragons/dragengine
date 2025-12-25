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

#ifndef _SKYEWPSKY_H_
#define _SKYEWPSKY_H_

#include "skyeWPSkyListener.h"
#include "../../sky/skyeSky.h"

#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class skyeWindowProperties;


/**
 * \brief Sky panel.
 */
class skyeWPSky : public igdeContainerScroll{
public:
	typedef deTObjectReference<skyeWPSky> Ref;
	
private:
	skyeWindowProperties &pWindowProperties;
	skyeSky::Ref pSky;
	skyeWPSkyListener::Ref pListener;
	
	igdeColorBox::Ref pClrBg;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	skyeWPSky(skyeWindowProperties &windowProperties);
	
	/** \brief Clean up panel. */
	virtual ~skyeWPSky();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Sky. */
	inline const skyeSky::Ref &GetSky() const{ return pSky; }
	
	/** \brief Set sky. */
	void SetSky(skyeSky *sky);
	
	/** \brief Update sky. */
	void UpdateSky();
	/*@}*/
};

#endif
