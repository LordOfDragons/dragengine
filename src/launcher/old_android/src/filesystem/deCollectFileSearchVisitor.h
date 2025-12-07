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

#ifndef _DECOLLECTFILESEARCHVISITOR_H_
#define _DECOLLECTFILESEARCHVISITOR_H_

#include "dePathList.h"
#include "deFileSearchVisitor.h"


/**
 * \brief File search visitor collecting visited files if matching pattern.
 * 
 * Used with deVirtualFileSystem::SearchFiles() call. Stores visited files. Does recursion
 * into directories if desired. Empty pattern list matches all files. Special files are
 * not supported.
 */
class deCollectFileSearchVisitor : public deFileSearchVisitor{
private:
	dePathList pPatterns;
	bool pRecursive;
	dePathList pFiles;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file search visitor. */
	deCollectFileSearchVisitor();
	
	/** \brief Create file search visitor with pattern. */
	deCollectFileSearchVisitor(const char *pattern, bool recursive = false);
	
	/** \brief Clean up file search visitor. */
	~deCollectFileSearchVisitor() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief List of patterns to match. */
	inline dePathList &GetPatterns(){ return pPatterns; }
	inline const dePathList &GetPatterns() const{ return pPatterns; }
	
	/** \brief Add pattern. */
	void AddPattern(const char *pattern);
	
	/** \brief Enable recursion. */
	inline bool GetRecursive() const{ return pRecursive; }
	
	/** \brief Set if recursion is enabled. */
	void SetRecursive(bool recursive);
	
	/** \brief Found files. */
	inline dePathList &GetFiles(){ return pFiles; }
	inline const dePathList &GetFiles() const{ return pFiles; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/**
	 * \brief Visit regular file.
	 * \param[in] vfs Virtual file system used for searching.
	 * \param[in] path Full file path.
	 * \retval true Continue searching.
	 * \retval false Stop searching.
	 */
	bool VisitFile(const deVirtualFileSystem &vfs, const decPath &path) override;
	
	/**
	 * \brief Visit directory.
	 * \param[in] vfs Virtual file system used for searching.
	 * \param[in] path Full directory path.
	 * \retval true Continue searching.
	 * \retval false Stop searching.
	 */
	bool VisitDirectory(const deVirtualFileSystem &vfs, const decPath &path) override;
	
	/**
	 * \brief Visit special.
	 * \param[in] vfs Virtual file system used for searching.
	 * \param[in] path Full file path.
	 * \retval true Continue searching.
	 * \retval false Stop searching.
	 */
	bool VisitSpecial(const deVirtualFileSystem &vfs, const decPath &path) override;
	/*@}*/
};

#endif
