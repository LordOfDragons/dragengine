/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDENATIVEBEOSVFSLISTITEM_H_
#define _IGDENATIVEBEOSVFSLISTITEM_H_

#include <dragengine/common/utils/decDateTime.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/filesystem/deVFSContainer.h>


/**
 * \brief Item of a virtual file system list.
 */
class igdeNativeBeosVFSListItem {
private:
	decPath pPath;
	decString pFileName;
	deVFSContainer::eFileTypes pFileType;
	int pFileSize;
	TIME_SYSTEM pFileModificationTime;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new virtual file system list item. */
	igdeNativeBeosVFSListItem(const decPath &path, const char *filename);
	/** \brief Cleans up the virtual file system list item. */
	~igdeNativeBeosVFSListItem();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the path. */
	inline const decPath &GetPath() const{ return pPath; }
	
	/** \brief Retrieves the file name. */
	inline const decString &GetFileName() const{ return pFileName; }
	/** \brief Sets the file name. */
	void SetFileName(const char *filename);
	
	/** \brief Retrieves the file type. */
	inline deVFSContainer::eFileTypes GetFileType() const{ return pFileType; }
	/** \brief Sets the file type. */
	void SetFileType(deVFSContainer::eFileTypes fileType);
	
	/** \brief Retrieves the file size. */
	inline int GetFileSize() const{ return pFileSize; }
	/** \brief Sets the file size. */
	void SetFileSize(int fileSize);
	
	/** \brief Retrieves the file modification time. */
	inline TIME_SYSTEM GetFileModificationTime() const{ return pFileModificationTime; }
	/** \brief Sets the file modification time. */
	void SetFileModificationTime(TIME_SYSTEM time);
	
	/** \brief Determines if this is a directory. */
	bool IsDirectory() const;
	
	/**
	 * \brief Update text with file information according to a pattern suitable for list display.
	 * \details Pattern consists of single characters defining the column layout.
	 *          |n|file name|
	 *          |s|file size|
	 *          |m|file modification time|
	 *          |t|file type|
	 */
	decString UpdateText(const char *pattern) const;
	/*@}*/
};

#endif
