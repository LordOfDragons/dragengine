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

#ifndef _SEMAPPEDLIST_H_
#define _SEMAPPEDLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class seMapped;



/**
 * Mapped list.
 */
class seMappedList{
private:
	decObjectOrderedSet pMapped;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create mapped list. */
	seMappedList();
	
	/** Cleans up the mapped list. */
	~seMappedList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Count of mapped. */
	int GetCount() const;
	
	/** Mapped at index. */
	seMapped *GetAt( int index ) const;
	
	/** Named mapped or nullptr if absent. */
	seMapped *GetNamed( const char *name ) const;
	
	/** Index of mapped or -1 if absent. */
	int IndexOf( seMapped *mapped ) const;
	
	/** Index of named mapped or -1 if absent. */
	int IndexOfNamed( const char *name ) const;
	
	/** Mapped is present. */
	bool Has( seMapped *mapped ) const;
	
	/** Named mapped is present. */
	bool HasNamed( const char *name ) const;
	
	/** Add mapped. */
	void Add( seMapped *mapped );
	
	/** Remove mapped. */
	void Remove( seMapped *mapped );
	
	/** Remove all mapped. */
	void RemoveAll();
	
	/** Set list from another list. */
	seMappedList &operator=( const seMappedList &list );
	/*@}*/
};

#endif
