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

#ifdef IGDE_TOOLKIT_FOX

#include "igdeNativeFoxVFSDirectoryBox.h"
#include "../foxIcons.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>



// Class igdeNativeFoxVFSDirectoryBox
////////////////////

FXIMPLEMENT( igdeNativeFoxVFSDirectoryBox, FXTreeListBox, nullptr, 0 )

// Constructor, destructor
////////////////////////////

igdeNativeFoxVFSDirectoryBox::igdeNativeFoxVFSDirectoryBox(){ }

igdeNativeFoxVFSDirectoryBox::igdeNativeFoxVFSDirectoryBox( deVirtualFileSystem *vfs, FXComposite *p,
FXObject* tgt, FXSelector sel, FXuint opts, FXint x, FXint y, FXint w, FXint h,
FXint pl, FXint pr, FXint pt, FXint pb ) :
FXTreeListBox( p,tgt,sel,opts,x,y,w,h, pl,pr,pt,pb )
{
	if( ! vfs ) DETHROW( deeInvalidParam );
	
	// prepare
	pVFS = vfs;
	
	// init
	pIconFolder = new FXGIFIcon( getApp(), FoxIcons::minifolder );
	
	// update
	setCurrentItem( appendItem( NULL, "/", pIconFolder, pIconFolder ) );
}

igdeNativeFoxVFSDirectoryBox::~igdeNativeFoxVFSDirectoryBox(){
	if( pIconFolder ) delete pIconFolder;
}



// Management
///////////////

void igdeNativeFoxVFSDirectoryBox::GetPath( decPath &path ){
	FXTreeItem *item = 	getCurrentItem();
	
	path.SetEmpty();
	
	if( item ){
		pGetPathFor( path, item );
		
	}else{
		path.SetPrefix( "/" );
	}
}

void igdeNativeFoxVFSDirectoryBox::SetPath( const decPath &path ){
	setCurrentItem( pBuildListFrom( path ) );
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

FXTreeItem *igdeNativeFoxVFSDirectoryBox::pBuildListFrom( const decPath &path ){
	int c, componentCount = path.GetComponentCount();
	FXTreeItem *item = NULL;
	
	// clear list
	clearItems();
	
	// add root
	item = appendItem( NULL, "/", pIconFolder, pIconFolder );
	
	// add the path
	for( c=0; c<componentCount; c++ ){
		item = appendItem( item, path.GetComponentAt( c ).GetString(), pIconFolder, pIconFolder );
	}
	
	// finished
	return item;
}

void igdeNativeFoxVFSDirectoryBox::pGetPathFor( decPath &path, FXTreeItem *item ){
	FXTreeItem *pparent = item->getParent();
	
	if( pparent ){
		pGetPathFor( path, pparent );
		path.AddComponent( item->getText().text() );
		
	}else{
		path.SetPrefix( item->getText().text() );
	}
	
}

#endif
