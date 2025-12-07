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

#ifdef IGDE_TOOLKIT_FOX

#include "igdeNativeFoxVFSDirectoryBox.h"
#include "../foxIcons.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>



// Class igdeNativeFoxVFSDirectoryBox
////////////////////

FXIMPLEMENT(igdeNativeFoxVFSDirectoryBox, FXTreeListBox, nullptr, 0)

// Constructor, destructor
////////////////////////////

igdeNativeFoxVFSDirectoryBox::igdeNativeFoxVFSDirectoryBox(){}

igdeNativeFoxVFSDirectoryBox::igdeNativeFoxVFSDirectoryBox(deVirtualFileSystem *vfs, FXComposite *p,
FXObject* tgt, FXSelector sel, FXuint opts, FXint x, FXint y, FXint w, FXint h,
FXint pl, FXint pr, FXint pt, FXint pb) :
FXTreeListBox(p,tgt,sel,opts,x,y,w,h, pl,pr,pt,pb)
{
	if(! vfs) DETHROW(deeInvalidParam);
	
	// prepare
	pVFS = vfs;
	
	// init
	pIconFolder = new FXGIFIcon(getApp(), FoxIcons::minifolder);
	
	// update
	setCurrentItem(appendItem(NULL, "/", pIconFolder, pIconFolder));
}

igdeNativeFoxVFSDirectoryBox::~igdeNativeFoxVFSDirectoryBox(){
	if(pIconFolder) delete pIconFolder;
}



// Management
///////////////

void igdeNativeFoxVFSDirectoryBox::GetPath(decPath &path){
	FXTreeItem *item = 	getCurrentItem();
	
	path.SetEmpty();
	
	if(item){
		pGetPathFor(path, item);
		
	}else{
		path.SetPrefix("/");
	}
}

void igdeNativeFoxVFSDirectoryBox::SetPath(const decPath &path){
	setCurrentItem(pBuildListFrom(path));
}



void igdeNativeFoxVFSDirectoryBox::create(){
	FXTreeListBox::create();
	
	pIconFolder->create();
}

void igdeNativeFoxVFSDirectoryBox::detach(){
	FXTreeListBox::detach();
	
	pIconFolder->detach();
}

void igdeNativeFoxVFSDirectoryBox::destroy(){
	FXTreeListBox::destroy();
	
	pIconFolder->destroy();
}



// Callbacks
//////////////




// Private Functions
//////////////////////

FXTreeItem *igdeNativeFoxVFSDirectoryBox::pBuildListFrom(const decPath &path){
	int c, componentCount = path.GetComponentCount();
	FXTreeItem *item = NULL;
	
	// clear list
	clearItems();
	
	// add root
	item = appendItem(NULL, "/", pIconFolder, pIconFolder);
	
	// add the path
	for(c=0; c<componentCount; c++){
		item = appendItem(item, path.GetComponentAt(c).GetString(), pIconFolder, pIconFolder);
	}
	
	// finished
	return item;
}

void igdeNativeFoxVFSDirectoryBox::pGetPathFor(decPath &path, FXTreeItem *item){
	FXTreeItem *pparent = item->getParent();
	
	if(pparent){
		pGetPathFor(path, pparent);
		path.AddComponent(item->getText().text());
		
	}else{
		path.SetPrefix(item->getText().text());
	}
	
}

#endif
