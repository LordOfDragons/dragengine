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

#ifndef _PEEWPVIEW_H_
#define _PEEWPVIEW_H_

#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/properties/igdeWPCamera.h>
#include <deigde/gui/properties/igdeWPWObject.h>
#include <deigde/gui/properties/igdeWPSky.h>

class peeEmitter;
class peeWindowProperties;
class peeWPViewListener;



/**
 * \brief View Panel.
 */
class peeWPView : public igdeContainerScroll{
private:
	peeWindowProperties &pWindowProperties;
	peeEmitter::Ref pEmitter;
	peeWPViewListener::Ref pListener;
	
	igdeWPSky::Ref pWPSky;
	igdeWPWObject::Ref pWPEnvObject;
	igdeWPCamera::Ref pWPCamera;
	
	igdeEditVector::Ref pEditEmitterPosition;
	igdeEditVector::Ref pEditEmitterRotation;
	igdeTextField::Ref pEditEmitterBurstInterval;
	igdeTextField::Ref pEditEmitterWarmUpTime;
	igdeCheckBox::Ref pChkEmitterEnableCasting;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	peeWPView(peeWindowProperties &windowProperties);
	
protected:
	/** \brief Clean up the rig. */
	virtual ~peeWPView();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Emitter. */
	inline peeEmitter *GetEmitter() const{ return pEmitter; }
	
	/** \brief Set emitter. */
	void SetEmitter(peeEmitter::Ref emitter);
	
	/** \brief Update view. */
	void UpdateView();
	
	/** \brief Update sky. */
	void UpdateSky();
	
	/** \brief Update environment object. */
	void UpdateEnvObject();
	
	/** \brief Update camera. */
	void UpdateCamera();
	/*@}*/
};

#endif
