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
	ceConversationFile *GetAt(int index) const;
	/** Retrieves the file with the given id or NULL if not found. */
	ceConversationFile *GetWithID(const char *id) const;
	/** Retrieves the index of the given file or -1 if not found. */
	int IndexOf(ceConversationFile *file) const;
	/** Retrieves the index of the file with the given id or -1 if not found. */
	int IndexOfWithID(const char *id) const;
	/** Determines if a file exists. */
	bool Has(ceConversationFile *file) const;
	/** Determines if a file with the given id exists. */
	bool HasWithID(const char *id) const;
	/** Adds a file. */
	void Add(ceConversationFile *file);
	/** Removes a file. */
	void Remove(ceConversationFile *file);
	/** Removes all files. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	ceConversationFileList &operator=(const ceConversationFileList &list);
	
	/** \brief Append files if absent. */
	ceConversationFileList &operator+=(const ceConversationFileList &list);
	/*@}*/
};

#endif
