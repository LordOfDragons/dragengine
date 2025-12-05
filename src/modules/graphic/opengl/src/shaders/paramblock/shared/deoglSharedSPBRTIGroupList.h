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

#ifndef _DEOGLSHAREDSPBRTIGROUPLIST_H_
#define _DEOGLSHAREDSPBRTIGROUPLIST_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decPointerList.h>

class deoglRenderThread;
class deoglSharedSPB;
class deoglSharedSPBRTIGroup;



/**
 * OpenGL shared SPB render task instance group list.
 */
class deoglSharedSPBRTIGroupList : public deObject{
public:
	typedef deTObjectReference<deoglSharedSPBRTIGroupList> Ref;
	
	
private:
	deoglRenderThread &pRenderThread;
	decPointerList pGroups;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shared SPB render task instance group list. */
	deoglSharedSPBRTIGroupList( deoglRenderThread &renderThread );
	
protected:
	/** Clean up shared SPB list. */
	virtual ~deoglSharedSPBRTIGroupList();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Count of groups. */
	int GetCount() const;
	
	/** Group at index. */
	deoglSharedSPBRTIGroup *GetAt( int index ) const;
	
	/**
	 * Group group with shared SPB or NULL if not found. Caller obtains reference to the group.
	 * Release reference if not used anymore. Group is removed from this list once all
	 * references are released.
	 */
	deoglSharedSPBRTIGroup *GetWith( deoglSharedSPB &sharedSPB, int textureCount = 1 ) const;
	
	/**
	 * Group group with shared SPB. If group does not exist creates it first. Caller obtains
	 * reference to the group. Release reference if not used anymore. Group is removed from
	 * this list once all references are released.
	 */
	deoglSharedSPBRTIGroup *GetOrAddWith( deoglSharedSPB &sharedSPB, int textureCount = 1 );
	
	/**
	 * Add group with shared SPB. Call only after GetWith returned NULL. If group does not
	 * exist creates it first. Caller obtains reference to the group. Release reference if
	 * not used anymore. Group is removed from this list once all references are released.
	 */
	deoglSharedSPBRTIGroup *AddWith( deoglSharedSPB &sharedSPB, int textureCount = 1 );
	
	
	
	/**
	 * Remove group.
	 * 
	 * \warning For use by deoglSharedSPBRTIGroup only.
	 */
	void Remove( deoglSharedSPBRTIGroup *group );
	/*@}*/
};

#endif
