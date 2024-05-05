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

#ifndef _IGDEGDCFORCEFIELDLIST_H_
#define _IGDEGDCFORCEFIELDLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDCForceField;



/**
 * \brief Game Definition Class Force Field List.
 */
class DE_DLL_EXPORT igdeGDCForceFieldList{
private:
	decObjectOrderedSet pFields;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeGDCForceFieldList();
	
	/** \brief Create copy of a game definition class particle field list. */
	igdeGDCForceFieldList( const igdeGDCForceFieldList &list );
	
	/** \brief Clean up list. */
	~igdeGDCForceFieldList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of fields. */
	int GetCount() const;
	
	/** \brief Emitter by index. */
	igdeGDCForceField *GetAt( int index ) const;
	
	/** \brief Index of an field or -1 if not found. */
	int IndexOf( igdeGDCForceField *field ) const;
	
	/** \brief Determines if an field exists. */
	bool Has( igdeGDCForceField *field ) const;
	
	/** \brief Adds an field. */
	void Add( igdeGDCForceField *field );
	
	/** \brief Inserts a new field. */
	void InsertAt( igdeGDCForceField *field, int index );
	
	/** \brief Moves an field to a new position. */
	void MoveTo( igdeGDCForceField *field, int index );
	
	/** \brief Removes an field. */
	void Remove( igdeGDCForceField *field );
	
	/** \brief Removes all fields. */
	void RemoveAll();
	
	/** \brief Set this list to a deep copy of another list. */
	void SetToDeepCopyFrom( const igdeGDCForceFieldList &list );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set list from another list. */
	igdeGDCForceFieldList &operator=( const igdeGDCForceFieldList &list );
	/*@}*/
};

#endif
