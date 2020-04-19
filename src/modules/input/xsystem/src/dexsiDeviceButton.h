/* 
 * Drag[en]gine X System Input Module
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

#ifndef _DEXSIDEVICEBUTTON_H_
#define _DEXSIDEVICEBUTTON_H_

#include "dexsiXInclude.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/resources/image/deImageReference.h>

class deInputDeviceButton;
class deXSystemInput;



/**
 * \brief X-System input device button.
 */
class dexsiDeviceButton : public deObject{
private:
	deXSystemInput &pModule;
	
	decString pID;
	decString pName;
	bool pPressed;
	
	deImageReference pDisplayImage;
	decObjectOrderedSet pDisplayIcons;
	decString pDisplayText;
	
	int pX11Code;
	int pEvdevCode;
	deInputEvent::eKeyCodes pKeyCode;
	int pMatchPriority;
	
	bool pDirtyValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device button. */
	dexsiDeviceButton( deXSystemInput &module );
	
protected:
	/** \brief Clean up device button. */
	virtual ~dexsiDeviceButton();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** \brief X-System input module. */
	inline deXSystemInput &GetModule() const{ return pModule; }
	
	/** \brief Identifier. */
	inline const decString &GetID() const{ return pID; }
	
	/** \brief Set identifier. */
	void SetID( const char *id );
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Button is pressed. */
	inline bool GetPressed() const{ return pPressed; }
	
	/** \brief Set if button is presssed. */
	void SetPressed( bool pressed );
	
	
	
	/** \brief Display image. */
	inline deImage *GetDisplayImage() const{ return pDisplayImage; }
	
	/** \brief Display icons (deImage*). */
	inline const decObjectOrderedSet &GetDisplayIcons() const{ return pDisplayIcons; }
	
	/** \brief Set display image and icons. */
	void SetDisplayImages( const char *name );
	
	/** \brief Set display image and icons. */
	void SetDisplayImages( const dexsiDeviceButton &button );
	
	/** \brief Display text. */
	inline const decString &GetDisplayText() const{ return pDisplayText; }
	
	/** \brief Set display text. */
	void SetDisplayText( const char *text );
	
	
	
	/** \brief X11 specific code. */
	inline int GetX11Code() const{ return pX11Code; }
	
	/** \brief Set X11 specific code. */
	void SetX11Code( int code );
	
	/** \brief Libevdev specific event code. */
	inline int GetEvdevCode() const{ return pEvdevCode; }
	
	/** \brief Set libevdev specific event code. */
	void SetEvdevCode( int code );
	
	/** \brief Input event key code. */
	inline deInputEvent::eKeyCodes GetKeyCode() const{ return pKeyCode; }
	
	/** \brief Set input event key code. */
	void SetKeyCode( deInputEvent::eKeyCodes keyCode );
	
	/** \brief Match priority. */
	inline int GetMatchPriority() const{ return pMatchPriority; }
	
	/** \brief Set match priority. */
	void SetMatchPriority( int priority );
	
	
	
	/** \brief Update engine input device information button. */
	void GetInfo( deInputDeviceButton &info ) const;
	/*@}*/
};

#endif
