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

#ifndef _GDEOCFORCEFIELDLIST_H_
#define _GDEOCFORCEFIELDLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeOCForceField;



/**
 * \brief Object class force field list.
 */
class gdeOCForceFieldList{
private:
	decObjectOrderedSet pForceFields;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object class force field list. */
	gdeOCForceFieldList();
	
	/** \brief Create copy of object class force field list. */
	gdeOCForceFieldList( const gdeOCForceFieldList &list );
	
	/** \brief Clean up object class force field list. */
	~gdeOCForceFieldList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of object class force fields. */
	int GetCount() const;
	
	/** \brief Object class force field at position. */
	gdeOCForceField *GetAt( int index ) const;
	
	/** \brief Index of named object class force field or -1 if absent. */
	int IndexOf( gdeOCForceField *field ) const;
	
	/** \brief Object class force field is present. */
	bool Has( gdeOCForceField *field ) const;
	
	/** \brief Add object class force field. */
	void Add( gdeOCForceField *field );
	
	/** \brief Remove object class force field. */
	void Remove( gdeOCForceField *field );
	
	/** \brief Remove all object class force fields. */
	void RemoveAll();
	
	/** \brief Set from another object class force field list. */
	gdeOCForceFieldList &operator=( const gdeOCForceFieldList &list );
	/*@}*/
};

#endif
