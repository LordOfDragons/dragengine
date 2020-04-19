/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLSKINSTATECALCULATED_H_
#define _DEOGLSKINSTATECALCULATED_H_

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglTexture;
class deoglSkinState;
class deoglRComponent;
class deoglSkinCalculatedProperty;

class deComponent;


/**
 * \brief Skin State Calculated.
 * 
 * Stores the calculated property state.
 */
class deoglSkinStateCalculated{
private:
	decColor pColor;
	deoglTexture *pTexture;
	deoglSkinCalculatedProperty *pProperty;
	const deoglRComponent *pComponent;
	int pBone[ 4 ];
	float pBoneInputValue[ 4 ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create skin state calculated. */
	deoglSkinStateCalculated();
	
	/** \brief Clean up skin state calculated. */
	~deoglSkinStateCalculated();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Calculated color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set calculated color. */
	void SetColor( const decColor &color );
	
	/** \brief Temporary texture or NULL if not existing. */
	inline deoglTexture *GetTexture() const{ return pTexture; }
	
	/** \brief Set temporary texture or NULL if not existing. */
	void SetTexture( deoglTexture *texture );
	
	/** \brief Skin calculated property. */
	deoglSkinCalculatedProperty *GetProperty() const{ return pProperty; }
	
	/** \brief Set skin calculated property. */
	void SetProperty( deoglSkinCalculatedProperty *property );
	
	/** \brief Component for bone calculate or NULL. */
	inline const deoglRComponent *GetComponent() const{ return pComponent; }
	
	/** \brief Set component for bone calculate or NULL. */
	void SetComponent( const deoglRComponent *component );
	
	/** \brief Bone index or -1 if not found. */
	int GetBone( int component ) const;
	
	/** \brief Bone input value. */
	float GetBoneInputValue( int component ) const;
	
	/** \brief Map bones. */
	void MapBones( const deComponent &component );
	
	/** \brief Update bones. */
	void UpdateBones( const deComponent &component );
	
	/** \brief Update. */
	void Update( deoglSkinState &skinState );
	/*@}*/
};

#endif
