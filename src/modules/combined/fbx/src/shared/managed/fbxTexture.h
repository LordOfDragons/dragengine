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
	fbxTexture(fbxScene &scene, fbxNode &nodeTexture);
	
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
	void SetName(const char *name);
	
	/** UV Set. */
	inline const decString &GetUVSet() const{ return pUVSet; }
	
	/** Use Mip Map. */
	inline bool GetUseMipMap() const{ return pUseMipMap; }
	
	/** Filename. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	/** Relative filename. */
	inline const decString &GetRelativeFilename() const{ return pRelativeFilename; }
	
	
	
	/** Debug print node structure. */
	void DebugPrintStructure(deBaseModule &module, const decString &prefix, bool verbose = false) const;
	/*@}*/
};

#endif
