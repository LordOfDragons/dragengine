/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CECONVERSATIONFILELIST_H_
#define _CECONVERSATIONFILELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceConversationFile;



/**
 * \brief Conversation File List.
 */
class ceConversationFileList{
private:
	decObjectOrderedSet pFiles;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new file list. */
	ceConversationFileList();
	/** Cleans up the file list. */
	~ceConversationFileList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of files. */
	int GetCount() const;
	/** Retrieves the file at the given position. */
	ceConversationFile *GetAt( int index ) const;
	/** Retrieves the file with the given id or NULL if not found. */
	ceConversationFile *GetWithID( const char *id ) const;
	/** Retrieves the index of the given file or -1 if not found. */
	int IndexOf( ceConversationFile *file ) const;
	/** Retrieves the index of the file with the given id or -1 if not found. */
	int IndexOfWithID( const char *id ) const;
	/** Determines if a file exists. */
	bool Has( ceConversationFile *file ) const;
	/** Determines if a file with the given id exists. */
	bool HasWithID( const char *id ) const;
	/** Adds a file. */
	void Add( ceConversationFile *file );
	/** Removes a file. */
	void Remove( ceConversationFile *file );
	/** Removes all files. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	ceConversationFileList &operator=( const ceConversationFileList &list );
	
	/** \brief Append files if absent. */
	ceConversationFileList &operator+=( const ceConversationFileList &list );
	/*@}*/
};

#endif
