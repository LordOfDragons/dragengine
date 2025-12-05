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

#ifndef _DEPROPFIELDTYPE_H_
#define _DEPROPFIELDTYPE_H_

#include "../model/deModel.h"
#include "../skin/deSkin.h"
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
	deModel::Ref pModel;
	deSkin::Ref pSkin;
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
