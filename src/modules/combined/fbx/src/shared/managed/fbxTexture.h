/* 
 * FBX Modules
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _FBXTEXTURE_H_
#define _FBXTEXTURE_H_


#include <stdint.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>


class fbxNode;
class fbxScene;

class deBaseModule;


/**
 * FBX managed texture.
 */
class fbxTexture : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<fbxTexture> Ref;
	
	
	
private:
	fbxScene &pScene;
	fbxNode &pNodeTexture;
	
	int64_t pTextureID;
	decString pName;
	decString pUVSet;
	bool pUseMipMap;
	decString pFilename;
	decString pRelativeFilename;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create connection. */
	fbxTexture( fbxScene &scene, fbxNode &nodeTexture );
	
protected:
	/** Clean up connection. */
	virtual ~fbxTexture();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Scene. */
	inline fbxScene &GetScene() const{ return pScene; }
	
	/** Texture node. */
	inline fbxNode &GetNodeTexture() const{ return pNodeTexture; }
	
	
	
	/** Texture ID. */
	inline int64_t GetTextureID() const{ return pTextureID; }
	
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Set name. */
	void SetName( const char *name );
	
	/** UV Set. */
	inline const decString &GetUVSet() const{ return pUVSet; }
	
	/** Use Mip Map. */
	inline bool GetUseMipMap() const{ return pUseMipMap; }
	
	/** Filename. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	/** Relative filename. */
	inline const decString &GetRelativeFilename() const{ return pRelativeFilename; }
	
	
	
	/** Debug print node structure. */
	void DebugPrintStructure( deBaseModule &module, const decString &prefix, bool verbose = false ) const;
	/*@}*/
};

#endif
