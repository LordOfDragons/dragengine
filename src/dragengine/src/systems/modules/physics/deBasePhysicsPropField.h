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

#ifndef _DEBASEPHYSICSPROPFIELD_H_
#define _DEBASEPHYSICSPROPFIELD_H_

#include "../../../common/math/decMath.h"

class dePropFieldType;
class dePropFieldGround;


/**
 * \brief Physics System Prop Field Peer.
 *
 * Peer for the prop field resource used by the physics system.
 */
class DE_DLL_EXPORT deBasePhysicsPropField{
private:
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deBasePhysicsPropField();
	
	/** \brief Clean up peer. */
	virtual ~deBasePhysicsPropField();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Type has been added. */
	virtual void TypeAdded( int index, dePropFieldType *type );
	
	/** \brief Type has been removed. */
	virtual void TypeRemoved( int index, dePropFieldType *type );
	
	/** \brief All types have been removed. */
	virtual void AllTypesRemoved();
	
	/** \brief Type changed. */
	virtual void TypeChanged( int index, dePropFieldType *type );
	
	/** \brief Instances changed. */
	virtual void InstancesChanged( int index, dePropFieldType *type );
	
	/** \brief Project all instances to the given ground. */
	virtual void ProjectInstances( const dePropFieldGround &ground, const decVector &direction );
	/*@}*/
};

#endif
