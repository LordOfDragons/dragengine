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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gdeWPGameDefinition.h"
#include "gdeWPGameDefinitionListener.h"
#include "gdeWindowProperties.h"
#include "gdeWPPathList.h"
#include "gdeWPPropertyList.h"
#include "../gdeWindowMain.h"
#include "../../clipboard/gdeClipboardDataProperty.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/filepattern/gdeFilePattern.h"
#include "../../gamedef/property/gdeProperty.h"
#include "../../undosys/gamedef/gdeUGDSetID.h"
#include "../../undosys/gamedef/gdeUGDSetDescription.h"
#include "../../undosys/gamedef/gdeUGDSetBasePath.h"
#include "../../undosys/gamedef/gdeUGDSetScriptModule.h"
#include "../../undosys/gamedef/gdeUGDSetVFSPath.h"
#include "../../undosys/gamedef/gdeUGDSetAutoFindPathObjectClasses.h"
#include "../../undosys/gamedef/gdeUGDSetAutoFindPathSkins.h"
#include "../../undosys/gamedef/gdeUGDSetAutoFindPathSkies.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPropertyAdd.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPropertyRemove.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPSetName.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPSetDescription.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPSetType.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPSetMinValue.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPSetMaxValue.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPSetDefaultValue.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPSetOptions.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPSetPathPatternType.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPSetIdentifierGroup.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPToggleIdentifierUsage.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPCFPAdd.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPCFPRemove.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPCFPSetName.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPCFPSetPattern.h"
#include "../../undosys/gamedef/worldProperty/gdeUGDWPCFPSetExtension.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPropertyAdd.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPropertyRemove.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPSetName.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPSetDescription.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPSetType.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPSetMinValue.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPSetMaxValue.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPSetDefaultValue.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPSetOptions.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPSetPathPatternType.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPSetIdentifierGroup.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPToggleIdentifierUsage.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPCFPAdd.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPCFPRemove.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPCFPSetName.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPCFPSetPattern.h"
#include "../../undosys/gamedef/decalProperty/gdeUGDDPCFPSetExtension.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeWindow.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeTextAreaListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decStringList.h>



// Actions
///////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener {
protected:
	gdeWPGameDefinition &pPanel;
	
public:
	cBaseTextFieldListener( gdeWPGameDefinition &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		gdeGameDefinition * const gameDefinition = pPanel.GetGameDefinition();
		if( ! gameDefinition || textField->GetText() == gameDefinition->GetID() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( *textField, gameDefinition ) );
		if( undo ){
			gameDefinition->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, gdeGameDefinition *gameDefinition ) = 0;
};

class cBaseAction : public igdeAction{
protected:
	gdeWPGameDefinition &pPanel;
	
public:
	cBaseAction( gdeWPGameDefinition &panel, const char *text, igdeIcon *icon,
		const char *description ) :
	igdeAction( text, icon, description ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		gdeGameDefinition * const gameDefinition = pPanel.GetGameDefinition();
		if( ! gameDefinition ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnActionGameDefinition( gameDefinition ) );
		if( undo ){
			gameDefinition->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnActionGameDefinition( gdeGameDefinition *gameDefinition ) = 0;
	
	virtual void Update(){
		SetEnabled( pPanel.GetGameDefinition() != NULL );
	}
};


class cEditId : public cBaseTextFieldListener {
public:
	cEditId( gdeWPGameDefinition &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, gdeGameDefinition *gameDefinition ){
		if( textField.GetText() == gameDefinition->GetID() ){
			return NULL;
		}
		return new gdeUGDSetID( gameDefinition, textField.GetText() );
	}
};

class cEditDescription : public igdeTextAreaListener {
	gdeWPGameDefinition &pPanel;
	
public:
	cEditDescription( gdeWPGameDefinition &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextArea *textArea ){
		gdeGameDefinition * const gameDefinition = pPanel.GetGameDefinition();
		if( ! gameDefinition || textArea->GetText() == gameDefinition->GetDescription() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new gdeUGDSetDescription( gameDefinition, textArea->GetText() ) );
		gameDefinition->GetUndoSystem()->Add( undo );
	}
};

class cEditBasePath : public cBaseTextFieldListener {
public:
	cEditBasePath( gdeWPGameDefinition &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, gdeGameDefinition *gameDefinition ){
		if( textField.GetText() == gameDefinition->GetBasePath() ){
			return NULL;
		}
		return new gdeUGDSetBasePath( gameDefinition, textField.GetText() );
	}
};

class cActionBasePath : public cBaseAction{
	igdeTextField &pTextField;
	
public:
	cActionBasePath( gdeWPGameDefinition &panel, igdeTextField &textField ) :
	cBaseAction( panel, "...", NULL, "Base path to project data files" ),
	pTextField( textField ){ }
	
	virtual igdeUndo *OnActionGameDefinition( gdeGameDefinition *gameDefinition ){
		decString basePath( gameDefinition->GetBasePath() );
		if( igdeCommonDialogs::GetDirectory( pPanel.GetParentWindow(), "Select Project Data Directory", basePath ) ){
			pTextField.SetText( basePath );
			pTextField.NotifyTextChanged();
		}
		return NULL;
	}
};

class cEditVfsPath : public cBaseTextFieldListener {
public:
	cEditVfsPath( gdeWPGameDefinition &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, gdeGameDefinition *gameDefinition ){
		if( textField.GetText() == gameDefinition->GetVFSPath() ){
			return NULL;
		}
		return new gdeUGDSetVFSPath( gameDefinition, textField.GetText() );
	}
};

class cEditScriptModule : public cBaseTextFieldListener {
public:
	cEditScriptModule( gdeWPGameDefinition &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, gdeGameDefinition *gameDefinition ){
		if( textField.GetText() == gameDefinition->GetScriptModule() ){
			return NULL;
		}
		return new gdeUGDSetScriptModule( gameDefinition, textField.GetText() );
	}
};


class cEditWorldProperties : public gdeWPPropertyList {
	gdeWPGameDefinition &pPanel;
	
public:
	cEditWorldProperties( gdeWPGameDefinition &panel ) :
	gdeWPPropertyList( panel.GetEnvironment() ), pPanel( panel ){
		SetClipboard( &panel.GetWindowProperties().GetWindowMain().GetClipboard() );
	}
	
	virtual igdeUndo *UndoAdd( gdeProperty *property ){
		return new gdeUGDWPropertyAdd( pPanel.GetGameDefinition(), property );
	}
	
	virtual igdeUndo *UndoRemove( gdeProperty *property ){
		return new gdeUGDWPropertyRemove( pPanel.GetGameDefinition(), property );
	}
	
	virtual igdeUndo *UndoPaste( gdeProperty *property ){
		gdeUGDWPropertyAdd * const undo = new gdeUGDWPropertyAdd( pPanel.GetGameDefinition(), property );
		undo->SetShortInfo( "Paste property" );
		return undo;
	}
	
	virtual igdeUndo *UndoName( gdeProperty *property, const decString &name ){
		return new gdeUGDWPSetName( pPanel.GetGameDefinition(), property, name );
	}
	
	virtual igdeUndo *UndoDescription( gdeProperty *property, const decString &description ){
		return new gdeUGDWPSetDescription( pPanel.GetGameDefinition(), property, description );
	}
	
	virtual igdeUndo *UndoType( gdeProperty *property, gdeProperty::ePropertyTypes type ){
		return new gdeUGDWPSetType( pPanel.GetGameDefinition(), property, type );
	}
	
	virtual igdeUndo *UndoMinimumValue( gdeProperty *property, float value ){
		return new gdeUGDWPSetMinValue( pPanel.GetGameDefinition(), property, value );
	}
	
	virtual igdeUndo *UndoMaximumValue( gdeProperty *property, float value ){
		return new gdeUGDWPSetMaxValue( pPanel.GetGameDefinition(), property, value );
	}
	
	virtual igdeUndo *UndoDefaultValue( gdeProperty *property, const decString &newValue, const decString &oldValue ){
		return new gdeUGDWPSetDefaultValue( pPanel.GetGameDefinition(), property, newValue, oldValue );
	}
	
	virtual igdeUndo *UndoOptions( gdeProperty *property, const decStringList &options ){
		return new gdeUGDWPSetOptions( pPanel.GetGameDefinition(), property, options );
	}
	
	virtual igdeUndo *UndoPathPatternType( gdeProperty *property, gdeProperty::ePathPatternTypes type ){
		return new gdeUGDWPSetPathPatternType( pPanel.GetGameDefinition(), property, type );
	}
	
	virtual igdeUndo *UndoIdentifierGroup( gdeProperty *property, const decString &identifier ){
		return new gdeUGDWPSetIdentifierGroup( pPanel.GetGameDefinition(), property, identifier );
	}
	
	virtual igdeUndo *UndoIdentifierUsage( gdeProperty *property ){
		return new gdeUGDWPToggleIdentifierUsage( pPanel.GetGameDefinition(), property );
	}
	
	virtual igdeUndo *UndoCustomFilePatternAdd( gdeProperty *property, gdeFilePattern *filePattern ){
		return new gdeUGDWPCFPAdd( pPanel.GetGameDefinition(), property, filePattern );
	}
	
	virtual igdeUndo *UndoCustomFilePatternRemove( gdeProperty *property, gdeFilePattern *filePattern ){
		return new gdeUGDWPCFPRemove( pPanel.GetGameDefinition(), property, filePattern );
	}
	
	virtual igdeUndo *UndoCustomFilePatternName( gdeProperty *property,
	gdeFilePattern *filePattern, const decString &name ){
		return new gdeUGDWPCFPSetName( pPanel.GetGameDefinition(), property, filePattern, name );
	}
	
	virtual igdeUndo *UndoCustomFilePatternPattern( gdeProperty *property,
	gdeFilePattern *filePattern, const decString &pattern ){
		return new gdeUGDWPCFPSetPattern( pPanel.GetGameDefinition(), property, filePattern, pattern );
	}
	
	virtual igdeUndo *UndoCustomFilePatternExtension( gdeProperty *property,
	gdeFilePattern *filePattern, const decString &extension ){
		return new gdeUGDWPCFPSetExtension( pPanel.GetGameDefinition(), property, filePattern, extension );
	}
};


class cEditDecalProperties : public gdeWPPropertyList {
	gdeWPGameDefinition &pPanel;
	
public:
	cEditDecalProperties( gdeWPGameDefinition &panel ) :
	gdeWPPropertyList( panel.GetEnvironment() ), pPanel( panel ){
		SetClipboard( &panel.GetWindowProperties().GetWindowMain().GetClipboard() );
	}
	
	virtual igdeUndo *UndoAdd( gdeProperty *property ){
		return new gdeUGDDPropertyAdd( pPanel.GetGameDefinition(), property );
	}
	
	virtual igdeUndo *UndoRemove( gdeProperty *property ){
		return new gdeUGDDPropertyRemove( pPanel.GetGameDefinition(), property );
	}
	
	virtual igdeUndo *UndoPaste( gdeProperty *property ){
		gdeUGDDPropertyAdd * const undo = new gdeUGDDPropertyAdd( pPanel.GetGameDefinition(), property );
		undo->SetShortInfo( "Paste property" );
		return undo;
	}
	
	virtual igdeUndo *UndoName( gdeProperty *property, const decString &name ){
		return new gdeUGDDPSetName( pPanel.GetGameDefinition(), property, name );
	}
	
	virtual igdeUndo *UndoDescription( gdeProperty *property, const decString &description ){
		return new gdeUGDDPSetDescription( pPanel.GetGameDefinition(), property, description );
	}
	
	virtual igdeUndo *UndoType( gdeProperty *property, gdeProperty::ePropertyTypes type ){
		return new gdeUGDDPSetType( pPanel.GetGameDefinition(), property, type );
	}
	
	virtual igdeUndo *UndoMinimumValue( gdeProperty *property, float value ){
		return new gdeUGDDPSetMinValue( pPanel.GetGameDefinition(), property, value );
	}
	
	virtual igdeUndo *UndoMaximumValue( gdeProperty *property, float value ){
		return new gdeUGDDPSetMaxValue( pPanel.GetGameDefinition(), property, value );
	}
	
	virtual igdeUndo *UndoDefaultValue( gdeProperty *property, const decString &newValue, const decString &oldValue ){
		return new gdeUGDDPSetDefaultValue( pPanel.GetGameDefinition(), property, newValue, oldValue );
	}
	
	virtual igdeUndo *UndoOptions( gdeProperty *property, const decStringList &options ){
		return new gdeUGDDPSetOptions( pPanel.GetGameDefinition(), property, options );
	}
	
	virtual igdeUndo *UndoPathPatternType( gdeProperty *property, gdeProperty::ePathPatternTypes type ){
		return new gdeUGDDPSetPathPatternType( pPanel.GetGameDefinition(), property, type );
	}
	
	virtual igdeUndo *UndoIdentifierGroup( gdeProperty *property, const decString &identifier ){
		return new gdeUGDDPSetIdentifierGroup( pPanel.GetGameDefinition(), property, identifier );
	}
	
	virtual igdeUndo *UndoIdentifierUsage( gdeProperty *property ){
		return new gdeUGDDPToggleIdentifierUsage( pPanel.GetGameDefinition(), property );
	}
	
	virtual igdeUndo *UndoCustomFilePatternAdd( gdeProperty *property, gdeFilePattern *filePattern ){
		return new gdeUGDDPCFPAdd( pPanel.GetGameDefinition(), property, filePattern );
	}
	
	virtual igdeUndo *UndoCustomFilePatternRemove( gdeProperty *property, gdeFilePattern *filePattern ){
		return new gdeUGDDPCFPRemove( pPanel.GetGameDefinition(), property, filePattern );
	}
	
	virtual igdeUndo *UndoCustomFilePatternName( gdeProperty *property,
	gdeFilePattern *filePattern, const decString &name ){
		return new gdeUGDDPCFPSetName( pPanel.GetGameDefinition(), property, filePattern, name );
	}
	
	virtual igdeUndo *UndoCustomFilePatternPattern( gdeProperty *property,
	gdeFilePattern *filePattern, const decString &pattern ){
		return new gdeUGDDPCFPSetPattern( pPanel.GetGameDefinition(), property, filePattern, pattern );
	}
	
	virtual igdeUndo *UndoCustomFilePatternExtension( gdeProperty *property,
	gdeFilePattern *filePattern, const decString &extension ){
		return new gdeUGDDPCFPSetExtension( pPanel.GetGameDefinition(), property, filePattern, extension );
	}
};


class cEditAutoFindPathObjectClasses : public gdeWPPathList {
	gdeWPGameDefinition &pPanel;
	
public:
	cEditAutoFindPathObjectClasses( gdeWPGameDefinition &panel ) :
	gdeWPPathList( panel.GetEnvironment().GetUIHelper(), 3, "Auto Find Path Object Classes" ),
	pPanel( panel ){ }
	
	virtual igdeUndo *UndoSet( const decStringSet &paths ){
		return new gdeUGDSetAutoFindPathObjectClasses( pPanel.GetGameDefinition(), paths );
	}
};

class cEditAutoFindPathSkins : public gdeWPPathList {
	gdeWPGameDefinition &pPanel;
	
public:
	cEditAutoFindPathSkins( gdeWPGameDefinition &panel ) :
	gdeWPPathList( panel.GetEnvironment().GetUIHelper(), 3, "Auto Find Path SkinsClasses" ),
	pPanel( panel ){ }
	
	virtual igdeUndo *UndoSet( const decStringSet &paths ){
		return new gdeUGDSetAutoFindPathSkins( pPanel.GetGameDefinition(), paths );
	}
};

class cEditAutoFindPathSkies : public gdeWPPathList {
	gdeWPGameDefinition &pPanel;
	
public:
	cEditAutoFindPathSkies( gdeWPGameDefinition &panel ) :
	gdeWPPathList( panel.GetEnvironment().GetUIHelper(), 3, "Auto Find Path Skies" ),
	pPanel( panel ){ }
	
	virtual igdeUndo *UndoSet( const decStringSet &paths ){
		return new gdeUGDSetAutoFindPathSkies( pPanel.GetGameDefinition(), paths );
	}
};

}



// Class gdeWPGameDefinition
//////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPGameDefinition::gdeWPGameDefinition( gdeWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pGameDefinition( NULL ),
pListener( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference content, groupBox, frameLine;
	
	pListener = new gdeWPGameDefinitionListener( *this );
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	// game definition
	helper.GroupBox( content, groupBox, "Game Definition:" );
	helper.EditString( groupBox, "Identifier:", "Identifier", pEditID, new cEditId( *this ) );
	helper.EditString( groupBox, "Description:", "Description",
		pEditDescription, 5, new cEditDescription( *this ) );
	
	helper.FormLineStretchFirst( groupBox, "Base Path:", "Base path to project data files", frameLine );
	helper.EditString( frameLine, "Base path to project data files",
		pEditBasePath, new cEditBasePath( *this ) );
	pActionBasePath.TakeOver( new cActionBasePath( *this, pEditBasePath ) );
	helper.Button( frameLine, pActionBasePath );
	
	helper.EditString( groupBox, "VFS Path:", "VFS path the base path will be visible at",
		pEditVFSPath, new cEditVfsPath( *this ) );
	helper.EditString( groupBox, "Script Module:", "Script module this game definition is compatible with",
		pEditScriptModule, new cEditScriptModule( *this ) );
	
	// information
	helper.GroupBox( content, groupBox, "Information:" );
	
	helper.EditString( groupBox, "Use Base Path:",
		"Actually used base path (can differ if this is the project game definition)",
		pEditUseBasePath, NULL );
	pEditUseBasePath->SetEditable( false );
	
	helper.CheckBox( groupBox, "Is Project Game Definition", "Is Project Game Definition", pChkIsProjectGameDef );
	pChkIsProjectGameDef->SetEnabled( false );
	
	// world properties
	helper.GroupBoxFlow( content, groupBox, "World Properties:", false, true );
	pEditWorldProperties.TakeOver( new cEditWorldProperties( *this ) );
	groupBox->AddChild( pEditWorldProperties );
	
	// decal properties
	helper.GroupBoxFlow( content, groupBox, "Decal Properties:", false, true );
	pEditDecalProperties.TakeOver( new cEditDecalProperties( *this ) );
	groupBox->AddChild( pEditDecalProperties );
	
	// auto find path
	helper.GroupBoxFlow( content, groupBox, "Auto Find Path Object Classes:", false, true );
	pEditAutoFindPathObjectClasses.TakeOver( new cEditAutoFindPathObjectClasses( *this ) );
	groupBox->AddChild( pEditAutoFindPathObjectClasses );
	
	helper.GroupBoxFlow( content, groupBox, "Auto Find Path Skins:", false, true );
	pEditAutoFindPathSkins.TakeOver( new cEditAutoFindPathSkins( *this ) );
	groupBox->AddChild( pEditAutoFindPathSkins );
	
	helper.GroupBoxFlow( content, groupBox, "Auto Find Path Skies:", false, true );
	pEditAutoFindPathSkies.TakeOver( new cEditAutoFindPathSkies( *this ) );
	groupBox->AddChild( pEditAutoFindPathSkies );
}

gdeWPGameDefinition::~gdeWPGameDefinition(){
	SetGameDefinition( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void gdeWPGameDefinition::SetGameDefinition( gdeGameDefinition *gameDefinition ){
	if( gameDefinition == pGameDefinition ){
		return;
	}
	
	gdeWPPropertyList &worldProperties = ( gdeWPPropertyList& )( igdeWidget& )pEditWorldProperties;
	worldProperties.SetPropertyList( NULL );
	worldProperties.SetGameDefinition( NULL );
	
	gdeWPPropertyList &decalProperties = ( gdeWPPropertyList& )( igdeWidget& )pEditDecalProperties;
	decalProperties.SetPropertyList( NULL );
	decalProperties.SetGameDefinition( NULL );
	
	gdeWPPathList &autoFindPathObjectClasses = ( gdeWPPathList& )( igdeWidget& )pEditAutoFindPathObjectClasses;
	autoFindPathObjectClasses.SetPathList( NULL );
	autoFindPathObjectClasses.SetUndoSystem( NULL );
	
	gdeWPPathList &autoFindPathSkins = ( gdeWPPathList& )( igdeWidget& )pEditAutoFindPathSkins;
	autoFindPathSkins.SetPathList( NULL );
	autoFindPathSkins.SetUndoSystem( NULL );
	
	gdeWPPathList &autoFindPathSkies = ( gdeWPPathList& )( igdeWidget& )pEditAutoFindPathSkies;
	autoFindPathSkies.SetPathList( NULL );
	autoFindPathSkies.SetUndoSystem( NULL );
	
	if( pGameDefinition ){
		pGameDefinition->RemoveListener( pListener );
		pGameDefinition->FreeReference();
	}
	
	pGameDefinition = gameDefinition;
	
	if( gameDefinition ){
		gameDefinition->AddListener( pListener );
		gameDefinition->AddReference();
		
		worldProperties.SetPropertyList( &gameDefinition->GetWorldProperties() );
		worldProperties.SetGameDefinition( gameDefinition );
		
		decalProperties.SetPropertyList( &gameDefinition->GetDecalProperties() );
		decalProperties.SetGameDefinition( gameDefinition );
		
		autoFindPathObjectClasses.SetPathList( &gameDefinition->GetAutoFindPathObjectClasses() );
		autoFindPathObjectClasses.SetUndoSystem( gameDefinition->GetUndoSystem() );
		
		autoFindPathSkins.SetPathList( &gameDefinition->GetAutoFindPathSkins() );
		autoFindPathSkins.SetUndoSystem( gameDefinition->GetUndoSystem() );
		
		autoFindPathSkies.SetPathList( &gameDefinition->GetAutoFindPathSkies() );
		autoFindPathSkies.SetUndoSystem( gameDefinition->GetUndoSystem() );
	}
	
	UpdateWorld();
}



gdeProperty *gdeWPGameDefinition::GetActiveWorldProperty() const{
	return ( ( gdeWPPropertyList& )( igdeWidget& )pEditWorldProperties ).GetProperty();
}

gdeProperty *gdeWPGameDefinition::GetActiveDecalProperty() const{
	return ( ( gdeWPPropertyList& )( igdeWidget& )pEditDecalProperties ).GetProperty();
}



void gdeWPGameDefinition::UpdateWorld(){
	if( pGameDefinition ){
		pEditID->SetText( pGameDefinition->GetID() );
		pEditDescription->SetText( pGameDefinition->GetDescription() );
		pEditBasePath->SetText( pGameDefinition->GetBasePath() );
		pEditVFSPath->SetText( pGameDefinition->GetVFSPath() );
		pEditScriptModule->SetText( pGameDefinition->GetScriptModule() );
		
		if( pGameDefinition->GetIsProjectGameDef() ){
			const igdeGameProject &gameProject = *GetEnvironment().GetGameProject();
			decPath path;
			path.SetFromNative( gameProject.GetDirectoryPath() );
			path.AddUnixPath( gameProject.GetPathData() );
			pEditUseBasePath->SetText( path.GetPathNative() );
			pChkIsProjectGameDef->SetChecked( true );
			
		}else{
			pEditUseBasePath->SetText( pGameDefinition->GetBasePath() );
			pChkIsProjectGameDef->SetChecked( false );
		}
		
	}else{
		pEditID->ClearText();
		pEditDescription->ClearText();
		pEditBasePath->ClearText();
		pEditVFSPath->ClearText();
		pEditScriptModule->ClearText();
		pEditUseBasePath->ClearText();
		pChkIsProjectGameDef->SetChecked( false );
	}
	
	const bool enabled = pGameDefinition != NULL;
	pEditID->SetEnabled( enabled );
	pEditDescription->SetEnabled( enabled );
	pEditBasePath->SetEnabled( enabled );
	pActionBasePath->Update();
	pEditVFSPath->SetEnabled( enabled );
	pEditScriptModule->SetEnabled( enabled );
	
	UpdateWorldProperties();
	UpdateDecalProperties();
	UpdateAutoFindPathObjectClasses();
	UpdateAutoFindPathSkins();
	UpdateAutoFindPathSkies();
}



void gdeWPGameDefinition::UpdateWorldProperties(){
	( ( gdeWPPropertyList& )( igdeWidget& )pEditWorldProperties ).UpdateList();
}

void gdeWPGameDefinition::UpdateWorldProperty(){
	( ( gdeWPPropertyList& )( igdeWidget& )pEditWorldProperties ).UpdateProperty();
}

void gdeWPGameDefinition::UpdateWorldPropertyCustomPatternList(){
	( ( gdeWPPropertyList& )( igdeWidget& )pEditWorldProperties ).UpdateCustomPatternList();
}

void gdeWPGameDefinition::UpdateWorldPropertyCustomPattern(){
	( ( gdeWPPropertyList& )( igdeWidget& )pEditWorldProperties ).UpdateCustomPattern();
}



void gdeWPGameDefinition::UpdateAutoFindPathObjectClasses(){
	( ( gdeWPPathList& )( igdeWidget& )pEditAutoFindPathObjectClasses ).UpdateList();
}

void gdeWPGameDefinition::UpdateAutoFindPathSkins(){
	( ( gdeWPPathList& )( igdeWidget& )pEditAutoFindPathSkins ).UpdateList();
}

void gdeWPGameDefinition::UpdateAutoFindPathSkies(){
	( ( gdeWPPathList& )( igdeWidget& )pEditAutoFindPathSkies ).UpdateList();
}



void gdeWPGameDefinition::UpdateDecalProperties(){
	( ( gdeWPPropertyList& )( igdeWidget& )pEditDecalProperties ).UpdateList();
}

void gdeWPGameDefinition::UpdateDecalProperty(){
	( ( gdeWPPropertyList& )( igdeWidget& )pEditDecalProperties ).UpdateProperty();
}

void gdeWPGameDefinition::UpdateDecalPropertyCustomPatternList(){
	( ( gdeWPPropertyList& )( igdeWidget& )pEditDecalProperties ).UpdateCustomPatternList();
}

void gdeWPGameDefinition::UpdateDecalPropertyCustomPattern(){
	( ( gdeWPPropertyList& )( igdeWidget& )pEditDecalProperties ).UpdateCustomPattern();
}
