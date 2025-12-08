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

#ifndef _MEOBJECTSHAPE_H_
#define _MEOBJECTSHAPE_H_

#include "../meColliderOwner.h"
#include "../object/meObject.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>


class meObject;
class meWorld;

class igdeWDebugDrawerShape;
class igdeEnvironment;

#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
class deEngine;
class decShape;



/**
 * \brief Object Shape.
 * \details A single shape in an object shape. Whereas the meObjectShapeList corresponds to a decShapeList the
 *          meObjectShape corresponds to an actual decShape inside the list. The shape is located in the coordinate
 *          system of the world. The user is responsible to transform the shape from and to the world coordinate
 *          system and whatever target element holds the object shape list.
 */
class meObjectShape : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meObjectShape> Ref;
	
	
private:
	igdeEnvironment *pEnvironment;
	
	meWorld *pWorld;
	meObject *pParentObject;
	
	deDebugDrawer::Ref pDebugDrawer;
	igdeWDebugDrawerShape *pDDSShape;
	deColliderVolume::Ref pEngCollider;
	
	decShape *pShape;
	
	bool pSelected;
	bool pActive;
	
	meColliderOwner pColliderOwner;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new object shape. */
	meObjectShape(igdeEnvironment *environment, const decShape &shape);
	/** \brief Cleans up the object shape. */
	virtual ~meObjectShape();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment *GetEnvironment() const{ return pEnvironment; }
	
	/** \brief Retrieves the engine collider. */
	inline const deColliderVolume::Ref &GetEngineCollider() const{ return pEngCollider; }
	
	/** \brief Retrieves the world or NULL. */
	inline meWorld *GetWorld() const{ return pWorld; }
	/** \brief Sets the world or NULL. */
	void SetWorld(meWorld *world);
	/** \brief Retrieves the parent object or NULL. */
	inline meObject *GetParentObject() const{ return pParentObject; }
	/** \brief Sets the parent object or NULL. */
	void SetParentObject(meObject *parentObject);
	
	/** \brief Retrieves the shape. */
	inline const decShape *GetShape() const{ return pShape; }
	/** \brief Sets the shape. Make sure it is of the matching shape type. */
	void SetShape(const decShape &shape);
	
	/** \brief Determines if the shape is selected. */
	inline bool GetSelected() const{ return pSelected; }
	/** \brief Sets if the shape is selected. */
	void SetSelected(bool selected);
	/** \brief Determines if the shape is active. */
	inline bool GetActive() const{ return pActive; }
	/** \brief Sets if the shape is active. */
	void SetActive(bool active);
	
	/** \brief Update collider and debug drawer shape, position, orientation and scaling. */
	void UpdateShape();
	/** \brief Update debug drawer colors. */
	void UpdateDDSColors();
	
	/** \brief Show states changed. This typically changes debug drawer shape visibilites. */
	void ShowStateChanged();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
