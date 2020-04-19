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

#ifndef _DEEXCEPTION_H_
#define _DEEXCEPTION_H_

#include <stddef.h>

#include "../string/decStringList.h"

/**
 * \brief Exception class.
 */
class deException{
private:
	decString pName;
	decString pDescription;
	decString pFile;
	int pLine;
	decStringList pBacktrace;
	
	
	
public:
	/** \name Constructors, Destructors */
	/*@{*/
	/**
	 * \brief Create a new exception object.
	 * \param[in] name Unique name to identify the exception. If NULL set to a default string.
	 * \param[in] description Description of the reason for the exception. If NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occured. If NULL set to a default string.
	 * \param[in] line Line number in file where the exception occured. If NULL set to a default string.
	 */
	deException( const char *name, const char *description, const char *file, int line );
	
	/**
	 * \brief Clean up exception object.
	 */
	virtual ~deException();
	/*@}*/
	
	
	
	/** \name Accessors */
	/*@{*/
	/** \brief Unique name to identify the exception. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Description of the reason for the exception. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Name of the source file where the exception occured. */
	inline const decString &GetFile() const{ return pFile; }
	
	/** \brief Line number in file where the exception occured. */
	inline int GetLine() const{ return pLine; }
	
	/** \brief Backtrace. */
	inline const decStringList &GetBacktrace() const{ return pBacktrace; }
	/*@}*/
	
	
	
	/** \name Tests */
	/*@{*/
	/** \brief Test if the exception has the given name. */
	bool IsNamed( const char *name ) const;
	/*@}*/
	
	
	
	/** \name Display Functions */
	/*@{*/
	/**
	 * \brief Subclass specific additional information.
	 * 
	 * Default implementation returns an empty list.
	 */
	virtual decStringList AdditionalInformation() const;
	
	/**
	 * \brief Format output into a list of strings.
	 * 
	 * Clears the list and adds the following entries:
	 * - "Exception:   {pName}"
	 * - "Description: {pDescription}"
	 * - "Source File: {pFile}"
	 * - "Source Line: {pLine}"
	 * - "BacktraceX: {pBacktrace[X]}"
	 * - AdditionalInformation
	 */
	decStringList FormatOutput() const;
	
	/**
	 * \brief Display a formated output of the stored informations.
	 * 
	 * Print each line obtained by FormatOutput to stdout.
	 */
	void PrintError() const;
	/*@}*/
	
	
	
private:
	/** \brief Build backtrace. */
	void pBuildBacktrace();
};

#endif

