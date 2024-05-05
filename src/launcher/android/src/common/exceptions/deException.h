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

#ifndef _DEEXCEPTION_H_
#define _DEEXCEPTION_H_

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
	/** \name Constructors */
	/*@{*/
	/**
	 * \brief Create a new exception object.
	 * \param[in] name Unique name to identify the exception. If \em NULL set to a default string.
	 * \param[in] description Description of the reason for the exception. If \em NULL set to a default string.
	 * \param[in] file Name of the source file where the exception occurred. If \em NULL set to a default string.
	 * \param[in] line Line number in file where the exception occurred. If \em NULL set to a default string.
	 */
	deException( const char *name, const char *description, const char *file, int line );
	/*@}*/
	
	/** \name Accessors */
	/*@{*/
	/** \brief Unique name to identify the exception. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Description of the reason for the exception. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Name of the source file where the exception occurred. */
	inline const decString &GetFile() const{ return pFile; }
	
	/** \brief Line number in file where the exception occurred. */
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
	 * \details Default implementation returns an empty list.
	 */
	virtual decStringList AdditionalInformation() const;
	
	/**
	 * \brief Format output into a list of strings.
	 * \details Clears the list and adds the following entries:
	 *          - "Exception:   {pName}"
	 *          - "Description: {pDescription}"
	 *          - "Source File: {pFile}"
	 *          - "Source Line: {pLine}"
	 *          - "BacktraceX: {pBacktrace[X]}"
	 *          - AdditionalInformation
	 */
	decStringList FormatOutput() const;
	
	/**
	 * \brief Display a formated output of the stored information.
	 * \details Print each line obtained by FormatOutput to stdout.
	 */
	void PrintError() const;
	/*@}*/
	
private:
	/** \brief Build backtrace. */
	void pBuildBacktrace();
};

#endif

