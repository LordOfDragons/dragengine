/* 
 * Drag[en]gine Android Launcher
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

// include only once
#ifndef _DECUNICODEARGUMENTLIST_H_
#define _DECUNICODEARGUMENTLIST_H_



// definitions
class decUnicodeString;



/**
 * @brief Manages argument list with unicode strings.
 *
 * Provides support to parse unicode strings into an argument list
 * using common separation schemas. Easy to use for obtaining
 * commands in unicode format and turning them into a simple to
 * use argument list.
 */
class decUnicodeArgumentList{
private:
	decUnicodeString **pArguments;
	int pArgumentCount, pArgumentSize;
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new empty argument list object. */
	decUnicodeArgumentList();
	/** Frees the argument list object. */
	~decUnicodeArgumentList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of arguments. */
	inline int GetArgumentCount() const{ return pArgumentCount; }
	/** Retrieves the argument at the given index. */
	const decUnicodeString *GetArgumentAt( int index ) const;
	/** Adds an argument to the end of the list. */
	void AddArgument( const decUnicodeString &argument );
	/** Determines if the given argument matches the provided utf8 string. */
	bool MatchesArgumentAt( int index, const char *string ) const;
	/** Removes all arguments. */
	void RemoveAllArguments();
	/**
	 * Parses command line passed into arguments which are then added
	 * to the end of the list. Arguments are considered separated by
	 * a white space. Quoted text strings are considered one argument.
	 * This function is present for operating systems not providing
	 * an entry point function which already splits up the arguments.
	 */
	void ParseCommand( const decUnicodeString &command );
	/*@}*/
};

// end of include only once
#endif
