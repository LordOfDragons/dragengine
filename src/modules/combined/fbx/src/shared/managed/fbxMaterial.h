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
