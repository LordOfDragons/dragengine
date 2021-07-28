/* 
 * Drag[en]gine GUI Launcher
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "deglPanelEngine.h"
#include "deglPEListItemModule.h"
#include "deglDialogModuleProps.h"
#include "../deglWindowMain.h"
#include "../../deglLauncher.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/deModuleSystem.h>



// Events
///////////

FXDEFMAP( deglPanelEngine ) deglPanelEngineMap[]={
	FXMAPFUNC( SEL_COMMAND, deglPanelEngine::ID_LIST_MODULES, deglPanelEngine::onListModulesChanged ),
	FXMAPFUNC( SEL_CHANGED, deglPanelEngine::ID_LIST_MODULES, deglPanelEngine::onListModulesChanged ),
	FXMAPFUNC( SEL_RIGHTBUTTONPRESS, deglPanelEngine::ID_LIST_MODULES, deglPanelEngine::onListModulesRDown ),
	FXMAPFUNC( SEL_RIGHTBUTTONRELEASE, deglPanelEngine::ID_LIST_MODULES, deglPanelEngine::onListModulesRUp ),
	FXMAPFUNC( SEL_DOUBLECLICKED, deglPanelEngine::ID_LIST_MODULES, deglPanelEngine::onListModulesDblClick ),
	FXMAPFUNC( deglPanelEngine::ExtIconList::SEL_HEADER_CLICKED, deglPanelEngine::ID_LIST_MODULES,
		deglPanelEngine::onListModulesHeaderClicked ),
	
	FXMAPFUNC( SEL_COMMAND, deglPanelEngine::ID_PU_MODULE_PROPS, deglPanelEngine::onPUModuleProps ),
};

FXDEFMAP( deglPanelEngine::ExtIconList ) deglPanelEngineExtIconListMap[] = {
	FXMAPFUNC( SEL_COMMAND, ICONLIST_HEADER_ID, deglPanelEngine::ExtIconList::onHeaderClicked )
};



// Class deglPanelEngine::ExtIconList
///////////////////////////////////////

FXIMPLEMENT( deglPanelEngine::ExtIconList, FXIconList, deglPanelEngineExtIconListMap,
	ARRAYNUMBER( deglPanelEngineExtIconListMap ) )

deglPanelEngine::ExtIconList::ExtIconList(){
}

deglPanelEngine::ExtIconList::ExtIconList( FXComposite *p, FXObject *tgt, FXSelector sel,
FXuint opts, FXint x, FXint y, FXint w, FXint h ) :
FXIconList( p, tgt, sel, opts, x, y, w, h ){
}

deglPanelEngine::ExtIconList::~ExtIconList(){
}

long deglPanelEngine::ExtIconList::onHeaderClicked( FXObject*, FXSelector, void* ){
	if( target ){
		return target->tryHandle( this, FXSEL( SEL_HEADER_CLICKED, message ), nullptr );
	}
	return 0;
}



// Class deglPanelEngine
//////////////////////////

FXIMPLEMENT( deglPanelEngine, FXVerticalFrame, deglPanelEngineMap, ARRAYNUMBER( deglPanelEngineMap ) )

// Constructor, destructor
////////////////////////////

deglPanelEngine::deglPanelEngine(){ }

deglPanelEngine::deglPanelEngine( deglWindowMain *windowMain, FXComposite *container ) :
FXVerticalFrame( container, LAYOUT_FILL_Y | LAYOUT_SIDE_LEFT | FRAME_SUNKEN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ),
pWindowMain( windowMain ),
pSortListModules( elmsNameAsc )
{
	pListModules = new ExtIconList( this, this,
		ID_LIST_MODULES, FOLDINGLIST_BROWSESELECT | LAYOUT_FILL_X | LAYOUT_FILL_Y );
	pListModules->appendHeader( "Name", nullptr, 200 );
	pListModules->appendHeader( "Type", nullptr, 150 );
	pListModules->appendHeader( "Version", nullptr, 60 );
	pListModules->appendHeader( "Status", nullptr, 150 );
	
	pListModules->setSortFunc( deglPanelEngine::SortModulesByNameAsc );
}

deglPanelEngine::~deglPanelEngine(){
}



// Management
///////////////

delEngineModule *deglPanelEngine::GetSelectedModule() const{
	int selection = pListModules->getCurrentItem();
	
	if( selection != -1 ){
		return ( ( deglPEListItemModule* )pListModules->getItem( selection ) )->GetModule();
	}
	
	return nullptr;
}



void deglPanelEngine::UpdateModuleList(){
	const delEngineModuleList &modules = pWindowMain->GetLauncher()->GetEngine().GetModules();
	int i, count = modules.GetCount();
	deglPEListItemModule *listItem;
	delEngineModule *module;
	
	pListModules->clearItems();
	
	for( i=0; i<count; i++ ){
		module = modules.GetAt( i );
		listItem = nullptr;
		
		try{
			listItem = new deglPEListItemModule( this, module );
			pListModules->appendItem( listItem );
			
		}catch( const deException & ){
			if( listItem ) delete listItem;
			throw;
		}
	}
	
	if( count > 0 ){
		pListModules->sortItems();
		pListModules->setCurrentItem( 0 );
	}
}



FXint deglPanelEngine::SortModulesByNameAsc( const FXIconItem *item1, const FXIconItem *item2 ){
	const delEngineModule &module1 = *( ( ( deglPEListItemModule* )item1 )->GetModule() );
	const delEngineModule &module2 = *( ( ( deglPEListItemModule* )item2 )->GetModule() );
	return module1.GetName().Compare( module2.GetName() );
}

FXint deglPanelEngine::SortModulesByNameDesc( const FXIconItem *item1, const FXIconItem *item2 ){
	const delEngineModule &module1 = *( ( ( deglPEListItemModule* )item1 )->GetModule() );
	const delEngineModule &module2 = *( ( ( deglPEListItemModule* )item2 )->GetModule() );
	return module2.GetName().Compare( module1.GetName() );
}

FXint deglPanelEngine::SortModulesByTypeAsc( const FXIconItem *item1, const FXIconItem *item2 ){
	const deglPEListItemModule &mitem1 = *( ( deglPEListItemModule* )item1 );
	const deglPEListItemModule &mitem2 = *( ( deglPEListItemModule* )item2 );
	return mitem1.GetTypeString().Compare( mitem2.GetTypeString() );
}

FXint deglPanelEngine::SortModulesByTypeDesc( const FXIconItem *item1, const FXIconItem *item2 ){
	const deglPEListItemModule &mitem1 = *( ( deglPEListItemModule* )item1 );
	const deglPEListItemModule &mitem2 = *( ( deglPEListItemModule* )item2 );
	return mitem2.GetTypeString().Compare( mitem1.GetTypeString() );
}

FXint deglPanelEngine::SortModulesByVersionAsc( const FXIconItem *item1, const FXIconItem *item2 ){
	const delEngineModule &module1 = *( ( ( deglPEListItemModule* )item1 )->GetModule() );
	const delEngineModule &module2 = *( ( ( deglPEListItemModule* )item2 )->GetModule() );
	return deModuleSystem::CompareVersion( module1.GetVersion(), module2.GetVersion() );
}

FXint deglPanelEngine::SortModulesByVersionDesc( const FXIconItem *item1, const FXIconItem *item2 ){
	const delEngineModule &module1 = *( ( ( deglPEListItemModule* )item1 )->GetModule() );
	const delEngineModule &module2 = *( ( ( deglPEListItemModule* )item2 )->GetModule() );
	return deModuleSystem::CompareVersion( module2.GetVersion(), module1.GetVersion() );
}

FXint deglPanelEngine::SortModulesByStatusAsc( const FXIconItem *item1, const FXIconItem *item2 ){
	const deglPEListItemModule &mitem1 = *( ( deglPEListItemModule* )item1 );
	const deglPEListItemModule &mitem2 = *( ( deglPEListItemModule* )item2 );
	return mitem1.GetStatusString().Compare( mitem2.GetStatusString() );
}

FXint deglPanelEngine::SortModulesByStatusDesc( const FXIconItem *item1, const FXIconItem *item2 ){
	const deglPEListItemModule &mitem1 = *( ( deglPEListItemModule* )item1 );
	const deglPEListItemModule &mitem2 = *( ( deglPEListItemModule* )item2 );
	return mitem2.GetStatusString().Compare( mitem1.GetStatusString() );
}



// Events
///////////

long deglPanelEngine::onListModulesChanged( FXObject*, FXSelector, void* ){
	return 1;
}

long deglPanelEngine::onListModulesRDown( FXObject*, FXSelector, void *data ){
	const FXEvent &event = *( ( const FXEvent * )data );
	delEngineModule *module = nullptr;
	FXMenuPane *popup = nullptr;
	int x = event.root_x;
	int y = event.root_y;
	
	pListModules->setCurrentItem( pListModules->getItemAt( event.win_x, event.win_y ) );
	
	module = GetSelectedModule();
	
	if( module ){
		try{
			popup = new FXMenuPane( this );
			if( ! popup ) DETHROW( deeOutOfMemory );
			
			if( ! new FXMenuCommand( popup, "Properties...", nullptr, this, ID_PU_MODULE_PROPS ) ) DETHROW( deeOutOfMemory );
			
			popup->create();
			
			popup->popup( nullptr, x + 1, y + 1 ); // popup-bug. do not show straight under the cursor
			pWindowMain->getApp()->runModalWhileShown( popup );
			
			delete popup;
			
		}catch( const deException &e ){
			if( popup ) delete popup;
			pWindowMain->DisplayException( e );
		}
	}
	
	return 1;
	
	//pListModules->translateCoordinatesTo( x, y, pWindowMain->getApp()->getRootWindow(), event. );
}

long deglPanelEngine::onListModulesRUp( FXObject*, FXSelector, void* ){
	return 1;
}

long deglPanelEngine::onListModulesDblClick( FXObject *sender, FXSelector selector, void *data ){
	return onPUModuleProps( sender, selector, data );
}

long deglPanelEngine::onListModulesHeaderClicked( FXObject*, FXSelector, void *data ){
	int colon = ( intptr_t )data;
	
	if( colon == 0 ){
		if( pSortListModules == elmsNameAsc ){
			pSortListModules = elmsNameDesc;
			pListModules->setSortFunc( deglPanelEngine::SortModulesByNameDesc );
			
		}else{
			pSortListModules = elmsNameAsc;
			pListModules->setSortFunc( deglPanelEngine::SortModulesByNameAsc );
		}
		
		pListModules->sortItems();
		
	}else if( colon == 1 ){
		if( pSortListModules == elmsTypeAsc ){
			pSortListModules = elmsTypeDesc;
			pListModules->setSortFunc( deglPanelEngine::SortModulesByTypeDesc );
			
		}else{
			pSortListModules = elmsTypeAsc;
			pListModules->setSortFunc( deglPanelEngine::SortModulesByTypeAsc );
		}
		
		pListModules->sortItems();
		
	}else if( colon == 2 ){
		if( pSortListModules == elmsVersionAsc ){
			pSortListModules = elmsVersionDesc;
			pListModules->setSortFunc( deglPanelEngine::SortModulesByVersionDesc );
			
		}else{
			pSortListModules = elmsVersionAsc;
			pListModules->setSortFunc( deglPanelEngine::SortModulesByVersionAsc );
		}
		
		pListModules->sortItems();
		
	}else if( colon == 3 ){
		if( pSortListModules == elmsStatusAsc ){
			pSortListModules = elmsStatusDesc;
			pListModules->setSortFunc( deglPanelEngine::SortModulesByStatusDesc );
			
		}else{
			pSortListModules = elmsStatusAsc;
			pListModules->setSortFunc( deglPanelEngine::SortModulesByStatusAsc );
		}
		
		pListModules->sortItems();
	}
	
	return 1;
}

long deglPanelEngine::onPUModuleProps( FXObject*, FXSelector, void* ){
	delEngineModule *module = GetSelectedModule();
	
	if( module ){
		deglDialogModuleProps dialog( pWindowMain, module, pWindowMain );
		
		dialog.execute( PLACEMENT_OWNER );
	}
	
	return 1;
}
