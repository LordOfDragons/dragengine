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

#ifndef _SEBODYLIST_H_
#define _SEBODYLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class seBody;



/**
 * \brief Sky body list.
 */
class seBodyList{
private:
	decObjectOrderedSet pBodies;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sky body list. */
	seBodyList();
	
	/** \brief Clean up sky body list. */
	~seBodyList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of bodies. */
	int GetCount() const;
	
	/** \brief Body at index. */
	seBody *GetAt( int index ) const;
	
	/** \brief Index of given body or -1 if absent. */
	int IndexOf( seBody *body ) const;
	
	/** \brief Body is present. */
	bool Has( seBody *body ) const;
	
	/** \brief Add body. */
	void Add( seBody *body );
	
	/** \brief Insert body. */
	void InsertAt( seBody *body, int index );
	
	/** \brief Move body. */
	void MoveTo( seBody *body, int index );
	
	/** \brief Remove body. */
	void Remove( seBody *body );
	
	/** \brief Remove all bodies. */
	void RemoveAll();
	
	/** \brief Set list from another list. */
	seBodyList &operator=( const seBodyList &list );
	/*@}*/
};

#endif
