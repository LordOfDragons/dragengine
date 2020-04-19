/* 
 * Drag[en]gine IGDE Skin Editor
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

#include <stdlib.h>
#include <string.h>

#include "seWindowMain.h"
#include "seDialogAddTexture.h"
#include "../skin/seSkin.h"
#include "../skin/texture/seTexture.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeLabelReference.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/resources/igdeIcon.h>
#include <deigde/gui/resources/igdeIconReference.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelTexture.h>



// Actions
////////////

namespace {

class cListTextureNames : public igdeListBoxListener{
	seDialogAddTexture &pDialog;
	
public:
	cListTextureNames( seDialogAddTexture &dialog ) : pDialog( dialog ){ }
	
	virtual void OnSelectionChanged( igdeListBox *listBox ){
		if( listBox->GetSelectedItem() ){
			pDialog.SetTextureName( listBox->GetSelectedItem()->GetText() );
		}
	}
	
	virtual void OnDoubleClickItem( igdeListBox *listBox, int index ){
		pDialog.SetTextureName( listBox->GetItemAt( index )->GetText() );
		pDialog.CloseDialog( true );
	}
};

class cTextTextureName : public igdeTextFieldListener{
	seDialogAddTexture &pDialog;
	
public:
	cTextTextureName( seDialogAddTexture &dialog ) : pDialog( dialog ){ }
	
	virtual void OnTextChanging( igdeTextField *textField ){
		pDialog.SetTextureName( textField->GetText() );
	}
};

}


// Class seDialogAddTexture
/////////////////////////////

// Constructor, destructor
////////////////////////////

seDialogAddTexture::seDialogAddTexture( seWindowMain &windowMain ) :
igdeDialog( windowMain.GetEnvironment(), "Add Texture" ),
pWindowMain( windowMain )
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	igdeContainerReference content, formLine;
	
	
	igdeLabelReference header;
	header.TakeOver( new igdeLabel( env, "Enter texture name or choose from model textures." ) );
	
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY, igdeContainerFlow::esLast, 5 ) );
	
	helper.ListBox( content, 5, "Textures names in model", pListModelTextureNames,
		new cListTextureNames( *this ) );
	pListModelTextureNames->SetDefaultSorter();
	
	formLine.TakeOver( new igdeContainerForm( env ) );
	helper.EditString( formLine, "Name:", "Name of texture to add", 25,
		pEditTextureName, new cTextTextureName( *this ) );
	content->AddChild( formLine );
	
	
	igdeContainerReference buttonBar;
	CreateButtonBar( buttonBar, "Create Texture", "Cancel" );
	
	
	AddContent( content, header, buttonBar );
	
	
	pUpdateModelTextureList();
	
	pEditTextureName->SetText( "Texture" );
	pEditTextureName->Focus();
}

seDialogAddTexture::~seDialogAddTexture(){
}



// Management
///////////////

const decString &seDialogAddTexture::GetTextureName() const{
	return pEditTextureName->GetText();
}

void seDialogAddTexture::SetTextureName( const char *name ){
	pEditTextureName->SetText( name );
	
	// name could be a string reference to the list box or edit field widget text
	// and changing the text field might have invalidated such a link. by using
	// the edit field GetText() we are on the safe side
	pListModelTextureNames->SetSelection( pListModelTextureNames->IndexOfItem( pEditTextureName->GetText() ) );
}


// Protected Functions
////////////////////////

void seDialogAddTexture::pUpdateModelTextureList(){
	igdeIconReference iconUsed( GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallPlus ) );
	igdeIconReference iconAvailable( GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallMinus ) );
	const seSkin * const skin = pWindowMain.GetSkin();
	const deModel *engModel = NULL;
	
	if( skin->GetEngineComponent() ){
		engModel = skin->GetEngineComponent()->GetModel();
	}
	
	if( engModel ){
		const seTextureList &list = skin->GetTextureList();
		int i, count = engModel->GetTextureCount();
		
		for( i=0; i<count; i++ ){
			const decString &modelTextureName = engModel->GetTextureAt( i )->GetName();
			
			if( list.HasNamed( modelTextureName ) ){
				pListModelTextureNames->AddItem( modelTextureName, iconUsed );
				
			}else{
				pListModelTextureNames->AddItem( modelTextureName, iconAvailable );
			}
		}
	}
	
	pListModelTextureNames->SortItems();
	
	if( pListModelTextureNames->GetItemCount() > 0 ){
		pListModelTextureNames->SetSelection( 0 );
	}
}
