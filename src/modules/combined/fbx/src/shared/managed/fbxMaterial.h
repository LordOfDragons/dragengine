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

#ifndef _FBXMATERIAL_H_
#define _FBXMATERIAL_H_


#include <stdint.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>


class fbxNode;
class fbxScene;

class deBaseModule;


/**
 * FBX managed rig.
 */
class fbxMaterial : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<fbxMaterial> Ref;
	
	
	
private:
	fbxScene &pScene;
	fbxNode &pNodeMaterial;
	
	int64_t pMaterialID;
	decString pName;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create connection. */
	fbxMaterial( fbxScene &scene, fbxNode &nodeMaterial );
	
protected:
	/** Clean up connection. */
	virtual ~fbxMaterial();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Scene. */
	inline fbxScene &GetScene() const{ return pScene; }
	
	/** Material node. */
	inline fbxNode &GetNodeMaterial() const{ return pNodeMaterial; }
	
	
	
	/** Material ID. */
	inline int64_t GetMaterialID() const{ return pMaterialID; }
	
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Set name. */
	void SetName( const char *name );
	
	
	
	/** Debug print node structure. */
	void DebugPrintStructure( deBaseModule &module, const decString &prefix, bool verbose = false ) const;
	/*@}*/
};

#endif
