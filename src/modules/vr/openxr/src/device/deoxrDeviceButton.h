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

#ifndef _DEOXRDEVICEBUTTON_H_
#define _DEOXRDEVICEBUTTON_H_

#include "deoxrDeviceComponent.h"
#include "deoxrDeviceAxis.h"
#include "../action/deoxrAction.h"

#include <stdint.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputDeviceButton.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/resources/image/deImage.h>

class deInputDeviceButton;
class deoxrDevice;


/**
 * VR input device button.
 */
class deoxrDeviceButton : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoxrDeviceButton> Ref;
	
	
	
private:
	deoxrDevice &pDevice;
	
	int pIndex;
	
	deoxrAction::Ref pActionPress;
	deoxrAction::Ref pActionTouch;
	deoxrDeviceAxis::Ref pFakeFromAxis;
	
	decString pID;
	decString pName;
	deInputDeviceButton::eButtonTypes pType;
	deoxrDeviceComponent::Ref pInputDeviceComponent;
	bool pPressed;
	bool pTouched;
	
	deImage::Ref pDisplayImage;
	decObjectOrderedSet pDisplayIcons;
	decString pDisplayText;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device button. */
	deoxrDeviceButton( deoxrDevice &device );
	
protected:
	/** Clean up device button. */
	virtual ~deoxrDeviceButton();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** Device. */
	inline deoxrDevice &GetDevice() const{ return pDevice; }
	
	/** Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** Set index. */
	void SetIndex( int index );
	
	
	
	/** Press button action. */
	inline deoxrAction *GetActionPress() const{ return pActionPress; }
	
	/** Set press button action. */
	void SetActionPress( deoxrAction *action );
	
	/** Touch button action. */
	inline deoxrAction *GetActionTouch() const{ return pActionTouch; }
	
	/** Set touch button action. */
	void SetActionTouch( deoxrAction *action );
	
	/** Button state is faked from axis state. */
	inline const deoxrDeviceAxis::Ref &GetFakeFromAxis() const{ return pFakeFromAxis; }

	/** Set button state is faked from axis state. */
	void SetFakeFromAxis( const deoxrDeviceAxis::Ref &axis );

	
	
	/** Identifier. */
	inline const decString &GetID() const{ return pID; }
	
	/** Set identifier. */
	void SetID( const char *id );
	
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Set name. */
	void SetName( const char *name );
	
	/** Type. */
	inline deInputDeviceButton::eButtonTypes GetType() const{ return pType; }
	
	/** Set type. */
	void SetType( deInputDeviceButton::eButtonTypes type );
	
	/** Input device component. */
	inline deoxrDeviceComponent *GetInputDeviceComponent() const{ return pInputDeviceComponent; }
	
	/** Set input device component. */
	void SetInputDeviceComponent( deoxrDeviceComponent *component );
	
	/** Button is pressed. */
	inline bool GetPressed() const{ return pPressed; }
	
	/** Set if button is pressed. */
	void SetPressed( bool pressed );
	
	/** Update pressed sending event if changed. */
	void UpdatePressed( bool pressed );
	
	/** Button is touched. */
	inline bool GetTouched() const{ return pTouched; }
	
	/** Set if button is touched. */
	void SetTouched( bool touched );
	
	/** Update touched sending event if changed. */
	void UpdateTouched( bool touched );
	
	
	
	/** Display image. */
	inline deImage *GetDisplayImage() const{ return pDisplayImage; }
	
	/** Display icons (deImage*). */
	inline const decObjectOrderedSet &GetDisplayIcons() const{ return pDisplayIcons; }
	
	/** Set display image and icons. */
	void SetDisplayImages( const char *name );
	
	/** Set display image and icons. */
	void SetDisplayImages( const deoxrDeviceButton &button );
	
	/** Display text. */
	inline const decString &GetDisplayText() const{ return pDisplayText; }
	
	/** Set display text. */
	void SetDisplayText( const char *text );
	
	
	
	/** Update engine input device information button. */
	void GetInfo( deInputDeviceButton &info ) const;
	
	/** Track state. */
	void TrackState();
	/*@}*/
};

#endif
