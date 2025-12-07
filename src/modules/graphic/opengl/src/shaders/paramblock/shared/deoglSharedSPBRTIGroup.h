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

#ifndef _DEOGLSHAREDSPBRTIGROUP_H_
#define _DEOGLSHAREDSPBRTIGROUP_H_

#include "deoglSharedSPBRTIGroupList.h"

#include <dragengine/deObject.h>

class deoglSharedSPB;
class deoglSharedSPBRTIGroupList;
class deoglRenderTaskSharedInstance;


/**
 * OpenGL shared SPB render task instance group.
 * 
 * Stores a deoglRenderTaskInstanceGroup used to group instances in render tasks.
 * The shared spb is used to find the matching group and is not reference counted.
 * 
 * If the last reference to this object is released the object removes itself from the
 * parent group list.
 */
class deoglSharedSPBRTIGroup : public deObject{
public:
	typedef deTObjectReference<deoglSharedSPBRTIGroup> Ref;
	
	
private:
	const deoglSharedSPBRTIGroupList::Ref pParent;
	deoglSharedSPB &pSharedSPB;
	int pTextureCount;
	deoglRenderTaskSharedInstance *pRTSInstance;
	unsigned int pUniqueKey;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shared SPB render task instance group. */
	deoglSharedSPBRTIGroup(deoglSharedSPBRTIGroupList *parent,
		deoglSharedSPB &sharedSPB, int textureCount);
	
protected:
	/** Clean up shared SPB render task instance group. */
	virtual ~deoglSharedSPBRTIGroup();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Parent list. */
	inline deoglSharedSPBRTIGroupList &GetParent() const{ return pParent; }
	
	/** Shared SPB. */
	inline deoglSharedSPB &GetSharedSPB() const{ return pSharedSPB; }
	
	/** Render task shared instance. */
	inline deoglRenderTaskSharedInstance *GetRTSInstance() const{ return pRTSInstance; }
	
	/** Count of following textures covered by the group. */
	inline int GetTextureCount() const{ return pTextureCount; }
	
	/** Unique key for use with dictionaries. */
	inline unsigned int GetUniqueKey() const{ return pUniqueKey; }
	/*@}*/
};

#endif
