/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEDECALBONESTATE_H_
#define _DEDECALBONESTATE_H_

#include "../../common/math/decMath.h"


/**
 * \brief Decal Bone State.
 *
 * Stores the state of a component bone. This is required if the
 * decal is attached to a component and the component is not
 * static. The bone state stores the state of a component bone
 * at the time the decal has been attached to the component.
 */
class DE_DLL_EXPORT deDecalBoneState{
private:
	decVector pPosition;
	decQuaternion pRotation;
	decVector pSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new decal bone state. */
	deDecalBoneState();
	
	/** \brief Clean up decal bone state. */
	~deDecalBoneState();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decVector &position );
	
	/** \brief Rotation. */
	inline const decQuaternion &GetRotation() const{ return pRotation; }
	
	/** \brief Set rotation. */
	void SetRotation( const decQuaternion &rotation );
	
	/** \brief Size. */
	inline const decVector &GetSize() const{ return pSize; }
	
	/** \brief Set size. */
	void SetSize( const decVector &size );
};

#endif
