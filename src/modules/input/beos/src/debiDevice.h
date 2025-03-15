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

#ifndef _DEBIDEVICE_H_
#define _DEBIDEVICE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/resources/image/deImage.h>

class deBeOSInput;
class debiDeviceAxis;
class debiDeviceButton;
class deInputDevice;



/**
 * \brief Input device.
 */
class debiDevice : public deObject{
public:
	/** \brief Source identifier. */
	enum eSources{
		/** \brief BeOS. */
		esBeOS,
		
		/** \brief BJoystick API. */
		esBJoystick
	};
	
	
	
private:
	deBeOSInput &pModule;
	
	int pIndex;
	eSources pSource;
	
	deInputDevice::eDeviceTypes pType;
	decString pID;
	decString pName;
	deImage::Ref pDisplayImage;
	decObjectOrderedSet pDisplayIcons;
	decString pDisplayText;
	
	decObjectOrderedSet pButtons;
	decObjectOrderedSet pAxes;
	
	bool pDirtyAxesValues;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device. */
	debiDevice( deBeOSInput &module );
	
	/** \brief Creat mouse device. */
	static debiDevice *CreateMouse( deBeOSInput &module );
	
	/** \brief Creat keyboard device. */
	static debiDevice *CreateKeyboard( deBeOSInput &module );
	
	
	
protected:
	/** \brief Clean up device. */
	virtual ~debiDevice();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** \brief BeOS input module. */
	inline deBeOSInput &GetModule() const{ return pModule; }
	
	/** \brief Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** \brief Set index. */
	void SetIndex( int index );
	
	/** \brief Source. */
	inline eSources GetSource() const{ return pSource; }
	
	
	
	/** \brief Device type. */
	inline deInputDevice::eDeviceTypes GetType() const{ return pType; }
	
	/** \brief Set device type. */
	void SetType( deInputDevice::eDeviceTypes type );
	
	/** \brief Identifier. */
	inline const decString &GetID() const{ return pID; }
	
	/** \brief Set identifier. */
	void SetID( const char *id );
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Display image. */
	inline const deImage::Ref &GetDisplayImage() const{ return pDisplayImage; }
	
	/** \brief Display icons (deImage*). */
	inline const decObjectOrderedSet &GetDisplayIcons() const{ return pDisplayIcons; }
	
	/** \brief Set display image and icons. */
	void SetDisplayImages( const char *name );
	
	/** \brief Display text. */
	inline const decString &GetDisplayText() const{ return pDisplayText; }
	
	/** \brief Set display text. */
	void SetDisplayText( const char *text );
	
	
	
	/** \brief Number of buttons. */
	int GetButtonCount() const;
	
	/** \brief Button at index. */
	debiDeviceButton *GetButtonAt( int index ) const;
	
	/** \brief Button with identifier or \em NULL if absent. */
	debiDeviceButton *GetButtonWithID( const char *id ) const;
	
	/** \brief Index of button with identifier or -1 if absent. */
	int IndexOfButtonWithID( const char *id ) const;
	
	/** \brief Index of button with beos input code or -1 if absent. */
	int IndexOfButtonWithBICode( int code ) const;
	
	/** \brief Add button. */
	void AddButton( debiDeviceButton *button );
	
	
	
	/** \brief Number of axiss. */
	int GetAxisCount() const;
	
	/** \brief Axis at index. */
	debiDeviceAxis *GetAxisAt( int index ) const;
	
	/** \brief Axis with identifier or \em NULL if absent. */
	debiDeviceAxis *GetAxisWithID( const char *id ) const;
	
	/** \brief Index of axis with identifier or -1 if absent. */
	int IndexOfAxisWithID( const char *id ) const;
	
	/** \brief Index of axis with beos input code or -1 if absent. */
	int IndexOfAxisWithBICode( int code ) const;
	
	/** \brief Add axis. */
	void AddAxis( debiDeviceAxis *axis );
	
	
	
	/** \brief One or more axes values are dirty. */
	inline bool GetDirtyAxesValues() const{ return pDirtyAxesValues; }
	
	/** \brief Set if one or more axes values are dirty. */
	void SetDirtyAxesValues( bool dirty );
	
	
	
	/** \brief Update engine input device information. */
	void GetInfo( deInputDevice &info ) const;
	
	/** \brief Update device state. */
	virtual void Update();
	
	/** \brief Send axis events if dirty. */
	void SendDirtyAxisEvents();
	/*@}*/
};

#endif
