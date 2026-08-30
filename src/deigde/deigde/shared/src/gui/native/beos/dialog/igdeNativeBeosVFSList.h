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

#ifndef _IGDENATIVEBEOSVFSLIST_H_
#define _IGDENATIVEBEOSVFSLIST_H_

#include <interface/View.h>
#include <interface/ListControl.h>
#include <interface/ScrollView.h>
#include <vector>

#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/filesystem/deVFSContainer.h>

class igdeEnvironment;
class deVirtualFileSystem;
class igdeNativeBeosVFSListItem;



/**
 * \brief List the content of a virtual file system directory.
 */
class igdeNativeBeosVFSList : public BView{
public:
	/** \brief Sorting. */
	enum eSorting {
		esNameAsc,
		esNameDesc,
		esSizeAsc,
		esSizeDesc,
		esModifyTimeAsc,
		esModifyTimeDesc
	};
	
private:
	igdeEnvironment *pEnvironment;
	deVirtualFileSystem *pVFS;
	BListView *pListView;
	BScrollView *pScrollView;
	
	decPath pCurPath;
	decString pPattern;
	eSorting pSorting;
	
	std::vector<igdeNativeBeosVFSListItem*> pItems;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new virtual file system list. */
	igdeNativeBeosVFSList(igdeEnvironment &environment, deVirtualFileSystem *vfs,
		BRect frame, const char *name);
	
	/** \brief Cleans up the virtual file system list. */
	~igdeNativeBeosVFSList() override;
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the virtual file system. */
	inline deVirtualFileSystem *GetVirtualFileSystem() const{ return pVFS; }
	
	/** \brief Retrieves the list view. */
	inline BListView *GetListView() const{ return pListView; }
	
	/** \brief Retrieves the scroll view. */
	inline BScrollView *GetScrollView() const{ return pScrollView; }
	
	/** \brief Update listing. */
	void UpdateListing();
	
	/** \brief Retrieves the current directory. */
	inline const decPath &GetPath() const{ return pCurPath; }
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
	
	/** \brief Retrieves the selected item or nullptr. */
	igdeNativeBeosVFSListItem *GetSelectedItem() const;
	
	/** \brief Get full path of selected file. */
	decPath GetSelectedPath() const;
	
	/** \brief BView overrides */
	void AttachedToWindow() override;
	void MessageReceived(BMessage *message) override;
	/*@}*/
	
private:
	void pClearItems();
	int pCountMatching() const;
	bool pMatchesPattern(const char *filename) const;
	void pSortItems();
	static bool pSortCompareNameAsc(const igdeNativeBeosVFSListItem *item1, const igdeNativeBeosVFSListItem *item2);
	static bool pSortCompareNameDesc(const igdeNativeBeosVFSListItem *item1, const igdeNativeBeosVFSListItem *item2);
	static bool pSortCompareSizeAsc(const igdeNativeBeosVFSListItem *item1, const igdeNativeBeosVFSListItem *item2);
	static bool pSortCompareSizeDesc(const igdeNativeBeosVFSListItem *item1, const igdeNativeBeosVFSListItem *item2);
	static bool pSortCompareTimeAsc(const igdeNativeBeosVFSListItem *item1, const igdeNativeBeosVFSListItem *item2);
	static bool pSortCompareTimeDesc(const igdeNativeBeosVFSListItem *item1, const igdeNativeBeosVFSListItem *item2);
};

#endif
