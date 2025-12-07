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
	FXDECLARE(igdeNativeFoxVFSDirectoryBox)
	
protected:
	igdeNativeFoxVFSDirectoryBox();
	
private:
	deVirtualFileSystem *pVFS;
	
	FXIcon *pIconFolder;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new virtual file system list. */
	igdeNativeFoxVFSDirectoryBox(deVirtualFileSystem *vfs, FXComposite *p,FXObject* tgt=NULL,FXSelector sel=0,FXuint opts=FRAME_SUNKEN|FRAME_THICK|TREELISTBOX_NORMAL,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=DEFAULT_PAD,FXint pr=DEFAULT_PAD,FXint pt=DEFAULT_PAD,FXint pb=DEFAULT_PAD);
	/** Cleans up the virtual file system list. */
	virtual ~igdeNativeFoxVFSDirectoryBox();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the virtual file system. */
	inline deVirtualFileSystem *GetVirtualFileSystem() const{ return pVFS; }
	
	/** Retrieves the current directory. */
	void GetPath(decPath &path);
	/** Sets the current directory. */
	void SetPath(const decPath &path);
	
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
	FXTreeItem *pBuildListFrom(const decPath &path);
	void pGetPathFor(decPath &path, FXTreeItem *item);
};

// end of include only once
#endif
