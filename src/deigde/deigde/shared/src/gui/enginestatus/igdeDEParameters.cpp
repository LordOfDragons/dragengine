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

#include <stdint.h>

#include "igdeDEParameters.h"
#include "igdeDialogEngine.h"
#include "../igdeCommonDialogs.h"
#include "../igdeUIHelper.h"
#include "../igdeComboBox.h"
#include "../igdeButton.h"
#include "../igdeTextArea.h"
#include "../igdeTextField.h"
#include "../igdeContainerReference.h"
#include "../layout/igdeContainerForm.h"
#include "../event/igdeAction.h"
#include "../event/igdeComboBoxListener.h"
#include "../model/igdeListItem.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deBaseModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>



// Actions
////////////

class igdeDEParameters_ComboModule : public igdeComboBoxListener{
	igdeDEParameters &pPanel;
	
public:
	igdeDEParameters_ComboModule( igdeDEParameters &panel ) : pPanel( panel ){}
	
	virtual void OnTextChanged( igdeComboBox* ){
		pPanel.UpdateParametersList();
		pPanel.UpdateParameter();
	}
};

class igdeDEParameters_ComboParameter : public igdeComboBoxListener{
	igdeDEParameters &pPanel;
	
public:
	igdeDEParameters_ComboParameter( igdeDEParameters &panel ) : pPanel( panel ){}
	
	virtual void OnTextChanged( igdeComboBox* ){
		pPanel.UpdateParameter();
	}
};

class igdeDEParameters_ActionSet : public igdeAction{
	igdeDEParameters &pPanel;
	
public:
	igdeDEParameters_ActionSet( igdeDEParameters &panel ) :
		igdeAction( "Set", NULL, "Set parameter value" ), pPanel( panel ){}
	
	virtual void OnAction(){
		pPanel.ParameterSetValue();
	}
	
	virtual void Update(){
		SetEnabled( ! pPanel.GetSelectedParameter().IsEmpty() );
	}
};

class igdeDEParameters_ActionReset : public igdeAction{
	igdeDEParameters &pPanel;
	
public:
	igdeDEParameters_ActionReset( igdeDEParameters &panel ) :
		igdeAction( "Reset", NULL, "Reset parameter value" ), pPanel( panel ){}
	
	virtual void OnAction(){
		pPanel.ParameterResetValue();
	}
	
	virtual void Update(){
		SetEnabled( ! pPanel.GetSelectedParameter().IsEmpty() );
	}
};



// class igdeDEParameters
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDEParameters::igdeDEParameters( igdeDialogEngine &dialogEngine ) :
igdeContainerFlow( dialogEngine.GetEnvironment(), igdeContainerFlow::eaY ),
pDialogEngine( dialogEngine )
{
	igdeEnvironment &env = dialogEngine.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	igdeContainerReference groupBox, line;
	
	
	line.TakeOver( new igdeContainerForm( env ) );
	AddChild( line );
	
	helper.ComboBox( line, "Module:", "Module to show parameters for",
		pCBModule, new igdeDEParameters_ComboModule( *this ) );
	pCBModule->SetDefaultSorter();
	
	helper.ComboBox( line, "Parameter:", "Parameter to show",
		pCBParameter, new igdeDEParameters_ComboParameter( *this ) );
	pCBParameter->SetDefaultSorter();
	
	
	// parameter information
	helper.GroupBoxStatic( *this, groupBox, "Parameter:" );
	
	helper.EditString( groupBox, "Description:", "Parameter description", pEditDescription, 3, NULL );
	pEditDescription->SetEditable( false );
	
	helper.EditString( groupBox, "Type:", "Parameter type", pEditType, NULL );
	pEditType->SetEditable( false );
	
	helper.EditString( groupBox, "Allowed Values:", "Allowed Values", pEditAllowedValues, 5, NULL );
	pEditAllowedValues->SetEditable( false );
	
	helper.FormLineStretchFirst( groupBox, "Value:", "Parameter value", line );
	helper.EditString( line, "Parameter value", pEditValue, NULL );
	helper.Button( line, pBtnSet, new igdeDEParameters_ActionSet( *this ), true );
	helper.Button( line, pBtnReset, new igdeDEParameters_ActionReset( *this ), true );
	
	
	// load the lists
	UpdateModulesList();
	UpdateParametersList();
	UpdateParameter();
}

igdeDEParameters::~igdeDEParameters(){
}



// Management
///////////////

void igdeDEParameters::UpdateParameter(){
	deLoadableModule * const loadableModule = GetSelectedModule();
	if( ! loadableModule || ! loadableModule->GetModule() || ! pCBParameter->GetSelectedItem() ){
		pEditDescription->SetText( "?" );
		pEditType->SetText( "?" );
		pEditAllowedValues->SetText( "?" );
		pEditValue->SetText( "?" );
		
		pBtnSet->GetAction()->Update();
		pBtnReset->GetAction()->Update();
		return;
	}
	
	deBaseModule &module = *loadableModule->GetModule();
	const decString &name = GetSelectedParameter();
	decString text;
	
	module.GetParameterInfo( module.IndexOfParameterNamed( name ), pParameterInfo );
	
	pEditDescription->SetText( pParameterInfo.GetDescription() );
	
	switch( pParameterInfo.GetType() ){
	case deModuleParameter::eptBoolean:
		pEditType->SetText( "Boolean" );
		pEditAllowedValues->SetText( "Boolean value: 1 or 0" );
		break;
		
	case deModuleParameter::eptNumeric:
		pEditType->SetText( "Numeric" );
		pEditAllowedValues->SetText( "Numeric value." );
		break;
		
	case deModuleParameter::eptRanged:
		pEditType->SetText( "Ranged" );
		text.Format( "Ranged value from %g to %g step size %g",
			pParameterInfo.GetMinimumValue(), pParameterInfo.GetMaximumValue(),
			pParameterInfo.GetValueStepSize() );
		pEditAllowedValues->SetText( text );
		break;
		
	case deModuleParameter::eptSelection:{
		pEditType->SetText( "Selection" );
		text = "A value from this list:\n";
		const int count = pParameterInfo.GetSelectionEntryCount();
		decStringList list;
		int i;
		for( i=0; i<count; i++ ){
			list.Add( pParameterInfo.GetSelectionEntryAt( i ).value );
		}
		pEditAllowedValues->SetText( text + list.Join( ", " ) );
		}break;
		
	case deModuleParameter::eptString:
		pEditType->SetText( "String" );
		pEditAllowedValues->SetText( "String value." );
		break;
		
	default:
		pEditType->SetText( "Unknown Type" );
		pEditAllowedValues->SetText( "Unknown" );
	}
	
	pEditValue->SetText( module.GetParameterValue( name ) );
	
	pBtnSet->GetAction()->Update();
	pBtnReset->GetAction()->Update();
}

void igdeDEParameters::UpdateParametersList(){
	deLoadableModule * const loadableModule = GetSelectedModule();
	
	pCBParameter->RemoveAllItems();
	
	if( loadableModule && loadableModule->GetModule() ){
		deBaseModule &module = *loadableModule->GetModule();
		const int count = module.GetParameterCount();
		int i;
		for( i=0; i<count; i++ ){
			module.GetParameterInfo( i, pParameterInfo );
			pCBParameter->AddItem( pParameterInfo.GetName() );
		}
		pCBParameter->SortItems();
	}
}

void igdeDEParameters::UpdateModulesList(){
	const deModuleSystem &moduleSystem = *GetEngine()->GetModuleSystem();
	const int count = moduleSystem.GetModuleCount();
	int i;
	
	pCBModule->RemoveAllItems();
	for( i=0; i<count; i++ ){
		deLoadableModule * const loadableModule = moduleSystem.GetModuleAt( i );
		if( ! pCBModule->HasItem( loadableModule->GetName() ) ){
			pCBModule->AddItem( loadableModule->GetName() );
		}
	}
	pCBModule->SortItems();
}

deLoadableModule *igdeDEParameters::GetSelectedModule() const{
	return pCBModule->GetSelectedItem() ? GetEngine()->GetModuleSystem()->
		GetModuleNamed( pCBModule->GetSelectedItem()->GetText() ) : NULL;
}

const decString &igdeDEParameters::GetSelectedParameter() const{
	static decString empty;
	return GetSelectedModule() && pCBParameter->GetSelectedItem() ? pCBParameter->GetSelectedItem()->GetText() : empty;
}

void igdeDEParameters::ParameterSetValue(){
	deLoadableModule * const loadableModule = GetSelectedModule();
	const decString &name = GetSelectedParameter();
	if( ! loadableModule || ! loadableModule->GetModule() || name.IsEmpty() ){
		return;
	}
	
	try{
		loadableModule->GetModule()->SetParameterValue( name, pEditValue->GetText() );
		
	}catch( const deException &e ){
		igdeCommonDialogs::Exception( this, e );
	}
	
	pEditValue->SetText( loadableModule->GetModule()->GetParameterValue( name ) );
}

void igdeDEParameters::ParameterResetValue(){
	deLoadableModule * const loadableModule = GetSelectedModule();
	const decString &name = GetSelectedParameter();
	if( ! loadableModule || ! loadableModule->GetModule() || name.IsEmpty() ){
		return;
	}
	
	deBaseModule &module = *loadableModule->GetModule();
	module.GetParameterInfo( module.IndexOfParameterNamed( name ), pParameterInfo );
	
	pEditValue->SetText( pParameterInfo.GetDefaultValue() );
}
