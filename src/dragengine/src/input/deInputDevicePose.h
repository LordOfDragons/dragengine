/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _INPUTDEVICEPOSE_H_
#define _INPUTDEVICEPOSE_H_

#include "../common/math/decMath.h"


/**
 * \brief Input device pose.
 * \version 1.6
 */
class deInputDevicePose{
private:
	/** \brief Position in meters. */
	decVector pPosition;
	
	/** \brief Orientation. */
	decQuaternion pOrientation;
	
	/** \brief Linear velocity in meters per second. */
	decVector pLinearVelocity;
	
	/** \brief Angular velocity in radians per second. */
	decVector pAngularVelocity;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create input device pose. */
	deInputDevicePose();
	
	/** \brief Create copy of input device pose. */
	deInputDevicePose( const deInputDevicePose &pose );
	
	/** \brief Clean up input device pose. */
	~deInputDevicePose();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position in meters. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position in meters. */
	void SetPosition( const decVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Linear velocity in meters per second. */
	inline const decVector &GetLinearVelocity() const{ return pLinearVelocity; }
	
	/** \brief Set linear velocity in meters per second. */
	void SetLinearVelocity( const decVector &velocity );
	
	/** \brief Angular velocity in radians per second. */
	inline const decVector &GetAngularVelocity() const{ return pAngularVelocity; }
	
	/** \brief Set angular velocity in radians per second. */
	void SetAngularVelocity( const decVector &velocity );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy input device pose. */
	deInputDevicePose &operator=( const deInputDevicePose &pose );
	/*@}*/
};

#endif
