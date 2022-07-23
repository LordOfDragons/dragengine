/* 
 * Drag[en]gine OpenXR VR Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOXRDEVICEBUTTON_H_
#define _DEOXRDEVICEBUTTON_H_

#include "deoxrDeviceComponent.h"
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
