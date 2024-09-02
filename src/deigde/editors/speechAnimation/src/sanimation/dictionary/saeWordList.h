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

#ifndef _SAEWORDLIST_H_
#define _SAEWORDLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class saeWord;



/**
 * Speech animation word list.
 */
class saeWordList{
private:
	decObjectOrderedSet pWords;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create word list. */
	saeWordList();
	
	/** Create copy of word list. */
	saeWordList( const saeWordList &list );
	
	/** Clean up word list. */
	~saeWordList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Number of words. */
	int GetCount() const;
	
	/** Word at index. */
	saeWord *GetAt( int index ) const;
	
	/** Named word or NULL if absent. */
	saeWord *GetNamed( const char *name ) const;
	
	/** Index of word or -1 if absent. */
	int IndexOf( saeWord *word ) const;
	
	/** Index of named word or -1 if absent. */
	int IndexOfNamed( const char *name ) const;
	
	/** Word is present. */
	bool Has( saeWord *word ) const;
	
	/** Named word is present. */
	bool HasNamed( const char *name ) const;
	
	/** Add word. */
	void Add( saeWord *word );
	
	/** Remove word. */
	void Remove( saeWord *word );
	
	/** Remove all words. */
	void RemoveAll();
	
	/** Set list. */
	saeWordList &operator=( const saeWordList &list );
	/*@}*/
};

#endif
