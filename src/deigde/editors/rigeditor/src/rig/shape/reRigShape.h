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
	/** \brief Shape type. */
	enum eShapeTypes{
		estSphere,
		estBox,
		estCylinder,
		estCapsule
	};
	
private:
	deEngine *pEngine;
	
	reRig *pRig;
	reRigBone *pRigBone;
	
	deDebugDrawer *pDebugDrawer;
	igdeWDebugDrawerShape *pDDSShape;
	deColliderVolume *pCollider;
	
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
	reRigShape( deEngine *engine, eShapeTypes shapeType );
	
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
	void SetRig( reRig *rig );
	
	/** \brief Parent rig bone. */
	inline reRigBone *GetRigBone() const{ return pRigBone; }
	
	/** \brief Set parent rig bone. */
	void SetRigBone( reRigBone *rigBone );
	
	/** \brief Collider. */
	inline deColliderVolume *GetCollider() const{ return pCollider; }
	
	/** \brief Shape type. */
	inline eShapeTypes GetShapeType() const{ return pShapeType; }
	
	/** \brief Position. */
	inline decVector GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decVector &position );
	
	/** \brief Orientation. */
	inline decVector GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decVector &orientation );
	
	/** \brief Property. */
	inline const decString &GetProperty() const{ return pProperty; }
	
	/** \brief Set property. */
	void SetProperty( const char *property );
	
	/** \brief Shape is selected. */
	inline bool GetSelected() const{ return pSelected; }
	
	/** \brief Set if shape is selected. */
	void SetSelected( bool selected );
	
	/** \brief Shape is the active one. */
	inline bool GetActive() const{ return pActive; }
	
	/** \brief Set if shape is the active one. */
	void SetActive( bool active );
	
	
	
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
	virtual void Scale( float scale ) = 0;
	
	/** \brief Create shape. */
	virtual decShape *CreateShape() = 0;
	/*@}*/
	
private:
	void pRSCleanUp();
	void pUpdateShapes();
	void pUpdateDDSColor();
	void pRepositionShapes();
};

#endif
