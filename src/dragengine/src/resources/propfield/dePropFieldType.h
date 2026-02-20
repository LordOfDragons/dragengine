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

#include "dePropFieldInstance.h"
#include "dePropFieldBendState.h"
#include "../model/deModel.h"
#include "../skin/deSkin.h"
#include "../../common/collection/decTUniqueList.h"
#include "../../common/utils/decCollisionFilter.h"


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
public:
	/** \brief Reference. */
	using Ref = deTUniqueReference<dePropFieldType>;
	
	/** \brief List type. */
	using List = decTUniqueList<dePropFieldType>;
	
private:
	deModel::Ref pModel;
	deSkin::Ref pSkin;
	float pRotationPerForce;
	float pRestitution;
	
	decCollisionFilter pCollisionFilter;
	
	decTList<dePropFieldInstance> pInstances;
	decTList<dePropFieldBendState> pBendStates;
	
	
	
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
	inline const deModel::Ref &GetModel() const{ return pModel; }
	
	/** \brief Set model or NULL it not set. */
	void SetModel(deModel *model);
	
	/** \brief Skin or NULL if not set. */
	inline const deSkin::Ref &GetSkin() const{ return pSkin; }
	
	/** \brief Set skin or NULL if not set. */
	void SetSkin(deSkin *skin);
	
	/** \brief Rotation per force. */
	inline float GetRotationPerForce() const{ return pRotationPerForce; }
	
	/** \brief Set rotation per force. */
	void SetRotationPerForce(float rotation);
	
	/** \brief Restitution. */
	inline float GetRestitution() const{ return pRestitution; }
	
	/** \brief Set restitution. */
	void SetRestitution(float restitution);
	
	/** \brief Collision filter. */
	inline const decCollisionFilter &GetCollisionFilter() const{ return pCollisionFilter; }
	
	/** \brief Set collision filter. */
	void SetCollisionFilter(const decCollisionFilter &collisionFilter);
	
	/** \brief Instances. */
	inline decTList<dePropFieldInstance> &GetInstances(){ return pInstances; }
	inline const decTList<dePropFieldInstance> &GetInstances() const{ return pInstances; }
	
	/** \brief Bend states. */
	inline decTList<dePropFieldBendState> &GetBendStates(){ return pBendStates; }
	inline const decTList<dePropFieldBendState> &GetBendStates() const{ return pBendStates; }
	/*@}*/
};

#endif
