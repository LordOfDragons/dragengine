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

#ifndef _GDESKINLIST_H_
#define _GDESKINLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeSkin;



/**
 * \brief Skin list.
 */
class gdeSkinList{
private:
	decObjectOrderedSet pSkins;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create skin list. */
	gdeSkinList();
	
	/** \brief Create copy of skin list. */
	gdeSkinList( const gdeSkinList &list );
	
	/** \brief Clean up skin list. */
	~gdeSkinList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of skins. */
	int GetCount() const;
	
	/** \brief Skin at position. */
	gdeSkin *GetAt( int index ) const;
	
	/** \brief Skin with path or \em NULL if absent. */
	gdeSkin *GetWithPath( const char *path ) const;
	
	/** \brief Index of skin or -1 if absent. */
	int IndexOf( gdeSkin *skin ) const;
	
	/** \brief Index of skin with path or -1 if absent. */
	int IndexOfWithPath( const char *path ) const;
	
	/** \brief Skin is present. */
	bool Has( gdeSkin *skin ) const;
	
	/** \brief Skin with path is present. */
	bool HasWithPath( const char *path ) const;
	
	/** \brief Add skin. */
	void Add( gdeSkin *skin );
	
	/** \brief Remove skin. */
	void Remove( gdeSkin *skin );
	
	/** \brief Remove all skins. */
	void RemoveAll();
	
	/** \brief Set from another skin list. */
	gdeSkinList &operator=( const gdeSkinList &list );
	/*@}*/
};

#endif
