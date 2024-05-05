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

#ifndef _IGDEGDCSNAPPOINTLIST_H_
#define _IGDEGDCSNAPPOINTLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDCSnapPoint;



/**
 * \brief Object class snap point list.
 */
class DE_DLL_EXPORT igdeGDCSnapPointList{
private:
	decObjectOrderedSet pSnapPoints;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object class snap point list. */
	igdeGDCSnapPointList();
	
	/** \brief Create copy of object class snap point list. */
	igdeGDCSnapPointList( const igdeGDCSnapPointList &list );
	
	/** \brief Clean up object class snap point list. */
	~igdeGDCSnapPointList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of object class snap points. */
	int GetCount() const;
	
	/** \brief Object class snapPoint at position. */
	igdeGDCSnapPoint *GetAt( int index ) const;
	
	/** \brief Index of named object class snap point or -1 if absent. */
	int IndexOf( igdeGDCSnapPoint *snapPoint ) const;
	
	/** \brief Object class snap point is present. */
	bool Has( igdeGDCSnapPoint *snapPoint ) const;
	
	/** \brief Add object class snap point. */
	void Add( igdeGDCSnapPoint *snapPoint );
	
	/** \brief Remove object class snap point. */
	void Remove( igdeGDCSnapPoint *snapPoint );
	
	/** \brief Remove all object class snap points. */
	void RemoveAll();
	
	/** \brief Set from another object class snap point list. */
	igdeGDCSnapPointList &operator=( const igdeGDCSnapPointList &list );
	/*@}*/
};

#endif
