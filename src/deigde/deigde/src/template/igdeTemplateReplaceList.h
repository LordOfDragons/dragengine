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

#ifndef _IGDETEMPLATEREPLACELIST_H_
#define _IGDETEMPLATEREPLACELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeTemplateReplace;



/**
 * \brief Project template replace list.
 */
class igdeTemplateReplaceList{
private:
	decObjectOrderedSet pReplaces;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create project template replace list. */
	igdeTemplateReplaceList();
	
	/** \brief Create copy of project template replace list. */
	igdeTemplateReplaceList( const igdeTemplateReplaceList &list );
	
	/** \brief Clean up project template replace list. */
	~igdeTemplateReplaceList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of project template replaces. */
	int GetCount() const;
	
	/** \brief Project template replace at index. */
	igdeTemplateReplace *GetAt( int index ) const;
	
	/** \brief Index of project template replace or -1 if absent. */
	int IndexOf( igdeTemplateReplace *replace ) const;
	
	/** \brief Project template replace is present. */
	bool Has( igdeTemplateReplace *replace ) const;
	
	/** \brief Add project template replace. */
	void Add( igdeTemplateReplace *replace );
	
	/** \brief Remove project template replace. */
	void Remove( igdeTemplateReplace *replace );
	
	/** \brief Remove all project template replaces. */
	void RemoveAll();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy list. */
	igdeTemplateReplaceList &operator=( const igdeTemplateReplaceList &list );
	/*@}*/
};

#endif
