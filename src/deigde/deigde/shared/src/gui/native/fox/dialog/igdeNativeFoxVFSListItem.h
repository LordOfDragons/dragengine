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

#ifndef _IGDENATIVEFOXVFSLISTITEM_H_
#define _IGDENATIVEFOXVFSLISTITEM_H_

#include "../foxtoolkit.h"

#include <dragengine/common/utils/decDateTime.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/filesystem/deVFSContainer.h>



/**
 * \brief Item of a virtual file system list.
 */
class igdeNativeFoxVFSListItem : public FXIconItem{
private:
	const decPath pPath;
	FXString pFileName;
	deVFSContainer::eFileTypes pFileType;
	int pFileSize;
	TIME_SYSTEM pFileModificationTime;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new virtual file system list item. */
	igdeNativeFoxVFSListItem( const decPath &path, const FXString &text, FXIcon *bi=NULL, FXIcon *mi=NULL );
	/** \brief Cleans up the virtual file system list. */
	virtual ~igdeNativeFoxVFSListItem();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the path. */
	inline const decPath &GetPath() const{ return pPath; }
	
	/** \brief Retrieves the file name. */
	inline const FXString &GetFileName() const{ return pFileName; }
	/** \brief Sets the file name. */
	void SetFileName( const char *filename );
	/** \brief Retrieves the file type. */
	inline deVFSContainer::eFileTypes GetFileType() const{ return pFileType; }
	/** \brief Sets the file type. */
	void SetFileType( deVFSContainer::eFileTypes fileType );
	/** \brief Retrieves the file size. */
	inline int GetFileSize() const{ return pFileSize; }
	/** \brief Sets the file size. */
	void SetFileSize( int fileSize );
	/** \brief Retrieves the file modification time. */
	inline TIME_SYSTEM GetFileModificationTime() const{ return pFileModificationTime; }
	/** \brief Sets the file modification time. */
	void SetFileModificationTime( TIME_SYSTEM time );
	
	/** \brief Determines if this is a directory. */
	bool IsDirectory() const;
	
	/**
	 * \brief Update text with file information according to a pattern suitable for long listing.
	 * \details Pattern consists of single characters defining the column layout.
	 *          |n|file name|
	 *          |s|file size|
	 *          |m|file modification time|
	 *          |t|file type|
	 */
	void UpdateText( const char *pattern );
	/*@}*/
	
	/** \name Sorting */
	/*@{*/
	/** \brief Sort ascending by file name. */
	static FXint fSortNameAsc( const FXIconItem *item1, const FXIconItem *item2 );
	/** \brief Sort descending by file name. */
	static FXint fSortNameDesc( const FXIconItem *item1, const FXIconItem *item2 );
	/** \brief Sort ascending by file size. */
	static FXint fSortSizeAsc( const FXIconItem *item1, const FXIconItem *item2 );
	/** \brief Sort descending by file size. */
	static FXint fSortSizeDesc( const FXIconItem *item1, const FXIconItem *item2 );
	/** \brief Sort ascending by file modification time. */
	static FXint fSortModTimeAsc( const FXIconItem *item1, const FXIconItem *item2 );
	/** \brief Sort descending by file modification time. */
	static FXint fSortModTimeDesc( const FXIconItem *item1, const FXIconItem *item2 );
	/*@}*/
};

#endif
