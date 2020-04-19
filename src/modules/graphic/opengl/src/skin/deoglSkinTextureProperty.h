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
 * \brief Skin texture property.
 */
class deoglSkinTextureProperty{
private:
	int pRenderable;
	int pCalculatedProperty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create skin texture property. */
	deoglSkinTextureProperty();
	
	/** \brief Clean up skin texture property. */
	~deoglSkinTextureProperty();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Renderable index or -1. */
	inline int GetRenderable() const{ return pRenderable; }
	
	/** \brief Set renderable index or -1. */
	void SetRenderable( int index );
	
	/** \brief Calculated property index or -1. */
	inline int GetCalculatedProperty() const{ return pCalculatedProperty; }
	
	/** \brief Set calculated property index or -1. */
	void SetCalculatedProperty( int index );
	
	/** \brief Property is dynamic. */
	bool IsDynamic() const;
	
	/**
	 * \brief Resolve as float value.
	 * 
	 * If the renderable in the dynamic skin is not a value renderable the defaultValue
	 * is returned.
	 */
	float ResolveAsFloat( const deoglSkinState *skinState,
		const deoglRDynamicSkin *dynamicSkin, float defaultValue ) const;
	
	/**
	 * \brief Resolve as boolean value.
	 * 
	 * If the renderable in the dynamic skin is not a value renderable the defaultValue
	 * is returned. True is returned if the value is larger than 0.5 and false otherwise.
	 */
	bool ResolveAsBool( const deoglSkinState *skinState,
		const deoglRDynamicSkin *dynamicSkin, bool defaultValue ) const;
	
	/**
	 * \brief Resolve as color.
	 * 
	 * If the renderable in the dynamic skin is not a value or color renderable the
	 * defaultValue is returned. If renderable is a value renderable the returned
	 * color is (value,value,value,1).
	 */
	decColor ResolveColor( const deoglSkinState *skinState,
		const deoglRDynamicSkin *dynamicSkin, const decColor &defaultValue ) const;
	
	/**
	 * \brief Resolve as vector2.
	 * 
	 * If the renderable in the dynamic skin is not a value or color renderable the
	 * defaultValue is returned. If the renderable is a value renderable the returned
	 * vector is (value,value). If renderable is a color renderable the return vector
	 * is (red,green).
	 */
	decVector2 ResolveVector2( const deoglSkinState *skinState,
		const deoglRDynamicSkin *dynamicSkin, const decVector2 &defaultValue ) const;
	
	/**
	 * \brief Resolve as vector.
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
