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

#ifndef _CETEXTBOXTEXTLIST_H_
#define _CETEXTBOXTEXTLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceTextBoxText;



/**
 * \brief Text Box Text List.
 */
class ceTextBoxTextList{
private:
	decObjectOrderedSet pTexts;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new text list. */
	ceTextBoxTextList();
	/** Cleans up the text list. */
	~ceTextBoxTextList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of texts. */
	int GetCount() const;
	/** Retrieves the text at the given position. */
	ceTextBoxText *GetAt( int index ) const;
	/** Retrieves the index of the given text or -1 if not found. */
	int IndexOf( ceTextBoxText *text ) const;
	/** Determines if a text exists. */
	bool Has( ceTextBoxText *text ) const;
	/** Adds a text. */
	void Add( ceTextBoxText *text );
	/** Inserts a new text. */
	void InsertAt( ceTextBoxText *text, int index );
	/** Moves a text to a new position. */
	void MoveTo( ceTextBoxText *text, int index );
	/** Removes a text. */
	void Remove( ceTextBoxText *text );
	/** Removes all texts. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	ceTextBoxTextList &operator=( const ceTextBoxTextList &list );
	/*@}*/
};

#endif
