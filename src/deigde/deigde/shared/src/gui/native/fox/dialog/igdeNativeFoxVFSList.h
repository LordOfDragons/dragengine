/* 
 * Drag[en]gine IGDE
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
	FXDECLARE( igdeNativeFoxVFSList )
	
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
	igdeNativeFoxVFSList( igdeEnvironment &environment, deVirtualFileSystem *vfs,
		FXComposite *p, FXObject* tgt=NULL, FXSelector sel=0, FXuint opts=0,
		FXint x=0, FXint y=0, FXint w=0, FXint h=0 );
	
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
	void SetPath( const decPath &path );
	
	/** \brief Retrieves the pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	/** \brief Sets the pattern. */
	void SetPattern( const char *pattern );
	
	/** \brief Retrieves the sorting. */
	inline eSorting GetSorting() const{ return pSorting; }
	/** \brief Sets the sorting. */
	void SetSorting( eSorting sorting );
	
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
	long onHeaderClicked( FXObject*, FXSelector, void* );
	/*@}*/
};

#endif
