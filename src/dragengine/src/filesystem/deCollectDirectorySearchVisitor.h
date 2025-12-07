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

#ifndef _DECOLLECTDIRECTORYSEARCHVISITOR_H_
#define _DECOLLECTDIRECTORYSEARCHVISITOR_H_

#include "dePathList.h"
#include "deFileSearchVisitor.h"


/**
 * \brief File search visitor collecting visited directories.
 * 
 * Used with deVirtualFileSystem::SearchFiles() call. Stores visited directories.
 * Does recursion into directories by default.
 */
class DE_DLL_EXPORT deCollectDirectorySearchVisitor : public deFileSearchVisitor{
private:
	bool pRecursion;
	dePathList pDirectories;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file search visitor. */
	deCollectDirectorySearchVisitor(bool recursive = false);
	
	/** \brief Clean up file search visitor. */
	~deCollectDirectorySearchVisitor() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Enable recursion. */
	inline bool GetRecursion() const{ return pRecursion; }
	
	/** \brief Set if recursion is enabled. */
	void SetRecursion(bool recursion);
	
	/** \brief Found directories. */
	inline dePathList &GetDirectories(){ return pDirectories; }
	inline const dePathList &GetDirectories() const{ return pDirectories; }
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
