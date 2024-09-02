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
	bool pWinRTInverseAxis;
	int pWinRTReadingIndexSwitch;
	int pWinRTReadingDirectionSwitch;
	bool pIsBatteryLevel;
	
	
	
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

	/** Inverse axis. */
	inline bool GetWinRTInverseAxis() const{ return pWinRTInverseAxis; }

	/** Set inverse axis. */
	void SetWinRTInverseAxis( bool winRTInverseAxis );

	/** WinRT reading index switch. */
	inline int GetWinRTReadingIndexSwitch() const{ return pWinRTReadingIndexSwitch; }

	/** Set WinRT reading index switch. */
	void SetWinRTReadingIndexSwitch( int index );

	/** WinRT reading switch direction. */
	inline int GetWinRTReadingDirectionSwitch() const{ return pWinRTReadingDirectionSwitch; }

	/** Set WinRT reading switch direction. */
	void SetWinRTReadingDirectionSwitch( int direction );

	/** Is battery level. */
	inline bool GetIsBatteryLevel() const{ return pIsBatteryLevel; }

	/** Set is battery level. */
	void SetIsBatteryLevel( bool isBatteryLevel );
	
	
	
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
