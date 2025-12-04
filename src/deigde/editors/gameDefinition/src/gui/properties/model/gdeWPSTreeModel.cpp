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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gdeWPSTreeModel.h"
#include "gdeWPSTreeItemModel.h"
#include "gdeWPSTreeModelListener.h"
#include "category/gdeWPSTIMCategories.h"
#include "category/gdeWPSTIMCategoriesObjectClass.h"
#include "category/gdeWPSTIMCategoriesParticleEmitter.h"
#include "category/gdeWPSTIMCategoriesSkin.h"
#include "category/gdeWPSTIMCategoriesSky.h"
#include "category/gdeWPSTIMCategoryObjectClass.h"
#include "category/gdeWPSTIMCategoryParticleEmitter.h"
#include "category/gdeWPSTIMCategorySkin.h"
#include "category/gdeWPSTIMCategorySky.h"
#include "objectClass/gdeWPSTIMObjectClass.h"
#include "objectClass/gdeWPSTIMObjectClasses.h"
#include "objectClass/gdeWPSTIMOCBillboard.h"
#include "objectClass/gdeWPSTIMOCCamera.h"
#include "objectClass/gdeWPSTIMOCComponent.h"
#include "objectClass/gdeWPSTIMOCEnvMapProbe.h"
#include "objectClass/gdeWPSTIMOCLight.h"
#include "objectClass/gdeWPSTIMOCNavBlocker.h"
#include "objectClass/gdeWPSTIMOCNavSpace.h"
#include "objectClass/gdeWPSTIMOCParticleEmitter.h"
#include "objectClass/gdeWPSTIMOCForceField.h"
#include "objectClass/gdeWPSTIMOCSnapPoint.h"
#include "objectClass/gdeWPSTIMOCSpeaker.h"
#include "objectClass/gdeWPSTIMOCWorld.h"
#include "particleEmitter/gdeWPSTIMParticleEmitter.h"
#include "particleEmitter/gdeWPSTIMParticleEmitters.h"
#include "skin/gdeWPSTIMSkin.h"
#include "skin/gdeWPSTIMSkins.h"
#include "sky/gdeWPSTIMSky.h"
#include "sky/gdeWPSTIMSkies.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeTreeItem::Ref.h>
#include <deigde/gui/model/igdeTreeItemSorter.h>
#include <deigde/gui/model/igdeTreeItemSorter::Ref.h>

#include <dragengine/common/exceptions.h>



// Sorter
///////////

namespace {

class cSorter : public igdeTreeItemSorter{
public:
	cSorter(){ }
	
	virtual bool Precedes( const igdeTreeItem &item1, const igdeTreeItem &item2 ){
		const gdeWPSTreeItemModel &wpsItem1 = ( const gdeWPSTreeItemModel & )item1;
		const gdeWPSTreeItemModel &wpsItem2 = ( const gdeWPSTreeItemModel & )item2;
		return wpsItem1.Compare( wpsItem2 ) <= 0;
	}
};

}



// Constructor, destructor
////////////////////////////

gdeWPSTreeModel::gdeWPSTreeModel( igdeTreeList &treeList, gdeWindowMain &windowMain,
	gdeGameDefinition *gameDefinition ) :
pTreeList( treeList ),
pWindowMain( windowMain ),
pGameDefinition( NULL ),
pListener( NULL ),

pCategories( NULL ),
pObjectClasses( NULL ),
pParticleEmitters( NULL ),
pSkins( NULL ),
pSkies( NULL ),

pIgnoreSelectionChange( false )
{
	if( ! gameDefinition ){
		DETHROW( deeInvalidParam );
	}
	
	treeList.RemoveAllItems();
	
	try{
		// game definition
		pGameDefinition = gameDefinition;
		gameDefinition->AddReference();
		
		// set sorter
		igdeTreeItemSorter::Ref sorter;
		sorter.TakeOver( new cSorter );
		treeList.SetSorter( sorter );
		
		// add items
		igdeTreeItem::Ref item;
		
		item.TakeOver( new gdeWPSTIMCategories( *this ) );
		treeList.AppendItem( NULL, item );
		pCategories = ( gdeWPSTIMCategories* )( igdeTreeItem* )item;
		pCategories->OnAddedToTree();
		
		item.TakeOver( new gdeWPSTIMObjectClasses( *this ) );
		treeList.AppendItem( NULL, item );
		pObjectClasses = ( gdeWPSTIMObjectClasses* )( igdeTreeItem* )item;
		pObjectClasses->OnAddedToTree();
		
		item.TakeOver( new gdeWPSTIMParticleEmitters( *this ) );
		treeList.AppendItem( NULL, item );
		pParticleEmitters = ( gdeWPSTIMParticleEmitters* )( igdeTreeItem* )item;
		pParticleEmitters->OnAddedToTree();
		
		item.TakeOver( new gdeWPSTIMSkins( *this ) );
		treeList.AppendItem( NULL, item );
		pSkins = ( gdeWPSTIMSkins* )( igdeTreeItem* )item;
		pSkins->OnAddedToTree();
		
		item.TakeOver( new gdeWPSTIMSkies( *this ) );
		treeList.AppendItem( NULL, item );
		pSkies = ( gdeWPSTIMSkies* )( igdeTreeItem* )item;
		pSkies->OnAddedToTree();
		
		// add listener
		pListener = new gdeWPSTreeModelListener( *this );
		gameDefinition->AddListener( pListener );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

gdeWPSTreeModel::~gdeWPSTreeModel(){
	pCleanUp();
}



// Management
///////////////

void gdeWPSTreeModel::ClearCurrentItem(){
	pTreeList.SetSelection( NULL );
}

void gdeWPSTreeModel::SetCurrentItemFromGameDef(){
	gdeWPSTIMObjectClass *modelObjectClass = NULL;
	
	switch( pGameDefinition->GetSelectedObjectType() ){
	case gdeGameDefinition::eotObjectClass:
	case gdeGameDefinition::eotOCBillboard:
	case gdeGameDefinition::eotOCCamera:
	case gdeGameDefinition::eotOCComponent:
	case gdeGameDefinition::eotOCEnvMapProbe:
	case gdeGameDefinition::eotOCLight:
	case gdeGameDefinition::eotOCNavigationBlocker:
	case gdeGameDefinition::eotOCNavigationSpace:
	case gdeGameDefinition::eotOCParticleEmitter:
	case gdeGameDefinition::eotOCForceField:
	case gdeGameDefinition::eotOCSnapPoint:
	case gdeGameDefinition::eotOCSpeaker:
	case gdeGameDefinition::eotOCWorld:
		if( ! pGameDefinition->GetActiveObjectClass() ){
			break;
		}
		modelObjectClass = pObjectClasses->GetChildWith( pGameDefinition->GetActiveObjectClass() );
		break;
		
	default:
		break;
	}
	
	gdeWPSTreeItemModel *model = NULL;
	
	switch( pGameDefinition->GetSelectedObjectType() ){
	case gdeGameDefinition::eotCategoryObjectClass:
		if( pGameDefinition->GetActiveCategory() ){
			model = pCategories->GetObjectClass().GetChildWith( pGameDefinition->GetActiveCategory(), true );
		}
		break;
		
	case gdeGameDefinition::eotCategoryParticleEmitter:
		if( pGameDefinition->GetActiveCategory() ){
			model = pCategories->GetParticleEmitter().GetChildWith( pGameDefinition->GetActiveCategory(), true );
		}
		break;
		
	case gdeGameDefinition::eotCategorySkin:
		if( pGameDefinition->GetActiveCategory() ){
			model = pCategories->GetSkin().GetChildWith( pGameDefinition->GetActiveCategory(), true );
		}
		break;
		
	case gdeGameDefinition::eotCategorySky:
		if( pGameDefinition->GetActiveCategory() ){
			model = pCategories->GetSky().GetChildWith( pGameDefinition->GetActiveCategory(), true );
		}
		break;
		
	case gdeGameDefinition::eotObjectClass:
		model = modelObjectClass;
		break;
		
	case gdeGameDefinition::eotOCBillboard:
		if( modelObjectClass && pGameDefinition->GetActiveOCBillboard() ){
			model = modelObjectClass->GetChildWith( pGameDefinition->GetActiveOCBillboard() );
		}
		break;
		
	case gdeGameDefinition::eotOCCamera:
		if( modelObjectClass && pGameDefinition->GetActiveOCCamera() ){
			model = modelObjectClass->GetChildWith( pGameDefinition->GetActiveOCCamera() );
		}
		break;
		
	case gdeGameDefinition::eotOCComponent:
		if( modelObjectClass && pGameDefinition->GetActiveOCComponent() ){
			model = modelObjectClass->GetChildWith( pGameDefinition->GetActiveOCComponent() );
		}
		break;
		
	case gdeGameDefinition::eotOCEnvMapProbe:
		if( modelObjectClass && pGameDefinition->GetActiveOCEnvMapProbe() ){
			model = modelObjectClass->GetChildWith( pGameDefinition->GetActiveOCEnvMapProbe() );
		}
		break;
		
	case gdeGameDefinition::eotOCLight:
		if( modelObjectClass && pGameDefinition->GetActiveOCLight() ){
			model = modelObjectClass->GetChildWith( pGameDefinition->GetActiveOCLight() );
		}
		break;
		
	case gdeGameDefinition::eotOCNavigationBlocker:
		if( modelObjectClass && pGameDefinition->GetActiveOCNavigationBlocker() ){
			model = modelObjectClass->GetChildWith( pGameDefinition->GetActiveOCNavigationBlocker() );
		}
		break;
		
	case gdeGameDefinition::eotOCNavigationSpace:
		if( modelObjectClass && pGameDefinition->GetActiveOCNavigationSpace() ){
			model = modelObjectClass->GetChildWith( pGameDefinition->GetActiveOCNavigationSpace() );
		}
		break;
		
	case gdeGameDefinition::eotOCParticleEmitter:
		if( modelObjectClass && pGameDefinition->GetActiveOCParticleEmitter() ){
			model = modelObjectClass->GetChildWith( pGameDefinition->GetActiveOCParticleEmitter() );
		}
		break;
		
	case gdeGameDefinition::eotOCForceField:
		if( modelObjectClass && pGameDefinition->GetActiveOCForceField() ){
			model = modelObjectClass->GetChildWith( pGameDefinition->GetActiveOCForceField() );
		}
		break;
		
	case gdeGameDefinition::eotOCSnapPoint:
		if( modelObjectClass && pGameDefinition->GetActiveOCSnapPoint() ){
			model = modelObjectClass->GetChildWith( pGameDefinition->GetActiveOCSnapPoint() );
		}
		break;
		
	case gdeGameDefinition::eotOCSpeaker:
		if( modelObjectClass && pGameDefinition->GetActiveOCSpeaker() ){
			model = modelObjectClass->GetChildWith( pGameDefinition->GetActiveOCSpeaker() );
		}
		break;
		
	case gdeGameDefinition::eotOCWorld:
		if(modelObjectClass && pGameDefinition->GetActiveOCWorld()){
			model = modelObjectClass->GetChildWith(pGameDefinition->GetActiveOCWorld());
		}
		break;
		
	case gdeGameDefinition::eotParticleEmitter:
		if( pGameDefinition->GetActiveParticleEmitter() ){
			model = pParticleEmitters->GetChildWith( pGameDefinition->GetActiveParticleEmitter() );
		}
		break;
		
	case gdeGameDefinition::eotSkin:
		if( pGameDefinition->GetActiveSkin() ){
			model = pSkins->GetChildWith( pGameDefinition->GetActiveSkin() );
		}
		break;
		
	case gdeGameDefinition::eotSky:
		if( pGameDefinition->GetActiveSky() ){
			model = pSkies->GetChildWith( pGameDefinition->GetActiveSky() );
		}
		break;
		
	case gdeGameDefinition::eotNoSelection:
	default:
		break;
	}
	
	if( model ){
		model->SetAsCurrentItem();
		
	}else{
		// if we do this we can not select stuff like group items without functionality
		//ClearCurrentItem();
	}
}



void gdeWPSTreeModel::OnContextMenu( igdeMenuCascade &contextMenu ){
	igdeUIHelper &helper = pWindowMain.GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( contextMenu, pWindowMain.GetActionCategoryObjectClassAdd() );
	helper.MenuCommand( contextMenu, pWindowMain.GetActionCategoryParticleEmitterAdd() );
	helper.MenuCommand( contextMenu, pWindowMain.GetActionCategorySkinAdd() );
	helper.MenuCommand( contextMenu, pWindowMain.GetActionCategorySkyAdd() );
	
	helper.MenuSeparator( contextMenu );
	helper.MenuCommand( contextMenu, pWindowMain.GetActionParticleEmitterAdd() );
	helper.MenuCommand( contextMenu, pWindowMain.GetActionSkinAdd() );
	helper.MenuCommand( contextMenu, pWindowMain.GetActionSkyAdd() );
}

void gdeWPSTreeModel::SelectBestMatching( const char * ){
}



void gdeWPSTreeModel::SetIgnoreSelectionChange( bool ignore ){
	pIgnoreSelectionChange = ignore;
}

gdeWPSTreeModel::HelperIgnoreSelection::HelperIgnoreSelection( gdeWPSTreeModel &tree ) :
pTree( tree )
{
	tree.SetIgnoreSelectionChange( true );
}

gdeWPSTreeModel::HelperIgnoreSelection::~HelperIgnoreSelection(){
	pTree.SetIgnoreSelectionChange( false );
}



// Private Functions
//////////////////////

void gdeWPSTreeModel::pCleanUp(){
	if( pListener ){
		if( pGameDefinition ){
			pGameDefinition->RemoveListener( pListener );
		}
		delete pListener;
	}
	
	pTreeList.RemoveAllItems();
	
	if( pGameDefinition ){
		pGameDefinition->FreeReference();
	}
}
