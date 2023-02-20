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

#ifndef _IGDETAGMANAGER_H_
#define _IGDETAGMANAGER_H_

#include <dragengine/common/string/decStringList.h>

class decStringSet;



/**
 * \brief List of string tags.
 */
class DE_DLL_EXPORT igdeTagManager{
private:
	decStringList pTags;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create tag manager. */
	igdeTagManager();
	
	/** \brief Create tag manager as a copy of another tag manager. */
	igdeTagManager( const igdeTagManager &tagManager );
	
	/** \brief Clean up tag manager. */
	~igdeTagManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of tags. */
	int GetTagCount() const;
	
	/** \brief Tag at the given index. */
	const decString &GetTagAt( int index ) const;
	
	/** \brief Determines if the given tag exists. */
	bool HasTag( const char *tag ) const;
	
	/** \brief Index of the given tag or -1 if not found. */
	int IndexOfTag( const char *tag ) const;
	
	/** \brief Adds a new tag if not already existing. */
	void AddTag( const char *tag );
	
	/** \brief Removes the given tag if existing. */
	void RemoveTag( const char *tag );
	
	/** \brief Removes all tags. */
	void RemoveAllTags();
	
	/**
	 * \brief Determines if all tags of another tag manager are present in this tag manager.
	 * 
	 * If the other tag manager contains no tags false is returned.
	 */
	bool HasAllOf( const igdeTagManager &tags ) const;
	
	/** \brief Determines if at least one tag of another tag manager is present in this tag manager. */
	bool HasAnyOf( const igdeTagManager &tags ) const;
	
	/** \brief Determines if no tag of another tag manager is present in this tag manager. */
	bool HasNoneOf( const igdeTagManager &tags ) const;
	
	/** \brief Adds tags from another tag manager if not existing yet. */
	void AddTagsFrom( const igdeTagManager &tags );
	
	/** \brief Get tags as string set. */
	decStringSet ToSet() const;
	/*@}*/
	
	
	
	/** \name Operators. */
	/*@{*/
	/** \brief Set tags to the same as another tag manager. */
	igdeTagManager &operator=( const igdeTagManager &tags );
	
	/** \brief Set tags. */
	igdeTagManager &operator=( const decStringSet &tags );
	
	/** \brief Get tags as string set. */
	operator decStringSet() const;
	/*@}*/
};

#endif
