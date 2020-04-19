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

#include "igdeDialogTexturePropertyList.h"
#include "../igdeUIHelper.h"
#include "../igdeListBox.h"
#include "../igdeTextArea.h"
#include "../igdeTextField.h"
#include "../igdeContainerReference.h"
#include "../event/igdeListBoxListener.h"
#include "../layout/igdeContainerFlow.h"
#include "../layout/igdeContainerForm.h"
#include "../layout/igdeContainerSplitted.h"
#include "../layout/igdeContainerSplittedReference.h"
#include "../model/igdeListItem.h"
#include "../../environment/igdeEnvironment.h"
#include "../../engine/textureProperties/igdeTextureProperty.h"
#include "../../engine/textureProperties/igdeTexturePropertyList.h"

#include <dragengine/common/exceptions.h>



// Actions
///////////

class igdeDialogTexturePropertyList_ListBox : public igdeListBoxListener {
	igdeDialogTexturePropertyList &pDialog;
public:
	igdeDialogTexturePropertyList_ListBox( igdeDialogTexturePropertyList &dialog ) : pDialog( dialog ){ }
	
	virtual void OnSelectionChanged( igdeListBox* ){
		pDialog.UpdateProperty();
	}
};



// Class igdeDialogTexturePropertyList
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDialogTexturePropertyList::igdeDialogTexturePropertyList( igdeEnvironment &environment ) :
igdeDialog( environment, "Texture Property List" ){
	igdeUIHelper &helper = environment.GetUIHelper();
	
	SetInitialSize( decPoint( 1000, 500 ) );
	
	igdeContainerSplittedReference content;
	content.TakeOver( new igdeContainerSplitted( environment, igdeContainerSplitted::espLeft, 300 ) );
	
	helper.ListBox( 15, "Textue Property", pListProperties, new igdeDialogTexturePropertyList_ListBox( *this ) );
	pListProperties->SetDefaultSorter();
	content->AddChild( pListProperties, igdeContainerSplitted::eaSide );
	
	igdeContainerReference form;
	form.TakeOver( new igdeContainerForm( environment, igdeContainerForm::esLast ) );
	content->AddChild( form, igdeContainerSplitted::eaCenter );
	
	helper.EditString( form, "Name:", "Property name", pEditName, NULL );
	pEditName->SetEditable( false );
	
	helper.EditString( form, "Type:", "Property type", pEditType, NULL );
	pEditType->SetEditable( false );
	
	helper.EditString( form, "Components:", "Used color components", pEditComponentCount, NULL );
	pEditComponentCount->SetEditable( false );
	
	helper.EditString( form, "Default Value:", "Default value", pEditDefault, NULL );
	pEditDefault->SetEditable( false );
	
	helper.EditString( form, "Affects:", "Modules affected by property", pEditAffectedModules, NULL );
	pEditAffectedModules->SetEditable( false );
	
	helper.EditString( form, "Description:", "Property description", pEditDescription, 40, 5, NULL );
	pEditDescription->SetEditable( false );
	
	
	igdeContainerReference buttonBar;
	CreateButtonBar( buttonBar, "Close" );
	
	AddContent( content, buttonBar );
	
	
	UpdatePropertyList();
}

igdeDialogTexturePropertyList::~igdeDialogTexturePropertyList(){
}



// Management
///////////////

void igdeDialogTexturePropertyList::UpdatePropertyList(){
	const igdeTexturePropertyList &list = *GetEnvironment().GetTexturePropertyList();
	const igdeTextureProperty *selection = NULL;
	const int count = list.GetCount();
	int i;
	
	pListProperties->RemoveAllItems();
	
	for( i=0; i<count; i++ ){
		const igdeTextureProperty * const property = list.GetAt( i );
		pListProperties->AddItem( property->GetName(), NULL, ( void* )property );
		if( property->GetName() == "color" ){
			selection = property;
		}
	}
	
	pListProperties->SortItems();
	if( selection ){
		pListProperties->SetSelectionWithData( ( void* )selection );
	}
	
	UpdateProperty();
}

void igdeDialogTexturePropertyList::UpdateProperty(){
	const igdeTextureProperty * const property = pListProperties->GetSelectedItem()
		? ( const igdeTextureProperty * )pListProperties->GetSelectedItem()->GetData() : NULL;
	
	if( property ){
		pEditName->SetText( property->GetName() );
		pEditDescription->SetText( property->GetDescription() );
		
		switch( property->GetType() ){
		case igdeTextureProperty::eptValue:
			pEditType->SetText( "Value" );
			break;
			
		case igdeTextureProperty::eptColor:
			pEditType->SetText( "Color" );
			break;
			
		case igdeTextureProperty::eptImage:
			pEditType->SetText( "Image" );
			break;
			
		default:
			pEditType->SetText( "?" );
		}
		
		pEditComponentCount->SetInteger( property->GetComponentCount() );
		
		decString defaultValue;
		if( property->GetType() == igdeTextureProperty::eptValue ){
			defaultValue.Format( "%g", property->GetDefaultValue() );
			
		}else{
			const decColor &color = property->GetDefaultColor();
			
			if( property->GetComponentCount() == 1 ){
				defaultValue.Format( "(%g)", color.r );
				
			}else if( property->GetComponentCount() == 2 ){
				defaultValue.Format( "(%g, %g)", color.r, color.g );
				
			}else if( property->GetComponentCount() == 3 ){
				defaultValue.Format( "(%g, %g, %g)", color.r, color.g, color.b );
				
			}else{
				defaultValue.Format( "(%g, %g, %g, %g)", color.r, color.g, color.b, color.a );
			}
		}
		pEditDefault->SetText( defaultValue );
		
		const decStringSet &affectedList = property->GetAffectedModules();
		const int affectedCount = affectedList.GetCount();
		decStringList affects;
		int i;
		for( i=0; i<affectedCount; i++ ){
			affects.Add( affectedList.GetAt( i ) );
		}
		pEditAffectedModules->SetText( affects.Join( ", " ) );
		
	}else{
		pEditName->ClearText();
		pEditDescription->ClearText();
		pEditType->ClearText();
		pEditComponentCount->ClearText();
		pEditDefault->ClearText();
		pEditAffectedModules->ClearText();
	}
}
