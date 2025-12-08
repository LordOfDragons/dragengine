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

#ifndef _SAEWPVIEW_H_
#define _SAEWPVIEW_H_

#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/properties/igdeWPSky.h>
#include <deigde/gui/properties/igdeWPCamera.h>

class saeWindowProperties;
class saeGameDefinition;
class saeWPViewListener;
class saeSAnimation;



/**
 * View property window.
 */
class saeWPView : public igdeContainerScroll{
private:
	saeWindowProperties &pWindowProperties;
	saeWPViewListener::Ref pListener;
	saeSAnimation::Ref pSAnimation;
	
	igdeEditPath::Ref pEditDisplayModelPath;
	igdeEditPath::Ref pEditDisplaySkinPath;
	igdeEditPath::Ref pEditDisplayRigPath;
	
	igdeWPSky::Ref pWPSky;
	igdeWPCamera::Ref pWPCamera;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	saeWPView(saeWindowProperties &windowProperties);
	
protected:
	/** Clean up panel. */
	virtual ~saeWPView();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Properties window. */
	inline saeWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** Speech animation. */
	inline const saeSAnimation::Ref &GetSAnimation() const{ return pSAnimation; }
	
	/** Set speech animation. */
	void SetSAnimation(saeSAnimation *sanimation);
	
	/** Update view. */
	void UpdateView();
	
	/** Update sky. */
	void UpdateSky();
	
	/** Update camera. */
	void UpdateCamera();
	/*@}*/
};

#endif
