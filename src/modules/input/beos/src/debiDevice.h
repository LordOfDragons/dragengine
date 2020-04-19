/* 
 * Drag[en]gine BeOS Input Module
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

#ifndef _DEBIDEVICE_H_
#define _DEBIDEVICE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputDevice.h>

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
	
	int pButtonCount;
	debiDeviceButton *pButtons;
	
	int pAxisCount;
	debiDeviceAxis *pAxes;
	
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
	
	
	
	/** \brief Number of buttons. */
	inline int GetButtonCount() const{ return pButtonCount; }
	
	/** \brief Set number of buttons. */
	void SetButtonCount( int count );
	
	/** \brief Button at index. */
	debiDeviceButton &GetButtonAt( int index ) const;
	
	/** \brief Button with identifier or \em NULL if absent. */
	debiDeviceButton *GetButtonWithID( const char *id ) const;
	
	/** \brief Index of button with identifier or -1 if absent. */
	int IndexOfButtonWithID( const char *id ) const;
	
	/** \brief Index of button with beos input code or -1 if absent. */
	int IndexOfButtonWithBICode( int code ) const;
	
	
	
	/** \brief Number of axiss. */
	inline int GetAxisCount() const{ return pAxisCount; }
	
	/** \brief Set number of axiss. */
	void SetAxisCount( int count );
	
	/** \brief Axis at index. */
	debiDeviceAxis &GetAxisAt( int index ) const;
	
	/** \brief Axis with identifier or \em NULL if absent. */
	debiDeviceAxis *GetAxisWithID( const char *id ) const;
	
	/** \brief Index of axis with identifier or -1 if absent. */
	int IndexOfAxisWithID( const char *id ) const;
	
	/** \brief Index of axis with beos input code or -1 if absent. */
	int IndexOfAxisWithBICode( int code ) const;
	
	
	
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
