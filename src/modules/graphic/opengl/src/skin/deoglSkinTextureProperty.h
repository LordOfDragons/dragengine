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

#ifndef _DEOGLSKINTEXTUREPROPERTY_H_
#define _DEOGLSKINTEXTUREPROPERTY_H_

#include <dragengine/common/math/decMath.h>

class deoglRDynamicSkin;
class deoglSkinState;

class deVideo;



/**
 * Skin texture property.
 */
class deoglSkinTextureProperty{
private:
	int pRenderable;
	int pCalculatedProperty;
	int pConstructedProperty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin texture property. */
	deoglSkinTextureProperty();
	
	/** Clean up skin texture property. */
	~deoglSkinTextureProperty();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Renderable index or -1. */
	inline int GetRenderable() const{ return pRenderable; }
	
	/** Set renderable index or -1. */
	void SetRenderable( int index );
	
	/** Calculated property index or -1. */
	inline int GetCalculatedProperty() const{ return pCalculatedProperty; }
	
	/** Set calculated property index or -1. */
	void SetCalculatedProperty( int index );
	
	/** Constructed property index or -1. */
	inline int GetConstructedProperty() const{ return pConstructedProperty; }
	
	/** Set constructed property index or -1. */
	void SetConstructedProperty( int index );
	
	/** Property is dynamic. */
	bool IsDynamic() const;
	
	/**
	 * Resolve as float value.
	 * 
	 * If the renderable in the dynamic skin is not a value renderable the defaultValue
	 * is returned.
	 */
	float ResolveAsFloat( const deoglSkinState *skinState,
		const deoglRDynamicSkin *dynamicSkin, float defaultValue ) const;
	
	/**
	 * Resolve as boolean value.
	 * 
	 * If the renderable in the dynamic skin is not a value renderable the defaultValue
	 * is returned. True is returned if the value is larger than 0.5 and false otherwise.
	 */
	bool ResolveAsBool( const deoglSkinState *skinState,
		const deoglRDynamicSkin *dynamicSkin, bool defaultValue ) const;
	
	/**
	 * Resolve as color.
	 * 
	 * If the renderable in the dynamic skin is not a value or color renderable the
	 * defaultValue is returned. If renderable is a value renderable the returned
	 * color is (value,value,value,1).
	 */
	decColor ResolveColor( const deoglSkinState *skinState,
		const deoglRDynamicSkin *dynamicSkin, const decColor &defaultValue ) const;
	
	/**
	 * Resolve as vector2.
	 * 
	 * If the renderable in the dynamic skin is not a value or color renderable the
	 * defaultValue is returned. If the renderable is a value renderable the returned
	 * vector is (value,value). If renderable is a color renderable the return vector
	 * is (red,green).
	 */
	decVector2 ResolveVector2( const deoglSkinState *skinState,
		const deoglRDynamicSkin *dynamicSkin, const decVector2 &defaultValue ) const;
	
	/**
	 * Resolve as vector.
	 * 
	 * If the renderable in the dynamic skin is not a value or color renderable the
	 * defaultValue is returned. If the renderable is a value renderable the returned
	 * vector is (value,value). If renderable is a color renderable the return vector
	 * is (red,green).
	 */
	decVector ResolveVector( const deoglSkinState *skinState,
		const deoglRDynamicSkin *dynamicSkin, const decVector &defaultValue ) const;
	/*@}*/
};

#endif
