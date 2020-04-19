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

#ifndef _DEXSIDEVICEAXIS_H_
#define _DEXSIDEVICEAXIS_H_

#include "dexsiXInclude.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputDeviceAxis.h>
#include <dragengine/resources/image/deImageReference.h>

class dexsiDevice;
class deInputDeviceAxis;
class deXSystemInput;



/**
 * \brief X-System input device axis.
 */
class dexsiDeviceAxis : public deObject{
private:
	deXSystemInput &pModule;
	
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
	timeval pLastEventTime;
	
	int pX11Code;
	int pEvdevCode;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device axis. */
	dexsiDeviceAxis( deXSystemInput &module );
	
protected:
	/** \brief Clean up device axis. */
	virtual ~dexsiDeviceAxis();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** \brief X-System input module. */
	inline deXSystemInput &GetModule() const{ return pModule; }
	
	/** \brief Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** \brief Set index. */
	void SetIndex( int index );
	
	/** \brief Identifier. */
	inline const decString &GetID() const{ return pID; }
	
	/** \brief Set identifier. */
	void SetID( const char *id );
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Type. */
	inline deInputDeviceAxis::eAxisTypes GetType() const{ return pType; }
	
	/** \brief Set type. */
	void SetType( deInputDeviceAxis::eAxisTypes type );
	
	
	
	/** \brief Display image. */
	inline deImage *GetDisplayImage() const{ return pDisplayImage; }
	
	/** \brief Display icons (deImage*). */
	inline const decObjectOrderedSet &GetDisplayIcons() const{ return pDisplayIcons; }
	
	/** \brief Set display image and icons. */
	void SetDisplayImages( const char *name );
	
	/** \brief Display text. */
	inline const decString &GetDisplayText() const{ return pDisplayText; }
	
	/** \brief Set display text. */
	void SetDisplayText( const char *text );
	
	
	
	/** \brief Minimum value. */
	inline int GetMinimum() const{ return pMinimum; }
	
	/** \brief Set minimum value. */
	void SetMinimum( int minimum );
	
	/** \brief Maximum value. */
	inline int GetMaximum() const{ return pMaximum; }
	
	/** \brief Set maximum value. */
	void SetMaximum( int maximum );
	
	/** \brief Fuzz. */
	inline int GetFuzz() const{ return pFuzz; }
	
	/** \brief Set fuzz. */
	void SetFuzz( int fuzz );
	
	/** \brief Flat. */
	inline int GetFlat() const{ return pFlat; }
	
	/** \brief Set flat. */
	void SetFlat( int flat );
	
	/** \brief Axis is absolute. */
	inline bool GetAbsolute() const{ return pAbsolute; }
	
	/** \brief Set if axis is absolute. */
	void SetAbsolute( bool absolute );
	
	/** \brief Wheel around other axis. */
	inline bool GetWheelOtherAxis() const{ return pWheelOtherAxis; }
	
	/** \brief Set to wheel around other axis. */
	void SetWheelOtherAxis( bool otherAxis );
	
	/** \brief Wheel changed value. */
	inline int GetWheelChange() const{ return pWheelChange; }
	
	/** \brief Set wheel change value. */
	void SetWheelChange( int change, int modifiers, const timeval &eventTime );
	
	/** \brief Increment wheel change value. */
	void IncrementWheelChange( int amount, int modifiers, const timeval &eventTime );
	
	/** \brief Value. */
	inline float GetValue() const{ return pValue; }
	
	/** \brief Set value. */
	void SetValue( float value );
	
	
	
	/** \brief X11 specific code. */
	inline int GetX11Code() const{ return pX11Code; }
	
	/** \brief Set X11 specific code. */
	void SetX11Code( int code );
	
	/** \brief Libevdev specific event code. */
	inline int GetEvdevCode() const{ return pEvdevCode; }
	
	/** \brief Set libevdev specific event code. */
	void SetEvdevCode( int code );
	
	
	
	/** \brief Update engine input device information axis. */
	void GetInfo( deInputDeviceAxis &info ) const;
	
	/** \brief Process event collecting changes. */
	void EvdevProcessEvent( dexsiDevice &device, const input_event &event );
	
	/** \brief Send events if present. */
	void SendEvents( dexsiDevice &device );
	/*@}*/
	
	
	
private:
	void pUpdateDeadZone();
};

#endif
