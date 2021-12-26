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

#ifndef _DEPROPFIELDTYPE_H_
#define _DEPROPFIELDTYPE_H_

#include "../model/deModelReference.h"
#include "../skin/deSkinReference.h"
#include "../../common/utils/decCollisionFilter.h"

class dePropFieldInstance;
class dePropFieldBendState;


/**
 * \brief Prop Field Type.
 *
 * Defines a prop type in a prop field. Each prop type has a static model,
 * a skin and a list of instances. Each instance uses the parameters of
 * the type but has a specific placement. Instances are projected onto the
 * hosting surface. For performance and memory reasons the number of
 * instances is an immutable parameter given during construction time. In
 * games you need to init this number anyways only during loading time and
 * during run time the number is constant. Editors can recreate the type
 * to alter the count of instances. The parent prop field allows to mutate
 * the list of types so this restriction is not a road blocking one.
 */
class DE_DLL_EXPORT dePropFieldType{
private:
	deModelReference pModel;
	deSkinReference pSkin;
	float pRotationPerForce;
	float pRestitution;
	
	decCollisionFilter pCollisionFilter;
	
	dePropFieldInstance *pInstances;
	int pInstanceCount;
	
	dePropFieldBendState *pBendStates;
	int pBendStateCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create new prop field type. The instance count has to be at least 1.
	 */
	dePropFieldType();
	
	/** \brief Clean up prop field type. */
	~dePropFieldType();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Model or NULL if not set. */
	inline deModel *GetModel() const{ return pModel; }
	
	/** \brief Set model or NULL it not set. */
	void SetModel( deModel *model );
	
	/** \brief Skin or NULL if not set. */
	inline deSkin *GetSkin() const{ return pSkin; }
	
	/** \brief Set skin or NULL if not set. */
	void SetSkin( deSkin *skin );
	
	/** \brief Rotation per force. */
	inline float GetRotationPerForce() const{ return pRotationPerForce; }
	
	/** \brief Set rotation per force. */
	void SetRotationPerForce( float rotation );
	
	/** \brief Restitution. */
	inline float GetRestitution() const{ return pRestitution; }
	
	/** \brief Set restitution. */
	void SetRestitution( float restitution );
	
	/** \brief Collision filter. */
	inline const decCollisionFilter &GetCollisionFilter() const{ return pCollisionFilter; }
	
	/** \brief Set collision filter. */
	void SetCollisionFilter( const decCollisionFilter &collisionFilter );
	
	/** \brief Number of instances. */
	inline int GetInstanceCount() const{ return pInstanceCount; }
	
	/** \brief Set number of instances. */
	void SetInstanceCount( int count );
	
	/** \brief Instance at the given index. */
	dePropFieldInstance &GetInstanceAt( int index ) const;
	
	/**
	 * Retrieves the list of instances. Be careful with this method call as it is
	 * one intended only for performance usage. Make sure you to not read or write
	 * more than GetInstanceCount elements. You can be sure that the returned array
	 * is the same for the entire lifetime of this object and contains the instances
	 * in a continuous list. If possible use the GetInstanceAt method call to obtain
	 * the individual instances as there boundary checks are conducted.
	 */
	inline dePropFieldInstance *GetInstances() const{ return pInstances; }
	
	/** \brief Number of bend states. */
	inline int GetBendStateCount() const{ return pBendStateCount; }
	
	/** \brief Set number of bend states. */
	void SetBendStateCount( int count );
	
	/** \brief Bend state at the given index. */
	dePropFieldBendState &GetBendStateAt( int index ) const;
	
	/**
	 * \brief List of bend states.
	 * 
	 * Be careful with this method call as it is one intended only for performance usage.
	 * Make sure you to not read or write more than GetBendStateCount elements. This array
	 * is not guaranteed to be the same for the entire lifetime of this object but is a
	 * continuous list. If possible use the GetBendStateAt method call to obtain the
	 * individual bend states as there boundary checks are conducted. If you need direct
	 * access due to performance reasons only keep to this pointer for the short time you
	 * do some work.
	 */
	inline dePropFieldBendState *GetBendStates() const{ return pBendStates; }
	/*@}*/
};

#endif
