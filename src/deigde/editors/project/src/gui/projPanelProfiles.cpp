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

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "projPanelProfiles.h"
#include "projPanelProfilesListener.h"
#include "projWindowMain.h"
#include "../project/projProject.h"
#include "../project/profile/projProfile.h"
#include "../undosys/profile/projUProfileSetName.h"
#include "../undosys/profile/projUProfileSetDescription.h"
#include "../undosys/profile/projUProfileSetScriptDirectory.h"
#include "../undosys/profile/projUProfileSetGameObject.h"
#include "../undosys/profile/projUProfileSetPathConfig.h"
#include "../undosys/profile/projUProfileSetPathCapture.h"
#include "../undosys/profile/projUProfileSetIdentifier.h"
#include "../undosys/profile/projUProfileSetAliasIdentifier.h"
#include "../undosys/profile/projUProfileSetTitle.h"
#include "../undosys/profile/projUProfileSetGameDescription.h"
#include "../undosys/profile/projUProfileSetWindowSize.h"
#include "../undosys/profile/projUProfileSetDelgaPath.h"
#include "../undosys/profile/projUProfileSetRunArguments.h"
#include "../undosys/profile/projUProfileSetIcons.h"
#include "../undosys/profile/projUProfileSetExcludePatterns.h"
#include "../undosys/profile/projUProfileSetRequiredExtensions.h"
#include "../undosys/profile/projUProfileSetCreator.h"
#include "../undosys/profile/projUProfileSetWebsite.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/composed/igdeEditPoint.h>
#include <deigde/gui/composed/igdeEditPointListener.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionSelectFile.h>
#include <deigde/gui/event/igdeActionExternOpen.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextAreaListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Events
///////////

namespace{

class cActionBase : public igdeAction{
protected:
	projPanelProfiles &pPanel;
	
public:
	cActionBase( projPanelProfiles &panel, const char *text, igdeIcon *icon, const char *description ) :
	igdeAction( text, icon, description ),
	pPanel( panel ){}
	
	virtual void OnAction(){
		projProject * const project = pPanel.GetProject();
		if( ! project || ! project->GetActiveProfile() ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( OnAction( project, project->GetActiveProfile() ) );
		if( undo ){
			project->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnAction( projProject *project, projProfile *profile ) = 0;
	
	virtual void Update(){
		const projProject * const project = pPanel.GetProject();
		SetEnabled( project && project->GetActiveProfile() );
	}
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	projPanelProfiles &pPanel;
	
public:
	cBaseTextFieldListener( projPanelProfiles &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		projProject * const project = pPanel.GetProject();
		if( ! project ){
			return;
		}
		
		projProfile * const profile = project->GetActiveProfile();
		if( ! profile ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( OnChanged( textField, project, profile ) );
		if( undo ){
			project->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, projProject *project,
		projProfile *profile ) = 0;
};

class cBaseTextAreaListener : public igdeTextAreaListener{
protected:
	projPanelProfiles &pPanel;
	
public:
	cBaseTextAreaListener( projPanelProfiles &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextArea *textArea ){
		projProject * const project = pPanel.GetProject();
		if( ! project ){
			return;
		}
		
		projProfile * const profile = project->GetActiveProfile();
		if( ! profile ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( OnChanged( textArea, project, profile ) );
		if( undo ){
			project->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextArea *textArea, projProject *project,
		projProfile *profile ) = 0;
};

class cBaseEditPointListener : public igdeEditPointListener{
protected:
	projPanelProfiles &pPanel;
	
public:
	cBaseEditPointListener( projPanelProfiles &panel ) : pPanel( panel ){ }
	
	virtual void OnPointChanged( igdeEditPoint *editPoint ){
		projProject * const project = pPanel.GetProject();
		if( ! project ){
			return;
		}
		
		projProfile * const profile = project->GetActiveProfile();
		if( ! profile ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( OnChanged( editPoint, project, profile ) );
		if( undo ){
			project->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeEditPoint *editPoint, projProject *project,
		projProfile *profile ) = 0;
};



class cListProfile : public igdeListBoxListener{
protected:
	projPanelProfiles &pPanel;
	
public:
	cListProfile( projPanelProfiles &panel ) : pPanel( panel ){ }
	
	virtual void OnSelectionChanged( igdeListBox *listBox ){
		projProject * const project = pPanel.GetProject();
		if( ! project ){
			return;
		}
		
		const igdeListItem * const selection = listBox->GetSelectedItem();
		if( selection ){
			project->SetActiveProfile( ( projProfile* )selection->GetData() );
			
		}else{
			project->SetActiveProfile( NULL );
		}
	}
};



class cTextName : public cBaseTextFieldListener{
public:
	cTextName( projPanelProfiles &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, projProject *project,
	projProfile *profile ){
		const decString &name = textField->GetText();
		if( name == profile->GetName() ){
			return NULL;
		}
		
		if( project->GetProfiles().HasNamed( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Rename profile",
				"A profile with this name exists already." );
			textField->SetText( profile->GetName() );
			return NULL;
		}
		
		return new projUProfileSetName( profile, name );
	}
};

class cTextDescription : public cBaseTextAreaListener{
public:
	cTextDescription( projPanelProfiles &panel ) : cBaseTextAreaListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextArea *textArea, projProject*,
	projProfile *profile ){
		if( textArea->GetText() == profile->GetDescription() ){
			return NULL;
		}
		return new projUProfileSetDescription( profile, textArea->GetText() );
	}
};

class cTextScriptDirectory : public cBaseTextFieldListener{
public:
	cTextScriptDirectory( projPanelProfiles &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, projProject*, projProfile *profile ){
		if( textField->GetText() == profile->GetScriptDirectory() ){
			return NULL;
		}
		return new projUProfileSetScriptDirectory( profile, textField->GetText() );
	}
};

class cActionScriptDirectory : public cActionBase{
public:
	cActionScriptDirectory( projPanelProfiles &panel ) : cActionBase( panel,
		"...", NULL, "VFS directory where the game scripts are located" ){}
	
	virtual igdeUndo *OnAction( projProject*, projProfile* ){
		return NULL; // TODO we need a VFS version of FXDirDialog since this is VFS path
	}
};

class cTextGameObject : public cBaseTextFieldListener{
public:
	cTextGameObject( projPanelProfiles &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, projProject*, projProfile *profile ){
		if( textField->GetText() == profile->GetGameObject() ){
			return NULL;
		}
		return new projUProfileSetGameObject( profile, textField->GetText() );
	}
};

class cTextPathConfig : public cBaseTextFieldListener{
public:
	cTextPathConfig( projPanelProfiles &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, projProject*, projProfile *profile ){
		if( textField->GetText() == profile->GetPathConfig() ){
			return NULL;
		}
		return new projUProfileSetPathConfig( profile, textField->GetText() );
	}
};

class cActionPathConfig : public cActionBase{
public:
	cActionPathConfig( projPanelProfiles &panel ) : cActionBase( panel,
		"...", NULL, "VFS directory where the game stores configuration files" ){}
	
	virtual igdeUndo *OnAction( projProject*, projProfile* ){
		return NULL; // TODO we need a VFS version of FXDirDialog since this is VFS path
	}
};

class cTextPathCapture : public cBaseTextFieldListener{
public:
	cTextPathCapture( projPanelProfiles &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, projProject*, projProfile *profile ){
		if( textField->GetText() == profile->GetPathCapture() ){
			return NULL;
		}
		return new projUProfileSetPathCapture( profile, textField->GetText() );
	}
};

class cActionPathCapture : public cActionBase{
public:
	cActionPathCapture( projPanelProfiles &panel ) : cActionBase( panel,
		"...", NULL, "VFS directory where the game stores captured files" ){}
	
	virtual igdeUndo *OnAction( projProject*, projProfile* ){
		return NULL; // TODO we need a VFS version of FXDirDialog since this is VFS path
	}
};

class cActionGenerateIdentifier : public cActionBase{
public:
	cActionGenerateIdentifier( projPanelProfiles &panel ) : cActionBase( panel,
		"Generate Identifier", nullptr, "Generate Identifier" ){}
	
	virtual igdeUndo *OnAction( projProject*, projProfile *profile ){
		if( igdeCommonDialogs::Question( &pPanel, igdeCommonDialogs::ebsYesNo, "Generate Identifier",
		"Generating new identifier can break the game. Do you really want to generate new identifier?" )
		== igdeCommonDialogs::ebYes ){
			return new projUProfileSetIdentifier( profile, decUuid::Random() );
		}
		return nullptr;
	}
};

class cActionMenuIdentifier : public igdeActionContextMenu{
	projPanelProfiles &pPanel;
	
public:
	cActionMenuIdentifier( projPanelProfiles &panel ) : igdeActionContextMenu( "",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallDown ), "Identifier menu" ),
	pPanel( panel ){}
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu ){
		if( ! pPanel.GetActiveProfile() ){
			return;
		}
		
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelper();
		helper.MenuCommand( contextMenu, new cActionGenerateIdentifier( pPanel ), true );
	}
};

class cTextIdentifier : public cBaseTextFieldListener{
public:
	cTextIdentifier( projPanelProfiles &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, projProject*, projProfile *profile ){
		try{
			const decUuid value( textField->GetText(), false );
			return value != profile->GetIdentifier() ? new projUProfileSetIdentifier( profile, value ) : NULL;
			
		}catch( const deException & ){
			igdeCommonDialogs::Error( &pPanel, "Invalid Input",
				"Identifier has to be a valid UUID in the format 8-4-4-4-6 groups of hex-encoded values" );
			textField->SetText( profile->GetIdentifier().ToHexString( false ) );
			return NULL;
		}
	}
};

class cTextAliasIdentifier : public cBaseTextFieldListener{
public:
	cTextAliasIdentifier( projPanelProfiles &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, projProject*, projProfile *profile ){
		if( textField->GetText() == profile->GetAliasIdentifier() ){
			return NULL;
		}
		return new projUProfileSetAliasIdentifier( profile, textField->GetText() );
	}
};

class cTextTitle : public cBaseTextFieldListener{
public:
	cTextTitle( projPanelProfiles &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, projProject*, projProfile *profile ){
		if( textField->GetText() == profile->GetTitle() ){
			return NULL;
		}
		return new projUProfileSetTitle( profile, textField->GetText() );
	}
};

class cTextGameDescription : public cBaseTextAreaListener{
public:
	cTextGameDescription( projPanelProfiles &panel ) : cBaseTextAreaListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextArea *textArea, projProject*, projProfile *profile ){
		if( textArea->GetText() == profile->GetGameDescription() ){
			return NULL;
		}
		return new projUProfileSetGameDescription( profile, textArea->GetText() );
	}
};

class cTextCreator : public cBaseTextFieldListener{
public:
	cTextCreator( projPanelProfiles &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, projProject*, projProfile *profile ){
		if( textField->GetText() == profile->GetCreator() ){
			return NULL;
		}
		return new projUProfileSetCreator( profile, textField->GetText() );
	}
};

class cTextWebsite : public cBaseTextFieldListener{
public:
	cTextWebsite( projPanelProfiles &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, projProject*, projProfile *profile ){
		if( textField->GetText() == profile->GetWebsite() ){
			return NULL;
		}
		return new projUProfileSetWebsite( profile, textField->GetText() );
	}
};

class cEditWindowSize : public cBaseEditPointListener{
public:
	cEditWindowSize( projPanelProfiles &panel ) : cBaseEditPointListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditPoint *editPoint, projProject*, projProfile *profile ){
		if( editPoint->GetPoint() == profile->GetWindowSize() ){
			return NULL;
		}
		return new projUProfileSetWindowSize( profile, editPoint->GetPoint() );
	}
};



class cActionAddIcon : public cActionBase{
	igdeEditPath &pEditPath;
	
public:
	cActionAddIcon( projPanelProfiles &panel, igdeEditPath &editPath ) :
	cActionBase( panel, "", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallPlus ), "Add icon" ),
	pEditPath( editPath ){}
	
	virtual igdeUndo *OnAction( projProject*, projProfile *profile ){
		if( pEditPath.GetPath().IsEmpty() || profile->GetIcons().Has( pEditPath.GetPath() ) ){
			return NULL;
		}
		
		decStringSet icons( profile->GetIcons() );
		icons.Add( pEditPath.GetPath() );
		return new projUProfileSetIcons( profile, icons );
	}
};

class cActionRemoveIcon : public cActionBase{
	igdeListBox &pListIcons;
	
public:
	cActionRemoveIcon( projPanelProfiles &panel, igdeListBox &listPatterns ) :
	cActionBase( panel, "", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallMinus ),
		"Remove selected icon" ),
	pListIcons( listPatterns ){}
	
	virtual igdeUndo *OnAction( projProject*, projProfile *profile ){
		const igdeListItem * const selection = pListIcons.GetSelectedItem();
		if( ! selection || ! profile->GetIcons().Has( selection->GetText() ) ){
			return NULL;
		}
		
		decStringSet icons( profile->GetIcons() );
		icons.Remove( selection->GetText() );
		return new projUProfileSetIcons( profile, icons );
	}
	
	virtual void Update(){
		const projProject * const project = pPanel.GetProject();
		SetEnabled( project && project->GetActiveProfile() && pListIcons.GetSelectedItem() );
	}
};



class cActionAddExcludePattern : public cActionBase{
	igdeTextField &pTextPattern;
	
public:
	cActionAddExcludePattern( projPanelProfiles &panel, igdeTextField &textPattern ) :
	cActionBase( panel, "", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallPlus ),
		"Add file pattern to set" ),
	pTextPattern( textPattern ){}
	
	virtual igdeUndo *OnAction( projProject*, projProfile *profile ){
		if( profile->GetExcludePatterns().Has( pTextPattern.GetText() ) ){
			return NULL;
		}
		
		decStringSet patterns( profile->GetExcludePatterns() );
		patterns.Add( pTextPattern.GetText() );
		return new projUProfileSetExcludePatterns( profile, patterns );
	}
};

class cActionRemoveExcludePattern : public cActionBase{
	igdeListBox &pListPatterns;
	
public:
	cActionRemoveExcludePattern( projPanelProfiles &panel, igdeListBox &listPatterns ) :
	cActionBase( panel, "", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallMinus ),
		"Remove selected file pattern from set" ),
	pListPatterns( listPatterns ){}
	
	virtual igdeUndo *OnAction( projProject*, projProfile *profile ){
		const igdeListItem * const selection = pListPatterns.GetSelectedItem();
		if( ! selection || ! profile->GetExcludePatterns().Has( selection->GetText() ) ){
			return NULL;
		}
		
		decStringSet patterns( profile->GetExcludePatterns() );
		patterns.Remove( selection->GetText() );
		return new projUProfileSetExcludePatterns( profile, patterns );
	}
	
	virtual void Update(){
		const projProject * const project = pPanel.GetProject();
		SetEnabled( project && project->GetActiveProfile() && pListPatterns.GetSelectedItem() );
	}
};



class cActionAddRequiredExtension : public cActionBase{
	igdeTextField &pTextExtension;
	
public:
	cActionAddRequiredExtension( projPanelProfiles &panel, igdeTextField &textExtension ) :
	cActionBase( panel, "", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallPlus ),
		"Add resource file extension to set" ),
	pTextExtension( textExtension ){}
	
	virtual igdeUndo *OnAction( projProject*, projProfile *profile ){
		if( profile->GetRequiredExtensions().Has( pTextExtension.GetText() ) ){
			return NULL;
		}
		
		decStringSet patterns( profile->GetRequiredExtensions() );
		patterns.Add( pTextExtension.GetText() );
		return new projUProfileSetRequiredExtensions( profile, patterns );
	}
};

class cActionRemoveRequiredExtension : public cActionBase{
	igdeListBox &pListPatterns;
	
public:
	cActionRemoveRequiredExtension( projPanelProfiles &panel, igdeListBox &listPatterns ) :
	cActionBase( panel, "", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallMinus ),
		"Remove selected resource file extension from set" ),
	pListPatterns( listPatterns ){}
	
	virtual igdeUndo *OnAction( projProject*, projProfile *profile ){
		const igdeListItem * const selection = pListPatterns.GetSelectedItem();
		if( ! selection || ! profile->GetRequiredExtensions().Has( selection->GetText() ) ){
			return NULL;
		}
		
		decStringSet patterns( profile->GetRequiredExtensions() );
		patterns.Remove( selection->GetText() );
		return new projUProfileSetRequiredExtensions( profile, patterns );
	}
	
	virtual void Update(){
		const projProject * const project = pPanel.GetProject();
		SetEnabled( project && project->GetActiveProfile() && pListPatterns.GetSelectedItem() );
	}
};



class cTextDelgaPath : public cBaseTextFieldListener{
public:
	cTextDelgaPath( projPanelProfiles &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, projProject*, projProfile *profile ){
		if( textField->GetText() == profile->GetDelgaPath() ){
			return NULL;
		}
		return new projUProfileSetDelgaPath( profile, textField->GetText() );
	}
};

class cActionDelgaPath : public cActionBase{
public:
	cActionDelgaPath( projPanelProfiles &panel ) : cActionBase( panel,
		"...", NULL, "VFS directory where to place the build DELGA file" ){}
	
	virtual igdeUndo *OnAction( projProject*, projProfile* ){
		return NULL; // TODO we need a VFS version of FXDirDialog since this is VFS path
	}
};



class cTextRunArguments : public cBaseTextFieldListener{
public:
	cTextRunArguments( projPanelProfiles &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, projProject*, projProfile *profile ){
		if( textField->GetText() == profile->GetRunArguments() ){
			return nullptr;
		}
		return new projUProfileSetRunArguments( profile, textField->GetText() );
	}
};

}



// Class projPanelProfiles
////////////////////////////

// Constructor, destructor
////////////////////////////

projPanelProfiles::projPanelProfiles( projWindowMain &windowMain ) :
igdeContainerSplitted(windowMain.GetEnvironment(), igdeContainerSplitted::espLeft,
	igdeApplication::app().DisplayScaled(250)),

pWindowMain( windowMain ),

pProject( NULL ),
pListener( NULL )
{
	igdeEnvironment &env = windowMain.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	
	pListener = new projPanelProfilesListener( *this );
	
	
	
	// side panel
	igdeContainerScroll::Ref scroll;
	igdeContainer::Ref sidePanel;
	helper.SidePanel( scroll, sidePanel, false, 5 );
	sidePanel->SetWidgetGuiThemeName( "" );
	AddChild( scroll, eaSide );
	
	helper.ListBox( sidePanel, 10, "Profiles", pListProfiles, new cListProfile( *this ) );
	helper.Button( sidePanel, windowMain.GetActionProfileAdd() );
	helper.Button( sidePanel, windowMain.GetActionProfileRemove() );
	helper.Button( sidePanel, windowMain.GetActionProfileDuplicate() );
	
	igdeContainer::Ref groupBox, formLine;
	helper.GroupBoxStaticFlow( sidePanel, groupBox, "Content:" );
	helper.Button( groupBox, windowMain.GetActionShowContent() );
	
	helper.GroupBoxStaticFlow( sidePanel, groupBox, "Test-Run:" );
	helper.Button( groupBox, windowMain.GetActionProfileTestRun() );
	
	helper.GroupBoxStaticFlow( sidePanel, groupBox, "Distribution:" );
	helper.Button( groupBox, windowMain.GetActionProfileDistribute() );
	helper.Button( groupBox, windowMain.GetActionShowDistribute() );
	
	
	
	// profile panel
	helper.SidePanel( scroll, sidePanel, false, 5 );
	sidePanel->SetWidgetGuiThemeName( "" );
	AddChild( scroll, eaCenter );
	
	igdeContainer::Ref frameLine;
	const char *description;
	
	groupBox.TakeOver( new igdeContainerForm( env ) );
	sidePanel->AddChild( groupBox );
	helper.EditString( groupBox, "Name:", "Profile name.", pEditName, new cTextName( *this ) );
	helper.EditString( groupBox, "Description:", "Profile description.",
		pEditDescription, 5, new cTextDescription( *this ) );
	
	
	// game parameters
	helper.GroupBox( sidePanel, groupBox, "Game Parameters:" );
	
	description = "VFS directory where the game scripts are located.";
	helper.FormLineStretchFirst( groupBox, "Script Directory:", description, frameLine );
	helper.EditString( frameLine, description, pEditScriptDirectory, new cTextScriptDirectory( *this ) );
	pActionScriptDirectory.TakeOver( new cActionScriptDirectory( *this ) );
	helper.Button( frameLine, pActionScriptDirectory );
	
	helper.EditString( groupBox, "Game Object:",
		"Game object the scripting module uses as entry point to run the game.",
		pEditGameObject, new cTextGameObject( *this ) );
	
	description = "VFS directory where the game stores configuration files.";
	helper.FormLineStretchFirst( groupBox, "Config Path:", description, frameLine );
	helper.EditString( frameLine, description, pEditPathConfig,
		new cTextPathConfig( *this ) );
	pActionPathConfig.TakeOver( new cActionPathConfig( *this ) );
	helper.Button( frameLine, pActionPathConfig );
	
	description = "VFS directory where the game stores captured files.";
	helper.FormLineStretchFirst( groupBox, "Capture Path:", description, frameLine );
	helper.EditString( frameLine, description, pEditPathCapture,
		new cTextPathCapture( *this ) );
	pActionPathCapture.TakeOver( new cActionPathCapture( *this ) );
	helper.Button( frameLine, pActionPathCapture );
	
	pActionMenuIdentifier.TakeOver( new cActionMenuIdentifier( *this ) );
	helper.FormLineStretchFirst( groupBox, "Identifier:",
		"Unique identifier of game used by Launchers. CHANGING THIS CAN BREAK YOUR GAME!", formLine );
	helper.EditString( formLine, "Unique identifier of game used by Launchers. CHANGING THIS CAN BREAK YOUR GAME!",
		pEditIdentifier, new cTextIdentifier( *this ) );
	helper.Button( formLine, pBtnMenuIdentifier, pActionMenuIdentifier );
	pActionMenuIdentifier->SetWidget( pBtnMenuIdentifier );
	
	helper.EditString( groupBox, "Alias Identifier:", "Alias identifier of game used by Launchers to simplify running the game.",
		pEditAliasIdentifier, new cTextAliasIdentifier( *this ) );
	
	helper.EditString( groupBox, "Title:", "Title to display for the game by Launchers.",
		pEditTitle, new cTextTitle( *this ) );
	
	helper.EditString( groupBox, "Description:", "Description to display for the game by Launchers.",
		pEditGameDescription, 10, new cTextGameDescription( *this ) );
	
	helper.EditString( groupBox, "Creator:", "Creator (or author) of the game.",
		pEditCreator, new cTextCreator( *this ) );
	
	helper.EditString( groupBox, "Website:", "Website (or any other kind of internet presence) of project or creator (author).",
		pEditWebsite, new cTextWebsite( *this ) );
	
	helper.EditPoint( groupBox, "Window Size:", "Size of run-time window or 0 to run fullscreen.",
		pEditWindowSize, new cEditWindowSize( *this ) );
	
	
	// processing parameters
	groupBox.TakeOver( new igdeGroupBox( env, "Processing Parameters:", false ) );
	sidePanel->AddChild( groupBox );
	
	igdeContainer::Ref subGroup, subGroup2;
	subGroup.TakeOver( new igdeContainerBox( env, igdeContainerBox::eaX ) );
	groupBox->AddChild( subGroup );
	
	// icons
	helper.GroupBoxStaticFlow( subGroup, subGroup2, "Icons:" );
	
	description = "Set of icons of different size representing the project.";
	helper.FormLineStretchFirst( subGroup2, "Path:", description, frameLine );
	helper.EditPath( frameLine, description, igdeEnvironment::efpltImage, pEditIconPath, NULL );
	pActionAddIcon.TakeOver( new cActionAddIcon( *this, pEditIconPath ) );
	helper.Button( frameLine, pActionAddIcon );
	
	helper.ListBox( subGroup2, 4, description, pListIcons, NULL );
	pListIcons->SetDefaultSorter();
	
	pActionRemoveIcon.TakeOver( new cActionRemoveIcon( *this, pListIcons ) );
	helper.Button( frameLine, pActionRemoveIcon );
	
	// exclude patterns
	helper.GroupBoxStaticFlow( subGroup, subGroup2, "Exclude Patterns:" );
	
	description = "Set of file patterns to exclude from projribution process.";
	helper.FormLineStretchFirst( subGroup2, "Pattern:", description, frameLine );
	helper.EditString( frameLine, description, pEditExcludePattern, NULL );
	pActionAddExcludePattern.TakeOver( new cActionAddExcludePattern( *this, pEditExcludePattern ) );
	helper.Button( frameLine, pActionAddExcludePattern );
	
	helper.ListBox( subGroup2, 4, description, pListExcludePatterns, NULL );
	pListExcludePatterns->SetDefaultSorter();
	
	pActionRemoveExcludePattern.TakeOver( new cActionRemoveExcludePattern( *this, pListExcludePatterns ) );
	helper.Button( frameLine, pActionRemoveExcludePattern );
	
	// required extensions
	helper.GroupBoxStaticFlow( subGroup, subGroup2, "Required Extensions:" );
	
	description = "Set of resource file extensions (.extension) required by the project";
	helper.FormLineStretchFirst( subGroup2, "Extension:", description, frameLine );
	helper.EditString( frameLine, description, pEditRequiredExtension, NULL );
	pActionAddRequiredExtension.TakeOver( new cActionAddRequiredExtension( *this, pEditRequiredExtension ) );
	helper.Button( frameLine, pActionAddRequiredExtension );
	
	helper.ListBox( subGroup2, 4, description, pListRequiredExtensions, NULL );
	pListRequiredExtensions->SetDefaultSorter();
	
	pActionRemoveRequiredExtension.TakeOver( new cActionRemoveRequiredExtension( *this, pListRequiredExtensions ) );
	helper.Button( frameLine, pActionRemoveRequiredExtension );
	
	
	// row
	subGroup.TakeOver( new igdeContainerBox( env, igdeContainerBox::eaX ) );
	sidePanel->AddChild( subGroup );
	
	// delga parameters
	helper.GroupBox( subGroup, groupBox, "DELGA Parameters:" );
	
	description = "VFS directory where to place the build DELGA file.";
	helper.FormLineStretchFirst( groupBox, "DELGA File:", description, frameLine );
	helper.EditString( frameLine, description, pEditDelgaPath, new cTextDelgaPath( *this ) );
	pActionDelgaPath.TakeOver( new cActionDelgaPath( *this ) );
	helper.Button( frameLine, pActionDelgaPath );
	
	
	// test running
	helper.GroupBox( subGroup, groupBox, "Test Run:" );
	
	helper.EditString( groupBox, "Arguments:", "Arguments to use while test running",
		pEditRunArguments, new cTextRunArguments( *this ) );
}

projPanelProfiles::~projPanelProfiles(){
	SetProject( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void projPanelProfiles::SetProject( projProject *project ){
	if( project == pProject ){
		return;
	}
	
	pEditIconPath->ClearPath();
	pEditExcludePattern->ClearText();
	pEditRequiredExtension->ClearText();
	
	if( pProject ){
		pProject->RemoveListener( pListener );
		pProject->FreeReference();
	}
	
	pProject = project;
	
	if( project ){
		project->AddReference();
		project->AddListener( pListener );
	}
	
	UpdateProject();
	UpdateProfiles();
}

void projPanelProfiles::UpdateProject(){
	pListProfiles->SetEnabled( pProject != NULL );
}

projProfile *projPanelProfiles::GetActiveProfile() const{
	if( ! pProject ){
		return NULL;
	}
	return pProject->GetActiveProfile();
}

void projPanelProfiles::SelectActiveProfile(){
	pListProfiles->SetSelection( pListProfiles->IndexOfItemWithData( GetActiveProfile() ) );
	if( pListProfiles->GetSelection() != -1 ){
		pListProfiles->MakeItemVisible( pListProfiles->GetSelection() );
	}
	UpdateProfile();
}

void projPanelProfiles::UpdateProfiles(){
	pListProfiles->RemoveAllItems();
	
	if( pProject ){
		const projProfileList &list = pProject->GetProfiles();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			projProfile * const profile = list.GetAt( i );
			pListProfiles->AddItem( profile->GetName(), NULL, profile );
		}
		
		pListProfiles->SortItems();
	}
	
	SelectActiveProfile();
}

void projPanelProfiles::UpdateProfile(){
	const projProfile * const profile = GetActiveProfile();
	
	pActionScriptDirectory->Update();
	pActionPathCapture->Update();
	pActionPathConfig->Update();
	pActionDelgaPath->Update();
	
	const bool enable = profile != NULL;
	pEditName->SetEnabled( enable );
	pEditDescription->SetEnabled( enable );
	pEditScriptDirectory->SetEnabled( enable );
	pEditGameObject->SetEnabled( enable );
	pEditPathConfig->SetEnabled( enable );
	pEditPathCapture->SetEnabled( enable );
	pEditIdentifier->SetEnabled( enable );
	pEditAliasIdentifier->SetEnabled( enable );
	pEditTitle->SetEnabled( enable );
	pEditGameDescription->SetEnabled( enable );
	pEditWindowSize->SetEnabled( enable );
	pEditDelgaPath->SetEnabled( enable );
	pEditRunArguments->SetEnabled( enable );
	
	if( profile ){
		pEditName->SetText( profile->GetName() );
		pEditDescription->SetText( profile->GetDescription() );
		pEditScriptDirectory->SetText( profile->GetScriptDirectory() );
		pEditGameObject->SetText( profile->GetGameObject() );
		pEditPathConfig->SetText( profile->GetPathConfig() );
		pEditPathCapture->SetText( profile->GetPathCapture() );
		pEditIdentifier->SetText( profile->GetIdentifier().ToHexString( false ) );
		pEditAliasIdentifier->SetText( profile->GetAliasIdentifier() );
		pEditTitle->SetText( profile->GetTitle() );
		pEditGameDescription->SetText( profile->GetGameDescription() );
		pEditCreator->SetText( profile->GetCreator() );
		pEditWebsite->SetText( profile->GetWebsite() );
		pEditWindowSize->SetPoint( profile->GetWindowSize() );
		pEditDelgaPath->SetText( profile->GetDelgaPath() );
		pEditRunArguments->SetText( profile->GetRunArguments() );
		
	}else{
		pEditName->ClearText();
		pEditDescription->ClearText();
		pEditScriptDirectory->ClearText();
		pEditGameObject->ClearText();
		pEditPathConfig->ClearText();
		pEditPathCapture->ClearText();
		pEditIdentifier->ClearText();
		pEditAliasIdentifier->ClearText();
		pEditTitle->ClearText();
		pEditGameDescription->ClearText();
		pEditCreator->ClearText();
		pEditWebsite->ClearText();
		pEditWindowSize->SetPoint( decPoint() );
		pEditDelgaPath->ClearText();
		pEditRunArguments->ClearText();
	}
	
	UpdateIcons();
	UpdateExcludePatterns();
	UpdateRequiredExtensions();
}

void projPanelProfiles::UpdateIcons(){
	decString selection;
	if( pListIcons->GetSelectedItem() ){
		selection = pListIcons->GetSelectedItem()->GetText();
	}
	
	pListIcons->RemoveAllItems();
	
	const projProfile * const profile = GetActiveProfile();
	if( profile ){
		const decStringSet &icons = profile->GetIcons();
		const int count = icons.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			pListIcons->AddItem( icons.GetAt( i ) );
		}
		
		pListIcons->SortItems();
	}
	
	if( ! selection.IsEmpty() ){
		pListIcons->SetSelection( pListIcons->IndexOfItem( selection ) );
	}
	
	if( pListIcons->GetSelection() == -1 && pListIcons->GetItemCount() > 0 ){
		pListIcons->SetSelection( 0 );
	}
	
	pEditIconPath->SetEnabled( profile != NULL );
	pListIcons->SetEnabled( profile != NULL );
	
	pActionAddIcon->Update();
	pActionRemoveIcon->Update();
}

void projPanelProfiles::UpdateExcludePatterns(){
	decString selection;
	if( pListExcludePatterns->GetSelectedItem() ){
		selection = pListExcludePatterns->GetSelectedItem()->GetText();
	}
	
	pListExcludePatterns->RemoveAllItems();
	
	const projProfile * const profile = GetActiveProfile();
	if( profile ){
		const decStringSet &patterns = profile->GetExcludePatterns();
		const int count = patterns.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			pListExcludePatterns->AddItem( patterns.GetAt( i ) );
		}
		
		pListExcludePatterns->SortItems();
	}
	
	if( ! selection.IsEmpty() ){
		pListExcludePatterns->SetSelection( pListExcludePatterns->IndexOfItem( selection ) );
	}
	
	if( pListExcludePatterns->GetSelection() == -1 && pListExcludePatterns->GetItemCount() > 0 ){
		pListExcludePatterns->SetSelection( 0 );
	}
	
	pEditExcludePattern->SetEnabled( profile != NULL );
	pListExcludePatterns->SetEnabled( profile != NULL );
	
	pActionAddExcludePattern->Update();
	pActionRemoveExcludePattern->Update();
}

void projPanelProfiles::UpdateRequiredExtensions(){
	decString selection;
	if( pListRequiredExtensions->GetSelectedItem() ){
		selection = pListRequiredExtensions->GetSelectedItem()->GetText();
	}
	
	pListRequiredExtensions->RemoveAllItems();
	
	const projProfile * const profile = GetActiveProfile();
	if( profile ){
		const decStringSet &extensions = profile->GetRequiredExtensions();
		const int count = extensions.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			pListRequiredExtensions->AddItem( extensions.GetAt( i ) );
		}
		
		pListRequiredExtensions->SortItems();
	}
	
	if( ! selection.IsEmpty() ){
		pListRequiredExtensions->SetSelection( pListRequiredExtensions->IndexOfItem( selection ) );
	}
	
	if( pListRequiredExtensions->GetSelection() == -1 && pListRequiredExtensions->GetItemCount() > 0 ){
		pListRequiredExtensions->SetSelection( 0 );
	}
	
	pEditRequiredExtension->SetEnabled( profile != NULL );
	pListRequiredExtensions->SetEnabled( profile != NULL );
	
	pActionAddRequiredExtension->Update();
	pActionRemoveRequiredExtension->Update();
}
