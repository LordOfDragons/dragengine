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

#include "meWPView.h"
#include "meWPViewListener.h"
#include "meWindowProperties.h"
#include "../meWindowMain.h"
#include "../../configuration/meConfiguration.h"
#include "../../world/meCamera.h"
#include "../../world/meWorld.h"
#include "../../world/meWorldGuiParameters.h"
#include "../../world/object/meObject.h"
#include "../../world/terrain/meTerrainGroup.h"
#include "../../undosys/properties/view/meUViewSetLimitBoxExtends.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/class/igdeGDClassManager.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/composed/igdeToggleTags.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/properties/igdeWPSky.h>
#include <deigde/gui/properties/igdeWPCamera.h>
#include <deigde/gui/properties/igdeWPWObject.h>
#include <deigde/gui/properties/igdeWPTriggerTable.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Actions
///////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	meWPView &pPanel;
	
public:
	cBaseTextFieldListener( meWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		meWorld * const world = pPanel.GetWorld();
		if( world ){
			OnChanged( textField, *world );
		}
	}
	
	virtual void OnChanged( igdeTextField *textField, meWorld &world ) = 0;
};

class cBaseAction : public igdeAction{
protected:
	meWPView &pPanel;
	
public:
	cBaseAction(meWPView &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	virtual void OnAction(){
		meWorld * const world = pPanel.GetWorld();
		if( world ){
			OnAction( *world );
		}
	}
	
	virtual void OnAction( meWorld &world ) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	meWPView &pPanel;
	
public:
	cBaseComboBoxListener( meWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		meWorld * const world = pPanel.GetWorld();
		if( world ){
			OnTextChanged( comboBox, *world );
		}
	}
	
	virtual void OnTextChanged( igdeComboBox *comboBox, meWorld &world ) = 0;
};


class cActionMoveSnap : public cBaseAction{
public:
	cActionMoveSnap( meWPView &panel ) :
	cBaseAction(panel, "Snap Move", nullptr, "Snap moving distance" ){ }
	
	virtual void OnAction( meWorld &world ){
		meConfiguration &configuration = pPanel.GetWindowProperties().GetWindowMain().GetConfiguration();
		configuration.SetMoveSnap( ! configuration.GetMoveSnap() );
		world.NotifyEditingChanged();
	}
};

class cTextMoveStep : public cBaseTextFieldListener{
public:
	cTextMoveStep( meWPView &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, meWorld &world ){
		meConfiguration &configuration = pPanel.GetWindowProperties().GetWindowMain().GetConfiguration();
		if( fabsf( textField->GetFloat() - configuration.GetMoveStep() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		configuration.SetMoveStep( textField->GetFloat() );
		world.NotifyEditingChanged();
	}
};

class cActionRotateSnap : public cBaseAction{
public:
	cActionRotateSnap( meWPView &panel ) :
	cBaseAction(panel, "Snap Rotate", nullptr, "Snap rotation angle"){}
	
	virtual void OnAction( meWorld &world ){
		meConfiguration &configuration = pPanel.GetWindowProperties().GetWindowMain().GetConfiguration();
		configuration.SetRotateSnap( ! configuration.GetRotateSnap() );
		world.NotifyEditingChanged();
	}
};

class cTextRotateStep : public cBaseTextFieldListener{
public:
	cTextRotateStep( meWPView &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, meWorld &world ){
		meConfiguration &configuration = pPanel.GetWindowProperties().GetWindowMain().GetConfiguration();
		if( fabsf( textField->GetFloat() - configuration.GetRotateStep() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		configuration.SetRotateStep( textField->GetFloat() );
		world.NotifyEditingChanged();
	}
};

class cActionScaleSnap : public cBaseAction{
public:
	cActionScaleSnap( meWPView &panel ) :
	cBaseAction(panel, "Snap Scale", nullptr, "Snap scaling factor"){ }
	
	virtual void OnAction( meWorld &world ){
		meConfiguration &configuration = pPanel.GetWindowProperties().GetWindowMain().GetConfiguration();
		configuration.SetScaleSnap( ! configuration.GetScaleSnap() );
		world.NotifyEditingChanged();
	}
};

class cTextScaleStep : public cBaseTextFieldListener{
public:
	cTextScaleStep( meWPView &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, meWorld &world ){
		meConfiguration &configuration = pPanel.GetWindowProperties().GetWindowMain().GetConfiguration();
		if( fabsf( textField->GetFloat() - configuration.GetScaleStep() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		configuration.SetScaleStep( textField->GetFloat() );
		world.NotifyEditingChanged();
	}
};

class cTextSensitivity : public cBaseTextFieldListener{
public:
	cTextSensitivity( meWPView &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, meWorld &world ){
		meConfiguration &configuration = pPanel.GetWindowProperties().GetWindowMain().GetConfiguration();
		if( fabsf( textField->GetFloat() - configuration.GetSensitivity() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		configuration.SetSensitivity( textField->GetFloat() );
		world.NotifyEditingChanged();
	}
};

class cActionSetSensitivity : public cBaseAction{
	float pSensitivity;
	
public:
	cActionSetSensitivity(meWPView &panel, float sensitivity, const char *text) :
	cBaseAction(panel, text, nullptr, "Set sensitivity"), pSensitivity(sensitivity){}
	
	void OnAction(meWorld &world) override{
		meConfiguration &conf = pPanel.GetWindowProperties().GetWindowMain().GetConfiguration();
		if(fabsf(pSensitivity - conf.GetSensitivity()) < FLOAT_SAFE_EPSILON){
			return;
		}
		conf.SetSensitivity(pSensitivity);
		world.NotifyEditingChanged();
	}
};

class cActionMenuSensitivity : public igdeActionContextMenu{
	meWPView &pPanel;
	
public:
	cActionMenuSensitivity(meWPView &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "Sensitivity menu"),
	pPanel(panel){}
	
	void AddContextMenuEntries(igdeMenuCascade &contextMenu) override{
		meWorld * const world = pPanel.GetWorld();
		if(!world){
			return;
		}
		
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, igdeAction::Ref::New(new cActionSetSensitivity(pPanel, 1.0f, "Set to 1")));
		helper.MenuCommand(contextMenu, igdeAction::Ref::New(new cActionSetSensitivity(pPanel, 0.1f, "Set to 0.1")));
		helper.MenuCommand(contextMenu, igdeAction::Ref::New(new cActionSetSensitivity(pPanel, 0.01f, "Set to 0.01")));
		helper.MenuCommand(contextMenu, igdeAction::Ref::New(new cActionSetSensitivity(pPanel, 10.0f, "Set to 10")));
	}
};

class cTextRectSelDist : public cBaseTextFieldListener{
public:
	cTextRectSelDist( meWPView &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, meWorld &world ){
		if( fabsf( textField->GetFloat() - world.GetGuiParameters().GetRectSelDistance() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		world.GetGuiParameters().SetRectSelDistance( textField->GetFloat() );
	}
};

class cTextRectSelDistStep : public cBaseTextFieldListener{
public:
	cTextRectSelDistStep( meWPView &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, meWorld &world ){
		if( fabsf( textField->GetFloat() - world.GetGuiParameters().GetRectSelDistanceStep() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		world.GetGuiParameters().SetRectSelDistanceStep( textField->GetFloat() );
	}
};

class cActionAutoUpdate : public cBaseAction{
public:
	cActionAutoUpdate( meWPView &panel ) :
	cBaseAction(panel, "Enable Auto Updating", nullptr, "Automatically update the screen"){ }
	
	virtual void OnAction( meWorld &world ){
		meConfiguration &configuration = pPanel.GetWindowProperties().GetWindowMain().GetConfiguration();
		configuration.SetAutoUpdate( ! configuration.GetAutoUpdate() );
		world.NotifyEditingChanged();
	}
};


class cActionClassHideTags : public cBaseAction {
	igdeToggleTagsReference &pToggleTags;
	
public:
	cActionClassHideTags( meWPView &panel, igdeToggleTagsReference &toggleTags ) :
	cBaseAction(panel, "", nullptr, "Hide classes matching one or more tags" ),
	pToggleTags( toggleTags ){}
	
	virtual void OnAction( meWorld &world ){
		world.GetGuiParameters().GetTagsHideClass() = pToggleTags->GetEnabledTags();
		world.NotifyClassHideTagsChanged();
	}
};

class cActionClassPartialHideTags : public cBaseAction {
	igdeToggleTagsReference &pToggleTags;
	
public:
	cActionClassPartialHideTags( meWPView &panel, igdeToggleTagsReference &toggleTags ) :
	cBaseAction(panel, "", nullptr, "Partially hide classes matching one or more tags"),
	pToggleTags( toggleTags ){}
	
	virtual void OnAction( meWorld &world ){
		world.GetGuiParameters().GetTagsPartialHideClass() = pToggleTags->GetEnabledTags();
		world.NotifyClassHideTagsChanged();
	}
};


class cActionCameraFreeRoaming : public cBaseAction{
public:
	cActionCameraFreeRoaming( meWPView &panel ) :
	cBaseAction(panel, "Free Roaming", nullptr, "Select free roaming camera"){}
	
	virtual void OnAction( meWorld &world ){
		world.SetActiveCamera( world.GetFreeRoamingCamera() );
	}
};

class cActionCameraPlayer : public cBaseAction{
public:
	cActionCameraPlayer( meWPView &panel ) :
	cBaseAction(panel, "Player", nullptr, "Select player camera"){ }
	
	virtual void OnAction( meWorld &world ){
		world.SetActiveCamera( world.GetPlayerCamera() );
	}
};

class cActionCameraObject : public cBaseAction{
public:
	cActionCameraObject( meWPView &panel ) :
	cBaseAction(panel, "Object", nullptr, "Select object camera"){}
	
	virtual void OnAction( meWorld &world ){
		meCamera * const camera = pPanel.GetSelectedCameraObject();
		if( camera ){
			world.SetActiveCamera( camera );
		}
	}
};

class cComboCamera: public igdeComboBoxListener{
	meWPView &pPanel;
public:
	cComboCamera( meWPView &panel ) : igdeComboBoxListener(), pPanel( panel ){}
	
	void OnTextChanged( igdeComboBox *comboBox ) override{
		meWorld * const world = pPanel.GetWorld();
		if( ! world ){
			return;
		}
		
		meCamera * const camera = ( meCamera* )comboBox->GetSelectedItemData();
		if( camera == world->GetActiveCamera()
		|| camera == world->GetFreeRoamingCamera()
		|| camera == world->GetPlayerCamera() ){
			return;
		}
		
		world->SetActiveCamera( camera );
	}
};


class cActionEnableAuralization : public cBaseAction{
public:
	cActionEnableAuralization( meWPView &panel ) :
	cBaseAction(panel, "Enable Auralization", nullptr,
		"Enable auralization if supported by audio module"){ }
	
	virtual void OnAction( meWorld &world ){
		meConfiguration &configuration = pPanel.GetWindowProperties().GetWindowMain().GetConfiguration();
		configuration.SetEnableAuralization( ! configuration.GetEnableAuralization() );
		world.NotifyEditingChanged();
	}
};


class cActionSkyChanged : public cBaseAction{
public:
	cActionSkyChanged( meWPView &panel ) : cBaseAction(panel, "", nullptr, ""){ }
	
	virtual void OnAction( meWorld &world ){
		world.NotifySkyChanged();
	}
};

class cEditBgObject : public cBaseAction{
public:
	cEditBgObject( meWPView &panel ) : cBaseAction(panel, "", nullptr, ""){ }
	
	virtual void OnAction( meWorld &world ){
		world.NotifyBgObjectChanged();
	}
};

class cEditLimitBoxMinExtend : public igdeEditVectorListener{
	meWPView &pPanel;
public:
	cEditLimitBoxMinExtend(meWPView &panel) : pPanel(panel){}
	
	void OnVectorChanged(igdeEditVector *editVector) override{
		meWorld * const world = pPanel.GetWorld();
		const decVector &vector = editVector->GetVector();
		if(world && !world->GetLimitBoxMinExtend().IsEqualTo(vector)){
			world->GetUndoSystem()->Add(igdeUndo::Ref::New(
				new meUViewSetLimitBoxExtends(world, vector, world->GetLimitBoxMaxExtend())));
		}
	}
};

class cEditLimitBoxMaxExtend : public igdeEditVectorListener{
	meWPView &pPanel;
public:
	cEditLimitBoxMaxExtend(meWPView &panel) : pPanel(panel){}
	
	void OnVectorChanged(igdeEditVector *editVector) override{
		meWorld * const world = pPanel.GetWorld();
		const decVector &vector = editVector->GetVector();
		if(world && !world->GetLimitBoxMaxExtend().IsEqualTo(vector)){
			world->GetUndoSystem()->Add(igdeUndo::Ref::New(
				new meUViewSetLimitBoxExtends(world, world->GetLimitBoxMinExtend(), vector)));
		}
	}
};

class cActionLimitBoxClear : public cBaseAction{
public:
	cActionLimitBoxClear(meWPView &panel) :
	cBaseAction(panel, "Clear", nullptr, "Clear limit box"){}
	
	void OnAction(meWorld &world) override{
		if(!world.GetLimitBoxMinExtend().IsZero() || !world.GetLimitBoxMaxExtend().IsZero()){
			world.GetUndoSystem()->Add(igdeUndo::Ref::New(
				new meUViewSetLimitBoxExtends(&world, decVector(), decVector())));
		}
	}
};

class cActionLimitBoxDefault : public cBaseAction{
public:
	cActionLimitBoxDefault(meWPView &panel) :
	cBaseAction(panel, "Default", nullptr, "Set limit box to 1m cube"){}
	
	void OnAction(meWorld &world) override{
		const decVector minExtend(-0.5f, -0.5f, -0.5f), maxExtend(0.5f, 0.5f, 0.5f);
		if(!world.GetLimitBoxMinExtend().IsEqualTo(minExtend)
		|| !world.GetLimitBoxMaxExtend().IsEqualTo(maxExtend)){
			world.GetUndoSystem()->Add(igdeUndo::Ref::New(
				new meUViewSetLimitBoxExtends(&world, minExtend, maxExtend)));
		}
	}
};

class cActionLimitBoxDefaultGround : public cBaseAction{
public:
	cActionLimitBoxDefaultGround(meWPView &panel) :
	cBaseAction(panel, "Default Ground", nullptr, "Set limit box to 1m cube sitting on ground"){}
	
	void OnAction(meWorld &world) override{
		const decVector minExtend(-0.5f, 0.0f, -0.5f), maxExtend(0.5f, 1.0f, 0.5f);
		if(!world.GetLimitBoxMinExtend().IsEqualTo(minExtend)
		|| !world.GetLimitBoxMaxExtend().IsEqualTo(maxExtend)){
			world.GetUndoSystem()->Add(igdeUndo::Ref::New(
				new meUViewSetLimitBoxExtends(&world, minExtend, maxExtend)));
		}
	}
};

class cActionMenuLimitBox : public igdeActionContextMenu{
	meWPView &pPanel;
	
public:
	cActionMenuLimitBox(meWPView &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "Limit box menu"),
	pPanel(panel){}
	
	void AddContextMenuEntries(igdeMenuCascade &contextMenu) override{
		meWorld * const world = pPanel.GetWorld();
		if(!world){
			return;
		}
		
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, new cActionLimitBoxClear(pPanel), true);
		helper.Separator(contextMenu);
		helper.MenuCommand(contextMenu, new cActionLimitBoxDefault(pPanel), true);
		helper.MenuCommand(contextMenu, new cActionLimitBoxDefaultGround(pPanel), true);
	}
};

class cActionCameraChanged : public cBaseAction{
public:
	cActionCameraChanged( meWPView &panel ) : cBaseAction(panel, "", nullptr, ""){}
	
	virtual void OnAction( meWorld &world ){
		meCamera * const camera = world.GetActiveCamera();
		if( camera ){
			pPanel.GetWindowProperties().GetWindowMain().GetConfiguration().SetEnableGI( camera->GetEnableGI() );
			world.NotifyCameraChanged( camera );
		}
	}
};

class cActionTriggerTable : public cBaseAction{
public:
	cActionTriggerTable( meWPView &panel ) : cBaseAction(panel, "", nullptr, "Trigger table"){}
	
	virtual void OnAction( meWorld &world ){
		world.NotifyTriggerTableChanged();
	}
};

}



// Class meWPView
///////////////////

// Constructor, destructor
////////////////////////////

meWPView::meWPView( meWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pListener( nullptr ),
pWorld( nullptr ),
pPreventUpdateCamera(false)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference content, groupBox, form, formLine;
	igdeActionContextMenu::Ref actionMenu;
	
	pListener = new meWPViewListener( *this );
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	
	// grid snapping and alignment
	helper.GroupBox( content, groupBox, "Grid/Snapping:" );
	
	helper.CheckBoxOnly( groupBox, pChkMoveSnap, new cActionMoveSnap( *this ) );
	helper.EditFloat( groupBox, "Snap moving distance", pEditMoveStep, new cTextMoveStep( *this ) );
	
	helper.CheckBoxOnly( groupBox, pChkRotSnap, new cActionRotateSnap( *this ) );
	helper.EditFloat( groupBox, "Snap rotation angle", pEditRotStep, new cTextRotateStep( *this ) );
	
	helper.CheckBoxOnly( groupBox, pChkScaleSnap, new cActionScaleSnap( *this ) );
	helper.EditFloat( groupBox, "Snap scaling factor", pEditScaleStep, new cTextScaleStep( *this ) );
	
	helper.FormLineStretchFirst(groupBox, "Sensitivity:", "Sensitivity of mouse input", formLine);
	helper.EditFloat(formLine, "Sensitivity of mouse input", pEditSensitivity, new cTextSensitivity(*this));
	actionMenu.TakeOver(new cActionMenuSensitivity(*this));
	helper.Button(formLine, pBtnSensitivity, actionMenu.Pointer());
	actionMenu->SetWidget(pBtnSensitivity);
	
	
	// selection
	helper.GroupBox( content, groupBox, "Selection:" );
	
	helper.EditFloat( groupBox, "Distance:", "Distance for rectangular selection",
		pEditRectSelDist, new cTextRectSelDist( *this ) );
	helper.EditFloat( groupBox, "Distance Step:",
		"Mouse wheel adjustment step for distance for rectangular selection",
		pEditRectSelDistStep, new cTextRectSelDistStep( *this ) );
	
	
	// auto updating
	helper.GroupBox( content, groupBox, "Auto Updating:" );
	helper.CheckBox( groupBox, pChkAutoUpdate, new cActionAutoUpdate( *this ) );
	
	
	// class hiding
	helper.GroupBoxFlow( content, groupBox, "Class Hide Tags:", false, true );
	helper.ToggleTags( groupBox, pEditClassHideTags,
		new cActionClassHideTags( *this, pEditClassHideTags ), true );
	
	helper.GroupBoxFlow( content, groupBox, "Class Partial Hide Tags:", false, true );
	helper.ToggleTags( groupBox, pEditClassPartialHideTags,
		new cActionClassPartialHideTags( *this, pEditClassPartialHideTags ), true );
	
	
	// camera
	helper.GroupBoxFlow( content, groupBox, "Camera:" );
	
	form.TakeOver( new igdeContainerForm( env ) );
	groupBox->AddChild( form );
	
	helper.EditString( form, "Active:", "Active camera", pEditActiveCamera, nullptr );
	pEditActiveCamera->SetEditable( false );
	
	helper.FormLine( form, "", "", formLine );
	pActionCameraFreeRoaming.TakeOver( new cActionCameraFreeRoaming( *this ) );
	helper.Button( formLine, pActionCameraFreeRoaming );
	pActionCameraPlayer.TakeOver( new cActionCameraPlayer( *this ) );
	helper.Button( formLine, pActionCameraPlayer );
	pActionCameraObject.TakeOver( new cActionCameraObject( *this ) );
	helper.Button( formLine, pActionCameraObject );
	
	helper.ComboBox( form, "Object Camera:", "Object camera", pCBCameraObjects, nullptr );
	pCBCameraObjects->SetDefaultSorter();
	
	helper.WPCamera( groupBox, pWPCamera, new cActionCameraChanged( *this ),
		"Camera Parameters:", false, false, true );
	
	
	// microphone
	helper.GroupBoxFlow( content, groupBox, "Microphone:" );
	helper.CheckBox( groupBox, pChkEnableAuralization, new cActionEnableAuralization( *this ) );
	
	
	// property panels
	helper.WPSky( content, pWPSky, new cActionSkyChanged( *this ), "Sky:", false, false, true );
	helper.WPWObject(content, pWPBgObject, new cEditBgObject(*this), "Background Object:", false, true, true);
	helper.WPTriggerTable( content, pWPTriggerTable, new cActionTriggerTable( *this ),
		"Trigger Table:", false, true, true );
	
	// limit box
	helper.GroupBox(content, groupBox, "Limit Box:");
	
	helper.FormLineStretchFirst(groupBox, "Minimum Extend:", "Minimum extend of limit box", formLine);
	helper.EditVector(formLine, "Minimum extend of limit box", pEditLimitBoxMinExtend,
		new cEditLimitBoxMinExtend(*this));
	actionMenu.TakeOver(new cActionMenuLimitBox(*this));
	helper.Button(formLine, pBtnLimitBoxMenu, actionMenu.Pointer());
	actionMenu->SetWidget(pBtnLimitBoxMenu);
	
	helper.EditVector(groupBox, "Maximum Extend:", "Maximum extend of limit box",
		pEditLimitBoxMaxExtend, new cEditLimitBoxMaxExtend(*this));
}

meWPView::~meWPView(){
	SetWorld( nullptr );
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void meWPView::SetWorld( meWorld *world ){
	if( world == pWorld ){
		return;
	}
	
	pWPSky->SetSky( nullptr );
	pWPBgObject->SetObject(nullptr);
	pWPBgObject->SetUndoSystem(nullptr);
	pWPCamera->SetCamera( nullptr );
	pWPTriggerTable->SetTriggerTargetList( nullptr );
	pEditClassHideTags->SetTags( decStringSet() );
	pEditClassPartialHideTags->Clear();
	
	if( pWorld ){
		pWorld->RemoveNotifier( pListener );
		pWorld->FreeReference();
	}
	
	pWorld = world;
	
	if( world ){
		world->AddNotifier( pListener );
		world->AddReference();
		
		pWPSky->SetSky( world->GetSky() );
		pWPBgObject->SetObject(world->GetBgObject());
		pWPBgObject->SetUndoSystem(world->GetUndoSystem());
		pWPTriggerTable->SetTriggerTargetList( &world->GetTriggerTable() );
		
		if( world->GetGameDefinition() ){
			const igdeGDClassManager &gdClasses = *pWorld->GetGameDefinition()->GetClassManager();
			pEditClassHideTags->SetTags( gdClasses.GetHideTags() );
			pEditClassPartialHideTags->SetTags( gdClasses.GetPartialHideTags() );
		}
	}
	
	UpdateView();
	UpdateCameraList();
	UpdateCamera();
	UpdateSky();
	UpdateBgObject();
	UpdateLimitBox();
	UpdateClassHideTagLists();
	UpdateTriggerTable();
}



meCamera *meWPView::GetSelectedCameraObject() const{
	return pCBCameraObjects->GetSelectedItem() ?
		( meCamera* )pCBCameraObjects->GetSelectedItem()->GetData() : nullptr;
}

void meWPView::UpdateView(){
	const meConfiguration &configuration = pWindowProperties.GetWindowMain().GetConfiguration();
	
	pChkMoveSnap->SetChecked( configuration.GetMoveSnap() );
	pEditMoveStep->SetFloat( configuration.GetMoveStep() );
	pChkRotSnap->SetChecked( configuration.GetRotateSnap() );
	pEditRotStep->SetFloat( configuration.GetRotateStep() );
	pChkScaleSnap->SetChecked( configuration.GetScaleSnap() );
	pEditScaleStep->SetFloat( configuration.GetScaleStep() );
	pEditSensitivity->SetFloat( configuration.GetSensitivity() );
	pChkAutoUpdate->SetChecked( configuration.GetAutoUpdate() );
	pChkEnableAuralization->SetChecked( configuration.GetEnableAuralization() );
	
	if( pWorld ){
		const meWorldGuiParameters &guiParams = pWorld->GetGuiParameters();
		pEditRectSelDist->SetFloat( guiParams.GetRectSelDistance() );
		pEditRectSelDistStep->SetFloat( guiParams.GetRectSelDistanceStep() );
		
	}else{
		pEditRectSelDist->ClearText();
		pEditRectSelDistStep->ClearText();
	}
	
	const bool enable = pWorld;
	pEditRectSelDist->SetEnabled( enable );
	pEditRectSelDistStep->SetEnabled( enable );
}

void meWPView::UpdateCameraList(){
	meCamera * const selectedCamera = ( meCamera* )pCBCameraObjects->GetSelectedItemData();
	
	pCBCameraObjects->RemoveAllItems();
	
	if( pWorld ){
		const meObjectList &objects = pWorld->GetObjects();
		const int count = objects.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			meCamera * const camera = objects.GetAt( i )->GetCamera();
			if( camera ){
				pCBCameraObjects->AddItem( camera->GetName(), nullptr, camera );
			}
		}
	}
	
	pCBCameraObjects->SortItems();
	
	pCBCameraObjects->SetSelectionWithData( selectedCamera );
	if( ! pCBCameraObjects->GetSelectedItem() && pCBCameraObjects->GetItemCount() > 0 ){
		pCBCameraObjects->SetSelection( 0 );
	}
	
	UpdateCamera();
}

void meWPView::UpdateCamera(){
	if(pPreventUpdateCamera){
		return;
	}
	
	pPreventUpdateCamera = true;
	try{
		meCamera * const camera = pWorld ? pWorld->GetActiveCamera() : nullptr;
		
		if( camera ){
			pEditActiveCamera->SetText( camera->GetName() );
			
		}else{
			pEditActiveCamera->ClearText();
		}
		
		pWPCamera->SetCamera( camera );
		pWPCamera->UpdateCamera();
		pWPCamera->UpdateViewDirection();
		
	}catch(const deException &){
		pPreventUpdateCamera = false;
		throw;
	}
	pPreventUpdateCamera = false;
}

void meWPView::UpdateSky(){
	pWPSky->UpdateSky();
}

void meWPView::UpdateBgObject(){
	pWPBgObject->UpdateObject();
}

void meWPView::UpdateLimitBox(){
	if(pWorld){
		pEditLimitBoxMinExtend->SetVector(pWorld->GetLimitBoxMinExtend());
		pEditLimitBoxMaxExtend->SetVector(pWorld->GetLimitBoxMaxExtend());
		
	}else{
		pEditLimitBoxMinExtend->SetVector(decVector());
		pEditLimitBoxMaxExtend->SetVector(decVector());
	}
}

void meWPView::UpdateClassHideTagLists(){
	if( pWorld ){
		pEditClassHideTags->EnableTags( pWorld->GetGuiParameters().GetTagsHideClass() );
		pEditClassPartialHideTags->EnableTags( pWorld->GetGuiParameters().GetTagsPartialHideClass() );
		
	}else{
		pEditClassHideTags->EnableTags( decStringSet() );
		pEditClassPartialHideTags->EnableTags( decStringSet() );
	}
}

void meWPView::UpdateTriggerTable(){
	pWPTriggerTable->UpdateTable();
}

void meWPView::OnGameDefinitionChanged(){
	UpdateSky();
	UpdateBgObject();
	UpdateClassHideTagLists();
}
