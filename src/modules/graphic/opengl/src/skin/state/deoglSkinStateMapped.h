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

#ifndef _DEOGLSKINSTATEMAPPED_H_
#define _DEOGLSKINSTATEMAPPED_H_

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglSkinState;
class deoglRComponent;
class deoglSkinMapped;

class deComponent;


/**
 * Skin mapped state.
 */
class deoglSkinStateMapped{
private:
	float pValue;
	const deoglSkinMapped *pMapped;
	const deoglRComponent *pComponent;
	int pBone;
	float pBoneInputValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin state mapped. */
	deoglSkinStateMapped();
	
	/** Clean up skin state mapped. */
	~deoglSkinStateMapped();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Mapped value. */
	inline float GetValue() const{ return pValue; }
	
	/** Set mapped value. */
	void SetValue( float value );
	
	/** Skin mapped. */
	const deoglSkinMapped *GetMapped() const{ return pMapped; }
	
	/** Set skin mapped. */
	void SetMapped( const deoglSkinMapped *mapped );
	
	/** Component for bone calculate or nullptr. */
	inline const deoglRComponent *GetComponent() const{ return pComponent; }
	
	/** Set component for bone calculate or nullptr. */
	void SetComponent( const deoglRComponent *component );
	
	/** Bone index or -1 if not found. */
	inline int GetBone() const{ return pBone; }
	
	/** Bone input value. */
	inline float GetBoneInputValue() const{ return pBoneInputValue; }
	
	/** Map bone. */
	void MapBone( const deComponent &component );
	
	/** Update bone. */
	void UpdateBone( const deComponent &component );
	
	/** Update. */
	void Update( deoglSkinState &skinState );
	/*@}*/
};

#endif
