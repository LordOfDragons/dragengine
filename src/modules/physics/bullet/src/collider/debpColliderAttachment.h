/* 
 * Drag[en]gine Bullet Physics Module
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

#ifndef _DEBPCOLLIDERATTACHMENT_H_
#define _DEBPCOLLIDERATTACHMENT_H_

#include <dragengine/common/math/decMath.h>

class deColliderAttachment;


/**
 * \brief Collider attachment working object.
 */
class debpColliderAttachment{
private:
	deColliderAttachment *pAttachment;
	int pTrackBone;
	int *pBoneMappings;
	int pBoneMappingCount;
	bool pDirtyMappings;
	
	decDMatrix pLocalMatrix;
	bool pDirtyLocalMatrix;
	bool pHasLocalMatrix;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a collider attachment. */
	debpColliderAttachment( deColliderAttachment *attachment );
	
	/** \brief Clean up the collider attachment. */
	~debpColliderAttachment();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieve the collider attachment. */
	inline deColliderAttachment *GetAttachment() const{ return pAttachment; }
	
	/** \brief Track bone index. */
	inline int GetTrackBone() const{ return pTrackBone; }
	
	/** \brief Set the track bone index. */
	void SetTrackBone( int boneIndex );
	
	/** \brief Set the number of bone mappings. */
	void SetBoneMappingCount( int count );
	
	/** \brief Bone mapping by index. */
	int GetBoneMappingAt( int index ) const;
	
	/** \brief Set bone mapping by index. */
	void SetBoneMappingAt( int index, int boneIndex );
	
	/** \brief Determine if mapping is dirty. */
	inline bool GetDirtyMappings() const{ return pDirtyMappings; }
	
	/** \brief Set if mapping is dirty. */
	void SetDirtyMappings( bool dirtyMappings );
	
	
	
	/** \brief Attachment changed. */
	void AttachmentChanged();
	
	
	
	/**
	 * \brief Reposition attached resource using a matrix modified by the local matrix.
	 * \details If the resource is a collider and the geometry change a ColliderChanged
	 *          notification will be send if changeNotify is true.
	 */
	void Reposition( const decDMatrix &matrix, bool changeNotify );
	
	/**
	 * \brief Reposition attached resource using a matrix modified by the local matrix.
	 * \details If the resource is a collider and the geometry change a ColliderChanged
	 *          notification will be send if changeNotify is true.
	 */
	void Reposition( const decDVector &position, const decQuaternion &orientation,
		const decDVector &scaling, bool changeNotify );
	
	/**
	 * \brief Transform attached resource using a relative matrix.
	 * \details If the resource is a collider and the geometry change a ColliderChanged
	 *          notification will be send if changeNotify is true.
	 */
	void Transform( const decDMatrix &matrix, bool changeNotify );
	/*@}*/
	
private:
	void pCleanUp();
	void pPrepareLocalMatrix();
	void pRepositionResource( const decDVector &position, const decQuaternion &orientation,
		const decDVector &scaling, bool changeNotify );
};

#endif
