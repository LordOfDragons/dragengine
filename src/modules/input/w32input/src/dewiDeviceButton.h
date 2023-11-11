/* 
 * Drag[en]gine Windows Input Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEWIDEVICEBUTTON_H_
#define _DEWIDEVICEBUTTON_H_

#include "dewiInclude.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputDeviceButton.h>
#include <dragengine/resources/image/deImageReference.h>

class deWindowsInput;
class dewiDeviceWinRTController;


/**
 * Windows input device button.
 */
class dewiDeviceButton : public deObject{
public:
	typedef deTObjectReference<dewiDeviceButton> Ref;
	
	
	
private:
	deWindowsInput &pModule;
	
	deInputDeviceButton::eButtonTypes pType;
	decString pID;
	decString pName;
	bool pPressed;
	
	deImageReference pDisplayImage;
	decObjectOrderedSet pDisplayIcons;
	decString pDisplayText;
	
	int pWICode;
	int pWIChar;
	deInputEvent::eKeyCodes pKeyCode;
	int pMatchPriority;
	deInputEvent::eKeyLocation pKeyLocation;
	int pWinRTReadingIndex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device button. */
	dewiDeviceButton( deWindowsInput &module );
	
protected:
	/** Clean up device button. */
	virtual ~dewiDeviceButton();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** Input module. */
	inline deWindowsInput &GetModule() const{ return pModule; }
	
	/** Identifier. */
	inline const decString &GetID() const{ return pID; }
	
	/** Set identifier. */
	void SetID( const char *id );
	
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Set name. */
	void SetName( const char *name );
	
	/** Button is pressed. */
	inline bool GetPressed() const{ return pPressed; }
	
	/** Set if button is presssed. */
	void SetPressed( bool pressed );
	
	/** Button type. */
	inline deInputDeviceButton::eButtonTypes GetType() const{ return pType; }
	
	/** Set button type. */
	void SetType( deInputDeviceButton::eButtonTypes type );
	
	
	
	/** Display image. */
	inline deImage *GetDisplayImage() const{ return pDisplayImage; }
	
	/** Display icons (deImage*). */
	inline const decObjectOrderedSet &GetDisplayIcons() const{ return pDisplayIcons; }
	
	/** Set display image and icons. */
	void SetDisplayImages( const char *name );
	
	/** Set display image and icons. */
	void SetDisplayImages( const dewiDeviceButton &button );
	
	/** Display text. */
	inline const decString &GetDisplayText() const{ return pDisplayText; }
	
	/** Set display text. */
	void SetDisplayText( const char *text );
	
	
	
	/** WI specific code. */
	inline int GetWICode() const{ return pWICode; }
	
	/** Set WI specific code. */
	void SetWICode( int code );
	
	/** WI specific character. */
	inline int GetWIChar() const{ return pWIChar; }
	
	/** Set WI specific character. */
	void SetWIChar( int character );
	
	/** Input event key code. */
	inline deInputEvent::eKeyCodes GetKeyCode() const{ return pKeyCode; }
	
	/** Set input event key code. */
	void SetKeyCode( deInputEvent::eKeyCodes keyCode );
	
	/** Match priority. */
	inline int GetMatchPriority() const{ return pMatchPriority; }
	
	/** Set match priority. */
	void SetMatchPriority( int priority );
	
	/** Key location. */
	inline deInputEvent::eKeyLocation GetKeyLocation() const{ return pKeyLocation; }
	
	/** Set key location. */
	void SetKeyLocation( deInputEvent::eKeyLocation location );

	/** WinRT reading index. */
	inline int GetWinRTReadingIndex() const{ return pWinRTReadingIndex; }

	/** Set WinRT reading index. */
	void SetWinRTReadingIndex( int index );
	
	
	
	/** Update engine input device information button. */
	void GetInfo( deInputDeviceButton &info ) const;

	/** Process WinRT reading. */
	void WinRTReading( dewiDeviceWinRTController &device );
	/*@}*/
};

#endif
