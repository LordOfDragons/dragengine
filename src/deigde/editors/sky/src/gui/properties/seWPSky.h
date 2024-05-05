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

#ifndef _SEWPSKY_H_
#define _SEWPSKY_H_

#include <deigde/gui/igdeColorBoxReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class seSky;
class seWindowProperties;
class seWPSkyListener;


/**
 * \brief Sky panel.
 */
class seWPSky : public igdeContainerScroll{
private:
	seWindowProperties &pWindowProperties;
	seSky *pSky;
	seWPSkyListener *pListener;
	
	igdeColorBoxReference pClrBg;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	seWPSky( seWindowProperties &windowProperties );
	
	/** \brief Clean up panel. */
	virtual ~seWPSky();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Sky. */
	inline seSky *GetSky() const{ return pSky; }
	
	/** \brief Set sky. */
	void SetSky( seSky *sky );
	
	/** \brief Update sky. */
	void UpdateSky();
	/*@}*/
};

#endif
