/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLSKINSTATEBONE_H_
#define _DEOGLSKINSTATEBONE_H_

#include <dragengine/common/math/decMath.h>

class deoglSkinState;
class deoglSkinBone;

class deComponent;


/**
 * Skin bone state.
 */
class deoglSkinStateBone{
private:
	decMatrix pBoneMatrix;
	const deoglSkinBone *pSkinBone;
	int pBoneIndex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin state bone. */
	deoglSkinStateBone();
	
	/** Clean up skin state bone. */
	~deoglSkinStateBone();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Bone matric. */
	inline const decMatrix &GetBoneMatrix() const{ return pBoneMatrix; }
	
	/** Skin bone. */
	const deoglSkinBone *GetSkinBone() const{ return pSkinBone; }
	
	/** Set skin bone. */
	void SetSkinBone( const deoglSkinBone *skinBone );
	
	/** Bone index or -1 if not found. */
	inline int GetBoneIndex() const{ return pBoneIndex; }
	
	/** Map bone. */
	void MapBone( const deComponent &component );
	
	/** Update bone. */
	void UpdateBone( const deComponent &component );
	/*@}*/
};

#endif
