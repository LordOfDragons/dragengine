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

#ifndef _IGDEGIZMOMANAGER_H_
#define _IGDEGIZMOMANAGER_H_

#include "igdeGizmo.h"

class deWorld;


/**
 * \brief IGDE interactive 3D gizmo manager.
 * 
 * Simplifies handling interactive 3D gizmos. Typically an instance of this class is used
 * in a igdeViewRenderWindow subclass as member. Upon receiving mouse events the subclass
 * can call into this class if gizmo handling is allowed.
 */
class DE_DLL_EXPORT igdeGizmoManager{
private:
	igdeGizmo::Ref pEditingGizmo;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create gizmo manager. */
	igdeGizmoManager();
	
	/** \brief Clean up gizmo manager. */
	~igdeGizmoManager();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/**
	 * \brief Start editing gizmo.
	 * 
	 * If editing started true is returned. Caller has to grab input on affected widget
	 * or otherwise ensure user can drag the mouse to send UpdateEditing() accordingly.
	 * 
	 * \param[in] rayOrigin Origin of ray hitting gizmo.
	 * \param[in] rayDirection Direction of ray (including length) hitting gizmo.
	 * \param[in] viewMatrix View oriented matrix.
	 * \param[in] distance Distance from 0 to 1 along rayDirection up to hit point.
	 * \param[in] shape Index of shape hit by ray or -1.
	 */
	bool StartEditing( igdeGizmo *gizmo, const decDVector &rayOrigin,
		const decDVector &rayDirection, const decDMatrix &viewMatrix, double distance, int shape );
	
	/** \brief Editing gizmo or nullptr. */
	inline const igdeGizmo::Ref &GetEditingGizmo() const{ return pEditingGizmo; }
	
	/**
	 * \brief Update editing.
	 * 
	 * Call if mouse moved and provide new ray position and direction for editing gizmo
	 * to update editing state. If a gizmo is editing it is updated otherwise nothing is done.
	 * 
	 * \param[in] rayOrigin Origin of ray.
	 * \param[in] rayDirection Normalized direction of ray.
	 * \param[in] viewMatrix View oriented matrix.
	 */
	void UpdateEditing( const decDVector &rayOrigin, const decDVector &rayDirection,
		const decDMatrix &viewMatrix );
	
	/**
	 * \brief Frame update.
	 * 
	 * Call this always on igdeWidget::OnFrameUpdate(). If a gizmo is editing it is updated.
	 * 
	 * \param[in] elapsed Elapsed time.
	 */
	void OnFrameUpdate( float elapsed );
	
	/** \brief Stop editing gizmo if one is editing right now. */
	void StopEditing();
	/*@}*/
};

#endif
