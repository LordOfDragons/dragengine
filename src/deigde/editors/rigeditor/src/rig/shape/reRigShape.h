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

#ifndef _RERIGSHAPE_H_
#define _RERIGSHAPE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/deObject.h>

class reRig;
class reRigBone;
class igdeWDebugDrawerShape;
class decShape;
class deDebugDrawer;
class deColliderVolume;
class deEngine;



/**
 * \brief Rig shape.
 */
class reRigShape : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<reRigShape> Ref;
	
	
	/** \brief Shape type. */
	enum eShapeTypes{
		estSphere,
		estBox,
		estCylinder,
		estCapsule,
		estHull
	};
	
private:
	deEngine *pEngine;
	
	reRig *pRig;
	reRigBone *pRigBone;
	
	deDebugDrawer::Ref pDebugDrawer;
	igdeWDebugDrawerShape *pDDSShape;
	deColliderVolume::Ref pCollider;
	
	eShapeTypes pShapeType;
	
	decVector pPosition;
	decVector pOrientation;
	
	decString pProperty;
	
	bool pSelected;
	bool pActive;
	
	bool pDirtyShape;
	bool pDirtyPosition;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create rig shape. */
	reRigShape(deEngine *engine, eShapeTypes shapeType);
	
	/** \brief Clean up rig shape. */
	virtual ~reRigShape();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Eengine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	
	/** \brief Parent rig. */
	inline reRig *GetRig() const{ return pRig; }
	
	/** \brief Set parent rig. */
	void SetRig(reRig *rig);
	
	/** \brief Parent rig bone. */
	inline reRigBone *GetRigBone() const{ return pRigBone; }
	
	/** \brief Set parent rig bone. */
	void SetRigBone(reRigBone *rigBone);
	
	/** \brief Collider. */
	inline const deColliderVolume::Ref &GetCollider() const{ return pCollider; }
	
	/** \brief Shape type. */
	inline eShapeTypes GetShapeType() const{ return pShapeType; }
	
	/** \brief Position. */
	inline decVector GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition(const decVector &position);
	
	/** \brief Orientation. */
	inline decVector GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation(const decVector &orientation);
	
	/** \brief Property. */
	inline const decString &GetProperty() const{ return pProperty; }
	
	/** \brief Set property. */
	void SetProperty(const char *property);
	
	/** \brief Shape is selected. */
	inline bool GetSelected() const{ return pSelected; }
	
	/** \brief Set if shape is selected. */
	void SetSelected(bool selected);
	
	/** \brief Shape is the active one. */
	inline bool GetActive() const{ return pActive; }
	
	/** \brief Set if shape is the active one. */
	void SetActive(bool active);
	
	
	
	/** \brief Show state changed. */
	void ShowStateChanged();
	
	/** \brief Update shape. */
	void Update();
	
	/** \brief Notify rig shape changed and invalidate shape. */
	void NotifyShapeChanged();
	
	/** \brief Mark shape dirty. */
	void InvalidateShape();
	
	/** \brief Invalidate world position and orientation of the shape. */
	void InvalidatePosition();
	
	/** \brief Shape is visible in the current state. */
	bool IsVisible() const;
	
	
	
	/** \brief Create copy of shape. */
	virtual reRigShape *Duplicate() const = 0;
	
	/** \brief Uniformly scale shape. */
	virtual void Scale(float scale) = 0;
	
	/** \brief Create shape. */
	virtual decShape *CreateShape() = 0;
	/*@}*/
	
private:
	void pRSCleanUp();
	void pUpdateShapes();
	void pUpdateDDSColor();
	void pRepositionShapes();
	void pSetRig(reRig *rig, bool update);
};

#endif
