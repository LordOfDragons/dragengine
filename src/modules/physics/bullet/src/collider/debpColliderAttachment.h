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
 * Collider attachment working object.
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
	
	decDMatrix pAccumRelMoveMatrix;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a collider attachment. */
	debpColliderAttachment( deColliderAttachment *attachment );
	
	/** Clean up the collider attachment. */
	~debpColliderAttachment();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieve the collider attachment. */
	inline deColliderAttachment *GetAttachment() const{ return pAttachment; }
	
	/** Track bone index. */
	inline int GetTrackBone() const{ return pTrackBone; }
	
	/** Set the track bone index. */
	void SetTrackBone( int boneIndex );
	
	/** Set the number of bone mappings. */
	void SetBoneMappingCount( int count );
	
	/** Bone mapping by index. */
	int GetBoneMappingAt( int index ) const;
	
	/** Set bone mapping by index. */
	void SetBoneMappingAt( int index, int boneIndex );
	
	/** Determine if mapping is dirty. */
	inline bool GetDirtyMappings() const{ return pDirtyMappings; }
	
	/** Set if mapping is dirty. */
	void SetDirtyMappings( bool dirtyMappings );
	
	/** Relative movement accumulation. */
	inline const decDMatrix &GetAccumRelMoveMatrix() const{ return pAccumRelMoveMatrix; }
	
	/** Set relative movement accumulation. */
	void SetAccumRelMoveMatrix( const decDMatrix &matrix );
	
	
	
	/** Attachment changed. */
	void AttachmentChanged();
	
	
	
	/**
	 * Reposition attached resource using a matrix modified by the local matrix.
	 * \details If the resource is a collider and the geometry change a ColliderChanged
	 *          notification will be send if changeNotify is true.
	 */
	void Reposition( const decDMatrix &matrix, bool changeNotify );
	
	/**
	 * Reposition attached resource using a matrix modified by the local matrix.
	 * \details If the resource is a collider and the geometry change a ColliderChanged
	 *          notification will be send if changeNotify is true.
	 */
	void Reposition( const decDVector &position, const decQuaternion &orientation,
		const decDVector &scaling, bool changeNotify );
	
	/**
	 * Transform attached resource using a relative matrix.
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
