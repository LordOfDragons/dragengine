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

#ifndef _MECDOOBJECT_H_
#define _MECDOOBJECT_H_

#include "../world/object/texture/meObjectTexture.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringDictionary.h>
#include <dragengine/common/string/decStringList.h>

class meObject;


/**
 * Object data for clipboard clips.
 * 
 * Stores information about an object suitable for clipboard clips.
 */
class meCDOObject : public deObject{
public:
	typedef deTObjectReference<meCDOObject> Ref;
	typedef decTObjectOrderedSet<meCDOObject> List;
	
	
private:
	decString pClassname;
	decDVector pPosition;
	decVector pRotation;
	decVector pScale;
	decStringDictionary pProperties;
	decStringList pAttachBehaviors;
	meObjectTexture::List pTextures;
	int pAttachToIndex;
	decString pAttachToID;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create object clip data. */
	explicit meCDOObject(const meObject &object);
	
protected:
	/** Clean up object data object. */
	~meCDOObject();
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Classname. */
	inline const decString &GetClassname() const{ return pClassname; }
	
	/** Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** Rotation. */
	inline const decVector &GetRotation() const{ return pRotation; }
	
	/** Scale. */
	inline const decVector &GetScale() const{ return pScale; }
	
	/** Properties. */
	inline const decStringDictionary &GetProperties() const{ return pProperties; }
	
	/** Textures. */
	inline const meObjectTexture::List &GetTextures() const{ return pTextures; }
	
	/** Attach to index or -1 if not attached to an object. */
	inline int GetAttachToIndex() const{ return pAttachToIndex; }
	
	/** Set attach to index or -1 if not attached to an object. */
	void SetAttachToIndex(int index);
	
	/** Attach to ID in hex string format or empty string  if not attached. */
	inline const decString &GetAttachToID() const{ return pAttachToID; }
	
	/** Set attach to ID in hex string format or empty string  if not attached. */
	void SetAttachToID(const char *id);
	
	/**
	 * Update object with stored data.
	 * \warning Call this only on a newly created object.
	 */
	void UpdateObject(meObject &object) const;
	/*@}*/
};

#endif
