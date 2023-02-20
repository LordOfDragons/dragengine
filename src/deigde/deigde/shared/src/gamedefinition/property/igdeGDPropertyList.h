/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _IGDEPROPERTYLIST_H_
#define _IGDEPROPERTYLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDProperty;



/**
 * \brief Property list.
 */
class DE_DLL_EXPORT igdeGDPropertyList{
private:
	decObjectOrderedSet pProperties;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create property list. */
	igdeGDPropertyList();
	
	/** \brief Create copy of property list. */
	igdeGDPropertyList( const igdeGDPropertyList &list );
	
	/** \brief Clean up property list. */
	~igdeGDPropertyList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of propertyes. */
	int GetCount() const;
	
	/** \brief Property at position. */
	igdeGDProperty *GetAt( int index ) const;
	
	/** \brief Named property or NULL if absent. */
	igdeGDProperty *GetNamed( const char *name ) const;
	
	/** \brief Index of named property or -1 if absent. */
	int IndexOf( igdeGDProperty *proeprty ) const;
	
	/** \brief Index of named property or -1 if absent. */
	int IndexOfNamed( const char *name ) const;
	
	/** \brief Property is present. */
	bool Has( igdeGDProperty *proeprty ) const;
	
	/** \brief Named property is present. */
	bool HasNamed( const char *name ) const;
	
	/** \brief Add property. */
	void Add( igdeGDProperty *proeprty );
	
	/** \brief Remove property. */
	void Remove( igdeGDProperty *proeprty );
	
	/** \brief Remove all propertyes. */
	void RemoveAll();
	
	/** \brief Set this list to a deep copy of another list. */
	void SetToDeepCopyFrom( const igdeGDPropertyList &list );
	
	/** \brief Set from another property list. */
	igdeGDPropertyList &operator=( const igdeGDPropertyList &list );
	/*@}*/
};

#endif
