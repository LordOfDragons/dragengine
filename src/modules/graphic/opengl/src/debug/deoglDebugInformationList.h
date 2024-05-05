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

#ifndef _DEOGLDEBUGINFORMATIONLIST_H_
#define _DEOGLDEBUGINFORMATIONLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class deoglDebugInformation;



/**
 * List of debug information for performance profiling.
 */
class deoglDebugInformationList{
private:
	decObjectOrderedSet pChildren;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create debug information list. */
	deoglDebugInformationList();
	
	/** Clean up debug information list. */
	virtual ~deoglDebugInformationList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Number of children. */
	int GetCount() const;
	
	/** Child at index. */
	deoglDebugInformation *GetAt( int index ) const;
	
	/** Add child. */
	void Add( deoglDebugInformation *child );
	
	/** Remove child. */
	void Remove( deoglDebugInformation *child );
	
	/** Remove child if present. */
	void RemoveIfPresent( deoglDebugInformation *child );
	
	/** Remove all children. */
	void RemoveAll();
	/*@}*/
};

#endif
