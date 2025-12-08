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

#ifndef _DEBPGHOSTOBJECT_H_
#define _DEBPGHOSTOBJECT_H_

#include "debpCollisionObject.h"

#include <dragengine/common/math/decMath.h>

#include "debpBulletShape.h"
class btGhostObject;
class debpCollisionWorld;



/**
 * \brief Implements a ghost object in the bullet physics system.
 * \details This object takes care of creating the bullet ghost object and updating
 *          its state. Furthermore a reference is kept to the dragengine object owning
 *          the ghost object. A dragengine object can own more than one ghost object.
 */
class debpGhostObject : public debpCollisionObject{
private:
	debpCollisionWorld *pDynWorld;
	btGhostObject *pGhostObject;
	debpBulletShape::Ref pShape;
	
	decDVector pPosition;
	decQuaternion pOrientation;
	decDMatrix pMatrix;
	bool pEnabled;
	bool pDirtyMatrix;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new bullet ghost object. */
	debpGhostObject();
	/** \brief Cleans up the bullet ghost object. */
	~debpGhostObject() override;
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the ghost object. */
	inline btGhostObject *GetGhostObject() const{ return pGhostObject; }
	/** \brief Retrieves the dynamics world or NULL if not assigned to one yet. */
	inline debpCollisionWorld *GetDynamicsWorld() const{ return pDynWorld; }
	/** \brief Sets the dynamics world or NULL if not assigned to one yet. */
	void SetDynamicsWorld(debpCollisionWorld *dynWorld);
	/** \brief Retrieves the collision shape or NULL if not set. */
	inline const debpBulletShape::Ref &GetShape() const{ return pShape; }
	/** \brief Sets the collision shape or NULL if not set. */
	void SetShape(debpBulletShape *shape);
	
	/** \brief Retrieves the position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	/** \brief Sets the position. */
	void SetPosition(const decDVector &position);
	/** \brief Retrieves the orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	/** \brief Sets the orientation. */
	void SetOrientation(const decQuaternion &orientation);
	
	/** \brief Determines if the ghost object is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	/** \brief Sets if the ghost object is enabled. */
	void SetEnabled(bool enabled);
	/** \brief Retrieves the ghost object matrix. */
	const decDMatrix &GetMatrix();
	
	/** \brief Update dynamic world AABB if dirty. */
	void UpdateAABB();
	/*@}*/
	
private:
	void pCleanUp();
	void pCreateGhostObject();
	void pFreeGhostObject();
	void pUpdateTransform();
};

#endif
