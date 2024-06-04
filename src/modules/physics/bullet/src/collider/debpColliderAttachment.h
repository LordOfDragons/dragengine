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
	int *pVpsMappings;
	int pVpsMappingCount;
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
	
	/** Set the number of vertex position set mappings. */
	void SetVpsMappingCount( int count );
	
	/** Vertex position set mapping by index. */
	int GetVpsMappingAt( int index ) const;
	
	/** Set vertex position set mapping by index. */
	void SetVpsMappingAt( int index, int boneIndex );
	
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
	void Reposition( const decDMatrix &matrix, const decVector &velocity, bool changeNotify );
	
	/**
	 * Reposition attached resource using a matrix modified by the local matrix.
	 * \details If the resource is a collider and the geometry change a ColliderChanged
	 *          notification will be send if changeNotify is true.
	 */
	void Reposition( const decDVector &position, const decQuaternion &orientation,
		const decDVector &scaling, const decVector &velocity, bool changeNotify );
	
	/**
	 * Transform attached resource using a relative matrix.
	 * \details If the resource is a collider and the geometry change a ColliderChanged
	 *          notification will be send if changeNotify is true.
	 */
	void Transform( const decDMatrix &matrix, const decVector &velocity, bool changeNotify );
	/*@}*/
	
private:
	void pCleanUp();
	void pPrepareLocalMatrix();
	void pRepositionResource( const decDVector &position, const decQuaternion &orientation,
		const decDVector &scaling, const decVector &velocity, bool changeNotify );
};

#endif
