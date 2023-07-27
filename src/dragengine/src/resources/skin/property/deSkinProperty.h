/* 
 * Drag[en]gine Game Engine
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

#ifndef _DESKINPROPERTY_H_
#define _DESKINPROPERTY_H_

#include "../../../common/string/decString.h"

class deSkinPropertyVisitor;


/**
 * \brief Skin Texture Property.
 *
 * Defines a property of a skin texture. To allow adding any kind of new
 * material properties without changing the engine code or other modules
 * a simple name based system is used. Depending on the name of a property
 * graphic modules can apply the property to different material properties
 * internally.
 * 
 * \par
 * A property is defined as mentioned by a name. To avoid storing a
 * large number of names only a number is used to determine the type of
 * a texture property. To obtain this number add the name of the property
 * to the deTexturePropertyMap object obtainable from the game engine.
 * 
 * \par
 * Properties can have different types of values depending on their
 * usage. Simple properties contain a static color or an entire static
 * image. Using a color instead of an image is allowed and equals to an
 * image composing of only this color. The other way is not allowed and
 * results in undefined behavior. The static color and image can be used
 * both either as gray-scale ( in which case only the red component is
 * used ), rgb ( in which case the red, green and blue component are used )
 * or rgba ( in which case all components are used ). Hence it's possible
 * to define up to four parameters for one property.
 * 
 * \par
 * There exists a special case and this are renderables. If a property
 * has a renderable name set its content can be changed dynamically during
 * runtime using renderables in components. This is not considered dynamic
 * since the content is provided by the game script and not produced on
 * the fly.
 * 
 * \par
 * For dynamic skins there exist the way to specify a source as the
 * value of a property. Such a sure describes the production process of
 * a dynamic texture. Dynamic skins tend to be slower and consuming more
 * resources than static skins composed of simple properties only. Using
 * a transformation on the texture itself does not make a texture complex
 * and is fast.
 * 
 * \par
 * Properties can be interprated by one or more modules. So far the
 * graphic, audio and physics module can use texture properties.
 * 
 * \par
 * In addition a bone name can be set. This is used by texture properties using vertex position
 * instead of texture coordinates as input. The vertex position is evaluated relative to the
 * bone coordinate system. Texture properties not using vertex position ignore this parameter.
 */
class DE_DLL_EXPORT deSkinProperty{
private:
	decString pType;
	decString pTexCoordSet;
	decString pRenderable;
	decString pBone;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create new named skin property with the given type.
	 * \param type Type of the skin property obtained from adding a property
	 * name to the texture property map object held by the engine.
	 */
	deSkinProperty( const char *type );
	
	/** \brief Clean up skin property. */
	virtual ~deSkinProperty();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Type. */
	inline const decString &GetType() const{ return pType; }
	
	/** \brief Texture coordinate set name or an empty string to use the default one. */
	inline const decString &GetTexCoordSet() const{ return pTexCoordSet; }
	
	/** \brief Set texture coordinate set name or an empty string to use the default one. */
	void SetTexCoordSet( const char *name );
	
	/** \brief Renderable name or empty string to use none. */
	inline const decString &GetRenderable() const{ return pRenderable; }
	
	/** \brief Set renderable name or empty string to use none. */
	void SetRenderable( const char *renderable );
	
	/** \brief Bone name or empty string. */
	inline const decString &GetBone() const{ return pBone; }
	
	/** \brief Set bone name or empty string. */
	void SetBone( const char *bone );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit property. */
	virtual void Visit( deSkinPropertyVisitor &visitor );
	/*@}*/
};

#endif
