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

#ifndef _GDEWPVIEW_H_
#define _GDEWPVIEW_H_

#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/properties/igdeWPCamera.h>
#include <deigde/gui/properties/igdeWPSky.h>

class gdeWindowProperties;
class gdeWPViewListener;
class gdeGameDefinition;



/**
 * \brief View property window.
 */
class gdeWPView : public igdeContainerScroll{
private:
	gdeWindowProperties &pWindowProperties;
	gdeGameDefinition *pGameDefinition;
	gdeWPViewListener *pListener;
	
	igdeWPSky::Ref pWPSky;
	igdeWPCamera::Ref pWPCamera;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create property panel. */
	gdeWPView(gdeWindowProperties &windowProperties);
	
protected:
	/** \brief Clean up property panel. */
	virtual ~gdeWPView();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Game definition. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set game definition or \em NULL if not present. */
	void SetGameDefinition(gdeGameDefinition *gameDefinition);
	
	/** \brief Update view. */
	void UpdateView();
	
	/** \brief Update sky. */
	void UpdateSky();
	
	/** \brief Update camera. */
	void UpdateCamera();
	/*@}*/
};

#endif
