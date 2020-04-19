/* 
 * Drag[en]gine Bullet Physics Module
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

// include only once
#ifndef _DEBPMOTIONSTATE_H_
#define _DEBPMOTIONSTATE_H_

// includes
#include "LinearMath/btMotionState.h"
#include "dragengine/common/math/decMath.h"

// predefintions



/**
 * @brief Bullet Motion State.
 * Provides an implementation of the MotionState.
 */
class debpMotionState : public btMotionState{
private:
	decDVector pPosition;
	decVector pScaling;
	decQuaternion pOrientation;
	
public:
	// constructor, destructor
	debpMotionState();
	~debpMotionState();
	// management
	inline const decDVector &GetPosition() const{ return pPosition; }
	inline const decVector &GetScaling() const{ return pScaling; }
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	void SetPosition( const decDVector &position );
	void SetScaling( const decVector &scaling );
	void SetOrientation( const decQuaternion &orientation );
	// bullet management
	virtual void getWorldTransform( btTransform &centerOfMassWorldTrans ) const;
	virtual void setWorldTransform( const btTransform &centerOfMassWorldTrans );
	virtual bool deactivationCallback( void *userPointer );
private:
	void pCleanUp();
};

// end of include only once
#endif
