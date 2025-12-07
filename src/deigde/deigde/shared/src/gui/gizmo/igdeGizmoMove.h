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
	igdeGizmoMove(igdeEnvironment &environment);
	
	
	
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
	inline eActions GetAction() const{return pAction;}
	
	/** \brief Get object position. */
	virtual decDVector GetObjectPosition() = 0;
	
	/** \brief Set object position. */
	virtual void SetObjectPosition(const decDVector &position) = 0;
	
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
	virtual decDVector LimitPosition(const decDVector &position);
	
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
	virtual eActions pShapeNameToAction(const decString &shapeName) const;
	
	/**
	 * \brief Start editing.
	 * 
	 * Called by StartEditing() if a named gizmo collider shape is hit. Subclass has to
	 * return true to start editing or false otherwise.
	 * 
	 * \param[in] rayOrigin Origin of ray hitting gizmo.
	 * \param[in] rayDirection Direction of ray (including length) hitting gizmo.
	 * \param[in] viewMatrix View oriented matrix.
	 * \param[in] hitPoint Hit point on gizmo shape.
	 * \param[in] shapeName Name of hit gizmo shape
	 * \param[in] modifiers Modifier keys pressed at the time of event. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	virtual bool OnStartEditing(const decDVector &rayOrigin, const decDVector &rayDirection,
		const decDMatrix &viewMatrix, const decDVector &hitPoint,
		const decString &shapeName, int modifiers) override;
	
	/**
	 * \brief Update editing.
	 * 
	 * Called between StartEditing() and StopEditing() on frame update.
	 * 
	 * \param[in] rayOrigin Origin of ray.
	 * \param[in] rayDirection Direction of ray (including length).
	 * \param[in] viewMatrix View oriented matrix.
	 * \param[in] modifiers Modifier keys pressed at the time of event. OR combination of
	 *                      values from deInputEvent::eStateModifiers.
	 */
	virtual void OnUpdateEditing(const decDVector &rayOrigin, const decDVector &rayDirection,
		const decDMatrix &viewMatrix, int modifiers) override;
	
	/**
	 * \brief Stop editing.
	 * 
	 * \param[in] cancel Cancel editing if possible.
	 * 
	 * Called by StopEditing() after a prior to an OnStartEditing() call returning true.
	 */
	virtual void OnStopEditing(bool cancel) override;
	
	/** \brief Added to world. */
	virtual void OnAddToWorld() override;
};

#endif
