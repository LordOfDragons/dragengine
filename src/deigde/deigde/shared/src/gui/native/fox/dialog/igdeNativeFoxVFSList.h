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

#ifndef _IGDENATIVEFOXVFSLIST_H_
#define _IGDENATIVEFOXVFSLIST_H_

#include "../foxtoolkit.h"

#include <dragengine/common/file/decPath.h>

class igdeEnvironment;
class deVirtualFileSystem;
class igdeNativeFoxVFSListItem;



/**
 * \brief List the content of a virtual file system directory.
 */
class igdeNativeFoxVFSList : public FXIconList{
	FXDECLARE(igdeNativeFoxVFSList)
	
protected:
	igdeNativeFoxVFSList();
	
public:
	enum eFoxIDs{
		ID_SELF = FXVerticalFrame::ID_LAST,
		ID_HEADER,
		ID_LAST
	};
	
public:
	/** \brief Sorting. */
	enum eSorting{
		/** Sort ascending by file name. */
		esNameAsc,
		/** Sort descending by file name. */
		esNameDesc,
		/** Sort ascending by file size. */
		esSizeAsc,
		/** Sort descending by file size. */
		esSizeDesc,
		/** Sort ascending by modification time. */
		esModifyTimeAsc,
		/** Sort descending by modification time. */
		esModifyTimeDesc
	};
	
private:
	igdeEnvironment *pEnvironment;
	deVirtualFileSystem *pVFS;
	decPath pCurPath;
	decString pPattern;
	eSorting pSorting;
	
	FXIcon *pIconFolderBig;
	FXIcon *pIconFolderSmall;
	FXIcon *pIconFileBig;
	FXIcon *pIconFileSmall;
	FXIcon *pIconSortAsc;
	FXIcon *pIconSortDesc;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new virtual file system list. */
	igdeNativeFoxVFSList(igdeEnvironment &environment, deVirtualFileSystem *vfs,
		FXComposite *p, FXObject* tgt=NULL, FXSelector sel=0, FXuint opts=0,
		FXint x=0, FXint y=0, FXint w=0, FXint h=0);
	
	/** \brief Cleans up the virtual file system list. */
	virtual ~igdeNativeFoxVFSList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the virtual file system. */
	inline deVirtualFileSystem *GetVirtualFileSystem() const{ return pVFS; }
	
	/** \brief Update listing. */
	void UpdateListing();
	
	/** \brief Retrieves the current directory. */
	inline const decPath &GetPath(){ return pCurPath; }
	/** \brief Sets the current directory. */
	void SetPath(const decPath &path);
	
	/** \brief Retrieves the pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	/** \brief Sets the pattern. */
	void SetPattern(const char *pattern);
	
	/** \brief Retrieves the sorting. */
	inline eSorting GetSorting() const{ return pSorting; }
	/** \brief Sets the sorting. */
	void SetSorting(eSorting sorting);
	
	/** \brief Retrieves the selected item or NULL. */
	igdeNativeFoxVFSListItem *GetSelectedItem() const;
	
	/** \brief Create server-side resources. */
	virtual void create();
	/** \brief Detach server-side resources. */
	virtual void detach();
	/** \brief Destroy server-side resources. */
	virtual void destroy();
	
	/** \brief Icons. */
	inline FXIcon *GetIconFolderBig() const{ return pIconFolderBig; }
	inline FXIcon *GetIconFolderSmall() const{ return pIconFolderSmall; }
	inline FXIcon *GetIconFileBig() const{ return pIconFileBig; }
	inline FXIcon *GetIconFileSmall() const{ return pIconFileSmall; }
	inline FXIcon *GetIconSortAsc() const{ return pIconSortAsc; }
	inline FXIcon *GetIconSortDesc() const{ return pIconSortDesc; }
	/*@}*/
	
	/** \name Events */
	/*@{*/
	long onHeaderClicked(FXObject*, FXSelector, void*);
	/*@}*/
};

#endif
