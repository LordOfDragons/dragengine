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

#ifndef _DEMOIDEVICEAXIS_H_
#define _DEMOIDEVICEAXIS_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputDeviceAxis.h>

class deInputDeviceAxis;
class demoiDevice;


/**
 * \brief MacOS input device axis.
 */
class demoiDeviceAxis{
private:
    int pIndex;
	decString pID;
	decString pName;
    deInputDeviceAxis::eAxisTypes pType;
	
    int pMinimum;
    int pMaximum;
    bool pAbsolute;
    bool pWheelOtherAxis;
    
    float pValue;
    float pChangedValue;
    timeval pLastEventTime;

    int pMOCode;
    
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device axis. */
	demoiDeviceAxis();
	
	/** \brief Clean up device axis. */
	~demoiDeviceAxis();
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

    
    
    /** \brief MacOS specific code. */
    inline int GetMOCode() const{ return pMOCode; }
    
    /** \brief Set MacOS specific code. */
    void SetMOCode( int code );
	
	
    
	/** \brief Update engine input device information axis. */
	void GetInfo( deInputDeviceAxis &info ) const;
    
    /** \brief Send events if present. */
    void SendEvents( demoiDevice &device );
	/*@}*/
};

#endif
