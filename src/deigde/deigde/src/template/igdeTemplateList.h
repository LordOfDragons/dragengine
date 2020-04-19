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

#ifndef _IGDETEMPLATELIST_H_
#define _IGDETEMPLATELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeTemplate;



/**
 * \brief Project template list.
 */
class igdeTemplateList{
private:
	decObjectOrderedSet pTemplates;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create project template list. */
	igdeTemplateList();
	
	/** \brief Create copy of project template list. */
	igdeTemplateList( const igdeTemplateList &list );
	
	/** \brief Clean up project template list. */
	~igdeTemplateList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of project templates. */
	int GetCount() const;
	
	/** \brief Project template at index. */
	igdeTemplate *GetAt( int index ) const;
	
	/** \brief Index of project template or -1 if absent. */
	int IndexOf( igdeTemplate *atemplate ) const;
	
	/** \brief Project template is present. */
	bool Has( igdeTemplate *atemplate ) const;
	
	/** \brief Add project template. */
	void Add( igdeTemplate *atemplate );
	
	/** \brief Remove project template. */
	void Remove( igdeTemplate *atemplate );
	
	/** \brief Remove all project templates. */
	void RemoveAll();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy list. */
	igdeTemplateList &operator=( const igdeTemplateList &list );
	/*@}*/
};

#endif
