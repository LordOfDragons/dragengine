/* 
 * Drag[en]gine Game Engine
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

#ifndef _DECUNICODEARGUMENTLIST_H_
#define _DECUNICODEARGUMENTLIST_H_

#include "../../../dragengine_export.h"


// definitions
class decUnicodeString;


/**
 * \brief Manages argument list with unicode strings.
 *
 * Provides support to parse unicode strings into an argument list
 * using common separation schemas. Easy to use for obtaining
 * commands in unicode format and turning them into a simple to
 * use argument list.
 */
class DE_DLL_EXPORT decUnicodeArgumentList{
private:
	decUnicodeString **pArguments;
	int pArgumentCount, pArgumentSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new empty argument list object. */
	decUnicodeArgumentList();
	
	/** \brief Frees the argument list object. */
	~decUnicodeArgumentList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of arguments. */
	inline int GetArgumentCount() const{ return pArgumentCount; }
	
	/** \brief Argument at the given index. */
	const decUnicodeString *GetArgumentAt( int index ) const;
	
	/** \brief Adds an argument to the end of the list. */
	void AddArgument( const decUnicodeString &argument );
	
	/** \brief Given argument matches the provided utf8 string. */
	bool MatchesArgumentAt( int index, const char *string ) const;
	
	/** \brief Removes all arguments. */
	void RemoveAllArguments();
	
	/**
	 * \brief Parse command line passed into arguments which are then added to the end of the list.
	 * 
	 * Arguments are considered separated by a white space. Quoted text strings are
	 * considered one argument. This function is present for operating systems not
	 * providing an entry point function which already splits up the arguments.
	 */
	void ParseCommand( const decUnicodeString &command );
	/*@}*/
};

#endif
