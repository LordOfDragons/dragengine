/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _IGDEGIZMOMOVE_H_
#define _IGDEGIZMOMOVE_H_

#include "igdeGizmo.h"


/**
 * \brief Interactive 3D gizmo for moving objects.
 * 
 * Shows a moving gizmo and handles moving an object. Subclass has to provide support for getting
 * and setting object position, orientation and scaling.
 */
class DE_DLL_EXPORT igdeGizmoMove : public igdeGizmo{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeGizmoMove> Ref;
	
	/** \brief Action. */
	enum eActions{
		/** \brief No action. */
		eaNone,
		
		/** \brief Move along X axis. */
		eaMoveX,
		
		/** \brief Move along Y axis. */
		eaMoveY,
		
		/** \brief Move along Z axis. */
		eaMoveZ,
		
		/** \brief Move along XY plane. */
		eaMoveXY,
		
		/** \brief Move along YZ plane. */
		eaMoveYZ,
		
		/** \brief Move along XZ plane. */
		eaMoveXZ,
		
		/** \brief Move along view plane. */
		eaMoveView
	};
	
	/** \brief Default shape names. */
	static const char * const ShapeNameMoveX;
	static const char * const ShapeNameMoveY;
	static const char * const ShapeNameMoveZ;
	static const char * const ShapeNameMoveXY;
	static const char * const ShapeNameMoveYZ;
	static const char * const ShapeNameMoveXZ;
	static const char * const ShapeNameMoveView;
	
	
	
private:
	eActions pAction;
	
	decDMatrix pViewMatrix;
	decDVector pInteractAxis;
	decDVector pInteractAxis2;
	decDVector pInteractOrigin;
	decDVector pMoveOrigin;
	decDVector pPlanePosition;
	decDVector pPlaneNormal;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create gizmo. */
	igdeGizmoMove( igdeEnvironment &environment );
	
	
	
protected:
	/**
	 * \brief Clean up gizmo.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeGizmoMove() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Active action or eaNone. */
	inline eActions GetAction() const{ return pAction; }
	
	/** \brief Get object position. */
	virtual decDVector GetObjectPosition() = 0;
	
	/** \brief Set object position. */
	virtual void SetObjectPosition( const decDVector &position ) = 0;
	
	/** \brief Get object orientation. */
	virtual decQuaternion GetObjectOrientation();
	
	/** \brief Get object scale. */
	virtual decVector GetObjectScale();
	
	/** \brief Object matrix with scaling. */
	decDMatrix GetObjectMatrix();
	
	/**
	 * \brief Limit position.
	 * 
	 * Default implementation applies no limits.
	 */
	virtual decDVector LimitPosition( const decDVector &position );
	
	/**
	 * \brief Object position or orientation changed.
	 * 
	 * Call this function after object position or orientation changed.
	 */
	void OnObjectGeometryChanged();
	/*@}*/
	
	
	
protected:
	/**
	 * \brief Map shape name to action.
	 * 
	 * Default implementation maps like this:
	 * - "move x" to eaMoveX
	 * - "move y" to eaMoveY
	 * - "move z" to eaMoveZ
	 * - "move xy" to eaMoveXY
	 * - "move yz" to eaMoveYZ
	 * - "move xz" to eaMoveXZ
	 * - "move view" to eaMoveView
	 * - everything else to eaNone
	 */
	virtual eActions pShapeNameToAction( const decString &shapeName ) const;
	
	/**
	 * \brief Start editing.
	 * 
	 * Called by StartEditing() if a named gizmo collider shape is hit. Subclass has to
	 * return true to start editing or false otherwise.
	 * 
	 * \param[in] rayOrigin Origin of ray hitting gizmo.
	 * \param[in] rayDirection Direction of ray (including length) hitting gizmo.
	 * \param[in] viewMatrix View oriented matrix.
	 */
	virtual bool OnStartEditing( const decDVector &rayOrigin, const decDVector &rayDirection,
		const decDMatrix &viewMatrix, const decDVector &hitPoint,
		const decString &shapeName ) override;
	
	/**
	 * \brief Update editing.
	 * 
	 * Called between StartEditing() and StopEditing() on frame update.
	 * 
	 * \param[in] rayOrigin Origin of ray.
	 * \param[in] rayDirection Direction of ray (including length).
	 * \param[in] viewMatrix View oriented matrix.
	 */
	virtual void OnUpdateEditing( const decDVector &rayOrigin, const decDVector &rayDirection,
		const decDMatrix &viewMatrix ) override;
	
	/**
	 * \brief Update editing.
	 * 
	 * Called between StartEditing() and StopEditing() on frame update.
	 * 
	 * \param[in] elapsed Elapsed time.
	 */
	virtual void OnFrameUpdateEditing( float elapsed ) override;
	
	/**
	 * \brief Stop editing.
	 * 
	 * \param[in] cancel Cancel editing if possible.
	 * 
	 * Called by StopEditing() after a prior to an OnStartEditing() call returning true.
	 */
	virtual void OnStopEditing( bool cancel );
	
	/** \brief Added to world. */
	virtual void OnAddToWorld() override;
};

#endif
