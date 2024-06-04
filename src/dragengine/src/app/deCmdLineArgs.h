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
