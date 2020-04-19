/* 
 * Drag[en]gine Android Input Module
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

#ifndef _DEAIDEVICEAXIS_H_
#define _DEAIDEVICEAXIS_H_

#include <unistd.h>

#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputDeviceAxis.h>

class deaiDevice;
class deInputDeviceAxis;



/**
 * \brief Android input device axis.
 */
class deaiDeviceAxis{
private:
	int pIndex;
	decString pID;
	decString pName;
	deInputDeviceAxis::eAxisTypes pType;
	
	int pMinimum;
	int pMaximum;
	int pFuzz;
	int pFlat;
	bool pAbsolute;
	bool pWheelOtherAxis;
	
	float pValue;
	float pChangedValue;
	timeval pLastEventTime;
	
	int pAICode;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device axis. */
	deaiDeviceAxis();
	
	/** \brief Clean up device axis. */
	~deaiDeviceAxis();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
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
	
	
	/** \brief Value. */
	inline float GetValue() const{ return pValue; }
	
	/** \brief Set value. */
	void SetValue( float value );
	
	
	
	/** \brief Android specific code. */
	inline int GetAICode() const{ return pAICode; }
	
	/** \brief Set Android specific code. */
	void SetAICode( int code );
	
	
	
	/** \brief Update engine input device information axis. */
	void GetInfo( deInputDeviceAxis &info ) const;
	
	/** \brief Send events if present. */
	void SendEvents( deaiDevice &device );
	/*@}*/
};

#endif
