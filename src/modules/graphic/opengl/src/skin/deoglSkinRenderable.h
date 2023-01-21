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

#ifndef _DEOGLSKINRENDERABLE_H_
#define _DEOGLSKINRENDERABLE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>



/**
 * Skin renderable usage information.
 */
class deoglSkinRenderable : public deObject{
private:
	decString pName;
	bool pRequiresTexture;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new skin renderable. */
	deoglSkinRenderable( const char *name );
	
protected:
	/** Cleans up the skin renderable. */
	virtual ~deoglSkinRenderable();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Renderable name. */
	inline const decString &GetName() const{ return pName; }
	
	/** One or more texture properties using this renderable require a texture. */
	inline bool GetRequiresTexture() const{ return pRequiresTexture; }
	
	/** Set if one or more texture properties using this renderable require a texture. */
	void SetRequiresTexture( bool requiresTexture );
	/*@}*/
};

#endif
