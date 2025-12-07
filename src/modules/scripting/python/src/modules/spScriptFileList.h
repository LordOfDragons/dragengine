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

#ifndef _SPSCRIPTFILELIST_H_
#define _SPSCRIPTFILELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

#include "../spCommon.h"

class spScriptFile;
class ScriptingPython;



/**
 * \brief List of python modules loaded from a script files.
 */
class spScriptFileList{
private:
	ScriptingPython *pSP;
	decObjectOrderedSet pFiles;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new list. */
	spScriptFileList(ScriptingPython *sp);
	/** \brief Cleans up the list. */
	~spScriptFileList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the number of files. */
	int GetCount() const;
	/** \brief Retrieves the file at the given position. */
	spScriptFile *GetAt(int index) const;
	/** \brief Retrieves the file with the given fullname or NULL if not found. */
	spScriptFile *GetWithFullName(const char *fullname) const;
	/** \brief Retrieves the index of the given file or -1 if not found. */
	int IndexOf(spScriptFile *file) const;
	/** \brief Retrieves the index of the file with the given fullname or -1 if not found. */
	int IndexOfWithFullName(const char *fullname) const;
	/** \brief Determines if a file exists. */
	bool Has(spScriptFile *file) const;
	/** \brief Determines if a file with the given fullname exists. */
	bool HasWithFullName(const char *fullname) const;
	/** \brief Adds a file. */
	void Add(spScriptFile *file);
	/** \brief Removes a file. */
	void Remove(spScriptFile *file);
	/** \brief Removes all files. */
	void RemoveAll();
	
	/** \brief Call python clean up on all files. */
	void PythonCleanUpAll();
	/*@}*/
};

#endif
