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

#ifndef _IGDETEMPLATEFILELIST_H_
#define _IGDETEMPLATEFILELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeTemplateFile;



/**
 * \brief Project template file list.
 */
class igdeTemplateFileList{
private:
	decObjectOrderedSet pFiles;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create project template file list. */
	igdeTemplateFileList();
	
	/** \brief Create copy of project template file list. */
	igdeTemplateFileList( const igdeTemplateFileList &list );
	
	/** \brief Clean up project template file list. */
	~igdeTemplateFileList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of project template files. */
	int GetCount() const;
	
	/** \brief Project template file at index. */
	igdeTemplateFile *GetAt( int index ) const;
	
	/** \brief Index of project template file or -1 if absent. */
	int IndexOf( igdeTemplateFile *file ) const;
	
	/** \brief Project template file is present. */
	bool Has( igdeTemplateFile *file ) const;
	
	/** \brief Add project template file. */
	void Add( igdeTemplateFile *file );
	
	/** \brief Remove project template file. */
	void Remove( igdeTemplateFile *file );
	
	/** \brief Remove all project template files. */
	void RemoveAll();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy list. */
	igdeTemplateFileList &operator=( const igdeTemplateFileList &list );
	/*@}*/
};

#endif
