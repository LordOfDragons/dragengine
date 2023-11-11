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

#ifndef _DEWIDEVICEAXIS_H_
#define _DEWIDEVICEAXIS_H_

#include "dewiInclude.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputDeviceAxis.h>
#include <dragengine/resources/image/deImageReference.h>

class dewiDevice;
class dewiDeviceWinRTController;
class deInputDeviceAxis;
class deWindowsInput;



/**
 * Windows input device axis.
 */
class dewiDeviceAxis : public deObject{
public:
	typedef deTObjectReference<dewiDeviceAxis> Ref;



private:
	deWindowsInput &pModule;
	
	int pIndex;
	decString pID;
	decString pName;
	deInputDeviceAxis::eAxisTypes pType;
	
	deImageReference pDisplayImage;
	decObjectOrderedSet pDisplayIcons;
	decString pDisplayText;
	
	int pMinimum;
	int pMaximum;
	int pDeadZoneLower;
	int pDeadZoneUpper;
	int pFuzz;
	int pFlat;
	bool pAbsolute;
	bool pWheelOtherAxis;
	int pWheelChange;
	
	float pValue;
	float pChangedValue;
	int pLastModifiers;
	DWORD pLastEventTime;
	
	int pWICode;
	int pWinRTReadingIndexAxis;
	int pWinRTReadingIndexSwitch;
	int pWinRTReadingDirectionSwitch;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device axis. */
	dewiDeviceAxis( deWindowsInput &module );
	
protected:
	/** Clean up device axis. */
	virtual ~dewiDeviceAxis();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** Input module. */
	inline deWindowsInput &GetModule() const{ return pModule; }
	
	/** Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** Set index. */
	void SetIndex( int index );
	
	/** Identifier. */
	inline const decString &GetID() const{ return pID; }
	
	/** Set identifier. */
	void SetID( const char *id );
	
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Set name. */
	void SetName( const char *name );
	
	/** Type. */
	inline deInputDeviceAxis::eAxisTypes GetType() const{ return pType; }
	
	/** Set type. */
	void SetType( deInputDeviceAxis::eAxisTypes type );
	
	
	
	/** Display image. */
	inline deImage *GetDisplayImage() const{ return pDisplayImage; }
	
	/** Display icons (deImage*). */
	inline const decObjectOrderedSet &GetDisplayIcons() const{ return pDisplayIcons; }
	
	/** Set display image and icons. */
	void SetDisplayImages( const char *name );
	
	/** Display text. */
	inline const decString &GetDisplayText() const{ return pDisplayText; }
	
	/** Set display text. */
	void SetDisplayText( const char *text );
	
	
	
	/** Minimum value. */
	inline int GetMinimum() const{ return pMinimum; }
	
	/** Set minimum value. */
	void SetMinimum( int minimum );
	
	/** Maximum value. */
	inline int GetMaximum() const{ return pMaximum; }
	
	/** Set maximum value. */
	void SetMaximum( int maximum );
	
	/** Fuzz. */
	inline int GetFuzz() const{ return pFuzz; }
	
	/** Set fuzz. */
	void SetFuzz( int fuzz );
	
	/** Flat. */
	inline int GetFlat() const{ return pFlat; }
	
	/** Set flat. */
	void SetFlat( int flat );
	
	/** Axis is absolute. */
	inline bool GetAbsolute() const{ return pAbsolute; }
	
	/** Set if axis is absolute. */
	void SetAbsolute( bool absolute );
	
	/** Wheel around other axis. */
	inline bool GetWheelOtherAxis() const{ return pWheelOtherAxis; }
	
	/** Set to wheel around other axis. */
	void SetWheelOtherAxis( bool otherAxis );
	
	/** Wheel changed value. */
	inline int GetWheelChange() const{ return pWheelChange; }
	
	/** Set wheel change value. */
	void SetWheelChange( int change, int modifiers, DWORD eventTime );
	
	/** Increment wheel change value. */
	void IncrementWheelChange( int amount, int modifiers, DWORD eventTime );
	
	/** Value. */
	inline float GetValue() const{ return pValue; }
	
	/** Set value. */
	void SetValue( float value );
	
	
	
	/** WI specific code. */
	inline int GetWICode() const{ return pWICode; }
	
	/** Set WI specific code. */
	void SetWICode( int code );

	/** WinRT reading index axis. */
	inline int GetWinRTReadingIndexAxis() const{ return pWinRTReadingIndexAxis; }

	/** Set WinRT reading index axis. */
	void SetWinRTReadingIndexAxis( int index );

	/** WinRT reading index switch. */
	inline int GetWinRTReadingIndexSwitch() const{ return pWinRTReadingIndexSwitch; }

	/** Set WinRT reading index switch. */
	void SetWinRTReadingIndexSwitch( int index );

	/** WinRT reading switch direction. */
	inline int GetWinRTReadingDirectionSwitch() const{ return pWinRTReadingDirectionSwitch; }

	/** Set WinRT reading switch direction. */
	void SetWinRTReadingDirectionSwitch( int direction );
	
	
	
	/** Update engine input device information axis. */
	void GetInfo( deInputDeviceAxis &info ) const;
	
	/** Send events if present. */
	void SendEvents( dewiDevice &device );

	/** Process WinRT reading. */
	void WinRTReading( dewiDeviceWinRTController &device );
	/*@}*/
	
	
	
private:
	void pUpdateDeadZone();
};

#endif
