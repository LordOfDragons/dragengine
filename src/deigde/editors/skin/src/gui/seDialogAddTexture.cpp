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
#include <deigde/gui/resources/igdeIcon::Ref.h>

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
	igdeIcon::Ref iconUsed( GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallPlus ) );
	igdeIcon::Ref iconAvailable( GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallMinus ) );
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
