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

#ifndef _DECMDLINEARGS_H_
#define _DECMDLINEARGS_H_

#include "../common/string/decStringList.h"


/**
 * \brief Command line arguments.
 * 
 * Basically wraps a string list with some additional helper methods.
 */
class DE_DLL_EXPORT deCmdLineArgs{
private:
	decStringList pArguments;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new command line object. */
	deCmdLineArgs();
	
	/** \brief Create a copy of an argument list. */
	deCmdLineArgs( const deCmdLineArgs &copy );
	
	/** \brief Clean up the command line object. */
	~deCmdLineArgs();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of arguments. */
	int GetCount() const;
	
	/**
	 * \brief Argument at index.
	 * 
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	const decString &GetArgument( int index ) const;
	
	/**
	 * \brief Index of first matching argument or -1 if not found.
	 * 
	 * \throws deeInvalidParam \em argument is NULL.
	 */
	int IndexOfArgument( const char *argument ) const;
	
	/**
	 * \brief Determine if argument exists.
	 * 
	 * \throws deeInvalidParam \em argument is NULL.
	 */
	bool HasArgument( const char *argument ) const;
	
	/**
	 * \brief Add an argument.
	 * 
	 * \throws deeInvalidParam \em argument is NULL.
	 */
	void AddArgument( const char *argument );
	
	/**
	 * \brief Remove an argument from index.
	 * 
	 * \throws deeInvalidParam \em index is less than 0 or larger than GetCount()-1.
	 */
	void RemoveArgument( int index );
	
	/** \brief Remove all arguments. */
	void RemoveAllArguments();
	
	/**
	 * \brief Add arguments parsed from a string command line.
	 * 
	 * Arguments are considered separated by a white space. Quoted text
	 * strings are considered one argument.
	 * 
	 * \throws deeInvalidParam \em commandLine is NULL.
	 */
	void AddArgsSplit( const char *commandLine );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy an argument list to this argument list. */
	deCmdLineArgs &operator=( const deCmdLineArgs &copy );
	/*@}*/
};

#endif
