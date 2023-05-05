/* 
 * Drag[en]gine Animator Module
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

#ifndef _DEARCOMPONENTBONESTATE_H_
#define _DEARCOMPONENTBONESTATE_H_

#include <dragengine/common/math/decMath.h>



/**
 * Component bone state.
 */
class dearComponentBoneState{
private:
	decVector pPosition;
	decQuaternion pRotation;
	decVector pScale;
	
	dearComponentBoneState *pParent;
	decMatrix pOriginalMatrix;
	decMatrix pMatrix;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create component bone state. */
	dearComponentBoneState();
	
	/** Clean up component bone state. */
	~dearComponentBoneState();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** Set position. */
	void SetPosition( const decVector &position );
	
	/** Rotation . */
	inline const decQuaternion &GetRotation() const{ return pRotation; }
	
	/** Set rotation. */
	void SetRotation( const decQuaternion &rotation );
	
	/** Scale. */
	inline const decVector &GetScale() const{ return pScale; }
	
	/** Set scale. */
	void SetScale( const decVector &scale );
	
	
	
	/** Parent state used to prepare the matrices or \em NULL. */
	inline dearComponentBoneState *GetParent() const{ return pParent; }
	
	/** Set parent state used to prepare the matrices or \em NULL. */
	void SetParent( dearComponentBoneState *parent );
	
	/** Original matrix required to prepate matrix. */
	inline const decMatrix &GetOriginalMatrix() const{ return pOriginalMatrix; }
	
	/** Set original matrix required to prepare matrix. */
	void SetOriginalMatrix( const decMatrix &matrix );
	
	/** Prepared matrix. */
	inline const decMatrix &GetMatrix() const{ return pMatrix; }
	
	/** Set prepared matrix. */
	void SetMatrix( const decMatrix &matrix );
	/*@}*/
};

#endif
