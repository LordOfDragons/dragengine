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

#ifndef _DEXSIDEVICEBUTTON_H_
#define _DEXSIDEVICEBUTTON_H_

#include "dexsiXInclude.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputDeviceButton.h>
#include <dragengine/resources/image/deImage.h>

class deXSystemInput;



/**
 * X-System input device button.
 */
class dexsiDeviceButton : public deObject{
public:
	typedef deTObjectReference<dexsiDeviceButton> Ref;
	
	
private:
	deXSystemInput &pModule;
	
	decString pID;
	decString pName;
	bool pPressed;
	deInputDeviceButton::eButtonTypes pType;
	
	deImage::Ref pDisplayImage;
	decObjectOrderedSet pDisplayIcons;
	decString pDisplayText;
	
	int pX11Code;
	int pEvdevCode;
	deInputEvent::eKeyCodes pKeyCode;
	int pMatchPriority;
	deInputEvent::eKeyLocation pKeyLocation;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device button. */
	dexsiDeviceButton( deXSystemInput &module );
	
protected:
	/** Clean up device button. */
	virtual ~dexsiDeviceButton();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** X-System input module. */
	inline deXSystemInput &GetModule() const{ return pModule; }
	
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
	
	/** Type. */
	inline deInputDeviceButton::eButtonTypes GetType() const{ return pType; }
	
	/** Set type. */
	void SetType( deInputDeviceButton::eButtonTypes type );
	
	
	
	/** Display image. */
	inline deImage *GetDisplayImage() const{ return pDisplayImage; }
	
	/** Display icons (deImage*). */
	inline const decObjectOrderedSet &GetDisplayIcons() const{ return pDisplayIcons; }
	
	/** Set display image and icons. */
	void SetDisplayImages( const char *name );
	
	/** Set display image and icons. */
	void SetDisplayImages( const dexsiDeviceButton &button );
	
	/** Display text. */
	inline const decString &GetDisplayText() const{ return pDisplayText; }
	
	/** Set display text. */
	void SetDisplayText( const char *text );
	
	
	
	/** X11 specific code. */
	inline int GetX11Code() const{ return pX11Code; }
	
	/** Set X11 specific code. */
	void SetX11Code( int code );
	
	/** Libevdev specific event code. */
	inline int GetEvdevCode() const{ return pEvdevCode; }
	
	/** Set libevdev specific event code. */
	void SetEvdevCode( int code );
	
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
	
	
	
	/** Update engine input device information button. */
	void GetInfo( deInputDeviceButton &info ) const;
	/*@}*/
};

#endif
