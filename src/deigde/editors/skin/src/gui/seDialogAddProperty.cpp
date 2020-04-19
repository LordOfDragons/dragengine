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
#include <deigde/gui/igdeLabelReference.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/dialog/igdeDialogTexturePropertyList.h>
#include <deigde/gui/dialog/igdeDialogReference.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/resources/igdeIcon.h>
#include <deigde/gui/resources/igdeIconReference.h>

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
		igdeDialogReference dialog;
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
	igdeContainerReference content, formLine;
	
	
	igdeLabelReference header;
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
	
	
	igdeContainerReference buttonBar;
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
	igdeIconReference iconUsed( environment.GetStockIcon( igdeEnvironment::esiSmallPlus ) );
	igdeIconReference iconAvailable( environment.GetStockIcon( igdeEnvironment::esiSmallMinus ) );
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
