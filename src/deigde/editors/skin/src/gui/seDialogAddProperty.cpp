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
#include "seDialogAddProperty.h"
#include "../skin/seSkin.h"
#include "../skin/texture/seTexture.h"
#include "../skin/property/seProperty.h"
#include "../skin/property/node/sePropertyNodeGroup.h"

#include <deigde/engine/textureProperties/igdeTextureProperty.h>
#include <deigde/engine/textureProperties/igdeTexturePropertyList.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/dialog/igdeDialogTexturePropertyList.h>
#include <deigde/gui/dialog/igdeDialog.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/resources/igdeIcon.h>
#include <deigde/gui/resources/igdeIcon.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringList.h>



// Actions
////////////

namespace {

class cActionDetails : public igdeAction{
	seDialogAddProperty &pDialog;
	
public:
	cActionDetails( seDialogAddProperty &dialog ) : igdeAction( "Texture Property Details" ),
	pDialog( dialog ){ }
	
	virtual void OnAction(){
		igdeDialog::Ref dialog;
		dialog.TakeOver( new igdeDialogTexturePropertyList( pDialog.GetEnvironment() ) );
		dialog->Run( &pDialog );
	}
};

}



// Class seDialogAddProperty
//////////////////////////////

// Constructor, destructor
////////////////////////////

seDialogAddProperty::seDialogAddProperty( seWindowMain &windowMain ) :
igdeDialog( windowMain.GetEnvironment(), "Add Property" ),
pWindowMain( windowMain )
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	igdeContainer::Ref content, formLine;
	
	
	igdeLabel::Ref header;
	header.TakeOver( new igdeLabel( env, "Multi-Select properties or enter custom name." ) );
	
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY, igdeContainerFlow::esLast, 5 ) );
	
	helper.ListBox( content, 15, "Property names", pListProperties, NULL );
	pListProperties->SetSelectionMode( igdeListBox::esmMultiple );
	pListProperties->SetDefaultSorter();
	
	helper.Button( content, new cActionDetails( *this ), true );
	
	formLine.TakeOver( new igdeContainerForm( env ) );
	helper.EditString( formLine, "Custom Name:", "Custom name (for special uses only)",
		15, pEditCustomPropertyName, NULL );
	content->AddChild( formLine );
	
	
	igdeContainer::Ref buttonBar;
	CreateButtonBar( buttonBar, "Add Properties", "Cancel" );
	
	
	AddContent( content, header, buttonBar );
	
	
	pInitPropertyList();
	
	pListProperties->Focus();
}

seDialogAddProperty::~seDialogAddProperty(){
}



// Management
///////////////

decStringSet seDialogAddProperty::GetSelectedPropertyNames() const{
	const int count = pListProperties->GetItemCount();
	decStringSet properties;
	int i;
	
	for( i=0; i<count; i++ ){
		const igdeListItem &item = *pListProperties->GetItemAt( i );
		if( item.GetSelected() ){
			properties.Add( item.GetText() );
		}
	}
	
	return properties;
}

void seDialogAddProperty::SetSelectedPropertyNames( const decStringSet &list ){
	const int count = list.GetCount();
	int i;
	
	pListProperties->DeselectAllItems();
	
	if( list.GetCount() > 0 ){
		pListProperties->SetSelection( pListProperties->IndexOfItem( list.GetAt( 0 ) ) );
		
	}else{
		pListProperties->SetSelection( -1 );
	}
	
	for( i=0; i<count; i++ ){
		pListProperties->SelectItem( pListProperties->IndexOfItem( list.GetAt( i ) ) );
	}
}

const decString &seDialogAddProperty::GetCustomPropertyName() const{
	return pEditCustomPropertyName->GetText();
}

void seDialogAddProperty::SetCustomPropertyName( const char *name ){
	pEditCustomPropertyName->SetText( name );
}



// Private Functions
//////////////////////

void seDialogAddProperty::pInitPropertyList(){
	igdeEnvironment &environment = pWindowMain.GetEnvironment();
	const igdeTexturePropertyList &envPropList = *environment.GetTexturePropertyList();
	seTexture &texture = *pWindowMain.GetSkin()->GetActiveTexture();
	const sePropertyList &propList = texture.GetPropertyList();
	const int propertyCount = envPropList.GetCount();
	igdeIcon::Ref iconUsed( environment.GetStockIcon( igdeEnvironment::esiSmallPlus ) );
	igdeIcon::Ref iconAvailable( environment.GetStockIcon( igdeEnvironment::esiSmallMinus ) );
	int i;
	
	for( i=0; i<propertyCount; i++ ){
		const decString &propertyName = envPropList.GetAt( i )->GetName();
		
		if( propList.HasNamed( propertyName ) ){
			pListProperties->AddItem( propertyName, iconUsed );
			
		}else{
			pListProperties->AddItem( propertyName, iconAvailable );
		}
	}
	
	pListProperties->SortItems();
}
