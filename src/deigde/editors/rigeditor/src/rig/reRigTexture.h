/* 
 * Drag[en]gine IGDE Rig Editor
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

// include only once
#ifndef _RERIGTEXTURE_H_
#define _RERIGTEXTURE_H_

// includes
#include <dragengine/common/string/decString.h>

#include <dragengine/common/math/decMath.h>

// predefinitions
class deEngine;
class deSkin;
class reRig;



/**
 * @brief Rig Texture.
 * Defines a texture for use with the loaded component.
 */
class reRigTexture{
private:
	deEngine *pEngine;
	
	decString pName;
	decString pSkinPath;
	deSkin *pEngSkin;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rig texture. */
	reRigTexture( deEngine *engine, const char *name );
	/** Cleans up the rig texture. */
	~reRigTexture();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the game engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	
	/** Retrieves the name of the texture. */
	inline const decString &GetName() const{ return pName; }
	/** Retrieves the skin file path. */
	inline const decString &GetSkinPath() const{ return pSkinPath; }
	/** Sets the skin file path. */
	void SetSkinPath( const char *skinPath );
	/** Loads the skin. */
	void LoadSkin();
	/** Retrieves the engine skin or NULL if not found. */
	inline deSkin *GetEngineSkin() const{ return pEngSkin; }
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
