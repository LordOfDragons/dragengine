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

#ifndef _IGDEGIZMO_H_
#define _IGDEGIZMO_H_


#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/world/deWorld.h>

class igdeEnvironment;
class deModel;
class deRig;
class decCollisionFilter;
class deCollisionInfo;
class deCollider;


/**
 * \brief Interactive 3D gizmo.
 */
class DE_DLL_EXPORT igdeGizmo : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeGizmo> Ref;
	
	
	
private:
	igdeEnvironment &pEnvironment;
	
	deWorld::Ref pWorld;
	deDebugDrawer::Ref pDebugDrawer;
	deColliderVolume::Ref pCollider;
	
	decStringList pShapeNames;
	decStringList pRigShapeNames;
	float pEdgeTransparency;
	float pFillTransparency;
	
	bool pIsEditing;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create gizmo. */
	igdeGizmo( igdeEnvironment &environment );
	
	
	
protected:
	/**
	 * \brief Clean up gizmo.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeGizmo() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief World or nullptr. */
	inline const deWorld::Ref &GetWorld() const{ return pWorld; }
	
	/** \brief Set world or nullptr. */
	void SetWorld( deWorld *world );
	
	/**
	 * \brief Set gizmo shape from model.
	 * 
	 * Each material is assigned an individual debug drawer shape. Stores the list of
	 * material names with the list index matching the debug drawer shape index.
	 */
	void SetShapeFromModel( const deModel &model );
	
	/** \brief List of shape names. */
	inline const decStringList &GetShapeNames() const{ return pShapeNames; }
	
	/** \brief Named shape is present. */
	inline bool HasShapeNamed( const char *name ) const{ return pShapeNames.Has( name ); }
	
	/** \brief List of rig shape names. */
	inline const decStringList &GetRigShapeNames() const{ return pRigShapeNames; }
	
	/**
	 * \brief Name of rig shape.
	 * 
	 * If index is out of range returns empty string.
	 */
	decString GetRigShapeName( int rigShapeIndex ) const;
	
	/**
	 * \brief Set color of named shape.
	 * 
	 * If named shape is absent nothing is done.
	 */
	void SetShapeColor( const char *name, const decColor &color );
	
	/** \brief Set rig or nullptr. */
	void SetRig( deRig *rig );
	
	/** \brief Collision filter. */
	const decCollisionFilter &GetCollisionFilter() const;
	
	/** \brief Set collision filter. */
	void SetCollisionFilter( const decCollisionFilter &filter );
	
	/** \brief Set collider user pointer. */
	void SetColliderUserPointer( void *userPointer );
	
	/** \brief Gizmo is visible. */
	bool GetVisible() const;
	
	/** \brief Set if gizmo is visible. */
	void SetVisible( bool visible );
	
	/** \brief Position. */
	const decDVector &GetPosition() const;
	
	/** \brief Set position. */
	void SetPosition( const decDVector &position );
	
	/** \brief Orientation. */
	const decQuaternion &GetOrientation() const;
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Scale. */
	const decVector &GetScale() const;
	
	/** \brief Set scale. */
	void SetScale( const decVector &scale );
	
	/** \brief Set position and orientation. */
	void SetGeometry( const decDVector &position, const decQuaternion &orientation );
	void SetGeometry( const decDMatrix &matrix );
	
	/** \brief Gizmo matrix without scaling. */
	decDMatrix GetMatrix() const;
	/*@}*/
	
	
	
	/** \name Interaction */
	/*@{*/
	/**
	 * \brief Start editing.
	 * 
	 * If gizmo started editing true is returned. If editing can not be started false is returned.
	 * 
	 * \param[in] rayOrigin Origin of ray hitting gizmo.
	 * \param[in] rayDirection Direction of ray (including length) hitting gizmo.
	 * \param[in] viewMatrix View oriented matrix.
	 * \param[in] distance Distance from 0 to 1 along rayDirection up to hit point.
	 * \param[in] shape Index of shape hit by ray or -1.
	 */
	bool StartEditing( const decDVector &rayOrigin, const decDVector &rayDirection,
		const decDMatrix &viewMatrix, double distance, int shape );
	
	/**
	 * \brief Update editing.
	 * 
	 * Called if ray position or direction changed between StartEditing() returning
	 * true and StopEditing().
	 * 
	 * \param[in] rayOrigin Origin of ray.
	 * \param[in] rayDirection Normalized direction of ray.
	 * \param[in] viewMatrix View oriented matrix.
	 */
	void UpdateEditing( const decDVector &rayOrigin, const decDVector &rayDirection,
		const decDMatrix &viewMatrix );
	
	/**
	 * \brief Frame update while editing.
	 * 
	 * Called between StartEditing() returning true and StopEditing() on frame update.
	 * 
	 * \param[in] elapsed Elapsed time.
	 */
	void OnFrameUpdate( float elapsed );
	
	/**
	 * \brief Stop editing.
	 * 
	 * Called if StartEditing() returned true.
	 */
	void StopEditing();
	
	/**
	 * \brief Gizmo is editing.
	 * 
	 * Returns true between StartEditing() returning true and StopEditing() otherwise false.
	 */
	inline bool IsEditing() const{ return pIsEditing; }
	/*@}*/
	
	
	
protected:
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
		const decDMatrix &viewMatrix, const decDVector &hitPoint, const decString &shapeName ) = 0;
	
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
		const decDMatrix &viewMatrix );
	
	/**
	 * \brief Update editing.
	 * 
	 * Called between StartEditing() and StopEditing() on frame update.
	 * 
	 * \param[in] elapsed Elapsed time.
	 */
	virtual void OnFrameUpdateEditing( float elapsed );
	
	/**
	 * \brief Stop editing.
	 * 
	 * Called by StopEditing() after a prior to an OnStartEditing() call returning true.
	 */
	virtual void OnStopEditing();
};

#endif
