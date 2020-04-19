/* 
 * Drag[en]gine MacOS Input Module
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

#ifndef _DEMOIDEVICEBUTTON_H_
#define _DEMOIDEVICEBUTTON_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputEvent.h>

class deInputDeviceButton;



/**
 * \brief MacOS input device button.
 */
class demoiDeviceButton{
private:
	decString pID;
	decString pName;
	bool pPressed;
	
    int pMOCode;
    int pMOChar;
    deInputEvent::eKeyCodes pKeyCode;
    int pMatchPriority;
    
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device button. */
	demoiDeviceButton();
	
	/** \brief Clean up device button. */
	~demoiDeviceButton();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
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
	
    
    
    /** \brief MacOS specific code. */
    inline int GetMOCode() const{ return pMOCode; }
    
    /** \brief Set MacOS specific code. */
    void SetMOCode( int code );
    
    /** \brief MacOS specific character for matching. */
    inline int GetMOChar() const{ return pMOChar; }
    
    /** \brief Set MacOS specific character for matching. */
    void SetMOChar( int character );

    /** \brief Input event code. */
    inline deInputEvent::eKeyCodes GetKeyCode() const{ return pKeyCode; }
    
    /** \brief Set input event code. */
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
 
 
 
