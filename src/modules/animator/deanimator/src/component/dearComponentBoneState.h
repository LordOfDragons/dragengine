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
 * \brief Component bone state.
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
	/** \brief Create component bone state. */
	dearComponentBoneState();
	
	/** \brief Clean up component bone state. */
	~dearComponentBoneState();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decVector &position );
	
	/** \brief Rotation . */
	inline const decQuaternion &GetRotation() const{ return pRotation; }
	
	/** \brief Set rotation. */
	void SetRotation( const decQuaternion &rotation );
	
	/** \brief Scale. */
	inline const decVector &GetScale() const{ return pScale; }
	
	/** \brief Set scale. */
	void SetScale( const decVector &scale );
	
	
	
	/** \brief Parent state used to prepare the matrices or \em NULL. */
	inline dearComponentBoneState *GetParent() const{ return pParent; }
	
	/** \brief Set parent state used to prepare the matrices or \em NULL. */
	void SetParent( dearComponentBoneState *parent );
	
	/** \brief Original matrix required to prepate matrix. */
	inline const decMatrix &GetOriginalMatrix() const{ return pOriginalMatrix; }
	
	/** \brief Set original matrix required to prepare matrix. */
	void SetOriginalMatrix( const decMatrix &matrix );
	
	/** \brief Prepared matrix. */
	inline const decMatrix &GetMatrix() const{ return pMatrix; }
	
	/** \brief Set prepared matrix. */
	void SetMatrix( const decMatrix &matrix );
	/*@}*/
};

#endif
