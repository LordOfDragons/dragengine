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

#include "meWindowMain.h"
#include "meWindowMainListener.h"
#include "meView3D.h"
#include "meTaskSyncGameDefinition.h"
#include "properties/meWindowProperties.h"
// #include "effects/meWindowEffects.h"
#include "changelog/meWindowChangelog.h"
#include "vegetation/meWindowVegetation.h"
#include "../actions/meASubclassAsEClass.h"
#include "../clipboard/meClipboardDataObject.h"
#include "../configuration/meConfiguration.h"
#include "../world/meWorld.h"
#include "../world/meCamera.h"
#include "../world/meWorldGuiParameters.h"
#include "../world/decal/meDecal.h"
#include "../world/decal/meDecalSelection.h"
#include "../world/object/meObject.h"
#include "../world/object/meObjectList.h"
#include "../world/object/meObjectSelection.h"
#include "../world/terrain/meHeightTerrain.h"
#include "../loadsave/meLoadSaveSystem.h"
#include "../loadsave/meSaveSupport.h"
#include "../undosys/gui/decal/meUAddDecal.h"
#include "../undosys/gui/decal/meURaiseDecalTop.h"
#include "../undosys/gui/decal/meURaiseDecalOne.h"
#include "../undosys/gui/decal/meULowerDecalOne.h"
#include "../undosys/gui/decal/meULowerDecalBottom.h"
#include "../undosys/gui/decal/meUDeleteDecal.h"
#include "../undosys/gui/decal/meUDecalDuplicate.h"
#include "../undosys/gui/object/meUAddObject.h"
#include "../undosys/gui/object/meUDeleteObject.h"
#include "../undosys/gui/object/meUObjDuplicate.h"
#include "../undosys/gui/object/meUObjectDropToGround.h"
#include "../undosys/gui/object/meUObjectSnapToGrid.h"
#include "../undosys/gui/object/meUObjectAttachTo.h"
#include "../undosys/gui/object/meUObjectCopyPosition.h"
#include "../undosys/gui/object/meUObjectCopyRotation.h"
#include "../undosys/gui/object/meUObjectCopyScale.h"
#include "../undosys/gui/objectshape/meUObjectShapeAdd.h"
#include "../undosys/gui/objectshape/meUObjectShapeDelete.h"
#include "../undosys/gui/navspace/meUDeleteNavSpace.h"
#include "../undosys/clipboard/meUPasteObject.h"
#include "../undosys/properties/object/meUSetObjectRotation.h"
#include "../undosys/properties/object/property/meUObjectAddProperty.h"
#include "../undosys/properties/object/property/meUObjectSetProperty.h"
#include "../utils/meHelpers.h"
#include "../meIGDEModule.h"
#include "../worldedit.h"

#include <deigde/clipboard/igdeClipboardDataReference.h>
#include <deigde/engine/igdeEngineController.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeTabBook.h>
#include <deigde/gui/igdeToolBar.h>
#include <deigde/gui/igdeToolBarDock.h>
#include <deigde/gui/igdeToolBarSeparator.h>
#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/dialog/igdeDialogReference.h>
#include <deigde/gui/layout/igdeContainerSplitted.h>
#include <deigde/gui/layout/igdeContainerSplittedReference.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>
#include <deigde/gui/menu/igdeMenuCommand.h>
#include <deigde/gui/menu/igdeMenuSeparator.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionRedo.h>
#include <deigde/gui/event/igdeActionUndo.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/resources/igdeIcon.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gamedefinition/class/igdeGDClassManager.h>
#include <deigde/gamedefinition/class/light/igdeGDCLight.h>
#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/collection/decIntList.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/systems/deAudioSystem.h>



// Class meWindowMain
///////////////////////

// Constructor, destructor
////////////////////////////

meWindowMain::meWindowMain( meIGDEModule &module ) :
igdeEditorWindow( module ),
pListener( NULL ),
pActiveModule( false ),
pConfiguration( NULL ),
pLoadSaveSystem( NULL ),
pSaveSupport( NULL ),
pWindowProperties( NULL ),
pView3D( NULL ),
pViewVegetation( NULL ),
pViewChangelog( NULL ),
pWorld( NULL ),
pUse3DCursor( false ),
pLoadWorld( NULL ),
pLoadTask( NULL )
{
	igdeEnvironment &env = GetEnvironment();
	
	pLoadIcons();
	pCreateActions();
	pCreateMenu();
	
	pListener = new meWindowMainListener( *this );
	pConfiguration = new meConfiguration( *this );
	pLoadSaveSystem = new meLoadSaveSystem( this );
	pSaveSupport = new meSaveSupport( this );
	
	pConfiguration->LoadConfiguration();
	
	// content
	pCreateToolBarFile();
	pCreateToolBarEdit();
	pCreateToolBarObject();
	pCreateToolBarDecal();
	
	igdeContainerSplittedReference splitted;
	splitted.TakeOver( new igdeContainerSplitted( env, igdeContainerSplitted::espLeft, 400 ) );
	AddChild( splitted );
	
	pWindowProperties = new meWindowProperties( *this );
	splitted->AddChild( pWindowProperties, igdeContainerSplitted::eaSide );
	
	pTabContent.TakeOver( new igdeTabBook( env ) );
	splitted->AddChild( pTabContent, igdeContainerSplitted::eaCenter );
	
	pView3D = new meView3D( *this );
	pTabContent->AddChild( pView3D, "World" );
	
	pViewVegetation = new meWindowVegetation( *this );
	pTabContent->AddChild( pViewVegetation, "Vegetation" );
	
	pViewChangelog = new meWindowChangelog( *this );
	pTabContent->AddChild( pViewChangelog, "Change-Log" );
	
	pTabContent->SetActivePanel( 0 ); // world
	
	CreateNewWorld();
	ResetViews();
}

meWindowMain::~meWindowMain(){
	if( pLoadTask ){
		delete pLoadTask;
		pLoadTask = NULL;
	}
	if( pLoadWorld ){
		pLoadWorld->FreeReference();
		pLoadWorld = NULL;
	}
	
	if( pConfiguration ){
		pConfiguration->SaveConfiguration();
	}
	
	SetWorld( NULL );
	
	if( pView3D ){
		pView3D->FreeReference();
	}
	if( pViewVegetation ){
		pViewVegetation->FreeReference();
	}
	if( pViewChangelog ){
		pViewChangelog->FreeReference();
	}
	if( pWindowProperties ){
		pWindowProperties->FreeReference();
	}
	
	pClipboard.ClearAll();
	
	if( pConfiguration ){
		delete pConfiguration;
	}
	if( pSaveSupport ){
		delete pSaveSupport;
	}
	if( pLoadSaveSystem ){
		delete pLoadSaveSystem;
	}
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

bool meWindowMain::QuitRequest(){
	return true;
}

void meWindowMain::ResetViews(){
	pView3D->ResetView();
}



void meWindowMain::SetWorld( meWorld *world ){
	if( world == pWorld ){
		return;
	}
	
	pWindowProperties->SetWorld( NULL );
	pView3D->SetWorld( NULL );
	pViewVegetation->SetWorld( NULL );
	pViewChangelog->SetWorld( NULL );
	
	pActionEditUndo->SetUndoSystem( NULL );
	pActionEditRedo->SetUndoSystem( NULL );
		
	if( pWorld ){
		pWorld->RemoveNotifier( pListener );
		pWorld->Dispose();
		pWorld->FreeReference();
	}
	
	pWorld = world;
	
	if( world ){
		world->AddReference();
		world->AddNotifier( pListener );
		
		pActionEditUndo->SetUndoSystem( world->GetUndoSystem() );
		pActionEditRedo->SetUndoSystem( world->GetUndoSystem() );
		
		pWindowProperties->SetWorld( world );
		pView3D->SetWorld( world );
		pViewVegetation->SetWorld( world );
		pViewChangelog->SetWorld( world );
	}
	
	if( GetActiveModule() && world ){
		world->ActivateMicrophone();
	}
	
	UpdateAllActions();
}

void meWindowMain::CreateNewWorld(){
	deObjectReference refWorld;
	
	refWorld.TakeOver( new meWorld( *this, &GetEnvironment() ) );
	meWorld * const world = ( meWorld* )( deObject* )refWorld;
	world->SetSaved( false );
	world->SetChanged( false );
	world->SetDepChanged( false );
	
	SetWorld( world );
	
	pUse3DCursor = true;
	p3DCursor.SetZero();
	
	if( GetEngineController().GetRunning() && pWorld ){
		pWorld->InitDelegates();
	}
}

void meWindowMain::LoadWorld( const char *filename ){
	pLoadFilename = filename;
	
	SetProgressVisible( true );
	SetProgress( 0.0f );
	
	decString message;
	message.Format( "Loading world from '%s'", filename );
	SetProgressText( message );
	
	GetRecentFiles().AddFile( filename );
}

void meWindowMain::ConfigEnableGIChanged(){
	if( pWorld ){
		pWorld->EnableGIChanged();
	}
}




void meWindowMain::OnBeforeEngineStart(){
}

void meWindowMain::OnAfterEngineStart(){
	pView3D->OnAfterEngineStart();
	
	if( pWorld ){
		pWorld->InitDelegates();
	}
}

void meWindowMain::OnBeforeEngineStop(){
	pView3D->OnBeforeEngineStop();
}

void meWindowMain::OnAfterEngineStop(){
}

void meWindowMain::OnActivate(){
	igdeEditorWindow::OnActivate();
	
	pView3D->SetEnableRendering( true );
	
	if( pWorld ){
		pWorld->ActivateMicrophone();
	}
}

void meWindowMain::OnDeactivate(){
	pView3D->SetEnableRendering( false );
	
	if( pWorld ){
		GetEngine()->GetAudioSystem()->SetActiveMicrophone( NULL );
	}
	
	igdeEditorWindow::OnDeactivate();
}

void meWindowMain::OnFrameUpdate( float elapsed ){
	if( ! GetActiveModule() ){
		return;
	}
	
	pView3D->OnFrameUpdate( elapsed );
	pUpdateLoading();
}

void meWindowMain::GetChangedDocuments( decStringList &list ){
	if( pWorld && pWorld->GetAnyChanged() ){
		list.Add( pWorld->GetFilePath() );
	}
}

void meWindowMain::LoadDocument( const char *filename ){
	if( pWorld && pWorld->GetAnyChanged() ){
		if( igdeCommonDialogs::Question( this, igdeCommonDialogs::ebsYesNo, "Open World",
		"Open world discards changes. Is this ok?" ) == igdeCommonDialogs::ebNo ){
			return;
		}
	}
	LoadWorld( filename );
}

bool meWindowMain::SaveDocument( const char *filename ){
	if( pWorld && pWorld->GetFilePath() == filename ){
		pActionFileSave->OnAction();
		return true; // TODO better implement this so failure can be detected
	}
	
	return false;
}

void meWindowMain::RecentFilesChanged(){
	pConfiguration->SaveConfiguration();
}

void meWindowMain::OnGameProjectChanged(){
	pConfiguration->LoadConfiguration();
	pWindowProperties->OnGameProjectChanged();
	CreateNewWorld();
}

igdeStepableTask *meWindowMain::OnGameDefinitionChanged(){
	return new meTaskSyncGameDefinition( *this );
}



void meWindowMain::SetUse3DCursor(bool useIt){
	pUse3DCursor = useIt;
}



void meWindowMain::RotateActiveObjectBy( const decVector &rotation ){
	meObject * const object = pWorld ? pWorld->GetSelectionObject().GetActive() : NULL;
	if( ! object ){
		return;
	}
	
	igdeUndoReference undo;
	undo.TakeOver( new meUSetObjectRotation( object, object->GetRotation() + rotation ) );
	pWorld->GetUndoSystem()->Add( undo );
}



void meWindowMain::SetProgressVisible( bool visible ){
	GetEnvironment().SetProgressVisible( visible );
}

void meWindowMain::SetProgress( float progress ){
	GetEnvironment().SetProgress( progress );
}

void meWindowMain::SetProgressText( const char *text ){
	GetEnvironment().SetProgressText( text );
}



// Actions
////////////

namespace{

class cActionBase : public igdeAction{
protected:
	meWindowMain &pWindow;
	
public:
	cActionBase( meWindowMain &window, const char *text, igdeIcon *icon, const char *description,
		int modifiers = deInputEvent::esmNone, deInputEvent::eKeyCodes keyCode = deInputEvent::ekcUndefined,
		deInputEvent::eKeyCodes mnemonic = deInputEvent::ekcUndefined ) :
	igdeAction( text, icon, description, mnemonic, igdeHotKey( modifiers, keyCode ) ),
	pWindow( window ){}
	
	cActionBase( meWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic ) :
	igdeAction( text, icon, description, mnemonic ),
	pWindow( window ){}
	
	virtual void OnAction(){
		if( ! pWindow.GetWorld() ){
			return;
		}
		igdeUndoReference undo;
		undo.TakeOver( OnAction( pWindow.GetWorld() ) );
		if( undo ){
			pWindow.GetWorld()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnAction( meWorld *world ) = 0;
	
	virtual void Update(){
		if( pWindow.GetWorld() ){
			Update( *pWindow.GetWorld() );
			
		}else{
			SetEnabled( false );
			SetSelected( false );
		}
	}
	
	virtual void Update( const meWorld & ){
		SetEnabled( true );
		SetSelected( false );
	}
};



class cActionFileNew : public igdeAction{
	meWindowMain &pWindow;
	
public:
	cActionFileNew( meWindowMain &window ) :
	igdeAction( "New", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiNew ),
		"Create new world", deInputEvent::ekcN, igdeHotKey( deInputEvent::esmControl, deInputEvent::ekcN ) ),
	pWindow( window ){}
	
	virtual void OnAction(){
		if( ! pWindow.GetWorld() || ! pWindow.GetWorld()->GetAnyChanged()
		|| igdeCommonDialogs::Question( &pWindow, igdeCommonDialogs::ebsYesNo, "New World",
		"Creating a new World discarding the current one is that ok?" ) == igdeCommonDialogs::ebYes ){
			pWindow.CreateNewWorld();
		}
	}
};

class cActionFileOpen : public igdeAction{
	meWindowMain &pWindow;
	
public:
	cActionFileOpen( meWindowMain &window ) : igdeAction( "Open...",
		window.GetEnvironment().GetStockIcon( igdeEnvironment::esiOpen ), "Open world from file",
		deInputEvent::ekcO, igdeHotKey( deInputEvent::esmControl, deInputEvent::ekcO ) ), pWindow( window ){}
	
	virtual void OnAction(){
		if( pWindow.GetWorld() && pWindow.GetWorld()->GetAnyChanged() ){
			if( igdeCommonDialogs::Question( &pWindow, igdeCommonDialogs::ebsYesNo, "Open World",
			"Open world discards changes. Is this ok?" ) == igdeCommonDialogs::ebNo ){
				return;
			}
		}
		
		decString filename( pWindow.GetWorld() ? pWindow.GetWorld()->GetFilePath()
			: pWindow.GetGameProject()->GetPathData() );
		if( igdeCommonDialogs::GetFileOpen( &pWindow, "Open World",
		*pWindow.GetEnvironment().GetFileSystemGame(),
		pWindow.GetLoadSaveSystem().GetFilePatternList(), filename ) ){
			pWindow.LoadWorld( filename );
		}
	}
};

class cActionFileSave : public cActionBase{
public:
	cActionFileSave( meWindowMain &window ) : cActionBase( window,
		"Save", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiSave ),
		"Save world to file", deInputEvent::esmControl, deInputEvent::ekcS, deInputEvent::ekcS ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		pWindow.GetSaveSupport().SaveWorldWithDependencies( world, false );
		return NULL;
	}
	
	virtual void Update( const meWorld &world ){
		SetEnabled( world.GetAnyChanged() );
	}
};

class cActionFileSaveAs : public cActionBase{
public:
	cActionFileSaveAs( meWindowMain &window ) : cActionBase( window,
		"Save As...", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiSaveAs ),
		"Save world under a differen file", deInputEvent::ekcA ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		pWindow.GetSaveSupport().SaveWorldWithDependencies( world, true );
		return NULL;
	}
};


class cActionEditCut : public cActionBase{
public:
	cActionEditCut( meWindowMain &window ) : cActionBase( window,
		"Cut", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiCut ),
		"Cut selected objects", deInputEvent::esmControl,
		deInputEvent::ekcX, deInputEvent::ekcT ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		igdeClipboardDataReference clip;
		
		switch( world->GetGuiParameters().GetElementMode() ){
		case meWorldGuiParameters::eemObject:
			if( world->GetSelectionObject().GetSelected().GetCount() == 0 ){
				return NULL;
			}
			
			clip.TakeOver( new meClipboardDataObject( *world ) );
			pWindow.GetClipboard().Set( clip );
			
			return new meUDeleteObject( world );
			
		default:
			return NULL;
		}
	}
};

class cActionEditCopy : public cActionBase{
public:
	cActionEditCopy( meWindowMain &window ) : cActionBase( window,
		"Copy", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ),
		"Copies selected objects", deInputEvent::esmControl,
		deInputEvent::ekcC, deInputEvent::ekcC ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		igdeClipboardDataReference clip;
		
		switch( world->GetGuiParameters().GetElementMode() ){
		case meWorldGuiParameters::eemObject:
			if( world->GetSelectionObject().GetSelected().GetCount() == 0 ){
				break;
			}
			
			clip.TakeOver( new meClipboardDataObject( *world ) );
			pWindow.GetClipboard().Set( clip );
			break;
			
		default:
			break;
		}
		return NULL;
	}
};

class cActionEditPaste : public cActionBase{
public:
	cActionEditPaste( meWindowMain &window ) : cActionBase( window,
		"Paste", window.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ),
		"Paste objects", deInputEvent::esmControl,
		deInputEvent::ekcV, deInputEvent::ekcP ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		switch( world->GetGuiParameters().GetElementMode() ){
		case meWorldGuiParameters::eemObject:
			if( pWindow.GetClipboard().HasWithTypeName( meClipboardDataObject::TYPE_NAME ) ){
				return new meUPasteObject( world, ( meClipboardDataObject* )
					pWindow.GetClipboard().GetWithTypeName( meClipboardDataObject::TYPE_NAME ) );
			}
			return NULL;
			
		default:
			return NULL;
		}
	}
	
	virtual void Update( const meWorld &world ){
		switch( world.GetGuiParameters().GetElementMode() ){
		case meWorldGuiParameters::eemObject:
			SetEnabled( pWindow.GetClipboard().HasWithTypeName( meClipboardDataObject::TYPE_NAME ) );
			break;
			
		default:
			SetEnabled( false );
		}
	}
};

class cActionEditDuplicate : public cActionBase{
public:
	cActionEditDuplicate( meWindowMain &window ) : cActionBase( window, "Duplicate",
		window.GetEnvironment().GetStockIcon( igdeEnvironment::esiDuplicate ),
		"Duplicate objects", deInputEvent::esmControl,
		deInputEvent::ekcD, deInputEvent::ekcD ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		switch( world->GetGuiParameters().GetElementMode() ){
		case meWorldGuiParameters::eemObject:
			if( world->GetSelectionObject().GetSelected().GetCount() > 0 ){
				return new meUObjDuplicate( world, decVector() );
			}
			return NULL;
			
		case meWorldGuiParameters::eemDecal:
			if( world->GetSelectionDecal().GetSelected().GetCount() > 0 ){
				return new meUDecalDuplicate( world, decVector() );
			}
			return NULL;
			
		default:
			return NULL;
		}
	}
	
	virtual void Update( const meWorld &world ){
		switch( world.GetGuiParameters().GetElementMode() ){
		case meWorldGuiParameters::eemObject:
			SetEnabled( world.GetSelectionObject().GetSelected().GetCount() > 0 );
			break;
			
		case meWorldGuiParameters::eemDecal:
			SetEnabled( world.GetSelectionDecal().GetSelected().GetCount() > 0 );
			break;
			
		default:
			SetEnabled( false );
		}
	}
};

class cActionEditDelete : public cActionBase{
public:
	cActionEditDelete( meWindowMain &window ) : cActionBase( window, "Delete",
		window.GetEnvironment().GetStockIcon( igdeEnvironment::esiDelete ),
		"Delete objects", deInputEvent::esmNone,
		deInputEvent::ekcDelete, deInputEvent::ekcE ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		switch( world->GetGuiParameters().GetElementMode() ){
		case meWorldGuiParameters::eemObject:
			if( world->GetSelectionObject().GetSelected().GetCount() > 0 ){
				return new meUDeleteObject( world );
			}
			return NULL;
			
		case meWorldGuiParameters::eemDecal:
			if( world->GetSelectionDecal().GetSelected().GetCount() > 0 ){
				return new meUDeleteDecal( world );
			}
			return NULL;
			
		case meWorldGuiParameters::eemNavSpace:
			if( world->GetSelectionNavigationSpace().GetSelected().GetCount() > 0 ){
				return new meUDeleteNavSpace( world );
			}
			return NULL;
			
		default:
			return NULL;
		}
	}
	
	virtual void Update( const meWorld &world ){
		switch( world.GetGuiParameters().GetElementMode() ){
		case meWorldGuiParameters::eemObject:
			SetEnabled( world.GetSelectionObject().GetSelected().GetCount() > 0 );
			break;
			
		case meWorldGuiParameters::eemDecal:
			SetEnabled( world.GetSelectionDecal().GetSelected().GetCount() > 0 );
			break;
			
		case meWorldGuiParameters::eemNavSpace:
			SetEnabled( world.GetSelectionNavigationSpace().GetSelected().GetCount() > 0 );
			break;
			
		default:
			SetEnabled( false );
		}
	}
};

class cActionEditElementMode : public cActionBase{
	const meWorldGuiParameters::eElementModes pMode;
	
public:
	cActionEditElementMode( meWindowMain &window, meWorldGuiParameters::eElementModes mode,
		const char *text, igdeIcon *icon, const char *description, deInputEvent::eKeyCodes keyCode,
		deInputEvent::eKeyCodes mnemonic ) : cActionBase( window, text, icon, description,
			deInputEvent::esmControl | deInputEvent::esmShift, keyCode, mnemonic ),
	pMode( mode ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		world->GetGuiParameters().SetElementMode( pMode );
		return NULL;
	}
	
	virtual void Update( const meWorld &world ){
		cActionBase::Update( world );
		SetSelected( world.GetGuiParameters().GetElementMode() == pMode );
	}
};

class cActionEditWorkMode : public cActionBase{
	const meWorldGuiParameters::eWorkModes pMode;
	
public:
	cActionEditWorkMode( meWindowMain &window, meWorldGuiParameters::eWorkModes mode,
		const char *text, igdeIcon *icon, const char *description, deInputEvent::eKeyCodes keyCode,
		deInputEvent::eKeyCodes mnemonic ) : cActionBase( window, text, icon, description,
			deInputEvent::esmNone, keyCode, mnemonic ),
	pMode( mode ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		world->GetGuiParameters().SetWorkMode( pMode );
		return NULL;
	}
	
	virtual void Update( const meWorld &world ){
		cActionBase::Update( world );
		SetSelected( world.GetGuiParameters().GetWorkMode() == pMode );
	}
};

class cActionEditLockAxisX : public cActionBase{
public:
	cActionEditLockAxisX( meWindowMain &window ) : cActionBase( window, "Lock X-Axis",
		window.GetIconEditLockAxisX(), "Lock X coordinates during editing",
		deInputEvent::esmControl | deInputEvent::esmShift, deInputEvent::ekcX ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		world->GetGuiParameters().SetLockAxisX( ! world->GetGuiParameters().GetLockAxisX() );
		return NULL;
	}
	
	virtual void Update( const meWorld &world ){
		cActionBase::Update( world );
		SetSelected( world.GetGuiParameters().GetLockAxisX() );
	}
};

class cActionEditLockAxisY : public cActionBase{
public:
	cActionEditLockAxisY( meWindowMain &window ) : cActionBase( window, "Lock Y-Axis",
		window.GetIconEditLockAxisY(), "Lock Y coordinates during editing",
		deInputEvent::esmControl | deInputEvent::esmShift, deInputEvent::ekcY ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		world->GetGuiParameters().SetLockAxisY( ! world->GetGuiParameters().GetLockAxisY() );
		return NULL;
	}
	
	virtual void Update( const meWorld &world ){
		cActionBase::Update( world );
		SetSelected( world.GetGuiParameters().GetLockAxisY() );
	}
};

class cActionEditLockAxisZ : public cActionBase{
public:
	cActionEditLockAxisZ( meWindowMain &window ) : cActionBase( window, "Lock Z-Axis",
		window.GetIconEditLockAxisZ(), "Lock Z coordinates during editing",
		deInputEvent::esmControl | deInputEvent::esmShift, deInputEvent::ekcY ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		world->GetGuiParameters().SetLockAxisZ( ! world->GetGuiParameters().GetLockAxisZ() );
		return NULL;
	}
	
	virtual void Update( const meWorld &world ){
		cActionBase::Update( world );
		SetSelected( world.GetGuiParameters().GetLockAxisZ() );
	}
};

class cActionEditUseLocal : public cActionBase{
public:
	cActionEditUseLocal( meWindowMain &window ) : cActionBase( window, "Use local coordinates",
		window.GetIconEditUseLocal(), "Uses local coordinates for editing instead of world coordinates",
		deInputEvent::esmControl | deInputEvent::esmShift, deInputEvent::ekcL ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		world->GetGuiParameters().SetUseLocal( ! world->GetGuiParameters().GetUseLocal() );
		return NULL;
	}
	
	virtual void Update( const meWorld &world ){
		cActionBase::Update( world );
		SetSelected( world.GetGuiParameters().GetUseLocal() );
	}
};

class cActionEditSnapSnapPoints : public cActionBase{
public:
	cActionEditSnapSnapPoints( meWindowMain &window ) : cActionBase( window, "Snap to Snap Points",
		window.GetIconEditSnap(), "Snap to Snap Points",
		deInputEvent::esmControl | deInputEvent::esmShift, deInputEvent::ekcP ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		world->GetGuiParameters().SetSnapToSnapPoints(
			! world->GetGuiParameters().GetSnapToSnapPoints() );
		return NULL;
	}
	
	virtual void Update( const meWorld &world ){
		cActionBase::Update( world );
		SetSelected( world.GetGuiParameters().GetSnapToSnapPoints() );
	}
};

class cActionEditRotationPivot : public cActionBase{
	meWorldGuiParameters::eRotationPivotCenters pRPC;
	
public:
	cActionEditRotationPivot( meWindowMain &window, meWorldGuiParameters::eRotationPivotCenters rpc,
		const char *text, igdeIcon *icon, const char *description ) :
	cActionBase( window, text, icon, description ),
	pRPC( rpc ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		world->GetGuiParameters().SetRotationPivotCenter( pRPC );
		return NULL;
	}
	
	virtual void Update( const meWorld &world ){
		cActionBase::Update( world );
		SetSelected( world.GetGuiParameters().GetRotationPivotCenter() == pRPC );
	}
};

class cActionEditUse3DCursor : public cActionBase{
public:
	cActionEditUse3DCursor( meWindowMain &window ) : cActionBase( window, "Use 3D-Cursor as Center",
		window.GetIconEdit3DCursor(), "Use 3D-Cursor as center for actions",
		deInputEvent::esmNone, deInputEvent::ekcUndefined, deInputEvent::ekc3 ){}
	
	virtual igdeUndo *OnAction( meWorld* ){
		pWindow.SetUse3DCursor( ! pWindow.GetUse3DCursor() );
		return NULL;
	}
	
	virtual void Update( const meWorld &world ){
		cActionBase::Update( world );
		SetSelected( pWindow.GetUse3DCursor() );
	}
};

class cActionEditSelectNone : public cActionBase{
public:
	cActionEditSelectNone( meWindowMain &window ) : cActionBase( window, "Select None",
		window.GetIconEditSelect(), "Select none", deInputEvent::esmControl, deInputEvent::ekcA ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		switch( world->GetGuiParameters().GetElementMode() ){
		case meWorldGuiParameters::eemObject:
			if( world->GetSelectionObject().GetSelected().GetCount() > 0 ){
				world->GetSelectionObject().Reset();
				world->NotifyObjectSelectionChanged();
			}
			break;
			
		case meWorldGuiParameters::eemObjectShape:
			if( world->GetSelectionObjectShape().GetSelected().GetCount() > 0 ){
				world->GetSelectionObjectShape().Reset();
				world->NotifyObjectShapeSelectionChanged();
			}
			break;
			
		case meWorldGuiParameters::eemDecal:
			if( world->GetSelectionDecal().GetSelected().GetCount() > 0 ){
				world->GetSelectionDecal().Reset();
				world->NotifyDecalSelectionChanged();
			}
			break;
			
		case meWorldGuiParameters::eemNavSpace:
			if( world->GetSelectionNavigationSpace().GetSelected().GetCount() > 0 ){
				world->GetSelectionNavigationSpace().Reset();
				world->NotifyNavSpaceSelectionChanged();
			}
			break;
			
		default:
			break;
		}
		return NULL;
	}
};



class cActionObjectDropToGround : public cActionBase{
public:
	cActionObjectDropToGround( meWindowMain &window ) : cActionBase( window, "Drop to ground",
		window.GetEnvironment().GetStockIcon( igdeEnvironment::esiStrongDown ),
		"Drops the selected objects to the ground", deInputEvent::esmControl,
		deInputEvent::ekcG, deInputEvent::ekcG ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		const meObjectList &list = world->GetSelectionObject().GetSelected();
		if( list.GetCount() == 0 ){
			return NULL;
		}
		return new meUObjectDropToGround( world, list );
	}
	
	virtual void Update( const meWorld &world ){
		SetEnabled( world.GetSelectionObject().GetSelected().GetCount() > 0 );
	}
};

class cActionObjectSnapToGrid : public cActionBase{
public:
	cActionObjectSnapToGrid( meWindowMain &window ) : cActionBase( window, "Snap to grid",
		window.GetIconEditSnap(), "Snap objects to grid", deInputEvent::esmNone,
		deInputEvent::ekcUndefined, deInputEvent::ekcS ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		const meObjectList &list = world->GetSelectionObject().GetSelected();
		if( list.GetCount() == 0 ){
			return NULL;
		}
		return new meUObjectSnapToGrid( world, list, pWindow.GetConfiguration().GetMoveStep() );
	}
	
	virtual void Update( const meWorld &world ){
		SetEnabled( world.GetSelectionObject().GetSelected().GetCount() > 0 );
	}
};

class cActionBaseObjectCopyToSelected : public cActionBase{
protected:
	bool pCopyX;
	bool pCopyY;
	bool pCopyZ;
	
public:
	cActionBaseObjectCopyToSelected( meWindowMain &window, const char *baseText, bool copyX, bool copyY, bool copyZ ) :
		cActionBase( window, Text( baseText, copyX, copyY, copyZ ), window.GetIconEditSnap(), baseText ),
		pCopyX( copyX ), pCopyY( copyY ), pCopyZ( copyZ ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		return world->GetSelectionObject().GetSelected().GetCount() > 1 ? OnActionCopy( world ) :  nullptr;
	}
	
	virtual igdeUndo *OnActionCopy( meWorld *world ) = 0;
	
	virtual void Update( const meWorld &world ){
		SetEnabled( world.GetSelectionObject().GetSelected().GetCount() > 1 );
	}
	
	static decString Text( const char *baseText, bool copyX, bool copyY, bool copyZ ){
		decString text;
		text.Format( "%s: %s%s%s", baseText, copyX ? "X" : "", copyY ? "Y" : "", copyZ ? "Z" : "" );
		return text;
	}
};

class cActionObjectCopyPosition : public cActionBaseObjectCopyToSelected{
public:
	cActionObjectCopyPosition( meWindowMain &window, bool copyX, bool copyY, bool copyZ ) :
		cActionBaseObjectCopyToSelected( window, "Copy Position To Selected", copyX, copyY, copyZ ){}
	
	virtual igdeUndo *OnActionCopy( meWorld *world ){
		return new meUObjectCopyPosition( world, pCopyX, pCopyY, pCopyZ );
	}
};

class cActionObjectCopyRotation : public cActionBaseObjectCopyToSelected{
public:
	cActionObjectCopyRotation( meWindowMain &window, bool copyX, bool copyY, bool copyZ ) :
		cActionBaseObjectCopyToSelected( window, "Copy Rotation To Selected", copyX, copyY, copyZ ){}
	
	virtual igdeUndo *OnActionCopy( meWorld *world ){
		return new meUObjectCopyRotation( world, pCopyX, pCopyY, pCopyZ );
	}
};

class cActionObjectCopyScale : public cActionBaseObjectCopyToSelected{
public:
	cActionObjectCopyScale( meWindowMain &window, bool copyX, bool copyY, bool copyZ ) :
		cActionBaseObjectCopyToSelected( window, "Copy Scale To Selected", copyX, copyY, copyZ ){}
	
	virtual igdeUndo *OnActionCopy( meWorld *world ){
		return new meUObjectCopyScale( world, pCopyX, pCopyY, pCopyZ );
	}
};

class cActionObjectAttachTo : public cActionBase{
public:
	cActionObjectAttachTo( meWindowMain &window ) : cActionBase( window, "Attach To",
		NULL, "Attach objects to another object", deInputEvent::esmNone,
		deInputEvent::ekcUndefined, deInputEvent::ekcA ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		meObject * const active = world->GetSelectionObject().GetActive();
		const meObjectList &list = world->GetSelectionObject().GetSelected();
		if( ! active || list.GetCount() == 0 ){
			return NULL;
		}
		return new meUObjectAttachTo( world, list, active );
	}
	
	virtual void Update( const meWorld &world ){
		SetEnabled( world.GetSelectionObject().HasActive()
			&& world.GetSelectionObject().GetSelected().GetCount() > 0 );
	}
};

class cActionObjectDetach : public cActionBase{
public:
	cActionObjectDetach( meWindowMain &window ) : cActionBase( window, "Detach",
		NULL, "Detach objects from their parents", deInputEvent::esmNone,
		deInputEvent::ekcUndefined, deInputEvent::ekcE ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		const meObjectList &list = world->GetSelectionObject().GetSelected();
		if( list.GetCount() == 0 ){
			return NULL;
		}
		return new meUObjectAttachTo( world, list, NULL );
	}
	
	virtual void Update( const meWorld &world ){
		SetEnabled( world.GetSelectionObject().GetSelected().GetCount() > 0 );
	}
};

class cActionObjectSelectAttached : public cActionBase{
public:
	cActionObjectSelectAttached( meWindowMain &window ) : cActionBase( window, "Select Attached",
		NULL, "Select all objects attached to the active object" ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		meObject * const active = world->GetSelectionObject().GetActive();
		if( ! active ){
			return NULL;
		}
		
		meObjectSelection &selection = world->GetSelectionObject();
		const meObjectList &attached = active->GetAttachedObjectsList();
		const int count = attached.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			selection.Add( attached.GetAt( i ) );
		}
		
		world->NotifyObjectSelectionChanged();
		return NULL;
	}
	
	virtual void Update( const meWorld &world ){
		SetEnabled( world.GetSelectionObject().HasActive() );
	}
};

class cActionObjectReassignIDs : public cActionBase{
public:
	cActionObjectReassignIDs( meWindowMain &window ) : cActionBase( window, "Reassign Object IDs",
		NULL, "Reassign Object IDs (WARNING! Dangerous Operation!)" ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		if( igdeCommonDialogs::QuestionFormat( &pWindow, igdeCommonDialogs::ebsYesNo, "Reassign Object IDs",
		"This action will assign new IDs starting at ID 1 for all objects.\n"
		"This can be used before shipping to compact IDs into a continuous range.\n"
		"This should NEVER be done after a production release.\n"
		"This WILL break save states and all other linking usage of the Object ID.\n\n"
		"Are you REALLY sure you want to reassign object IDs now?" ) == igdeCommonDialogs::ebYes ){
			world->ReassignObjectIDs();
		}
		return NULL;
	}
};

class cActionObjectRotate : public cActionBase{
	const decVector pRotation;
public:
	cActionObjectRotate( meWindowMain &window, const decVector &rotation, const char *text,
		igdeIcon *icon, const char *description, deInputEvent::eKeyCodes mnemonic ) :
	cActionBase( window, text, icon, description, deInputEvent::esmNone,
		deInputEvent::ekcUndefined, mnemonic ), pRotation( rotation ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		meObject * const object = world->GetSelectionObject().GetActive();
		if( ! object ){
			return NULL;
		}
		return new meUSetObjectRotation( object, object->GetRotation() + pRotation );
	}
	
	virtual void Update( const meWorld &world ){
		SetEnabled( world.GetSelectionObject().HasActive() );
	}
};


class cActionObjectLightToggle : public cActionBase{
public:
	cActionObjectLightToggle( meWindowMain &window ) : cActionBase( window, "Toggle light",
		window.GetIconObjectLightToggle(), "Toggle light on and off",
		deInputEvent::esmNone, deInputEvent::ekcUndefined, deInputEvent::ekcT ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		meObject * const object = world->GetSelectionObject().GetActive();
		decString propertyName;
		if( object && ActivatePropName( *world, propertyName ) && object->GetProperties().Has( propertyName ) ){
			const decString &value = object->GetProperties().GetAt( propertyName );
			return new meUObjectSetProperty( object, propertyName, value, value == "0" ? "1" : "0" );
			
		}else{
			return new meUObjectAddProperty( object, propertyName, "1" );
		}
	}
	
	virtual void Update( const meWorld &world ){
		decString propertyName;
		SetEnabled( ActivatePropName( world, propertyName ) );
	}
	
	bool ActivatePropName( const meWorld &world, decString &propertyName ) const{
		meObject * const object = world.GetSelectionObject().GetActive();
		if( ! object || ! object->GetGDClass() ){
			return false;
		}
		
		igdeGDCLight *gdLight = NULL;
		decString gdpPrefix;
		if( meHelpers::FindFirstLight( *object->GetGDClass(), gdpPrefix, gdLight )
		&& gdLight->IsPropertySet( igdeGDCLight::epActivated ) ){
			propertyName = gdpPrefix + gdLight->GetPropertyName( igdeGDCLight::epActivated );
			return true;
		}
		return false;
	}
};


class cActionObjectShapeBase : public cActionBase{
public:
	cActionObjectShapeBase( meWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic ) : cActionBase( window,
			text, icon, description, deInputEvent::esmNone, deInputEvent::ekcUndefined, mnemonic ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		meObject *object = world->GetSelectionObject().GetActive();
		const char * const property = ActivatePropName( *world );
		if( ! property || ! object ){
			return NULL;
		}
		return OnActionShape( world, object, property );
	}
	
	virtual igdeUndo *OnActionShape( meWorld *world, meObject *object, const char *property ) = 0;
	
	virtual void Update( const meWorld &world ){
		SetEnabled( ActivatePropName( world ) );
	}
	
	const char *ActivatePropName( const meWorld &world ) const{
		meObject * const object = world.GetSelectionObject().GetActive();
		if( ! object ){
			return NULL;
		}
		
		const decString &property = object->GetActiveProperty();
		if( object->IsPropertyShape( property ) ){
			if( world.GetObjectShapes().GetCount() > 0 ){
				return NULL;
			}
			
		}else if( ! object->IsPropertyShapeList( property ) ){
			return NULL;
		}
		
		return property;
	}
};

class cActionObjectShapeAdd : public cActionObjectShapeBase{
public:
	cActionObjectShapeAdd( meWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic ) :
		cActionObjectShapeBase( window, text, icon, description, mnemonic ){}
	
	virtual igdeUndo * OnActionShape( meWorld *world, meObject *object, const char *property ){
		meObjectShapeSelection &selection = world->GetSelectionObjectShape();
		igdeUndoReference undo;
		
		decShape * const shape = CreateShape();
		undo.TakeOver( new meUObjectShapeAdd( object, property, *shape ) );
		delete shape;
		
		world->GetUndoSystem()->Add( undo );
		
		selection.Reset();
		selection.Add( world->GetObjectShapes().GetAt( world->GetObjectShapes().GetCount() - 1 ) );
		selection.ActivateNext();
		world->NotifyObjectShapeSelectionChanged();
		return NULL;
	}
	
	virtual decShape *CreateShape() = 0;
};

class cActionObjectShapeAddSphere : public cActionObjectShapeAdd{
public:
	cActionObjectShapeAddSphere( meWindowMain &window ) : cActionObjectShapeAdd( window,
		"Add Sphere Shape", NULL, "Add sphere shape", deInputEvent::ekcS ){}
	
	virtual decShape *CreateShape(){
		return new decShapeSphere( 0.5f );
	}
};

class cActionObjectShapeAddBox : public cActionObjectShapeAdd{
public:
	cActionObjectShapeAddBox( meWindowMain &window ) : cActionObjectShapeAdd( window,
		"Add Box Shape", NULL, "Add box shape", deInputEvent::ekcB ){}
	
	virtual decShape *CreateShape(){
		return new decShapeBox( decVector( 0.5f, 0.5f, 0.5f ) );
	}
};

class cActionObjectShapeAddCylinder : public cActionObjectShapeAdd{
public:
	cActionObjectShapeAddCylinder( meWindowMain &window ) : cActionObjectShapeAdd( window,
		"Add Cylinder Shape", NULL, "Add cylinder shape", deInputEvent::ekcC ){}
	
	virtual decShape *CreateShape(){
		return new decShapeCylinder( 0.5f, 0.25f );
	}
};

class cActionObjectShapeAddCapsule : public cActionObjectShapeAdd{
public:
	cActionObjectShapeAddCapsule( meWindowMain &window ) : cActionObjectShapeAdd( window,
		"Add Capsule Shape", NULL, "Add capsule shape", deInputEvent::ekcP ){}
	
	virtual decShape *CreateShape(){
		return new decShapeCapsule( 0.5f, 0.25f, 0.25f );
	}
};

class cActionObjectShapeDelete : public cActionObjectShapeBase{
public:
	cActionObjectShapeDelete( meWindowMain &window ) : cActionObjectShapeBase( window,
		"Delete Shapes", NULL, "Delete selected shapes", deInputEvent::ekcD ){}
	
	virtual igdeUndo * OnActionShape( meWorld *world, meObject *object, const char *property ){
		return new meUObjectShapesDelete( object, property, world->GetSelectionObjectShape().GetSelected() );
	}
};


class cActionDecalDelete : public cActionBase{
public:
	cActionDecalDelete( meWindowMain &window ) : cActionBase( window, "Delete Decals",
		NULL, "Deletes the selected decals" ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		if( world->GetSelectionDecal().GetSelected().GetCount() == 0 ){
			return NULL;
		}
		return new meUDeleteDecal( world );
	}
	
	virtual void Update( const meWorld &world ){
		SetEnabled( world.GetSelectionDecal().GetSelected().GetCount() > 0 );
	}
};

class cActionDecalReorder : public cActionBase{
public:
	cActionDecalReorder( meWindowMain &window, const char *text, igdeIcon *icon,
		const char *description, deInputEvent::eKeyCodes mnemonic ) :
		cActionBase( window, text, icon, description, mnemonic ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		meDecal * const decal = world->GetSelectionDecal().GetActive();
		if( ! decal ){
			return NULL;
		}
		meObject * const object = decal->GetParentObject();
		if( ! object ){
			return NULL;
		}
		if( ! CanReorder( *object, object->IndexOfDecal( decal ) ) ){
			return NULL;
		}
		return OnActionDecal( world, decal );
	}
	
	virtual void Update( const meWorld &world ){
		meDecal * const decal = world.GetSelectionDecal().GetActive();
		if( decal ){
			const meObject * const object = decal->GetParentObject();
			SetEnabled( object && CanReorder( *object, object->IndexOfDecal( decal ) ) );
			
		}else{
			SetEnabled( false );
		}
	}
	
	virtual bool CanReorder( const meObject &object, int index ) = 0;
	virtual igdeUndo *OnActionDecal( meWorld *world, meDecal *decal ) = 0;
};

class cActionDecalRaiseTop : public cActionDecalReorder{
public:
	cActionDecalRaiseTop( meWindowMain &window ) : cActionDecalReorder( window, "Raise to Top",
		window.GetEnvironment().GetStockIcon( igdeEnvironment::esiStrongUp ),
		"Raise decal to the top", deInputEvent::ekcT ){}
	
	virtual bool CanReorder( const meObject &, int index ){
		return index > 0;
	}
	
	virtual igdeUndo *OnActionDecal( meWorld *world, meDecal *decal ){
		return new meURaiseDecalTop( world, decal );
	}
};

class cActionDecalRaiseOne : public cActionDecalReorder{
public:
	cActionDecalRaiseOne( meWindowMain &window ) : cActionDecalReorder( window, "Raise by one",
		window.GetEnvironment().GetStockIcon( igdeEnvironment::esiUp ),
		"Raise decal by one level", deInputEvent::ekcR ){}
	
	virtual bool CanReorder( const meObject &, int index ){
		return index > 0;
	}
	
	virtual igdeUndo *OnActionDecal( meWorld *world, meDecal *decal ){
		return new meURaiseDecalOne( world, decal );
	}
};

class cActionDecalLowerOne : public cActionDecalReorder{
public:
	cActionDecalLowerOne( meWindowMain &window ) : cActionDecalReorder( window, "Lower by one",
		window.GetEnvironment().GetStockIcon( igdeEnvironment::esiDown ),
		"Lower decal by one level", deInputEvent::ekcL ){}
	
	virtual bool CanReorder( const meObject &object, int index ){
		return index < object.GetDecalCount() - 1;
	}
	
	virtual igdeUndo *OnActionDecal( meWorld *world, meDecal *decal ){
		return new meULowerDecalOne( world, decal );
	}
};

class cActionDecalLowerBottom : public cActionDecalReorder{
public:
	cActionDecalLowerBottom( meWindowMain &window ) : cActionDecalReorder( window, "Lower to Bottom",
		window.GetEnvironment().GetStockIcon( igdeEnvironment::esiStrongDown ),
		"Lower decal to the bottom", deInputEvent::ekcB ){}
	
	virtual bool CanReorder( const meObject &object, int index ){
		return index < object.GetDecalCount() - 1;
	}
	
	virtual igdeUndo *OnActionDecal( meWorld *world, meDecal *decal ){
		return new meULowerDecalBottom( world, decal );
	}
};


class cActionFullBright : public cActionBase{
public:
	cActionFullBright( meWindowMain &window ) : cActionBase( window, "Full Bright Mode",
		window.GetIconViewFullBrightOn(), "Toggles full bright mode",
		deInputEvent::esmNone, deInputEvent::ekcUndefined, deInputEvent::ekcB ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		world->SetFullBright( ! world->GetFullBright() );
		return NULL;
	}
	
	virtual void Update( const meWorld &world ){
		cActionBase::Update( world );
		SetSelected( world.GetFullBright() );
	}
};

class cActionMuteSound : public cActionBase{
public:
	cActionMuteSound( meWindowMain &window ) : cActionBase( window, "Mute Sound",
		window.GetIconViewMuteSoundOn(), "Toggles mute sound",
		deInputEvent::esmNone, deInputEvent::ekcUndefined, deInputEvent::ekcM ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		world->GetMicrophone()->SetMuted( ! world->GetMicrophone()->GetMuted() );
		return NULL;
	}
	
	virtual void Update( const meWorld &world ){
		cActionBase::Update( world );
		SetSelected( world.GetMicrophone()->GetMuted() );
	}
};

class cActionShowOcclusionMeshes : public cActionBase{
public:
	cActionShowOcclusionMeshes( meWindowMain &window ) : cActionBase( window,
		"Show Occlusion Meshes", NULL, "Show occlusion meshes" ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		world->GetGuiParameters().SetShowOcclusionMeshes(
			! world->GetGuiParameters().GetShowOcclusionMeshes() );
		return NULL;
	}
	
	virtual void Update( const meWorld &world ){
		cActionBase::Update( world );
		SetSelected( world.GetGuiParameters().GetShowOcclusionMeshes() );
	}
};

class cActionShowOcclusionMeshesSelected : public cActionBase{
public:
	cActionShowOcclusionMeshesSelected( meWindowMain &window ) : cActionBase( window,
		"Show Occlusion Meshes Selected", NULL, "Show occlusion meshes of selected objects" ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		world->GetGuiParameters().SetShowOcclusionMeshesSelected(
			! world->GetGuiParameters().GetShowOcclusionMeshesSelected() );
		return NULL;
	}
	
	virtual void Update( const meWorld &world ){
		cActionBase::Update( world );
		SetSelected( world.GetGuiParameters().GetShowOcclusionMeshesSelected() );
	}
};

class cActionShowNavigationSpaces : public cActionBase{
public:
	cActionShowNavigationSpaces( meWindowMain &window ) : cActionBase( window,
		"Show Navigation Spaces", NULL, "Show navigation spaces" ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		world->GetGuiParameters().SetShowNavigationSpaces(
			! world->GetGuiParameters().GetShowNavigationSpaces() );
		return NULL;
	}
	
	virtual void Update( const meWorld &world ){
		cActionBase::Update( world );
		SetSelected( world.GetGuiParameters().GetShowNavigationSpaces() );
	}
};

class cActionShowNavigationSpacesSelected : public cActionBase{
public:
	cActionShowNavigationSpacesSelected( meWindowMain &window ) : cActionBase( window,
		"Show Navigation Spaces Selected", NULL, "Show navigation spaces of selected objects" ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		world->GetGuiParameters().SetShowNavigationSpacesSelected(
			! world->GetGuiParameters().GetShowNavigationSpacesSelected() );
		return NULL;
	}
	
	virtual void Update( const meWorld &world ){
		cActionBase::Update( world );
		SetSelected( world.GetGuiParameters().GetShowNavigationSpacesSelected() );
	}
};


class cActionNavTestLoad : public cActionBase{
public:
	cActionNavTestLoad( meWindowMain &window ) : cActionBase( window,
		"Load navigation test", NULL, "Load Navigation Test" ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		decString filename( world->GetPathNavTest() );
		if( igdeCommonDialogs::GetFileOpen( &pWindow, "Open Navigation Test",
		*pWindow.GetLoadSaveSystem().GetNavTestFilePatterns(), filename ) ){
			world->SetPathNavTest( filename );
			pWindow.GetEditorModule().LogInfoFormat( "Loading Navigation Test %s", filename.GetString() );
			pWindow.GetLoadSaveSystem().LoadNavTest( filename, *world );
		}
		return NULL;
	}
};

class cActionNavTestSave : public cActionBase{
public:
	cActionNavTestSave( meWindowMain &window ) : cActionBase( window,
		"Save navigation test", NULL, "Save Navigation Test" ){}
	
	virtual igdeUndo *OnAction( meWorld *world ){
		decString filename( world->GetPathNavTest() );
		if( igdeCommonDialogs::GetFileSave( &pWindow, "Save Navigation Test",
		*pWindow.GetLoadSaveSystem().GetNavTestFilePatterns(), filename ) ){
			world->SetPathNavTest( filename );
			pWindow.GetEditorModule().LogInfoFormat( "Saving Navigation Test %s", filename.GetString() );
			pWindow.GetLoadSaveSystem().SaveNavTest( filename, *world );
		}
		return NULL;
	}
};

}



// Private Functions
//////////////////////

void meWindowMain::pLoadIcons(){
	pIconEditObject.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_object.png" ) );
	pIconEditDecal.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_decal.png" ) );
	pIconEditNavSpace.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_navspace.png" ) );
	pIconEditObjectShape.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_objectshape.png" ) );
	pIconEditSelect.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_select.png" ) );
	pIconEditMove.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_move.png" ) );
	pIconEditScale.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_scale.png" ) );
	pIconEditRotate.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_rotate.png" ) );
	pIconEdit3DCursor.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_3d_cursor.png" ) );
	pIconEditMaskPaint.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_maskpaint.png" ) );
	pIconEditHeightPaint.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_heightpaint.png" ) );
	pIconEditVisibilityPaint.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_visibilitypaint.png" ) );
	pIconEditLockAxisX.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_lock_axis_x.png" ) );
	pIconEditLockAxisY.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_lock_axis_y.png" ) );
	pIconEditLockAxisZ.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_lock_axis_z.png" ) );
	pIconEditUseLocal.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_use_local.png" ) );
	pIconEditSnap.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_snap.png" ) );
	pIconViewFullBrightOn.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_full_bright_on.png" ) );
	pIconViewFullBrightOff.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_full_bright_off.png" ) );
	pIconViewMuteSoundOn.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_mute_sound_on.png" ) );
	pIconViewMuteSoundOff.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/edit_mute_sound_off.png" ) );
	pIconObjectLightToggle.TakeOver( igdeIcon::LoadPNG( GetEditorModule(), "icons/object_light_toggle.png" ) );
}

void meWindowMain::pCreateActions(){
	igdeEnvironment &environment = GetEnvironment();
	
	pActionFileNew.TakeOver( new cActionFileNew( *this ) );
	pActionFileOpen.TakeOver( new cActionFileOpen( *this ) );
	pActionFileSave.TakeOver( new cActionFileSave( *this ) );
	pActionFileSaveAs.TakeOver( new cActionFileSaveAs( *this ) );
	
	pActionEditUndo.TakeOver( new igdeActionUndo( environment ) );
	pActionEditRedo.TakeOver( new igdeActionRedo( environment ) );
	
	pActionEditCut.TakeOver( new cActionEditCut( *this ) );
	pActionEditCopy.TakeOver( new cActionEditCopy( *this ) );
	pActionEditPaste.TakeOver( new cActionEditPaste( *this ) );
	pActionEditDuplicate.TakeOver( new cActionEditDuplicate( *this ) );
	pActionEditDelete.TakeOver( new cActionEditDelete( *this ) );
	
	pActionEditEModeObject.TakeOver( new cActionEditElementMode( *this,
		meWorldGuiParameters::eemObject, "Object Mode", pIconEditObject, "Object mode",
		deInputEvent::ekcQ, deInputEvent::ekcO ) );
	pActionEditEModeDecal.TakeOver( new cActionEditElementMode( *this,
		meWorldGuiParameters::eemDecal, "Decal Mode", pIconEditDecal, "Decal mode",
		deInputEvent::ekcW, deInputEvent::ekcD ) );
	pActionEditEModeNavSpace.TakeOver( new cActionEditElementMode( *this,
		meWorldGuiParameters::eemNavSpace, "Navigation Space Mode", pIconEditNavSpace, "Navigation space mode",
		deInputEvent::ekcR, deInputEvent::ekcN ) );
	pActionEditEModeObjectShape.TakeOver( new cActionEditElementMode( *this,
		meWorldGuiParameters::eemObjectShape, "Object Shape Mode", pIconEditObjectShape, "Object shape mode",
		deInputEvent::ekcT, deInputEvent::ekcO ) );
	
	pActionEditSelectMode.TakeOver( new cActionEditWorkMode( *this,
		meWorldGuiParameters::ewmSelect, "Select Mode", pIconEditSelect, "Select mode",
		deInputEvent::ekc1, deInputEvent::ekcUndefined ) );
	pActionEditMoveMode.TakeOver( new cActionEditWorkMode( *this,
		meWorldGuiParameters::ewmMove, "Move Mode", pIconEditMove, "Move mode",
		deInputEvent::ekc2, deInputEvent::ekcUndefined ) );
	pActionEditScaleMode.TakeOver( new cActionEditWorkMode( *this,
		meWorldGuiParameters::ewmScale, "Scale Mode", pIconEditScale, "Scale mode",
		deInputEvent::ekc3, deInputEvent::ekcUndefined ) );
	pActionEditRotateMode.TakeOver( new cActionEditWorkMode( *this,
		meWorldGuiParameters::ewmRotate, "Rotation Mode", pIconEditRotate, "Rotation mode",
		deInputEvent::ekc4, deInputEvent::ekcUndefined ) );
	pActionEditAddMode.TakeOver( new cActionEditWorkMode( *this,
		meWorldGuiParameters::ewmAddNew, "Add New Mode",
		environment.GetStockIcon( igdeEnvironment::esiPlus ), "Add new mode",
		deInputEvent::ekc5, deInputEvent::ekcUndefined ) );
	pActionEditHeightPaintMode.TakeOver( new cActionEditWorkMode( *this,
		meWorldGuiParameters::ewmHeightPaint, "Height Paint Mode", pIconEditHeightPaint, "Height paint mode",
		deInputEvent::ekc6, deInputEvent::ekcUndefined ) );
	pActionEditMaskPaintMode.TakeOver( new cActionEditWorkMode( *this,
		meWorldGuiParameters::ewmMaskPaint, "Mask Paint Mode", pIconEditMaskPaint, "Mask paint mode",
		deInputEvent::ekc7, deInputEvent::ekcUndefined ) );
	pActionEditVisPaintMode.TakeOver( new cActionEditWorkMode( *this,
		meWorldGuiParameters::ewmVisibilityPaint, "Visibility Paint Mode", pIconEditVisibilityPaint, "Visibility paint mode",
		deInputEvent::ekc8, deInputEvent::ekcUndefined ) );
	pActionEditModeNavSpaceEdit.TakeOver( new cActionEditWorkMode( *this,
		meWorldGuiParameters::ewmNavSpaceEdit, "Navigation Space Edit Mode", pIconEditNavSpace, "Navigation space edit mode",
		deInputEvent::ekc9, deInputEvent::ekcUndefined ) );
// 	pActionEdit3DCursorMode.TakeOver( new cActionEditWorkMode( *this,
// 		meWorldGuiParameters::ewm3DCursor, "3D-Cursor Mode", pIconEdit3DCursor, "3D-Cursor mode",
// 		deInputEvent::ekc0, deInputEvent::ekcUndefined ) );
	
	pActionEditLockAxisX.TakeOver( new cActionEditLockAxisX( *this ) );
	pActionEditLockAxisY.TakeOver( new cActionEditLockAxisY( *this ) );
	pActionEditLockAxisZ.TakeOver( new cActionEditLockAxisZ( *this ) );
	pActionEditUseLocal.TakeOver( new cActionEditUseLocal( *this ) );
	pActionEditSnapSnapPoints.TakeOver( new cActionEditSnapSnapPoints( *this ) );
	
	pActionEditRPCenterActive.TakeOver( new cActionEditRotationPivot( *this,
		meWorldGuiParameters::erpcActive, "Rotation Pivot Center Active",
		NULL, "Rotate around selected element position" ) );
	pActionEditRPCenterSelected.TakeOver( new cActionEditRotationPivot( *this,
		meWorldGuiParameters::erpcSelected, "Rotation Pivot Center Selected",
		NULL, "Rotate around average position of all selected elements" ) );
	pActionEditRPCenterIndividual.TakeOver( new cActionEditRotationPivot( *this,
		meWorldGuiParameters::erpcIndividual, "Rotation Pivot Center Individual",
		NULL, "Rotate around individual element positions" ) );
	
	pActionEditUse3DCursor.TakeOver( new cActionEditUse3DCursor( *this ) );
	pActionEditSelectNone.TakeOver( new cActionEditSelectNone( *this ) );
	
	pActionObjectLightToggle.TakeOver( new cActionObjectLightToggle( *this ) );
	
	pActionObjectRotateL45.TakeOver( new cActionObjectRotate( *this, decVector( 0.0f, 45.0f, 0.0f ),
		"Left 45°", environment.GetStockIcon( igdeEnvironment::esiLeft ),
		"Rotate object left by 45°", deInputEvent::ekcUndefined ) );
	pActionObjectRotateL90.TakeOver( new cActionObjectRotate( *this, decVector( 0.0f, 90.0f, 0.0f ),
		"Left 90°", environment.GetStockIcon( igdeEnvironment::esiStrongLeft ),
		"Rotate object left by 90°", deInputEvent::ekcUndefined ) );
	pActionObjectRotateR45.TakeOver( new cActionObjectRotate( *this, decVector( 0.0f, -45.0f, 0.0f ),
		"Right 45°", environment.GetStockIcon( igdeEnvironment::esiRight ),
		"Rotate object right by 45°", deInputEvent::ekcUndefined ) );
	pActionObjectRotateR90.TakeOver( new cActionObjectRotate( *this, decVector( 0.0f, -90.0f, 0.0f ),
		"Right 90°", environment.GetStockIcon( igdeEnvironment::esiStrongRight ),
		"Rotate object right by 90°", deInputEvent::ekcUndefined ) );
	pActionObjectRotate180.TakeOver( new cActionObjectRotate( *this, decVector( 0.0f, 180.0f, 0.0f ),
		"Turn around 180°", environment.GetStockIcon( igdeEnvironment::esiStrongDown ),
		"Rotate object by 180°", deInputEvent::ekcUndefined ) );
	
	pActionObjectDropToGround.TakeOver( new cActionObjectDropToGround( *this ) );
	pActionObjectSnapToGrid.TakeOver( new cActionObjectSnapToGrid( *this ) );
	pActionObjectCopyPositionX.TakeOver( new cActionObjectCopyPosition( *this, true, false, false ) );
	pActionObjectCopyPositionY.TakeOver( new cActionObjectCopyPosition( *this, false, true, false ) );
	pActionObjectCopyPositionZ.TakeOver( new cActionObjectCopyPosition( *this, false, false, true ) );
	pActionObjectCopyPositionXZ.TakeOver( new cActionObjectCopyPosition( *this, true, false, true ) );
	pActionObjectCopyPositionXYZ.TakeOver( new cActionObjectCopyPosition( *this, true, true, true ) );
	pActionObjectCopyRotationX.TakeOver( new cActionObjectCopyRotation( *this, true, false, false ) );
	pActionObjectCopyRotationY.TakeOver( new cActionObjectCopyRotation( *this, false, true, false ) );
	pActionObjectCopyRotationZ.TakeOver( new cActionObjectCopyRotation( *this, false, false, true ) );
	pActionObjectCopyRotationXYZ.TakeOver( new cActionObjectCopyRotation( *this, true, true, true ) );
	pActionObjectCopyScaleX.TakeOver( new cActionObjectCopyScale( *this, true, false, false ) );
	pActionObjectCopyScaleY.TakeOver( new cActionObjectCopyScale( *this, false, true, false ) );
	pActionObjectCopyScaleZ.TakeOver( new cActionObjectCopyScale( *this, false, false, true ) );
	pActionObjectCopyScaleXYZ.TakeOver( new cActionObjectCopyScale( *this, true, true, true ) );
	pActionObjectAttachTo.TakeOver( new cActionObjectAttachTo( *this ) );
	pActionObjectDetach.TakeOver( new cActionObjectDetach( *this ) );
	pActionObjectSelectAttached.TakeOver( new cActionObjectSelectAttached( *this ) );
	pActionObjectReassignIDs.TakeOver( new cActionObjectReassignIDs( *this ) );
	pActionObjectSubclassAsEclass.TakeOver( new meASubclassAsEClass( *this ) );
	
	pActionObjectShapeAddSphere.TakeOver( new cActionObjectShapeAddSphere( *this ) );
	pActionObjectShapeAddBox.TakeOver( new cActionObjectShapeAddBox( *this ) );
	pActionObjectShapeAddCylinder.TakeOver( new cActionObjectShapeAddCylinder( *this ) );
	pActionObjectShapeAddCapsule.TakeOver( new cActionObjectShapeAddCapsule( *this ) );
	pActionObjectShapeDelete.TakeOver( new cActionObjectShapeDelete( *this ) );
	
	pActionDecalDelete.TakeOver( new cActionDecalDelete( *this ) );
	pActionDecalRaiseTop.TakeOver( new cActionDecalRaiseTop( *this ) );
	pActionDecalRaiseOne.TakeOver( new cActionDecalRaiseOne( *this ) );
	pActionDecalLowerOne.TakeOver( new cActionDecalLowerOne( *this ) );
	pActionDecalLowerBottom.TakeOver( new cActionDecalLowerBottom( *this ) );
	
	pActionFullBright.TakeOver( new cActionFullBright( *this ) );
	pActionMuteSound.TakeOver( new cActionMuteSound( *this ) );
	pActionShowOcclusionMeshes.TakeOver( new cActionShowOcclusionMeshes( *this ) );
	pActionShowOcclusionMeshesSelected.TakeOver( new cActionShowOcclusionMeshesSelected( *this ) );
	pActionShowNavigationSpaces.TakeOver( new cActionShowNavigationSpaces( *this ) );
	pActionShowNavigationSpacesSelected.TakeOver( new cActionShowNavigationSpacesSelected( *this ) );
	pActionNavTestLoad.TakeOver( new cActionNavTestLoad( *this ) );
	pActionNavTestSave.TakeOver( new cActionNavTestSave( *this ) );
	
	
	// register for updating
	AddUpdateAction( pActionFileNew );
	AddUpdateAction( pActionFileOpen );
	AddUpdateAction( pActionFileSave );
	AddUpdateAction( pActionFileSaveAs );
	
	AddUpdateAction( pActionEditUndo );
	AddUpdateAction( pActionEditRedo );
	
	AddUpdateAction( pActionEditCut );
	AddUpdateAction( pActionEditCopy );
	AddUpdateAction( pActionEditPaste );
	AddUpdateAction( pActionEditDuplicate );
	AddUpdateAction( pActionEditDelete );
	AddUpdateAction( pActionEditEModeObject );
	AddUpdateAction( pActionEditEModeDecal );
	AddUpdateAction( pActionEditEModeNavSpace );
	AddUpdateAction( pActionEditEModeObjectShape );
	AddUpdateAction( pActionEditSelectMode );
// 	AddUpdateAction( pActionEdit3DCursorMode );
	AddUpdateAction( pActionEditMoveMode );
	AddUpdateAction( pActionEditScaleMode );
	AddUpdateAction( pActionEditRotateMode );
	AddUpdateAction( pActionEditAddMode );
	AddUpdateAction( pActionEditHeightPaintMode );
	AddUpdateAction( pActionEditMaskPaintMode );
	AddUpdateAction( pActionEditVisPaintMode );
	AddUpdateAction( pActionEditModeNavSpaceEdit );
	AddUpdateAction( pActionEditLockAxisX );
	AddUpdateAction( pActionEditLockAxisY );
	AddUpdateAction( pActionEditLockAxisZ );
	AddUpdateAction( pActionEditUseLocal );
	AddUpdateAction( pActionEditSnapSnapPoints );
	AddUpdateAction( pActionEditRPCenterActive );
	AddUpdateAction( pActionEditRPCenterSelected );
	AddUpdateAction( pActionEditRPCenterIndividual );
	AddUpdateAction( pActionEditUse3DCursor );
	AddUpdateAction( pActionEditSelectNone );
	
	AddUpdateAction( pActionObjectLightToggle );
	AddUpdateAction( pActionObjectRotateL45 );
	AddUpdateAction( pActionObjectRotateL90 );
	AddUpdateAction( pActionObjectRotateR45 );
	AddUpdateAction( pActionObjectRotateR90 );
	AddUpdateAction( pActionObjectRotate180 );
	AddUpdateAction( pActionObjectDropToGround );
	AddUpdateAction( pActionObjectSnapToGrid );
	AddUpdateAction( pActionObjectCopyPositionX );
	AddUpdateAction( pActionObjectCopyPositionY );
	AddUpdateAction( pActionObjectCopyPositionZ );
	AddUpdateAction( pActionObjectCopyPositionXZ );
	AddUpdateAction( pActionObjectCopyPositionXYZ );
	AddUpdateAction( pActionObjectCopyRotationX );
	AddUpdateAction( pActionObjectCopyRotationY );
	AddUpdateAction( pActionObjectCopyRotationZ );
	AddUpdateAction( pActionObjectCopyRotationXYZ );
	AddUpdateAction( pActionObjectCopyScaleX );
	AddUpdateAction( pActionObjectCopyScaleY );
	AddUpdateAction( pActionObjectCopyScaleZ );
	AddUpdateAction( pActionObjectCopyScaleXYZ );
	AddUpdateAction( pActionObjectAttachTo );
	AddUpdateAction( pActionObjectDetach );
	AddUpdateAction( pActionObjectSelectAttached );
	AddUpdateAction( pActionObjectReassignIDs );
	AddUpdateAction( pActionObjectSubclassAsEclass );
	
	AddUpdateAction( pActionObjectShapeAddSphere );
	AddUpdateAction( pActionObjectShapeAddBox );
	AddUpdateAction( pActionObjectShapeAddCylinder );
	AddUpdateAction( pActionObjectShapeAddCapsule );
	AddUpdateAction( pActionObjectShapeDelete );
	
	AddUpdateAction( pActionDecalDelete );
	AddUpdateAction( pActionDecalRaiseTop );
	AddUpdateAction( pActionDecalRaiseOne );
	AddUpdateAction( pActionDecalLowerOne );
	AddUpdateAction( pActionDecalLowerBottom );
	
	AddUpdateAction( pActionFullBright );
	AddUpdateAction( pActionMuteSound );
	AddUpdateAction( pActionShowOcclusionMeshes );
	AddUpdateAction( pActionShowOcclusionMeshesSelected );
	AddUpdateAction( pActionShowNavigationSpaces );
	AddUpdateAction( pActionShowNavigationSpacesSelected );
	AddUpdateAction( pActionNavTestLoad );
	AddUpdateAction( pActionNavTestSave );
}

void meWindowMain::pCreateToolBarFile(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBFile.TakeOver( new igdeToolBar( GetEnvironment() ) );
	
	helper.ToolBarButton( pTBFile, pActionFileNew );
	helper.ToolBarButton( pTBFile, pActionFileOpen );
	helper.ToolBarButton( pTBFile, pActionFileSave );
	
	AddSharedToolBar( pTBFile );
}

void meWindowMain::pCreateToolBarEdit(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBEdit.TakeOver( new igdeToolBar( GetEnvironment() ) );
	
	helper.ToolBarButton( pTBEdit, pActionEditUndo );
	helper.ToolBarButton( pTBEdit, pActionEditRedo );
	
	helper.ToolBarSeparator( pTBEdit );
	helper.ToolBarButton( pTBEdit, pActionEditCut );
	helper.ToolBarButton( pTBEdit, pActionEditCopy );
	helper.ToolBarButton( pTBEdit, pActionEditPaste );
	
	helper.ToolBarSeparator( pTBEdit );
	helper.ToolBarButton( pTBEdit, pActionEditDuplicate );
	helper.ToolBarButton( pTBEdit, pActionEditDelete );
	
	helper.ToolBarSeparator( pTBEdit );
	helper.ToolBarToggleButton( pTBEdit, pActionEditEModeObject );
	helper.ToolBarToggleButton( pTBEdit, pActionEditEModeDecal );
	helper.ToolBarToggleButton( pTBEdit, pActionEditEModeNavSpace );
	helper.ToolBarToggleButton( pTBEdit, pActionEditEModeObjectShape );
	
	helper.ToolBarSeparator( pTBEdit );
	helper.ToolBarToggleButton( pTBEdit, pActionEditSelectMode );
	helper.ToolBarToggleButton( pTBEdit, pActionEditMoveMode );
	helper.ToolBarToggleButton( pTBEdit, pActionEditScaleMode );
	helper.ToolBarToggleButton( pTBEdit, pActionEditRotateMode );
	helper.ToolBarToggleButton( pTBEdit, pActionEditAddMode );
	helper.ToolBarToggleButton( pTBEdit, pActionEditHeightPaintMode );
	helper.ToolBarToggleButton( pTBEdit, pActionEditMaskPaintMode );
	helper.ToolBarToggleButton( pTBEdit, pActionEditVisPaintMode );
	helper.ToolBarToggleButton( pTBEdit, pActionEditModeNavSpaceEdit );
// 	helper.ToolBarToggleButton( pTBEdit, pActionEdit3DCursorMode );
	
	helper.ToolBarSeparator( pTBEdit );
	helper.ToolBarToggleButton( pTBEdit, pActionEditLockAxisX );
	helper.ToolBarToggleButton( pTBEdit, pActionEditLockAxisY );
	helper.ToolBarToggleButton( pTBEdit, pActionEditLockAxisZ );
	helper.ToolBarToggleButton( pTBEdit, pActionEditUseLocal );
	
	helper.ToolBarSeparator( pTBEdit );
	helper.ToolBarToggleButton( pTBEdit, pActionEditSnapSnapPoints );
	
	helper.ToolBarSeparator( pTBEdit );
	helper.ToolBarToggleButton( pTBEdit, pActionFullBright );
	helper.ToolBarToggleButton( pTBEdit, pActionMuteSound );
	
	AddSharedToolBar( pTBEdit );
}

void meWindowMain::pCreateToolBarObject(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBObject.TakeOver( new igdeToolBar( GetEnvironment() ) );
	
	helper.ToolBarSeparator( pTBObject );
	helper.ToolBarButton( pTBObject, pActionObjectRotateL45 );
	helper.ToolBarButton( pTBObject, pActionObjectRotateL90 );
	helper.ToolBarButton( pTBObject, pActionObjectRotateR45 );
	helper.ToolBarButton( pTBObject, pActionObjectRotateR90 );
	helper.ToolBarButton( pTBObject, pActionObjectRotate180 );
	
	helper.ToolBarSeparator( pTBObject );
	helper.ToolBarToggleButton( pTBObject, pActionObjectLightToggle );
	
	helper.ToolBarSeparator( pTBObject );
	helper.ToolBarButton( pTBObject, pActionObjectSnapToGrid );
	helper.ToolBarButton( pTBObject, pActionObjectDropToGround );
	
	AddSharedToolBar( pTBObject );
}

void meWindowMain::pCreateToolBarDecal(){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	pTBDecal.TakeOver( new igdeToolBar( GetEnvironment() ) );
	
	helper.ToolBarButton( pTBDecal, pActionDecalDelete );
	
	helper.ToolBarSeparator( pTBDecal );
	helper.ToolBarButton( pTBDecal, pActionDecalRaiseTop );
	helper.ToolBarButton( pTBDecal, pActionDecalRaiseOne );
	helper.ToolBarButton( pTBDecal, pActionDecalLowerOne );
	helper.ToolBarButton( pTBDecal, pActionDecalLowerBottom );
	
	AddSharedToolBar( pTBDecal );
}

void meWindowMain::pCreateMenu(){
	igdeEnvironment &env = GetEnvironment();
	igdeMenuCascadeReference cascade;
	
	cascade.TakeOver( new igdeMenuCascade( env, "File", deInputEvent::ekcF ) );
	pCreateMenuFile( cascade );
	AddSharedMenu( cascade );
	
	cascade.TakeOver( new igdeMenuCascade( env, "Edit", deInputEvent::ekcE ) );
	pCreateMenuEdit( cascade );
	AddSharedMenu( cascade );
	
	cascade.TakeOver( new igdeMenuCascade( env, "Object", deInputEvent::ekcO ) );
	pCreateMenuObject( cascade );
	AddSharedMenu( cascade );
	
	cascade.TakeOver( new igdeMenuCascade( env, "Decal", deInputEvent::ekcD ) );
	pCreateMenuDecal( cascade );
	AddSharedMenu( cascade );
	
	cascade.TakeOver( new igdeMenuCascade( env, "View", deInputEvent::ekcV ) );
	pCreateMenuView( cascade );
	AddSharedMenu( cascade );
}

void meWindowMain::pCreateMenuFile( igdeMenuCascade &menu ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( menu, pActionFileNew );
	helper.MenuCommand( menu, pActionFileOpen );
	helper.MenuRecentFiles( menu, GetRecentFiles() );
	helper.MenuCommand( menu, pActionFileSave );
	helper.MenuCommand( menu, pActionFileSaveAs );
}

void meWindowMain::pCreateMenuEdit( igdeMenuCascade &menu ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( menu, pActionEditUndo );
	helper.MenuCommand( menu, pActionEditRedo );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pActionEditCut );
	helper.MenuCommand( menu, pActionEditCopy );
	helper.MenuCommand( menu, pActionEditPaste );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pActionEditDuplicate );
	helper.MenuCommand( menu, pActionEditDelete );
	
	helper.MenuSeparator( menu );
	helper.MenuOption( menu, pActionEditEModeObject );
	helper.MenuOption( menu, pActionEditEModeDecal );
	helper.MenuOption( menu, pActionEditEModeNavSpace );
	helper.MenuOption( menu, pActionEditEModeObjectShape );
	
	helper.MenuSeparator( menu );
	helper.MenuOption( menu, pActionEditSelectMode );
	helper.MenuOption( menu, pActionEditMoveMode );
	helper.MenuOption( menu, pActionEditScaleMode );
	helper.MenuOption( menu, pActionEditRotateMode );
	helper.MenuOption( menu, pActionEditAddMode );
	helper.MenuOption( menu, pActionEditHeightPaintMode );
	helper.MenuOption( menu, pActionEditMaskPaintMode );
	helper.MenuOption( menu, pActionEditVisPaintMode );
// 	helper.MenuOption( menu, pActionEdit3DCursorMode );
	
	helper.MenuSeparator( menu );
	helper.MenuCheck( menu, pActionEditLockAxisX );
	helper.MenuCheck( menu, pActionEditLockAxisY );
	helper.MenuCheck( menu, pActionEditLockAxisZ );
	helper.MenuCheck( menu, pActionEditUseLocal );
	
	helper.MenuSeparator( menu );
	helper.MenuCheck( menu, pActionEditSnapSnapPoints );
	
	helper.MenuSeparator( menu );
	helper.MenuOption( menu, pActionEditRPCenterActive );
	helper.MenuOption( menu, pActionEditRPCenterSelected );
	helper.MenuOption( menu, pActionEditRPCenterIndividual );
	
	helper.MenuSeparator( menu );
	helper.MenuCheck( menu, pActionEditUse3DCursor );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pActionEditSelectNone );
}

void meWindowMain::pCreateMenuObject( igdeMenuCascade &menu ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pActionObjectDropToGround );
	helper.MenuCommand( menu, pActionObjectSnapToGrid );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pActionObjectAttachTo );
	helper.MenuCommand( menu, pActionObjectDetach );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pActionObjectSelectAttached );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pActionObjectReassignIDs );
	helper.MenuCommand( menu, pActionObjectSubclassAsEclass );
	
	helper.MenuSeparator( menu );
	igdeMenuCascadeReference active;
	active.TakeOver( new igdeMenuCascade( GetEnvironment(), "Active Object",
		NULL, "Active object", deInputEvent::ekcA ) );
		
		menu.AddChild( active );
		
		igdeMenuCascadeReference activeRotate;
		activeRotate.TakeOver( new igdeMenuCascade( GetEnvironment(), "Rotate",
			NULL, "Rotate active object", deInputEvent::ekcR ) );
			
			active->AddChild( activeRotate );
			helper.MenuCommand( activeRotate, pActionObjectRotateL45 );
			helper.MenuCommand( activeRotate, pActionObjectRotateL90 );
			helper.MenuCommand( activeRotate, pActionObjectRotateR45 );
			helper.MenuCommand( activeRotate, pActionObjectRotateR90 );
			helper.MenuCommand( activeRotate, pActionObjectRotate180 );
		
		igdeMenuCascadeReference activeCopySelected;
		activeCopySelected.TakeOver( new igdeMenuCascade( GetEnvironment(),
			"Copy To Selected", nullptr, "Copy To Selected" ) );
			
			active->AddChild( activeCopySelected );
			helper.MenuCommand( activeCopySelected, pActionObjectCopyPositionX );
			helper.MenuCommand( activeCopySelected, pActionObjectCopyPositionY );
			helper.MenuCommand( activeCopySelected, pActionObjectCopyPositionZ );
			helper.MenuCommand( activeCopySelected, pActionObjectCopyPositionXZ );
			helper.MenuCommand( activeCopySelected, pActionObjectCopyPositionXYZ );
			
			helper.MenuSeparator( activeCopySelected );
			helper.MenuCommand( activeCopySelected, pActionObjectCopyRotationX );
			helper.MenuCommand( activeCopySelected, pActionObjectCopyRotationY );
			helper.MenuCommand( activeCopySelected, pActionObjectCopyRotationZ );
			helper.MenuCommand( activeCopySelected, pActionObjectCopyRotationXYZ );
			
			helper.MenuSeparator( activeCopySelected );
			helper.MenuCommand( activeCopySelected, pActionObjectCopyScaleX );
			helper.MenuCommand( activeCopySelected, pActionObjectCopyScaleY );
			helper.MenuCommand( activeCopySelected, pActionObjectCopyScaleZ );
			helper.MenuCommand( activeCopySelected, pActionObjectCopyScaleXYZ );
			
		igdeMenuCascadeReference activeLight;
		activeLight.TakeOver( new igdeMenuCascade( GetEnvironment(), "Light",
			NULL, "Active object light", deInputEvent::ekcL ) );
			
			active->AddChild( activeLight );
			helper.MenuCommand( activeLight, pActionObjectLightToggle );
			
		igdeMenuCascadeReference activeShapes;
		activeShapes.TakeOver( new igdeMenuCascade( GetEnvironment(), "Property Shapes",
			NULL, "Active object property shapes", deInputEvent::ekcS ) );
			
			active->AddChild( activeShapes );
			helper.MenuCommand( activeShapes, pActionObjectShapeAddSphere );
			helper.MenuCommand( activeShapes, pActionObjectShapeAddBox );
			helper.MenuCommand( activeShapes, pActionObjectShapeAddCylinder );
			helper.MenuCommand( activeShapes, pActionObjectShapeAddCapsule );
			
			helper.MenuSeparator( activeShapes );
			helper.MenuCommand( activeShapes, pActionObjectShapeDelete );
}

void meWindowMain::pCreateMenuDecal( igdeMenuCascade &menu ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCommand( menu, pActionDecalDelete );
	
	helper.MenuCommand( menu, pActionDecalRaiseTop );
	helper.MenuCommand( menu, pActionDecalRaiseOne );
	helper.MenuCommand( menu, pActionDecalLowerOne );
	helper.MenuCommand( menu, pActionDecalLowerBottom );
}

void meWindowMain::pCreateMenuView( igdeMenuCascade &menu ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	helper.MenuCheck( menu, pActionFullBright );
	helper.MenuCheck( menu, pActionMuteSound );
	
	helper.MenuSeparator( menu );
	helper.MenuCheck( menu, pActionShowOcclusionMeshes );
	helper.MenuCheck( menu, pActionShowOcclusionMeshesSelected );
	helper.MenuCheck( menu, pActionShowNavigationSpaces );
	helper.MenuCheck( menu, pActionShowNavigationSpacesSelected );
	
	helper.MenuSeparator( menu );
	helper.MenuCommand( menu, pActionNavTestLoad );
	helper.MenuCommand( menu, pActionNavTestSave );
}



void meWindowMain::pUpdateLoading(){
	if( pLoadFilename.IsEmpty() ){
		return;
	}
	
	try{
		if( ! pLoadWorld ){
			GetEditorModule().LogInfoFormat( "Loading world %s", pLoadFilename.GetString() );
			pLoadWorld = pLoadSaveSystem->LoadWorld( pLoadFilename, GetGameDefinition(), &pLoadTask );
			
		}else if( pLoadTask->Step() ){
			SetProgress( pLoadTask->GetProgress() );
			SetProgressText( pLoadTask->GetMessage() );
			
		}else{
			delete pLoadTask;
			pLoadTask = NULL;
			
			pLoadWorld->SetFilePath( pLoadFilename );
			pLoadWorld->SetSaved( true );
			pLoadWorld->SetChanged( false );
			pLoadWorld->SetDepChanged( false );
			pLoadWorld->CheckDepChanged();
			pLoadWorld->CheckChanged();
			pLoadFilename = "";
			
			pLoadWorld->ClearScalingOfNonScaledElements();
			
			SetWorld( pLoadWorld );
			pLoadWorld->FreeReference();
			pLoadWorld = NULL;
			
			SetProgressVisible( false );
			SetProgressText( "" );
			
			pUse3DCursor = true;
			p3DCursor.SetZero();
			
			// update delegates if engine is running
			if( GetEngineController().GetRunning() ){
				pWorld->InitDelegates();
			}
			
			pWorld->ForceUpdateVegetation( false );
			pWindowProperties->OnWorldPathChanged();
		}
		
	}catch( const deException &e ){
		if( pLoadTask ){
			delete pLoadTask;
			pLoadTask = NULL;
		}
		if( pLoadWorld ){
			pLoadWorld->FreeReference();
			pLoadWorld = NULL;
		}
		pLoadFilename = "";
		
		DisplayException( e );
		SetProgressVisible( false );
		SetProgressText( "" );
		
		CreateNewWorld();
	}
}
