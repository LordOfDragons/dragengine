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

#ifndef _DECOLLIDERVOLUME_H_
#define _DECOLLIDERVOLUME_H_

#include "deCollider.h"
#include "../../common/shape/decShapeList.h"


/**
 * \brief Volume collider.
 *
 * Colliders are used by the physics module to provide collision
 * detection and optional collision response. After the collision
 * detection stage is done each Collider contains the new state
 * of motion. The user can then update the game state depending
 * on the changes. The scripting peer receives all relevant events
 * during a collision detection run. The Collider is defined by a
 * list of Collision Volume objects. The final test volume is
 * considered to be the boolean union of all the volumes.
 */
class DE_DLL_EXPORT deColliderVolume : public deCollider{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deColliderVolume> Ref;
	
	
private:
	decShapeList pShapes;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create volume collider. */
	deColliderVolume( deColliderManager *manager );
	
protected:
	/**
	 * \brief Clean up volume collider.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deColliderVolume();
	/*@}*/
	
	
	
public:
	/** \name Shapes */
	/*@{*/
	/** \brief Shapes. */
	inline const decShapeList &GetShapes() const{ return pShapes; }
	
	/** \brief Set shapes. */
	void SetShapes( const decShapeList &shapes );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit collider. */
	virtual void Visit( deColliderVisitor &visitor );
	/*@}*/
};

#endif
