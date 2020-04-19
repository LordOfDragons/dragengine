/* 
 * Drag[en]gine IGDE Rig Editor
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
#ifndef _RERIGSHAPEBOX_H_
#define _RERIGSHAPEBOX_H_

// includes
#include "reRigShape.h"



/**
 * @brief Rig Box Shape.
 * Working object for rig box shape.
 */
class reRigShapeBox : public reRigShape{
private:
	decVector pHalfExtends;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rig box shape. */
	reRigShapeBox( deEngine *engine );
	/** Cleans up the rig shape shape. */
	virtual ~reRigShapeBox();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the half extends. */
	inline const decVector &GetHalfExtends() const{ return pHalfExtends; }
	/** Sets the half extends. */
	void SetHalfExtends( const decVector &halfExtends );
	
	/** Creates a copy of this shape. */
	virtual reRigShape *Duplicate() const;
	/** Uniformly scale shape. */
	virtual void Scale( float scale );
	
	/** Creates shape. */
	virtual decShape *CreateShape();
	/*@}*/
};

// end of include only once
#endif
