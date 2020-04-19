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

#ifndef _IGDENATIVEFOXVFSDIRECTORYBOX_H_
#define _IGDENATIVEFOXVFSDIRECTORYBOX_H_

#include "../foxtoolkit.h"

#include <dragengine/common/file/decPath.h>

class deVirtualFileSystem;



/**
 * \brief Virtual File System Directory Box.
 * 
 * Shows a path in a virtual file system as a combo box.
 */
class igdeNativeFoxVFSDirectoryBox : public FXTreeListBox{
	FXDECLARE( igdeNativeFoxVFSDirectoryBox )
	
protected:
	igdeNativeFoxVFSDirectoryBox();
	
private:
	deVirtualFileSystem *pVFS;
	
	FXIcon *pIconFolder;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new virtual file system list. */
	igdeNativeFoxVFSDirectoryBox( deVirtualFileSystem *vfs, FXComposite *p,FXObject* tgt=NULL,FXSelector sel=0,FXuint opts=FRAME_SUNKEN|FRAME_THICK|TREELISTBOX_NORMAL,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=DEFAULT_PAD,FXint pr=DEFAULT_PAD,FXint pt=DEFAULT_PAD,FXint pb=DEFAULT_PAD );
	/** Cleans up the virtual file system list. */
	virtual ~igdeNativeFoxVFSDirectoryBox();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the virtual file system. */
	inline deVirtualFileSystem *GetVirtualFileSystem() const{ return pVFS; }
	
	/** Retrieves the current directory. */
	void GetPath( decPath &path );
	/** Sets the current directory. */
	void SetPath( const decPath &path );
	
	/** Create server-side resources. */
	virtual void create();
	/** Detach server-side resources. */
	virtual void detach();
	/** Destroy server-side resources. */
	virtual void destroy();
	/*@}*/
	
	/** \name Events */
	/*@{*/
	/*@}*/
	
private:
	FXTreeItem *pBuildListFrom( const decPath &path );
	void pGetPathFor( decPath &path, FXTreeItem *item );
};

// end of include only once
#endif
