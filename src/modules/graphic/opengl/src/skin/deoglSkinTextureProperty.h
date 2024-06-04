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
	int pBone;
	
	
	
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
	
	/** Bone index or -1. */
	inline int GetBone() const{ return pBone; }
	
	/** Set bone index or -1. */
	void SetBone( int index );
	
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
	
	/** Resolve as matrix. */
	decMatrix ResolveMatrix( const deoglSkinState *skinState, const decMatrix &defaultValue ) const;
	/*@}*/
};

#endif
