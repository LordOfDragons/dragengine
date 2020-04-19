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
#ifndef _DEBPPROPFIELD_H_
#define _DEBPPROPFIELD_H_

// includes
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/physics/deBasePhysicsPropField.h>

// predefinitions
class debpWorld;
class debpPropFieldType;
class dePhysicsBullet;

class dePropField;



/**
 * @brief Prop Field.
 *
 * Peer for the force field resource.
 */
class debpPropField : public deBasePhysicsPropField{
private:
	dePhysicsBullet *pBullet;
	dePropField *pPropField;
	
	debpPropFieldType **pTypes;
	int pTypeCount;
	int pTypeSize;
	
	bool pDirty;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new peer. */
	debpPropField( dePhysicsBullet *bullet, dePropField *forceField );
	/** Cleans up the peer. */
	virtual ~debpPropField();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline dePhysicsBullet *GetBullet() const{ return pBullet; }
	/** Retrieves the force field. */
	inline dePropField *GetPropField() const{ return pPropField; }
	
	/** Retrieves the number of types. */
	inline int GetTypeCount() const{ return pTypeCount; }
	/** Retrieves the type at the given index. */
	debpPropFieldType *GetTypeAt( int index ) const;
	
	/** Updates the prop field if required. */
	void Update( float elapsed );
	/*@}*/
	
	/** @name Notification */
	/*@{*/
	/** Position changed. */
	virtual void PositionChanged();
	
	/** Type has been added. */
	virtual void TypeAdded( int index, dePropFieldType *type );
	/** Type has been removed. */
	virtual void TypeRemoved( int index, dePropFieldType *type );
	/** All types have been removed. */
	virtual void AllTypesRemoved();
	/** Type changed. */
	virtual void TypeChanged( int index, dePropFieldType *type );
	
	/** Instances changed. */
	virtual void InstancesChanged( int index, dePropFieldType *type );
	
	/** Project all instances to the given ground. */
	virtual void ProjectInstances( const dePropFieldGround &ground, const decVector &direction );
	/*@}*/
	
private:
	void pCleanUp();
	void pProjectInstancesDown( const dePropFieldGround &ground );
};

// end of include only once
#endif
