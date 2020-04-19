/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include "gdeWPPropertyList.h"
#include "gdeDefaultPropertyValue.h"
#include "../../clipboard/gdeClipboardDataProperty.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/filepattern/gdeFilePattern.h"
#include "../../gamedef/filepattern/gdeFilePatternList.h"
#include "../../gamedef/property/gdeProperty.h"
#include "../../gamedef/property/gdePropertyList.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditPropertyValue.h>
#include <deigde/gui/composed/igdeEditPropertyValueListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/event/igdeTextAreaListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/clipboard/igdeClipboardDataReference.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Actions
////////////

namespace{

class cComboProperty : public igdeComboBoxListener {
	gdeWPPropertyList &pPanel;
	
public:
	cComboProperty( gdeWPPropertyList &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox* ){
		pPanel.UpdateProperty();
	}
};


class cActionPropertyMenu : public igdeActionContextMenu {
	gdeWPPropertyList &pPanel;
	
public:
	cActionPropertyMenu( gdeWPPropertyList &panel ) :
	igdeActionContextMenu( "", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallDown ),
		"Show Properties Menu" ),
	pPanel( panel ){ }
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu ){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand( contextMenu, pPanel.GetActionPropertyAdd() );
		helper.MenuCommand( contextMenu, pPanel.GetActionPropertyRemove() );
		helper.MenuSeparator( contextMenu );
		helper.MenuCommand( contextMenu, pPanel.GetActionPropertyCopy() );
		helper.MenuCommand( contextMenu, pPanel.GetActionPropertyCut() );
		helper.MenuCommand( contextMenu, pPanel.GetActionPropertyPaste() );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetPropertyList() != NULL );
	}
};

class cActionPropertyAdd : public igdeAction {
	gdeWPPropertyList &pPanel;
	
public:
	cActionPropertyAdd( gdeWPPropertyList &panel ) :
	igdeAction( "Add...", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ),
		"Add property" ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		if( ! pPanel.GetPropertyList() || ! pPanel.GetUndoSystem() ){
			return;
		}
		
		const gdePropertyList &list = *pPanel.GetPropertyList();
		decString name( "Property" );
		
		while( igdeCommonDialogs::GetString( &pPanel, "Add Property", "Name:", name ) ){
			if( list.HasNamed( name ) ){
				igdeCommonDialogs::Error( &pPanel, "Add Property", "Name exists already." );
				continue;
			}
			
			deObjectReference property;
			property.TakeOver( new gdeProperty( name ) );
			
			igdeUndoReference undo;
			undo.TakeOver( pPanel.UndoAdd( ( gdeProperty* )( deObject* )property ) );
			if( undo ){
				pPanel.GetUndoSystem()->Add( undo );
				pPanel.SelectProperty( ( gdeProperty* )( deObject* )property );
			}
			return;
		}
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetPropertyList() != NULL );
	}
};

class cActionPropertyRemove : public igdeAction {
	gdeWPPropertyList &pPanel;
	
public:
	cActionPropertyRemove( gdeWPPropertyList &panel ) :
	igdeAction( "Remove", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ),
		"Remove selected property" ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		gdeProperty * const property = pPanel.GetProperty();
		if( ! property || ! pPanel.GetUndoSystem() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( pPanel.UndoRemove( property ) );
		if( undo ){
			pPanel.GetUndoSystem()->Add( undo );
		}
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetProperty() != NULL );
	}
};

class cActionPropertyCopy : public igdeAction {
protected:
	gdeWPPropertyList &pPanel;
	
public:
	cActionPropertyCopy( gdeWPPropertyList &panel ) :
	igdeAction( "Copy", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ),
		"Copy selected property" ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		gdeProperty * const property = pPanel.GetProperty();
		if( ! property || ! pPanel.GetClipboard() ){
			return;
		}
		
		deObjectReference clipProperty;
		clipProperty.TakeOver( new gdeProperty( *property ) );
		
		igdeClipboardDataReference clipData;
		clipData.TakeOver( new gdeClipboardDataProperty( ( gdeProperty* )( deObject* )clipProperty ) );
		
		pPanel.GetClipboard()->Set( clipData );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetProperty() != NULL );
	}
};

class cActionPropertyCut : public cActionPropertyCopy {
public:
	cActionPropertyCut( gdeWPPropertyList &panel ) : cActionPropertyCopy( panel ){
		SetText( "Cut" );
		SetIcon( panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiCut ) );
		SetDescription( "Cut selected property" );
	}
	
	virtual void OnAction(){
		gdeProperty * const property = pPanel.GetProperty();
		if( ! property || ! pPanel.GetUndoSystem() || ! pPanel.GetClipboard() ){
			return;
		}
		
		cActionPropertyCopy::OnAction();
		
		igdeUndoReference undo;
		undo.TakeOver( pPanel.UndoRemove( property ) );
		if( undo ){
			pPanel.GetUndoSystem()->Add( undo );
		}
	}
};

class cActionPropertyPaste : public igdeAction {
	gdeWPPropertyList &pPanel;
	
public:
	cActionPropertyPaste( gdeWPPropertyList &panel ) :
	igdeAction( "Paste", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ),
		"Paste property" ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		if( ! pPanel.GetClipboard() ){
			return;
		}
		
		const gdeClipboardDataProperty * const clip = ( const gdeClipboardDataProperty * )
			pPanel.GetClipboard()->GetWithTypeName( gdeClipboardDataProperty::TYPE_NAME );
		if( ! clip ){
			return;
		}
		
		const gdePropertyList &list = *pPanel.GetPropertyList();
		decString name( clip->GetProperty()->GetName() );
		
		while( list.HasNamed( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Paste Property", "Name exists already." );
			if( ! igdeCommonDialogs::GetString( &pPanel, "Paste Property", "Name:", name ) ){
				return;
			}
		}
		
		deObjectReference refProperty;
		refProperty.TakeOver( new gdeProperty( *clip->GetProperty() ) );
		gdeProperty * const property = ( gdeProperty* )( deObject* )refProperty;
		property->SetName( name );
		
		igdeUndoReference undo;
		undo.TakeOver( pPanel.UndoPaste( property ) );
		if( undo ){
			pPanel.GetUndoSystem()->Add( undo );
			pPanel.SelectProperty( ( gdeProperty* )( deObject* )property );
		}
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetPropertyList() && pPanel.GetClipboard()
			&& pPanel.GetClipboard()->HasWithTypeName( gdeClipboardDataProperty::TYPE_NAME ) );
	}
};


class cBaseTextFieldListener : public igdeTextFieldListener {
protected:
	gdeWPPropertyList &pPanel;
	
public:
	cBaseTextFieldListener( gdeWPPropertyList &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		gdeProperty * const property = pPanel.GetProperty();
		if( ! property || ! pPanel.GetUndoSystem() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( textField, property ) );
		if( undo ){
			pPanel.GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, gdeProperty *property ) = 0;
};

class cBaseTextAreaListener : public igdeTextAreaListener {
protected:
	gdeWPPropertyList &pPanel;
	
public:
	cBaseTextAreaListener( gdeWPPropertyList &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextArea *textArea ){
		gdeProperty * const property = pPanel.GetProperty();
		if( ! property || ! pPanel.GetUndoSystem() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( textArea, property ) );
		if( undo ){
			pPanel.GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextArea *textArea, gdeProperty *property ) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener {
protected:
	gdeWPPropertyList &pPanel;
	
public:
	cBaseComboBoxListener( gdeWPPropertyList &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		gdeProperty * const property = pPanel.GetProperty();
		if( ! property || ! pPanel.GetUndoSystem() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( comboBox, property ) );
		if( undo ){
			pPanel.GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, gdeProperty *property ) = 0;
};

class cBaseAction : public igdeAction {
protected:
	gdeWPPropertyList &pPanel;
	
public:
	cBaseAction( gdeWPPropertyList &panel, const char *text, igdeIcon *icon, const char *description ) :
	igdeAction( text, icon, description ), pPanel( panel ){ }
	
	virtual void OnAction(){
		gdeProperty * const property = pPanel.GetProperty();
		if( ! property || ! pPanel.GetUndoSystem() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnActionUndo( property ) );
		if( undo ){
			pPanel.GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnActionUndo( gdeProperty *property ) = 0;
	
	virtual void Update(){
		SetEnabled( pPanel.GetProperty() != NULL );
	}
};


class cEditName : public cBaseTextFieldListener {
public:
	cEditName( gdeWPPropertyList &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, gdeProperty *property ){
		const decString &name = textField->GetText();
		if( name == property->GetName() ){
			return NULL;
		}
		
		if( pPanel.GetPropertyList()->HasNamed( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Rename property", "Name exists already." );
			textField->SetText( property->GetName() );
			return NULL;
		}
		
		return pPanel.UndoName( property, name );
	}
};

class cEditDescription : public cBaseTextAreaListener {
public:
	cEditDescription( gdeWPPropertyList &panel ) : cBaseTextAreaListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextArea *textArea, gdeProperty *property ){
		if( textArea->GetText() == property->GetDescription() ){
			return NULL;
		}
		return pPanel.UndoDescription( property, textArea->GetText() );
	}
};

class cComboType : public cBaseComboBoxListener {
public:
	cComboType( gdeWPPropertyList &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, gdeProperty *property ){
		const gdeProperty::ePropertyTypes type =
			( gdeProperty::ePropertyTypes )( intptr_t )comboBox->GetSelectedItem()->GetData();
		if( type == property->GetType() ){
			return NULL;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( pPanel.UndoType( property, type ) );
		pPanel.GetUndoSystem()->Add( undo );
		
		pPanel.SetDefaultValueFromType();
		return NULL;
	}
};

class cEditDefault : public igdeEditPropertyValueListener {
	gdeWPPropertyList &pPanel;
	igdeUndoReference pUndo;
	decString pOldValue;
	
public:
	cEditDefault( gdeWPPropertyList &panel ) : pPanel( panel ){ }
	
	virtual void OnPropertyValueChanged( igdeEditPropertyValue *editPropertyValue ){
		gdeProperty * const property = pPanel.GetProperty();
		if( ! property || ! pPanel.GetUndoSystem() ){
			return;
		}
		
		if( ! pUndo ){
			pOldValue = property->GetDefaultValue();
		}
		pUndo.TakeOver( pPanel.UndoDefaultValue( property, editPropertyValue->GetValue(), pOldValue ) );
		if( pUndo ){
			pPanel.GetUndoSystem()->Add( pUndo );
			pUndo = NULL;
		}
	}
	
	virtual void OnPropertyValueChanging( igdeEditPropertyValue *editPropertyValue ){
		gdeProperty * const property = pPanel.GetProperty();
		if( ! property || ! pPanel.GetUndoSystem() ){
			return;
		}
		
		if( ! pUndo ){
			pOldValue = property->GetDefaultValue();
		}
		pUndo.TakeOver( pPanel.UndoDefaultValue( property, editPropertyValue->GetValue(), pOldValue ) );
		if( pUndo ){
			pUndo->Redo();
		}
	}
};

class cEditMinimum : public cBaseTextFieldListener {
public:
	cEditMinimum( gdeWPPropertyList &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, gdeProperty *property ){
		const float value = textField->GetFloat();
		if( fabsf( value - property->GetMinimumValue() ) < FLOAT_SAFE_EPSILON ){
			return NULL;
		}
		return pPanel.UndoMinimumValue( property, value );
	}
};

class cEditMaximum : public cBaseTextFieldListener {
public:
	cEditMaximum( gdeWPPropertyList &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, gdeProperty *property ){
		const float value = textField->GetFloat();
		if( fabsf( value - property->GetMaximumValue() ) < FLOAT_SAFE_EPSILON ){
			return NULL;
		}
		return pPanel.UndoMaximumValue( property, value );
	}
};


class cListOptions : public igdeListBoxListener {
	gdeWPPropertyList &pPanel;
	
public:
	cListOptions( gdeWPPropertyList &panel ) : pPanel( panel ){ }
	
	virtual void OnSelectionChanged( igdeListBox* ){
	}
	
	virtual void AddContextMenuEntries( igdeListBox *listBox, igdeMenuCascade &menu ){
		igdeUIHelper &helper = listBox->GetEnvironment().GetUIHelper();
		helper.MenuCommand( menu, pPanel.GetActionOptionAdd() );
		helper.MenuCommand( menu, pPanel.GetActionOptionRemove() );
	}
};

class cActionOptionAdd : public cBaseAction {
public:
	cActionOptionAdd( gdeWPPropertyList &panel ) : cBaseAction( panel, "Add Option...",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add option" ){ }
	
	virtual igdeUndo *OnActionUndo( gdeProperty *property ){
		decString option( "Option" );
		
		while( igdeCommonDialogs::GetString( &pPanel, "Add Option", "Option:", option ) ){
			if( property->GetOptions().Has( option ) ){
				igdeCommonDialogs::Error( &pPanel, "Add Option", "Option exists already." );
				continue;
			}
			
			decStringList options( property->GetOptions() );
			options.Add( option );
			return pPanel.UndoOptions( property, options );
		}
		
		return NULL;
	}
};

class cActionOptionRemove : public cBaseAction {
public:
	cActionOptionRemove( gdeWPPropertyList &panel ) : cBaseAction( panel, "Remove Option",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove option" ){ }
	
	virtual igdeUndo *OnActionUndo( gdeProperty *property ){
		const decString option( pPanel.GetOption() );
		if( option.IsEmpty() ){
			return NULL;
		}
		
		decStringList options( property->GetOptions() );
		options.RemoveFrom( options.IndexOf( option ) );
		return pPanel.UndoOptions( property, options );
	}
};


class cComboPathPatternType : public cBaseComboBoxListener {
public:
	cComboPathPatternType( gdeWPPropertyList &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, gdeProperty *property ){
		const gdeProperty::ePathPatternTypes type =
			( gdeProperty::ePathPatternTypes )( intptr_t )comboBox->GetSelectedItem()->GetData();
		if( type == property->GetPathPatternType() ){
			return NULL;
		}
		return pPanel.UndoPathPatternType( property, type );
	}
};

class cComboCustomPattern : public igdeComboBoxListener {
	gdeWPPropertyList &pPanel;
	
public:
	cComboCustomPattern( gdeWPPropertyList &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox* ){
		pPanel.UpdateCustomPattern();
	}
};


class cActionCustomPatternMenu : public igdeActionContextMenu {
	gdeWPPropertyList &pPanel;
	
public:
	cActionCustomPatternMenu( gdeWPPropertyList &panel ) :
	igdeActionContextMenu( "", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallDown ),
		"Show Custom Pattern Menu" ),
	pPanel( panel ){ }
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu ){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand( contextMenu, pPanel.GetActionCustomPatternAdd() );
		helper.MenuCommand( contextMenu, pPanel.GetActionCustomPatternRemove() );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetPropertyList() != NULL );
	}
};

class cActionCustomPatternAdd : public igdeAction {
	gdeWPPropertyList &pPanel;
	
public:
	cActionCustomPatternAdd( gdeWPPropertyList &panel ) : igdeAction( "Add...",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add custom pattern" ),
		pPanel( panel ){ }
	
	virtual void OnAction(){
		gdeProperty * const property = pPanel.GetProperty();
		if( ! property || ! pPanel.GetUndoSystem() ){
			return;
		}
		
		decString name( "File Pattern" );
		if( ! igdeCommonDialogs::GetString( &pPanel, "Add File Pattern", "Name:", name ) ){
			return;
		}
		
		deObjectReference refFilePattern;
		refFilePattern.TakeOver( new gdeFilePattern( name, "*.ext", "*.ext" ) );
		gdeFilePattern * const filePattern = ( gdeFilePattern* )( deObject* )refFilePattern;
		
		igdeUndoReference undo;
		undo.TakeOver( pPanel.UndoCustomFilePatternAdd( property, filePattern ) );
		pPanel.GetUndoSystem()->Add( undo );
		
		pPanel.SelectCustomPattern( filePattern );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetProperty() != NULL );
	}
};

class cActionCustomPatternRemove : public cBaseAction {
public:
	cActionCustomPatternRemove( gdeWPPropertyList &panel ) : cBaseAction( panel, "Remove",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove custom pattern" ){ }
	
	virtual igdeUndo *OnActionUndo( gdeProperty *property ){
		gdeFilePattern * const filePattern = pPanel.GetCustomPattern();
		if( ! filePattern ){
			return NULL;
		}
		return pPanel.UndoCustomFilePatternRemove( property, filePattern );
	}
};


class cBaseTextFieldListenerCustomPattern : public cBaseTextFieldListener {
public:
	cBaseTextFieldListenerCustomPattern( gdeWPPropertyList &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, gdeProperty *property ){
		gdeFilePattern * const filePattern = pPanel.GetCustomPattern();
		return filePattern ? OnChangedPattern( textField, property, filePattern ) : NULL;
	}
	
	virtual igdeUndo *OnChangedPattern( igdeTextField *textField,
		gdeProperty *property, gdeFilePattern *filePattern ) = 0;
};


class cEditCustomPatternName : public cBaseTextFieldListenerCustomPattern {
public:
	cEditCustomPatternName( gdeWPPropertyList &panel ) : cBaseTextFieldListenerCustomPattern( panel ){ }
	
	virtual igdeUndo *OnChangedPattern( igdeTextField *textField, gdeProperty *property, gdeFilePattern *filePattern ){
		if( textField->GetText() == filePattern->GetName() ){
			return NULL;
		}
		return pPanel.UndoCustomFilePatternName( property, filePattern, textField->GetText() );
	}
};

class cEditCustomPatternPattern : public cBaseTextFieldListenerCustomPattern {
public:
	cEditCustomPatternPattern( gdeWPPropertyList &panel ) : cBaseTextFieldListenerCustomPattern( panel ){ }
	
	virtual igdeUndo *OnChangedPattern( igdeTextField *textField, gdeProperty *property, gdeFilePattern *filePattern ){
		if( textField->GetText() == filePattern->GetPattern() ){
			return NULL;
		}
		return pPanel.UndoCustomFilePatternPattern( property, filePattern, textField->GetText() );
	}
};

class cEditCustomPatternExtension : public cBaseTextFieldListenerCustomPattern {
public:
	cEditCustomPatternExtension( gdeWPPropertyList &panel ) : cBaseTextFieldListenerCustomPattern( panel ){ }
	
	virtual igdeUndo *OnChangedPattern( igdeTextField *textField, gdeProperty *property, gdeFilePattern *filePattern ){
		if( textField->GetText() == filePattern->GetDefaultExtension() ){
			return NULL;
		}
		return pPanel.UndoCustomFilePatternExtension( property, filePattern, textField->GetText() );
	}
};


class cComboIdentifierGroup : public cBaseComboBoxListener {
public:
	cComboIdentifierGroup( gdeWPPropertyList &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, gdeProperty *property ){
		if( comboBox->GetText() == property->GetIdentifierGroup() ){
			return NULL;
		}
		return pPanel.UndoIdentifierGroup( property, comboBox->GetText() );
	}
};

class cActionIdentifierUsage : public cBaseAction {
public:
	cActionIdentifierUsage( gdeWPPropertyList &panel ) : cBaseAction( panel,
		"Defines Identifier", NULL, "Property defines identifier" ){ }
	
	virtual igdeUndo *OnActionUndo( gdeProperty *property ){
		return pPanel.UndoIdentifierUsage( property );
	}
	
	virtual void Update(){ /* empty on purpose! */ }
};

}



// Class gdeWPPropertyList
////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPPropertyList::gdeWPPropertyList( igdeEnvironment &environment ) :
igdeContainerFlow( environment, igdeContainerFlow::eaY ),
pPropertyList( NULL ),
pGameDefinition( NULL ),
pClipboard( NULL )
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference form, frameLine, group;
	
	
	pIconIdentifierUsage = env.GetStockIcon( igdeEnvironment::esiSmallPlus );
	pIconIdentifierNoUsage = env.GetStockIcon( igdeEnvironment::esiSmallMinus );
	
	
	pActionPropertiesMenu.TakeOver( new cActionPropertyMenu( *this ) );
	pActionPropertyAdd.TakeOver( new cActionPropertyAdd( *this ) );
	pActionPropertyRemove.TakeOver( new cActionPropertyRemove( *this ) );
	pActionPropertyCopy.TakeOver( new cActionPropertyCopy( *this ) );
	pActionPropertyCut.TakeOver( new cActionPropertyCut( *this ) );
	pActionPropertyPaste.TakeOver( new cActionPropertyPaste( *this ) );
	
	pActionOptionAdd.TakeOver( new cActionOptionAdd( *this ) );
	pActionOptionRemove.TakeOver( new cActionOptionRemove( *this ) );
	
	pActionCustomPatternAdd.TakeOver( new cActionCustomPatternAdd( *this ) );
	pActionCustomPatternRemove.TakeOver( new cActionCustomPatternRemove( *this ) );
	
	pActionCustomPatternMenu.TakeOver( new cActionCustomPatternMenu( *this ) );
	
	form.TakeOver( new igdeContainerForm( env ) );
	AddChild( form );
	
	
	helper.FormLineStretchFirst( form, "Property:", "Property to edit", frameLine );
	helper.ComboBox( frameLine, "Property to edit", pCBProperties, new cComboProperty( *this ) );
	pCBProperties->SetDefaultSorter();
	helper.Button( frameLine, pBtnMenuProperties, pActionPropertiesMenu );
	pActionPropertiesMenu->SetWidget( pBtnMenuProperties );
	
	helper.EditString( form, "Name:", "Property name", pEditName, new cEditName( *this ) );
	helper.EditString( form, "Description:", "Property description", pEditDescription,
		15, 3, new cEditDescription( *this ) );
	
	helper.ComboBox( form, "Type:", "Property type", pCBType, new cComboType( *this ) );
	pCBType->AddItem( "String", NULL, ( void* )( intptr_t )gdeProperty::eptString );
	pCBType->AddItem( "Integer", NULL, ( void* )( intptr_t )gdeProperty::eptInteger );
	pCBType->AddItem( "Point2", NULL, ( void* )( intptr_t )gdeProperty::eptPoint2 );
	pCBType->AddItem( "Point3", NULL, ( void* )( intptr_t )gdeProperty::eptPoint3 );
	pCBType->AddItem( "Float", NULL, ( void* )( intptr_t )gdeProperty::eptFloat );
	pCBType->AddItem( "Vector2", NULL, ( void* )( intptr_t )gdeProperty::eptVector2 );
	pCBType->AddItem( "Vector3", NULL, ( void* )( intptr_t )gdeProperty::eptVector3 );
	pCBType->AddItem( "Color", NULL, ( void* )( intptr_t )gdeProperty::eptColor );
	pCBType->AddItem( "Boolean", NULL, ( void* )( intptr_t )gdeProperty::eptBoolean );
	pCBType->AddItem( "Path", NULL, ( void* )( intptr_t )gdeProperty::eptPath );
	pCBType->AddItem( "Range", NULL, ( void* )( intptr_t )gdeProperty::eptRange );
	pCBType->AddItem( "Select", NULL, ( void* )( intptr_t )gdeProperty::eptSelect );
	pCBType->AddItem( "List", NULL, ( void* )( intptr_t )gdeProperty::eptList );
	pCBType->AddItem( "Trigger expression", NULL, ( void* )( intptr_t )gdeProperty::eptTriggerExpression );
	pCBType->AddItem( "Trigger target", NULL, ( void* )( intptr_t )gdeProperty::eptTriggerTarget );
	pCBType->AddItem( "Shape", NULL, ( void* )( intptr_t )gdeProperty::eptShape );
	pCBType->AddItem( "Shape list", NULL, ( void* )( intptr_t )gdeProperty::eptShapeList );
	pCBType->AddItem( "Identifier", NULL, ( void* )( intptr_t )gdeProperty::eptIdentifier );
	
	gdeDefaultPropertyValue::CreateAndAdd( form, helper, "Default:", "Default property value",
		pEditDefault, new cEditDefault( *this ) );
	
	
	// type specific parameters
	helper.GroupBoxFlow( *this, group, "Parameters:" );
	pSwiParameters.TakeOver( new igdeSwitcher( env ) );
	group->AddChild( pSwiParameters );
	
	
	// no additional parameters
	helper.Label( pSwiParameters, "< No Parameters >" );
	
	
	// range
	form.TakeOver( new igdeContainerForm( env ) );
	pSwiParameters->AddChild( form );
	
	helper.EditFloat( form, "Minimum:", "Minium value for range type property",
		pEditMinimum, new cEditMinimum( *this ) );
	helper.EditFloat( form, "Maximum:", "Maximum value for range type property",
		pEditMaximum, new cEditMaximum( *this ) );
	
	
	// selection
	helper.ListBox( pSwiParameters, 6, "Allowed options", pListOptions, new cListOptions( *this ) );
	
	
	// path
	form.TakeOver( new igdeContainerForm( env ) );
	pSwiParameters->AddChild( form );
	
	helper.ComboBox( form, "Pattern type:", "Path pattern type",
		pCBPathPatternType, new cComboPathPatternType( *this ) );
	pCBPathPatternType->AddItem( "All files", NULL, ( void* )( intptr_t )gdeProperty::epptAll );
	pCBPathPatternType->AddItem( "Model resources", NULL, ( void* )( intptr_t )gdeProperty::epptModel );
	pCBPathPatternType->AddItem( "Skin resources", NULL, ( void* )( intptr_t )gdeProperty::epptSkin );
	pCBPathPatternType->AddItem( "Rig resources", NULL, ( void* )( intptr_t )gdeProperty::epptRig );
	pCBPathPatternType->AddItem( "Animation resources", NULL, ( void* )( intptr_t )gdeProperty::epptAnimation );
	pCBPathPatternType->AddItem( "Animator resources", NULL, ( void* )( intptr_t )gdeProperty::epptAnimator );
	pCBPathPatternType->AddItem( "Image resources", NULL, ( void* )( intptr_t )gdeProperty::epptImage );
	pCBPathPatternType->AddItem( "Occlusion mesh resources", NULL, ( void* )( intptr_t )gdeProperty::epptOcclusionMesh );
	pCBPathPatternType->AddItem( "Navigation space resources", NULL, ( void* )( intptr_t )gdeProperty::epptNavigationSpace );
	pCBPathPatternType->AddItem( "Particle emitter resources", NULL, ( void* )( intptr_t )gdeProperty::epptParticleEmitter );
	pCBPathPatternType->AddItem( "Sound resources", NULL, ( void* )( intptr_t )gdeProperty::epptSound );
	pCBPathPatternType->AddItem( "Synthesizer resources", NULL, ( void* )( intptr_t )gdeProperty::epptSynthesizer );
	pCBPathPatternType->AddItem( "Video resources", NULL, ( void* )( intptr_t )gdeProperty::epptVideo );
	pCBPathPatternType->AddItem( "Font resources", NULL, ( void* )( intptr_t )gdeProperty::epptFont );
	pCBPathPatternType->AddItem( "Sky resources", NULL, ( void* )( intptr_t )gdeProperty::epptSky );
	pCBPathPatternType->AddItem( "Custom file pattern", NULL, ( void* )( intptr_t )gdeProperty::epptCustom );
	
	helper.FormLineStretchFirst( form, "Custom pattern:", "Custom pattern to edit.", frameLine );
	helper.ComboBox( frameLine, "Custom pattern to edit.", pCBCustomPattern,
		new cComboCustomPattern( *this ) );
	pCBCustomPattern->SetDefaultSorter();
	helper.Button( frameLine, pBtnCustomPatternMenu, pActionCustomPatternMenu );
	pActionCustomPatternMenu->SetWidget( pBtnCustomPatternMenu );
	
	helper.EditString( form, "Name:", "Name of pattern in file dialogs",
		pCustomPatternEditName, new cEditCustomPatternName( *this ) );
	helper.EditString( form, "Pattern:", "Pattern",
		pCustomPatternEditPattern, new cEditCustomPatternPattern( *this ) );
	helper.EditString( form, "Extension:", "Extension to use for saving files",
		pCustomPatternEditExtension, new cEditCustomPatternExtension( *this ) );
	
	
	// identifier
	form.TakeOver( new igdeContainerForm( env ) );
	pSwiParameters->AddChild( form );
	
	helper.ComboBox( form, "Group:", true, "Identifier group name", pCBIdentifierGroup,
		new cComboIdentifierGroup( *this ) );
	pCBIdentifierGroup->SetDefaultSorter();
	
	helper.CheckBox( form, pChkIdentifierUsage, new cActionIdentifierUsage( *this ), true );
}

gdeWPPropertyList::~gdeWPPropertyList(){
}



// Management
///////////////

void gdeWPPropertyList::SetPropertyList( const gdePropertyList *propertyList ){
	if( propertyList == pPropertyList ){
		return;
	}
	
	pPropertyList = propertyList;
	
	UpdateList();
}

void gdeWPPropertyList::SetGameDefinition( gdeGameDefinition *gameDefinition ){
	pGameDefinition = gameDefinition;
}

igdeUndoSystem *gdeWPPropertyList::GetUndoSystem() const{
	return pGameDefinition ? pGameDefinition->GetUndoSystem() : NULL;
}

void gdeWPPropertyList::SetClipboard( igdeClipboard *clipboard ){
	pClipboard = clipboard;
}



gdeProperty *gdeWPPropertyList::GetProperty() const{
	if( ! pPropertyList ){
		return NULL;
	}
	
	const igdeListItem * const selection = pCBProperties->GetSelectedItem();
	return selection ? ( gdeProperty* )selection->GetData() : NULL;
}

decString gdeWPPropertyList::GetOption() const{
	const gdeProperty * const property = GetProperty();
	return property && pListOptions->GetSelectedItem() ? pListOptions->GetSelectedItem()->GetText() : "";
}

int gdeWPPropertyList::GetCustomPatternIndex() const{
	const gdeProperty * const property = GetProperty();
	return property ? pCBCustomPattern->GetSelection() : -1;
}

gdeFilePattern *gdeWPPropertyList::GetCustomPattern() const{
	const gdeProperty * const property = GetProperty();
	return property && pCBCustomPattern->GetSelectedItem() ?
		( gdeFilePattern* )pCBCustomPattern->GetSelectedItem()->GetData() : NULL;
}



void gdeWPPropertyList::UpdateList(){
	gdeProperty * const property = GetProperty();
	
	pCBProperties->RemoveAllItems();
	
	if( pPropertyList ){
		const int count = pPropertyList->GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			gdeProperty * const property = pPropertyList->GetAt( i );
			pCBProperties->AddItem( property->GetName(), NULL, property );
		}
		
		pCBProperties->SortItems();
	}
	
	pCBProperties->SetSelectionWithData( property );
	if( pCBProperties->GetSelection() == -1 && pCBProperties->GetItemCount() > 0 ){
		pCBProperties->SetSelection( 0 );
	}
	
	UpdateProperty();
}

void gdeWPPropertyList::UpdateProperty(){
	const gdeProperty * const property = GetProperty();
	
	if( property ){
		pEditName->SetText( property->GetName() );
		pEditDescription->SetText( property->GetDescription() );
		pCBType->SetSelectionWithData( ( void* )( intptr_t )property->GetType() );
		pEditMinimum->SetFloat( property->GetMinimumValue() );
		pEditMaximum->SetFloat( property->GetMaximumValue() );
		( ( gdeDefaultPropertyValue& )( igdeEditPropertyValue& )pEditDefault ).SetValue(
			property->GetDefaultValue(), *property );
		
		const decString selectedOption( pListOptions->GetSelectedItem()
			? pListOptions->GetSelectedItem()->GetText() : "" );
		const decStringList &options = property->GetOptions();
		const int optionCount = options.GetCount();
		int i;
		pListOptions->RemoveAllItems();
		for( i=0; i<optionCount; i++ ){
			pListOptions->AddItem( options.GetAt( i ) );
		}
		pListOptions->SetSelection( pListOptions->IndexOfItem( selectedOption ) );
		
		pCBPathPatternType->SetSelectionWithData( ( void* )( intptr_t )property->GetPathPatternType() );
		
		UpdatePropertyIdentifierList();
		pCBIdentifierGroup->SetText( property->GetIdentifierGroup() );
		
		pChkIdentifierUsage->SetChecked( property->GetIdentifierUsage() );
		
	}else{
		pEditName->ClearText();
		pEditDescription->ClearText();
		pCBType->SetSelectionWithData( ( void* )( intptr_t )gdeProperty::eptString );
		pEditMinimum->ClearText();
		pEditMaximum->ClearText();
		pEditDefault->ClearValue();
		pListOptions->RemoveAllItems();
		pCBPathPatternType->SetSelectionWithData( ( void* )( intptr_t )gdeProperty::epptAll );
		pCBIdentifierGroup->RemoveAllItems();
		pCBIdentifierGroup->ClearText();
		pChkIdentifierUsage->SetChecked( false );
	}
	
	UpdateCustomPatternList();
	UpdateEnabled();
}

void gdeWPPropertyList::UpdatePropertyIdentifierList(){
	pCBIdentifierGroup->RemoveAllItems();
	
	if( pGameDefinition ){
		const decStringSet &definedIDs = pGameDefinition->GetDefinedIDs();
		int i, count = definedIDs.GetCount();
		for( i=0; i<count; i++ ){
			pCBIdentifierGroup->AddItem( definedIDs.GetAt( i ), pIconIdentifierNoUsage );
		}
		
		const decStringSet &usedIDs = pGameDefinition->GetUsedIDs();
		count = usedIDs.GetCount();
		for( i=0; i<count; i++ ){
			const decString &id = usedIDs.GetAt( i );
			const int index = pCBIdentifierGroup->IndexOfItem( id );
			if( index == -1 ){
				pCBIdentifierGroup->AddItem( id, pIconIdentifierUsage );
				
			}else{
				igdeListItem &item = *pCBIdentifierGroup->GetItemAt( index );
				if( item.GetIcon() == pIconIdentifierNoUsage ){
					item.SetIcon( pIconIdentifierUsage );
					pCBIdentifierGroup->ItemChangedAt( index );
				}
			}
		}
	}
	
	if( pPropertyList ){
		const int propertyCount = pPropertyList->GetCount();
		int i;
		
		for( i=0; i<propertyCount; i++ ){
			const gdeProperty &property = *pPropertyList->GetAt( i );
			if( property.GetType() != gdeProperty::eptIdentifier ){
				continue;
			}
			
			const decString &group = property.GetIdentifierGroup();
			if( group.IsEmpty() ){
				continue;
			}
			
			const int index = pCBIdentifierGroup->IndexOfItem( group );
			if( index == -1 ){
				pCBIdentifierGroup->AddItem( group, property.GetIdentifierUsage()
					? pIconIdentifierUsage : pIconIdentifierNoUsage );
				
			}else if( property.GetIdentifierUsage() ){
				igdeListItem &item = *pCBIdentifierGroup->GetItemAt( index );
				if( item.GetIcon() == pIconIdentifierNoUsage ){
					item.SetIcon( pIconIdentifierUsage );
					pCBIdentifierGroup->ItemChangedAt( index );
				}
			}
		}
	}
}

void gdeWPPropertyList::UpdateCustomPatternList(){
	const gdeProperty * const property = GetProperty();
	
	if( property ){
		gdeFilePattern * const selection = pCBCustomPattern->GetSelectedItem()
			? ( gdeFilePattern* )pCBCustomPattern->GetSelectedItem()->GetData() : NULL;
		
		const gdeFilePatternList &list = property->GetCustomPathPattern();
		const int count = list.GetCount();
		int i;
		
		pCBCustomPattern->RemoveAllItems();
		for( i=0; i<count; i++ ){
			gdeFilePattern * const pattern = list.GetAt( i );
			pCBCustomPattern->AddItem( pattern->GetName(), NULL, pattern );
		}
		pCBCustomPattern->SortItems();
		pCBCustomPattern->SetSelectionWithData( selection );
		
	}else{
		pCBCustomPattern->RemoveAllItems();
	}
	
	UpdateCustomPattern();
}

void gdeWPPropertyList::UpdateCustomPattern(){
	const gdeFilePattern * const filePattern = GetCustomPattern();
	
	if( filePattern ){
		pCustomPatternEditName->SetText( filePattern->GetName() );
		pCustomPatternEditPattern->SetText( filePattern->GetPattern() );
		pCustomPatternEditExtension->SetText( filePattern->GetDefaultExtension() );
		
	}else{
		pCustomPatternEditName->ClearText();
		pCustomPatternEditPattern->ClearText();
		pCustomPatternEditExtension->ClearText();
	}
}

void gdeWPPropertyList::UpdateEnabled(){
	const gdeProperty * const property = GetProperty();
	
	const bool hasList = pPropertyList;
	const bool hasProperty = property;
	
	pCBProperties->SetEnabled( hasList );
	pBtnMenuProperties->SetEnabled( hasList );
	pEditName->SetEnabled( hasProperty );
	pEditDescription->SetEnabled( hasProperty );
	pCBType->SetEnabled( hasProperty );
	
	pEditDefault->SetEnabled( hasProperty );
	
	pEditMinimum->SetEnabled( hasProperty );
	pEditMaximum->SetEnabled( hasProperty );
	
	pListOptions->SetEnabled( hasProperty );
	
	const bool hasCustomPattern = property && property->GetPathPatternType() == gdeProperty::epptCustom;
	pCBPathPatternType->SetEnabled( hasProperty );
	pCBCustomPattern->SetEnabled( hasCustomPattern );
	pBtnCustomPatternMenu->SetEnabled( hasCustomPattern );
	pCustomPatternEditName->SetEnabled( hasCustomPattern );
	pCustomPatternEditPattern->SetEnabled( hasCustomPattern );
	pCustomPatternEditExtension->SetEnabled( hasCustomPattern );
	
	pCBIdentifierGroup->SetEnabled( hasProperty );
	pChkIdentifierUsage->SetEnabled( hasProperty );
	
	if( property ){
		switch( property->GetType() ){
		case gdeProperty::eptRange:
			pSwiParameters->SetCurrent( 1 );  // range
			break;
			
		case gdeProperty::eptSelect:
			pSwiParameters->SetCurrent( 2 );  // select
			break;
			
		case gdeProperty::eptPath:
			pSwiParameters->SetCurrent( 3 );  // path
			break;
			
		case gdeProperty::eptIdentifier:
			pSwiParameters->SetCurrent( 4 );  // identifier
			break;
			
		default:
			pSwiParameters->SetCurrent( 0 );  // nothing
		}
		
	}else{
		pSwiParameters->SetCurrent( 0 );  // nothing
	}
}

void gdeWPPropertyList::SelectProperty( gdeProperty *property ){
	pCBProperties->SetSelectionWithData( property );
	UpdateProperty();
}

void gdeWPPropertyList::SelectCustomPattern( gdeFilePattern *filePattern ){
	pCBCustomPattern->SetSelectionWithData( filePattern );
	UpdateCustomPattern();
}

void gdeWPPropertyList::SetDefaultValueFromType(){
	gdeProperty * const property = GetProperty();
	if( ! pGameDefinition || ! property ){
		return;
	}
	
	decString value;
	
	switch( ( gdeProperty::ePropertyTypes )( intptr_t )pCBType->GetSelectedItem()->GetData() ){
	case gdeProperty::eptInteger:
	case gdeProperty::eptFloat:
	case gdeProperty::eptBoolean:
		value = "0";
		break;
		
	case gdeProperty::eptPoint2:
	case gdeProperty::eptVector2:
		value = "0 0";
		break;
		
	case gdeProperty::eptPoint3:
	case gdeProperty::eptVector3:
	case gdeProperty::eptColor:
		value = "0 0 0";
		break;
		
	case gdeProperty::eptRange:
		pEditMinimum->GetText();
		break;
		
	case gdeProperty::eptSelect:
		if( pListOptions->GetItemCount() > 0 ){
			value = pListOptions->GetItemAt( 0 )->GetText();
		}
		break;
		
	default:
		break;
	}
	
	if( value == pEditDefault->GetValue() ){
		return;
	}
	
	igdeUndoReference undo;
	undo.TakeOver( UndoDefaultValue( property, value, pEditDefault->GetValue() ) );
	pGameDefinition->GetUndoSystem()->Add( undo );
}
