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

#include <stdint.h>

#include "igdeNativeFoxVFSList.h"
#include "igdeNativeFoxVFSListItem.h"
#include "../foxIcons.h"
#include "../../../resources/igdeIcon.h"
#include "../../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deCollectDirectorySearchVisitor.h>
#include <dragengine/filesystem/deCollectFileSearchVisitor.h>



// Events
///////////

FXDEFMAP( igdeNativeFoxVFSList ) igdeNativeFoxVFSListMap[] = {
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxVFSList::ID_HEADER, igdeNativeFoxVFSList::onHeaderClicked ),
};



// Class igdeNativeFoxVFSList
////////////////////

FXIMPLEMENT( igdeNativeFoxVFSList, FXIconList, igdeNativeFoxVFSListMap, ARRAYNUMBER( igdeNativeFoxVFSListMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxVFSList::igdeNativeFoxVFSList(){ }

igdeNativeFoxVFSList::igdeNativeFoxVFSList( igdeEnvironment &environment, deVirtualFileSystem *vfs,
FXComposite *p, FXObject* tgt, FXSelector sel, FXuint opts, FXint x, FXint y, FXint w, FXint h ) :
FXIconList( p, tgt, sel, opts, x, y, w, h ),
pEnvironment( &environment )
{
	if( ! vfs ){
		DETHROW( deeInvalidParam );
	}
	
	// prepare
	pVFS = vfs;
	pPattern = "*";
	pSorting = igdeNativeFoxVFSList::esNameAsc;
	
	// load icons
	pIconFolderBig = new FXGIFIcon( getApp(), FoxIcons::bigfolder );
	pIconFolderSmall = new FXGIFIcon( getApp(), FoxIcons::minifolder );
	pIconFileBig = new FXGIFIcon( getApp(), FoxIcons::bigdoc );
	pIconFileSmall = new FXGIFIcon( getApp(), FoxIcons::minidoc );
	
	pIconSortAsc = ( FXIcon* )environment.GetStockIcon( igdeEnvironment::esiSmallDown )->GetNativeIcon();
	pIconSortDesc = ( FXIcon* )environment.GetStockIcon( igdeEnvironment::esiSmallUp )->GetNativeIcon();
	
	// init
	appendHeader( "Name", pIconSortAsc, 300 );
	appendHeader( "Size", NULL, 100 );
	appendHeader( "Time", NULL, 120 );
	getHeader()->setTarget( this );
	getHeader()->setSelector( ID_HEADER );
	
	// sort function
	setSortFunc( igdeNativeFoxVFSListItem::fSortNameAsc );
}

igdeNativeFoxVFSList::~igdeNativeFoxVFSList(){
	if( pIconFolderBig ){
		delete pIconFolderBig;
	}
	if( pIconFolderSmall ){
		delete pIconFolderSmall;
	}
	if( pIconFileBig ){
		delete pIconFileBig;
	}
	if( pIconFileSmall ){
		delete pIconFileSmall;
	}
}



// Management
///////////////

class igdeNativeFoxVFSListFind : public deFileSearchVisitor{
private:
	igdeNativeFoxVFSList &pOwner;
	deObjectReference pSkin;
	const char * const pColumnPattern;
	
public:
	igdeNativeFoxVFSListFind( igdeNativeFoxVFSList &powner, const char * const columnPattern ) :
	pOwner( powner ), pColumnPattern( columnPattern ){
	}
	
	virtual bool VisitFile( const deVirtualFileSystem &vfs, const decPath &path ){
		if( ! path.GetPathUnix().MatchesPattern( pOwner.GetPattern() ) ){
			return true;
		}
		
		try{
			const uint64_t fileSize = vfs.GetFileSize( path );
			const TIME_SYSTEM fileMTime = vfs.GetFileModificationTime( path );
			igdeNativeFoxVFSListItem * const item = new igdeNativeFoxVFSListItem(
				path, "", pOwner.GetIconFileBig(), pOwner.GetIconFileSmall() );
			item->SetFileName( path.GetLastComponent() );
			item->SetFileType( deVFSContainer::eftRegularFile );
			item->SetFileSize( ( int )fileSize );
			item->SetFileModificationTime( fileMTime );
			item->UpdateText( pColumnPattern );
			pOwner.appendItem( item );
			
		}catch( const deException & ){
		}
		
		return true;
	}
	
	virtual bool VisitDirectory( const deVirtualFileSystem &, const decPath &path ){
		try{
			igdeNativeFoxVFSListItem * const item = new igdeNativeFoxVFSListItem(
				path, "", pOwner.GetIconFolderBig(), pOwner.GetIconFolderSmall() );
			item->SetFileName( path.GetLastComponent() );
			item->SetFileType( deVFSContainer::eftDirectory );
			item->UpdateText( pColumnPattern );
			pOwner.appendItem( item );
			
		}catch( const deException & ){
		}
		
		return true;
	}
	
	virtual bool VisitSpecial( const deVirtualFileSystem &vfs, const decPath &path ){
		if( ! path.GetPathUnix().MatchesPattern( pOwner.GetPattern() ) ){
			return true;
		}
		
		try{
			const uint64_t fileSize = vfs.GetFileSize( path );
			const TIME_SYSTEM fileMTime = vfs.GetFileModificationTime( path );
			igdeNativeFoxVFSListItem * const item = new igdeNativeFoxVFSListItem(
				path, "", pOwner.GetIconFileBig(), pOwner.GetIconFileSmall() );
			item->SetFileName( path.GetLastComponent() );
			item->SetFileType( deVFSContainer::eftSpecial );
			item->SetFileSize( ( int )fileSize );
			item->SetFileModificationTime( fileMTime );
			item->UpdateText( pColumnPattern );
			pOwner.appendItem( item );
			
		}catch( const deException & ){
		}
		
		return true;
	}
};

void igdeNativeFoxVFSList::UpdateListing(){
	const char * const columnPattern = "nsm"; // name, size, modification time
	igdeNativeFoxVFSListItem *item;
	
	// clear list
	clearItems();
	
	// add the default entries
	item = new igdeNativeFoxVFSListItem( pCurPath, "", pIconFolderBig, pIconFolderSmall );
	item->SetFileName( "." );
	item->SetFileType( deVFSContainer::eftDirectory );
	item->UpdateText( columnPattern );
	appendItem( item );
	
	if( pCurPath.GetComponentCount() > 0 ){
		decPath path;
		path.SetFrom( pCurPath );
		path.RemoveLastComponent();
		item = new igdeNativeFoxVFSListItem( path, "", pIconFolderBig, pIconFolderSmall );
		item->SetFileName( ".." );
		item->SetFileType( deVFSContainer::eftDirectory );
		item->UpdateText( columnPattern );
		appendItem( item );
	}
	
	// add directories and files
	igdeNativeFoxVFSListFind visitor( *this, columnPattern );
	pVFS->SearchFiles( pCurPath, visitor );
	
	// sort
	sortItems();
}

void igdeNativeFoxVFSList::SetPath( const decPath &path ){
	if( path != pCurPath ){
		pCurPath.SetFrom( path );
		UpdateListing();
	}
}

void igdeNativeFoxVFSList::SetPattern( const char *pattern ){
	if( ! pattern ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPattern != pattern ){
		pPattern = pattern;
		UpdateListing();
	}
}

void igdeNativeFoxVFSList::SetSorting( igdeNativeFoxVFSList::eSorting sorting ){
	if( sorting != pSorting ){
		pSorting = sorting;
		
		setHeaderIcon( 0, NULL ); // name
		setHeaderIcon( 1, NULL ); // size
		setHeaderIcon( 2, NULL ); // modification time
		
		if( pSorting == igdeNativeFoxVFSList::esNameAsc ){
			setSortFunc( igdeNativeFoxVFSListItem::fSortNameAsc );
			setHeaderIcon( 0, pIconSortAsc );
			
		}else if( pSorting == igdeNativeFoxVFSList::esNameDesc ){
			setSortFunc( igdeNativeFoxVFSListItem::fSortNameDesc );
			setHeaderIcon( 0, pIconSortDesc );
			
		}else if( pSorting == igdeNativeFoxVFSList::esSizeAsc ){
			setSortFunc( igdeNativeFoxVFSListItem::fSortSizeAsc );
			setHeaderIcon( 1, pIconSortAsc );
			
		}else if( pSorting == igdeNativeFoxVFSList::esSizeDesc ){
			setSortFunc( igdeNativeFoxVFSListItem::fSortSizeDesc );
			setHeaderIcon( 1, pIconSortDesc );
			
		}else if( pSorting == igdeNativeFoxVFSList::esModifyTimeAsc ){
			setSortFunc( igdeNativeFoxVFSListItem::fSortModTimeAsc );
			setHeaderIcon( 2, pIconSortAsc );
			
		}else if( pSorting == igdeNativeFoxVFSList::esModifyTimeDesc ){
			setSortFunc( igdeNativeFoxVFSListItem::fSortModTimeDesc );
			setHeaderIcon( 2, pIconSortDesc );
		}
		
		sortItems();
	}
}

igdeNativeFoxVFSListItem *igdeNativeFoxVFSList::GetSelectedItem() const{
	const int index = getCurrentItem();
	
	if( index < 0 ){
		return NULL;
		
	}else{
		return ( igdeNativeFoxVFSListItem* )getItem( index );
	}
}



void igdeNativeFoxVFSList::create(){
	FXIconList::create();
	
	pIconFolderBig->create();
	pIconFolderSmall->create();
	pIconFileBig->create();
	pIconFileSmall->create();
	
	UpdateListing();
}

void igdeNativeFoxVFSList::detach(){
	FXIconList::detach();
	
	pIconFolderBig->detach();
	pIconFolderSmall->detach();
	pIconFileBig->detach();
	pIconFileSmall->detach();
}

void igdeNativeFoxVFSList::destroy(){
	FXIconList::destroy();
	
	pIconFolderBig->destroy();
	pIconFolderSmall->destroy();
	pIconFileBig->destroy();
	pIconFileSmall->destroy();
}



// Events
///////////

long igdeNativeFoxVFSList::onHeaderClicked( FXObject*, FXSelector, void *pdata ){
	const int column = ( int )( intptr_t )pdata;
	
	if( column == 0 ){ // sort by name
		if( pSorting == esNameAsc ){
			SetSorting( esNameDesc );
			
		}else{
			SetSorting( esNameAsc );
		}
		
	}else if( column == 1 ){ // sort by size
		if( pSorting == esSizeAsc){
			SetSorting( esSizeDesc );
			
		}else{
			SetSorting( esSizeAsc );
		}
		
	}else if( column == 2 ){ // sort by modification time
		if( pSorting == esModifyTimeAsc ){
			SetSorting( esModifyTimeDesc );
			
		}else{
			SetSorting( esModifyTimeAsc );
		}
	}
	
	return 1;
}

#endif
