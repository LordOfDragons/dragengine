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

#ifndef _INPUTDEVICEPOSE_H_
#define _INPUTDEVICEPOSE_H_

#include "../common/math/decMath.h"


/**
 * \brief Input device pose.
 * \version 1.6
 */
class DE_DLL_EXPORT deInputDevicePose{
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
