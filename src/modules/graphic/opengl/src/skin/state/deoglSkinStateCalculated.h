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
class deoglSkinCalculatedProperty;


/** Skin state calculated. */
class deoglSkinStateCalculated{
private:
	decColor pColor;
	deoglTexture *pTexture;
	deoglSkinCalculatedProperty *pProperty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin state calculated. */
	deoglSkinStateCalculated();
	
	/** Clean up skin state calculated. */
	~deoglSkinStateCalculated();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Calculated color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** Set calculated color. */
	void SetColor( const decColor &color );
	
	/** Temporary texture or nullptr if not existing. */
	inline deoglTexture *GetTexture() const{ return pTexture; }
	
	/** Set temporary texture or nullptr if not existing. */
	void SetTexture( deoglTexture *texture );
	
	/** Skin calculated property. */
	deoglSkinCalculatedProperty *GetProperty() const{ return pProperty; }
	
	/** Set skin calculated property. */
	void SetProperty( deoglSkinCalculatedProperty *property );
	
	/** Update. */
	void Update( deoglSkinState &skinState );
	/*@}*/
};

#endif
