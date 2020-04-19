/* 
 * Drag[en]gine IGDE World Editor
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

#include "meWPBrowser.h"
#include "meWPBrowserListener.h"
#include "meWindowProperties.h"
#include "../meWindowMain.h"
#include "../../undosys/properties/decal/meUDecalSkin.h"
#include "../../undosys/properties/object/meUSetObjectClass.h"
#include "../../undosys/properties/object/texture/meUObjectTextureSetSkin.h"
#include "../../undosys/properties/object/texture/meUObjectAddTexture.h"
#include "../../world/meWorld.h"
#include "../../world/meWorldGuiParameters.h"
#include "../../world/decal/meDecal.h"
#include "../../world/decal/meDecalList.h"
#include "../../world/decal/meDecalSelection.h"
#include "../../world/object/meObject.h"
#include "../../world/object/meObjectList.h"
#include "../../world/object/meObjectSelection.h"
#include "../../world/object/texture/meObjectTexture.h"
#include "../../world/object/texture/meObjectTextureList.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGDCategory.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gamedefinition/class/igdeGDClassManager.h>
#include <deigde/gamedefinition/skin/igdeGDSkin.h>
#include <deigde/gamedefinition/skin/igdeGDSkinManager.h>
#include <deigde/gamedefinition/sky/igdeGDSky.h>
#include <deigde/gamedefinition/sky/igdeGDSkyManager.h>
#include <deigde/gamedefinition/preview/igdeGDPreviewManager.h>
#include <deigde/gamedefinition/visitor/igdeGDAddToListVisitor.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/igdeIconListBox.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/browse/igdeBrowseItemGDPreviewListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeIconListBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/event/igdeTreeListListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerSplitted.h>
#include <deigde/gui/layout/igdeContainerSplittedReference.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>
#include <deigde/gui/menu/igdeMenuCommand.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/model/igdeTreeItem.h>
#include <deigde/gui/resources/igdeIcon.h>
#include <deigde/gui/resources/igdeIconReference.h>
#include <deigde/gui/wrapper/igdeWSky.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>



// Actions
///////////

namespace{

class cComboType : public igdeComboBoxListener{
	meWPBrowser &pPanel;
	
public:
	cComboType( meWPBrowser &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox* ){
		if( pPanel.GetWorld() ){
			pPanel.UpdateCategoryList();
			pPanel.UpdateItemList();
		}
	}
};

class cActionSelectionMode : public igdeAction{
	meWPBrowser &pPanel;
	const meWPBrowser::eSelectionMode pMode;
	
public:
	cActionSelectionMode( meWPBrowser &panel, meWPBrowser::eSelectionMode mode,
	const char *text, igdeIcon *icon, const char *description ) :
		igdeAction( text, icon, description ), pPanel( panel ), pMode( mode ){ }
	
	virtual void OnAction(){
		pPanel.SetSelectionMode( pMode );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetWorld() );
		SetSelected( pPanel.GetSelectionMode() == pMode );
	}
};

class cTreeCategories : public igdeTreeListListener{
	meWPBrowser &pPanel;
	
public:
	cTreeCategories( meWPBrowser &panel ) : pPanel( panel ){}
	
	virtual void OnSelectionChanged( igdeTreeList* ){
		if( pPanel.GetWorld() ){
			pPanel.UpdateItemList();
		}
	}
};

class cTextFilter : public igdeTextFieldListener{
	meWPBrowser &pPanel;
	
public:
	cTextFilter( meWPBrowser &panel ) : pPanel( panel ){}
	
	virtual void OnTextChanged( igdeTextField* ){
		pPanel.UpdateItemList();
	}
	
	virtual void OnTextChanging( igdeTextField* ){
		pPanel.UpdateItemList();
	}
};

class cActionSetSkinObjTex : public igdeAction{
	meWPBrowser &pPanel;
	const decString pName;
	
public:
	cActionSetSkinObjTex( meWPBrowser &panel, const decString &name ) : igdeAction( name, NULL,
		"Set active object texture" ), pPanel( panel ), pName( name ){ }
	
	virtual void OnAction(){
		igdeGDSkin * const gdskin = pPanel.GetSelectedSkin();
		if( ! gdskin ){
			return;
		}
		
		meObject * const object = pPanel.GetWorld()->GetSelectionObject().GetActive();
		if( ! object ){
			return;
		}
		
		meObjectTexture *texture = object->GetTextureNamed( pName );
		const decString &newskin = gdskin->GetPath();
		igdeUndoReference undo;
		
		if( texture ){
			if( newskin.Equals( texture->GetSkinPath() ) ){
				return;
			}
			
			undo.TakeOver( new meUObjectTextureSetSkin( texture, newskin ) );
			pPanel.GetWorld()->GetUndoSystem()->Add( undo );
			
		}else{
			deObjectReference refTexture;
			refTexture.TakeOver( new meObjectTexture( object->GetEnvironment(), pName ) );
			texture = ( meObjectTexture* )( deObject* )refTexture;
			texture->SetSkinPath( newskin );
			
			undo.TakeOver( new meUObjectAddTexture( object, texture ) );
			pPanel.GetWorld()->GetUndoSystem()->Add( undo );
		}
	}
};

class cListItems : public igdeIconListBoxListener{
	meWPBrowser &pPanel;
	
public:
	cListItems( meWPBrowser &panel ) : pPanel( panel ){}
	
	virtual void OnSelectionChanged( igdeIconListBox* ){
		pPanel.CurrentItemChanged();
	}
	
	virtual void OnDoubleClickItem( igdeIconListBox*, int ){
		if( ! pPanel.GetWorld() ){
			return;
		}
		
		switch( pPanel.GetPreviewItemType() ){
		case meWPBrowser::epitObjectClass:
			pPanel.GetActionSetClass()->OnAction();
			break;
			
		case meWPBrowser::epitSkin:
			switch( pPanel.GetWorld()->GetGuiParameters().GetElementMode() ){
			case meWorldGuiParameters::eemObject:
				pPanel.GetActionSetSkin()->OnAction();
				break;
				
			case meWorldGuiParameters::eemDecal:
				pPanel.GetActionSetDecal()->OnAction();
				break;
				
			default:
				break;
			}
			break;
			
		case meWPBrowser::epitSky:
			break;
		}
	}
	
	virtual void AddContextMenuEntries( igdeIconListBox*, igdeMenuCascade &menu ){
		if( ! pPanel.GetWorld() ){
			return;
		}
		
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelper();
		
		switch( pPanel.GetPreviewItemType() ){
		case meWPBrowser::epitObjectClass:
			helper.MenuCommand( menu, pPanel.GetActionSetClass() );
			break;
			
		case meWPBrowser::epitSkin:
			helper.MenuCommand( menu, pPanel.GetActionSetSkin() );
			
			switch( pPanel.GetWorld()->GetGuiParameters().GetElementMode() ){
			case meWorldGuiParameters::eemObject:{
				const meObject * const object = pPanel.GetWorld()->GetSelectionObject().GetActive();
				
				if( object ){
					igdeMenuCascadeReference subMenu;
					subMenu.TakeOver( new igdeMenuCascade( pPanel.GetEnvironment(), "Set Object Texture" ) );
					
					decStringList names;
					object->GetModelTextureNameList( names );
					names.SortAscending();
					
					const int textureCount = names.GetCount();
					int i;
					for( i=0; i<textureCount; i++ ){
						helper.MenuCommand( subMenu, new cActionSetSkinObjTex( pPanel, names.GetAt( i ) ), true );
					}
					
					menu.AddChild( subMenu );
				}
				}break;
				
			case meWorldGuiParameters::eemDecal:
				helper.MenuCommand( menu, pPanel.GetActionSetDecal() );
				break;
				
			default:
				break;
			}
			break;
			
		case meWPBrowser::epitSky:
			helper.MenuCommand( menu, pPanel.GetActionSetSky() );
			break;
		}
		
		helper.MenuSeparator( menu );
		helper.MenuCommand( menu, pPanel.GetActionPIRebuild() );
		
		// view
		igdeMenuCascadeReference menuView;
		menuView.TakeOver( new igdeMenuCascade( helper.GetEnvironment(), "View" ) );
		
		helper.MenuOption( menuView, pPanel.GetActionPISizeSmall() );
		helper.MenuOption( menuView, pPanel.GetActionPISizeMedium() );
		helper.MenuOption( menuView, pPanel.GetActionPISizeLarge() );
		
		helper.MenuSeparator( menuView );
		helper.MenuOption( menuView, pPanel.GetActionPIViewList() );
		helper.MenuOption( menuView, pPanel.GetActionPIViewPreview() );
		
		helper.MenuSeparator( menu );
		menu.AddChild( menuView );
	}
};


class cActionSetClass : public igdeAction{
	meWPBrowser &pPanel;
	
public:
	cActionSetClass( meWPBrowser &panel ) : igdeAction( "Set Class", NULL,
		"Set class to selected objects" ), pPanel( panel ){ }
	
	virtual void OnAction(){
		const igdeGDClass * const gdclass = pPanel.GetSelectedObjectClass();
		if( ! gdclass || pPanel.GetWorld()->GetGuiParameters().GetElementMode() != meWorldGuiParameters::eemObject ){
			return;
		}
		
		const meObjectList &selection = pPanel.GetWorld()->GetSelectionObject().GetSelected();
		const decString &cname = gdclass->GetName();
		const int count = selection.GetCount();
		meObjectList list;
		int i;
		
		for( i=0; i<count; i++ ){
			meObject * const object = selection.GetAt( i );
			if( ! cname.Equals( object->GetClassName() ) ){
				list.Add( object );
			}
		}
		
		if( list.GetCount() == 0 ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new meUSetObjectClass( list, cname ) );
		pPanel.GetWorld()->GetUndoSystem()->Add( undo );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetSelectedObjectClass() && pPanel.GetWorld()->GetSelectionObject().GetSelected().GetCount() > 0
			&& pPanel.GetWorld()->GetGuiParameters().GetElementMode() == meWorldGuiParameters::eemObject );
	}
};

class cActionSetSkin : public igdeAction{
	meWPBrowser &pPanel;
	
public:
	cActionSetSkin( meWPBrowser &panel ) : igdeAction( "Set Skin", NULL,
		"Set skin to selected objects active textures" ), pPanel( panel ){ }
	
	virtual void OnAction(){
		const igdeGDSkin * const gdskin = pPanel.GetSelectedSkin();
		if( ! gdskin || pPanel.GetWorld()->GetGuiParameters().GetElementMode() != meWorldGuiParameters::eemObject ){
			return;
		}
		
		const meObjectList &selection = pPanel.GetWorld()->GetSelectionObject().GetSelected();
		const decString &newskin = gdskin->GetPath();
		const int count = selection.GetCount();
		meObjectTextureList list;
		int i;
		
		for( i=0; i<count; i++ ){
			meObjectTexture * const texture = selection.GetAt( i )->GetActiveTexture();
			if( texture && ! newskin.Equals( texture->GetSkinPath() ) ){
				list.AddTexture( texture );
			}
		}
		
		if( list.GetTextureCount() == 0 ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new meUObjectTextureSetSkin( list, newskin ) );
		pPanel.GetWorld()->GetUndoSystem()->Add( undo );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetSelectedSkin() && pPanel.GetWorld()->GetSelectionObject().GetSelected().GetCount() > 0
			&& pPanel.GetWorld()->GetGuiParameters().GetElementMode() == meWorldGuiParameters::eemObject );
	}
};

class cActionSetDecal : public igdeAction{
	meWPBrowser &pPanel;
	
public:
	cActionSetDecal( meWPBrowser &panel ) : igdeAction( "Set Skin", NULL,
		"Set skin to selected decals" ), pPanel( panel ){ }
	
	virtual void OnAction(){
		const igdeGDSkin * const gdskin = pPanel.GetSelectedSkin();
		if( ! gdskin || pPanel.GetWorld()->GetGuiParameters().GetElementMode() != meWorldGuiParameters::eemDecal ){
			return;
		}
		
		const meDecalList &selection = pPanel.GetWorld()->GetSelectionDecal().GetSelected();
		const decString &newskin = gdskin->GetPath();
		const int count = selection.GetCount();
		meDecalList list;
		int i;
		
		for( i=0; i<count; i++ ){
			meDecal * const decal = selection.GetAt( i );
			if( ! newskin.Equals( decal->GetSkinPath() ) ){
				list.Add( decal );
			}
		}
		
		if( list.GetCount() == 0 ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new meUDecalSkin( list, newskin ) );
		pPanel.GetWorld()->GetUndoSystem()->Add( undo );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetSelectedSkin() && pPanel.GetWorld()->GetSelectionDecal().GetSelected().GetCount() > 0
			&& pPanel.GetWorld()->GetGuiParameters().GetElementMode() == meWorldGuiParameters::eemDecal );
	}
};

class cActionSetSky : public igdeAction{
	meWPBrowser &pPanel;
	
public:
	cActionSetSky( meWPBrowser &panel ) : igdeAction( "Set Sky", NULL,
		"Change preview sky of world to the selected sky" ), pPanel( panel ){ }
	
	virtual void OnAction(){
		igdeGDSky * const gdsky = pPanel.GetSelectedSky();
		if( gdsky ){
			pPanel.GetWorld()->GetSky()->SetGDSky( gdsky );
		}
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetSelectedSky() );
	}
};

class cActionPreviewSize : public igdeAction{
	meWPBrowser &pPanel;
	const meWPBrowser::ePreviewSize pSize;
	
public:
	cActionPreviewSize( meWPBrowser &panel, meWPBrowser::ePreviewSize size,
	const char *text, igdeIcon *icon, const char *description ) :
		igdeAction( text, icon, description ), pPanel( panel ), pSize( size ){ }
	
	virtual void OnAction(){
		pPanel.SetPreviewSize( pSize );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetWorld() );
		SetSelected( pPanel.GetPreviewSize() == pSize );
	}
};

class cActionViewMode : public igdeAction{
	meWPBrowser &pPanel;
	const meWPBrowser::eViewModes pMode;
	
public:
	cActionViewMode( meWPBrowser &panel, meWPBrowser::eViewModes mode,
	const char *text, igdeIcon *icon, const char *description ) :
		igdeAction( text, icon, description ), pPanel( panel ), pMode( mode ){ }
	
	virtual void OnAction(){
		pPanel.SetViewMode( pMode );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetWorld() );
		SetSelected( pPanel.GetViewMode() == pMode );
	}
};

class cActionPIRebuild : public igdeAction{
	meWPBrowser &pPanel;
	
public:
	cActionPIRebuild( meWPBrowser &panel ) : igdeAction( "Rebuild Preview", NULL,
		"Rebuild Preview" ), pPanel( panel ){ }
	
	virtual void OnAction(){
		pPanel.RebuildPISelectedItem();
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetWorld() && pPanel.GetSelectedListItem() );
	}
};

}



// Class meWPBrowser
//////////////////////

// Constructor, destructor
////////////////////////////

meWPBrowser::meWPBrowser( meWindowProperties &windowProperties ) :
igdeContainerBox( windowProperties.GetEnvironment(), igdeContainerBox::eaY ),
pWindowProperties( windowProperties ),
pListener( NULL ),
pWorld( NULL ),
pPreviewSize( epsLarge ),
pSelectionMode( esmCategory ),
pViewMode( evmPreview )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerSplittedReference content;
	igdeContainerReference groupBox, form, frameLine;
	
	
	pListener = new meWPBrowserListener( *this );
	
	
	pActionSetClass.TakeOver( new cActionSetClass( *this ) );
	pActionSetSkin.TakeOver( new cActionSetSkin( *this ) );
	pActionSetDecal.TakeOver( new cActionSetDecal( *this ) );
	pActionSetSky.TakeOver( new cActionSetSky( *this ) );
	
	pActionPISizeSmall.TakeOver( new cActionPreviewSize( *this, epsSmall,
		"Small Preview Image", NULL, "Small Preview Image" ) );
	pActionPISizeMedium.TakeOver( new cActionPreviewSize( *this, epsMedium,
		"Medium Preview Image", NULL, "Medium Preview Image" ) );
	pActionPISizeLarge.TakeOver( new cActionPreviewSize( *this, epsLarge,
		"Large Preview Image", NULL, "Large Preview Image" ) );
	
	pActionPIViewList.TakeOver( new cActionViewMode( *this, evmList,
		"List Mode", NULL, "List Mode" ) );
	pActionPIViewPreview.TakeOver( new cActionViewMode( *this, evmPreview,
		"Preview Mode", NULL, "Preview Mode" ) );
	
	pActionPIRebuild.TakeOver( new cActionPIRebuild( *this ) );
	
	
	content.TakeOver( new igdeContainerSplitted( env, igdeContainerSplitted::espTop, 400 ) );
	AddChild( content );
	
	
	// browser
	frameLine.TakeOver( new igdeGroupBox( env, "Browser:" ) );
	( ( igdeGroupBox& )( igdeWidget& )frameLine ).SetCanCollapse( false );
	( ( igdeGroupBox& )( igdeWidget& )frameLine ).SetStretchLast( true );
	content->AddChild( frameLine, igdeContainerSplitted::eaSide );
	groupBox.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY, igdeContainerFlow::esLast ) );
	frameLine->AddChild( groupBox );
	
	frameLine.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst ) );
	groupBox->AddChild( frameLine );
	
	helper.ComboBox( frameLine, "Type of items to browse", pCBTypes, new cComboType( *this ) );
	pCBTypes->AddItem( "Object Class", NULL, ( void* )( intptr_t )epitObjectClass );
	pCBTypes->AddItem( "Skin", NULL, ( void* )( intptr_t )epitSkin );
	pCBTypes->AddItem( "Sky", NULL, ( void* )( intptr_t )epitSky );
	
	helper.Button( frameLine, pBtnSelByCat, new cActionSelectionMode( *this, esmCategory,
		"Category", NULL, "Browse by category" ), true );
	helper.Button( frameLine, pBtnSelByFilter, new cActionSelectionMode( *this, esmFilter,
		"Filter", NULL, "Browse by filtering" ), true );
	
	
	pSwitcherSelBy.TakeOver( new igdeSwitcher( env ) );
	groupBox->AddChild( pSwitcherSelBy );
	
	
	// category tree
	helper.TreeList( pSwitcherSelBy, pTreeCategories, 10, "Categories", new cTreeCategories( *this ) );
	pTreeCategories->SetDefaultSorter();
	
	
	// filter
	form.TakeOver( new igdeContainerForm( env ) );
	pSwitcherSelBy->AddChild( form );
	
	helper.EditString( form, "Filter:", "Show items containing filter case insensitive",
		pEditFilter, new cTextFilter( *this ) );
	
	
	// items
	frameLine.TakeOver( new igdeGroupBox( env, "Items:" ) );
	( ( igdeGroupBox& )( igdeWidget& )frameLine ).SetCanCollapse( false );
	( ( igdeGroupBox& )( igdeWidget& )frameLine ).SetStretchLast( true );
	content->AddChild( frameLine, igdeContainerSplitted::eaCenter );
	groupBox.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY, igdeContainerFlow::esFirst ) );
	frameLine->AddChild( groupBox );
	
	const igdeUIHelper::sColumnHeader headers[] = {
		igdeUIHelper::sColumnHeader( "Name", NULL, 200 )
	};
	helper.IconListBox( groupBox, pListItems, 10, headers, 1, "Items", new cListItems( *this ) );
	pListItems->SetDefaultSorter();
	pListItems->SetViewMode( igdeIconListBox::evmIconVertical );
	
	helper.EditString( groupBox, "Item information", pEditInfos, 50, 5, NULL );
	pEditInfos->SetEditable( false );
}

meWPBrowser::~meWPBrowser(){
	SetWorld( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void meWPBrowser::SetWorld( meWorld *world ){
	if( world == pWorld ){
		return;
	}
	
	if( pWorld ){
		pWorld->RemoveNotifier( pListener );
		pWorld->FreeReference();
	}
	
	pWorld = world;
	
	if( world ){
		world->AddNotifier( pListener );
		world->AddReference();
	}
	
	UpdateCategoryList();
	UpdateItemList();
}



void meWPBrowser::UpdateCategoryList(){
	const igdeGameDefinition * const gameDefinition = pWorld ? pWorld->GetGameDefinition() : NULL;
	igdeGDCategory * const selection = GetSelectedCategory();
	
	pTreeCategories->RemoveAllItems();
	
	if( gameDefinition ){
		switch( GetPreviewItemType() ){
		case epitObjectClass:
			UpdateCategoryListWith( gameDefinition->GetClassManager()->GetCategories() );
			break;
			
		case epitSkin:
			UpdateCategoryListWith( gameDefinition->GetSkinManager()->GetCategories() );
			break;
			
		case epitSky:
			UpdateCategoryListWith( gameDefinition->GetSkyManager()->GetCategories() );
			break;
		}
		
		pTreeCategories->SortAllItems();
	}
	
	SelectCategory( selection );
}

void meWPBrowser::UpdateCategoryListWith( igdeGDCategory *category ){
	if( ! category ){
		DETHROW( deeInvalidParam );
	}
	
	const int categoryCount = category->GetCategoryCount();
	int i;
	
	pTreeCategories->AppendItem( NULL, "< No Category >" );
	
	for( i=0; i<categoryCount; i++ ){
		igdeGDCategory * const child = category->GetCategoryAt( i );
		if( ! child->GetHidden() ){
			AddCategoryToList( child, NULL );
		}
	}
}

void meWPBrowser::AddCategoryToList( igdeGDCategory *category, igdeTreeItem *parent ){
	if( ! category ){
		DETHROW( deeInvalidParam );
	}
	
	const int categoryCount = category->GetCategoryCount();
	int i;
	
	igdeTreeItemReference item;
	item.TakeOver( new igdeTreeItem( category->GetName(), category ) );
	pTreeCategories->AppendItem( parent, item );
	
	for( i=0; i<categoryCount; i++ ){
		igdeGDCategory * const child = category->GetCategoryAt( i );
		if( ! child->GetHidden() ){
			AddCategoryToList( child, item );
		}
	}
}

void meWPBrowser::UpdateItemList(){
	const igdeGameDefinition * const gameDefinition = pWorld ? pWorld->GetGameDefinition() : NULL;
	void * const selection = pListItems->GetSelectedItem() ? pListItems->GetSelectedItem()->GetData() : NULL;
	
	pListItems->RemoveAllItems();
	
	if( gameDefinition ){
		igdeGDAddToListVisitor visitor( GetEnvironment(), pListItems, GetPreviewIconSize() );
		
		switch( pSelectionMode ){
		case esmCategory:
			switch( GetPreviewItemType() ){
			case epitObjectClass:
				gameDefinition->GetClassManager()->VisitClassesMatchingCategory( visitor, GetSelectedCategory() );
				break;
				
			case epitSkin:
				gameDefinition->GetSkinManager()->VisitSkinsMatchingCategory( visitor, GetSelectedCategory() );
				break;
				
			case epitSky:
				gameDefinition->GetSkyManager()->VisitSkiesMatchingCategory( visitor, GetSelectedCategory() );
				break;
			}
			break;
			
		case esmFilter:
			switch( GetPreviewItemType() ){
			case epitObjectClass:
				gameDefinition->GetClassManager()->VisitMatchingFilter( visitor, pEditFilter->GetText() );
				break;
				
			case epitSkin:
				gameDefinition->GetSkinManager()->VisitMatchingFilter( visitor, pEditFilter->GetText() );
				break;
				
			case epitSky:
				gameDefinition->GetSkyManager()->VisitMatchingFilter( visitor, pEditFilter->GetText() );
				break;
			}
			break;
		}
	}
	
	pListItems->SortItems();
	
	pListItems->SetSelectionWithData( selection );
	if( ! pListItems->GetSelectedItem() && pListItems->GetItemCount() > 0 ){
		pListItems->SetSelection( 0 );
	}
}

void meWPBrowser::RebuildPISelectedItem(){
	igdeListItem * const item = GetSelectedListItem();
	if( ! item ){
		return;
	}
	
	igdeGDPreviewManager &pvmgr = *GetEnvironment().GetGDPreviewManager();
	igdeGDAddToListVisitor visitor( GetEnvironment(), pListItems, GetPreviewIconSize() );
	
	igdeIconReference icon;
	const int iconSize = GetPreviewIconSize();
	icon.TakeOver( new igdeIcon( *pvmgr.GetImageCreating(), iconSize, iconSize ) );
	
	deObjectReference listener;
	listener.TakeOver( new igdeBrowseItemGDPreviewListener( pListItems, item, iconSize ) );
	
	switch( GetPreviewItemType() ){
	case meWPBrowser::epitObjectClass:{
		igdeGDClass * const gdclass = GetSelectedObjectClass();
		if( ! gdclass ){
			break;
		}
		
		pvmgr.ClearPreviewObjectClass( gdclass );
		item->SetIcon( icon );
		pListItems->ItemChangedAt( pListItems->GetSelection() );
		pvmgr.CreatePreviewObjectClass( gdclass, ( igdeGDPreviewListener* )( deObject* )listener );
		}break;
		
	case meWPBrowser::epitSkin:{
		igdeGDSkin * const gdskin = GetSelectedSkin();
		if( ! gdskin ){
			break;
		}
		
		pvmgr.ClearPreviewSkin( gdskin );
		item->SetIcon( icon );
		pListItems->ItemChangedAt( pListItems->GetSelection() );
		pvmgr.CreatePreviewSkin( gdskin, ( igdeGDPreviewListener* )( deObject* )listener );
		}break;
		
	case meWPBrowser::epitSky:
		break;
	}
}

void meWPBrowser::CurrentItemChanged(){
	const igdeGameDefinition * const gameDefinition = pWorld ? pWorld->GetGameDefinition() : NULL;
	if( ! gameDefinition ){
		pEditInfos->ClearText();
		return;
	}
	
	const igdeListItem * const selection = pListItems->GetSelectedItem();
	decString text;
	
	switch( GetPreviewItemType() ){
	case epitObjectClass:
		if( selection ){
			const igdeGDClass * const gdclass = ( igdeGDClass* )selection->GetData();
			pWorld->GetGuiParameters().SetBrowseClass( gdclass->GetName() );
			text.Format( "%s:\n%s", gdclass->GetName().GetString(), gdclass->GetDescription().GetString() );
			
		}else{
			pWorld->GetGuiParameters().SetBrowseClass( "" );
		}
		break;
		
	case epitSkin:
		if( selection ){
			const igdeGDSkin * const gdskin = ( igdeGDSkin* )selection->GetData();
			pWorld->GetGuiParameters().SetBrowseSkin( gdskin->GetPath() );
			text.Format( "%s:\n%s", gdskin->GetName().GetString(), gdskin->GetDescription().GetString() );
			
		}else{
			pWorld->GetGuiParameters().SetBrowseSkin( "" );
		}
		break;
		
	case epitSky:
		break;
	}
	
	pEditInfos->SetText( text );
}

meWPBrowser::ePreviewItemType meWPBrowser::GetPreviewItemType() const{
	const igdeListItem * const selection = pCBTypes->GetSelectedItem();
	return selection ? ( ePreviewItemType )( intptr_t )selection->GetData() : epitObjectClass;
}

void meWPBrowser::SetPreviewItemType( ePreviewItemType type ){
	pCBTypes->SetSelectionWithData( ( void* )( intptr_t )type );
}

igdeGDCategory *meWPBrowser::GetSelectedCategory() const{
	const igdeTreeItem * const selection = pTreeCategories->GetSelection();
	return selection ? ( igdeGDCategory* )selection->GetData() : NULL;
}

void meWPBrowser::SelectCategory( igdeGDCategory *category ){
	pTreeCategories->SetSelectionWithData( category );
	if( ! pTreeCategories->GetSelection() && pTreeCategories->GetFirstChild() ){
		pTreeCategories->SetSelection( pTreeCategories->GetFirstChild() );
	}
}

void meWPBrowser::SetSelectionMode( eSelectionMode mode ){
	if( mode == pSelectionMode ){
		return;
	}
	
	pSelectionMode = mode;
	
	switch( mode ){
	case esmCategory:
		pSwitcherSelBy->SetCurrent( 0 );
		UpdateItemList();
		break;
		
	case esmFilter:
		pSwitcherSelBy->SetCurrent( 1 );
		UpdateItemList();
		break;
	}
}

igdeListItem *meWPBrowser::GetSelectedListItem() const{
	return pWorld ? pListItems->GetSelectedItem() : NULL;
}

igdeGDClass *meWPBrowser::GetSelectedObjectClass() const{
	return pWorld && GetPreviewItemType() == epitObjectClass && pListItems->GetSelectedItem()
		? ( igdeGDClass* )pListItems->GetSelectedItem()->GetData() : NULL;
}

igdeGDSkin *meWPBrowser::GetSelectedSkin() const{
	return pWorld && GetPreviewItemType() == epitSkin && pListItems->GetSelectedItem()
		? ( igdeGDSkin* )pListItems->GetSelectedItem()->GetData() : NULL;
}

igdeGDSky *meWPBrowser::GetSelectedSky() const{
	return pWorld && GetPreviewItemType() == epitSky && pListItems->GetSelectedItem()
		? ( igdeGDSky* )pListItems->GetSelectedItem()->GetData() : NULL;
}

void meWPBrowser::SetPreviewSize( ePreviewSize size ){
	if( size == pPreviewSize ){
		return;
	}
	
	pPreviewSize = size;
	UpdateItemList();
}

void meWPBrowser::SetViewMode( eViewModes viewMode ){
	if( viewMode == pViewMode ){
		return;
	}
	
	pViewMode = viewMode;
	
	switch( viewMode ){
	case evmList:
		pListItems->SetViewMode( igdeIconListBox::evmList );
		break;
		
	case evmPreview:
		pListItems->SetViewMode( igdeIconListBox::evmIconVertical );
		break;
		
	default:
		pListItems->SetViewMode( igdeIconListBox::evmIconVertical );
	}
}



void meWPBrowser::OnGameDefinitionChanged(){
	void * const selection = pListItems->GetSelectedItem() ? pListItems->GetSelectedItem()->GetData() : NULL;
	
	UpdateCategoryList();
	UpdateItemList();
	
	pListItems->SetSelectionWithData( selection );
	if( ! pListItems->GetSelectedItem() && pListItems->GetItemCount() > 0 ){
		pListItems->SetSelection( 0 );
	}
}



void meWPBrowser::SelectObjectClass( igdeGDClass *gdclass ){
	if( ! gdclass ){
		DETHROW( deeInvalidParam );
	}
	
	// find object class category
	const igdeGameDefinition * const gameDefinition = pWorld ? pWorld->GetGameDefinition() : NULL;
	if( ! gameDefinition ){
		return;
	}
	
	igdeGDCategory * const category = gameDefinition->GetClassManager()->GetCategories()
		->GetCategoryWithPath( decPath::CreatePathUnix( gdclass->GetCategory() ) );
	
	// select category
	SetPreviewItemType( epitObjectClass );
	SelectCategory( category );
	
	// select object class
	pListItems->SetSelectionWithData( gdclass );
}



int meWPBrowser::GetPreviewIconSize() const{
	switch( pPreviewSize ){
	case epsLarge:
		return 128;
		
	case epsMedium:
		return 96;
		
	case epsSmall:
		return 64;
	}
	
	return 128;
}
