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
 
 
 
