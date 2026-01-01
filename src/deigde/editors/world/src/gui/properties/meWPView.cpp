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
#include "../../undosys/properties/view/meUViewSetLimitBoxExtends.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/class/igdeGDClassManager.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/composed/igdeToggleTags.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
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
	cBaseTextFieldListener(meWPView &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		meWorld * const world = pPanel.GetWorld();
		if(world){
			OnChanged(textField, *world);
		}
	}
	
	virtual void OnChanged(igdeTextField *textField, meWorld &world) = 0;
};

class cBaseAction : public igdeAction{
protected:
	meWPView &pPanel;
	
public:
	cBaseAction(meWPView &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		meWorld * const world = pPanel.GetWorld();
		if(world){
			OnAction(*world);
		}
	}
	
	virtual void OnAction(meWorld &world) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	meWPView &pPanel;
	
public:
	cBaseComboBoxListener(meWPView &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		meWorld * const world = pPanel.GetWorld();
		if(world){
			OnTextChanged(comboBox, *world);
		}
	}
	
	virtual void OnTextChanged(igdeComboBox *comboBox, meWorld &world) = 0;
};


class cActionMoveSnap : public cBaseAction{
public:
	typedef deTObjectReference<cActionMoveSnap> Ref;
	cActionMoveSnap(meWPView &panel) :
	cBaseAction(panel, "Snap Move", nullptr, "Snap moving distance"){}
	
	void OnAction(meWorld &world) override{
		meConfiguration &configuration = pPanel.GetWindowProperties().GetWindowMain().GetConfiguration();
		configuration.SetMoveSnap(!configuration.GetMoveSnap());
		world.NotifyEditingChanged();
	}
};

class cTextMoveStep : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextMoveStep> Ref;
	cTextMoveStep(meWPView &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField *textField, meWorld &world){
		meConfiguration &configuration = pPanel.GetWindowProperties().GetWindowMain().GetConfiguration();
		if(fabsf(textField->GetFloat() - configuration.GetMoveStep()) < FLOAT_SAFE_EPSILON){
			return;
		}
		configuration.SetMoveStep(textField->GetFloat());
		world.NotifyEditingChanged();
	}
};

class cActionRotateSnap : public cBaseAction{
public:
	typedef deTObjectReference<cActionRotateSnap> Ref;
	cActionRotateSnap(meWPView &panel) :
	cBaseAction(panel, "Snap Rotate", nullptr, "Snap rotation angle"){}
	
	void OnAction(meWorld &world) override{
		meConfiguration &configuration = pPanel.GetWindowProperties().GetWindowMain().GetConfiguration();
		configuration.SetRotateSnap(!configuration.GetRotateSnap());
		world.NotifyEditingChanged();
	}
};

class cTextRotateStep : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextRotateStep> Ref;
	cTextRotateStep(meWPView &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField *textField, meWorld &world){
		meConfiguration &configuration = pPanel.GetWindowProperties().GetWindowMain().GetConfiguration();
		if(fabsf(textField->GetFloat() - configuration.GetRotateStep()) < FLOAT_SAFE_EPSILON){
			return;
		}
		configuration.SetRotateStep(textField->GetFloat());
		world.NotifyEditingChanged();
	}
};

class cActionScaleSnap : public cBaseAction{
public:
	typedef deTObjectReference<cActionScaleSnap> Ref;
	cActionScaleSnap(meWPView &panel) :
	cBaseAction(panel, "Snap Scale", nullptr, "Snap scaling factor"){}
	
	void OnAction(meWorld &world) override{
		meConfiguration &configuration = pPanel.GetWindowProperties().GetWindowMain().GetConfiguration();
		configuration.SetScaleSnap(!configuration.GetScaleSnap());
		world.NotifyEditingChanged();
	}
};

class cTextScaleStep : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextScaleStep> Ref;
	cTextScaleStep(meWPView &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField *textField, meWorld &world){
		meConfiguration &configuration = pPanel.GetWindowProperties().GetWindowMain().GetConfiguration();
		if(fabsf(textField->GetFloat() - configuration.GetScaleStep()) < FLOAT_SAFE_EPSILON){
			return;
		}
		configuration.SetScaleStep(textField->GetFloat());
		world.NotifyEditingChanged();
	}
};

class cTextSensitivity : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextSensitivity> Ref;
	cTextSensitivity(meWPView &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField *textField, meWorld &world){
		meConfiguration &configuration = pPanel.GetWindowProperties().GetWindowMain().GetConfiguration();
		if(fabsf(textField->GetFloat() - configuration.GetSensitivity()) < FLOAT_SAFE_EPSILON){
			return;
		}
		configuration.SetSensitivity(textField->GetFloat());
		world.NotifyEditingChanged();
	}
};

class cActionSetSensitivity : public cBaseAction{
	float pSensitivity;
	
public:
	typedef deTObjectReference<cActionSetSensitivity> Ref;
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
	typedef deTObjectReference<cActionMenuSensitivity> Ref;
	cActionMenuSensitivity(meWPView &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "Sensitivity menu"),
	pPanel(panel){}
	
	void AddContextMenuEntries(igdeMenuCascade &contextMenu) override{
		meWorld * const world = pPanel.GetWorld();
		if(!world){
			return;
		}
		
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, cActionSetSensitivity::Ref::New(pPanel, 1.0f, "Set to 1"));
		helper.MenuCommand(contextMenu, cActionSetSensitivity::Ref::New(pPanel, 0.1f, "Set to 0.1"));
		helper.MenuCommand(contextMenu, cActionSetSensitivity::Ref::New(pPanel, 0.01f, "Set to 0.01"));
		helper.MenuCommand(contextMenu, cActionSetSensitivity::Ref::New(pPanel, 10.0f, "Set to 10"));
	}
};

class cTextRectSelDist : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextRectSelDist> Ref;
	cTextRectSelDist(meWPView &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField *textField, meWorld &world){
		if(fabsf(textField->GetFloat() - world.GetGuiParameters().GetRectSelDistance()) < FLOAT_SAFE_EPSILON){
			return;
		}
		world.GetGuiParameters().SetRectSelDistance(textField->GetFloat());
	}
};

class cTextRectSelDistStep : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextRectSelDistStep> Ref;
	cTextRectSelDistStep(meWPView &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField *textField, meWorld &world){
		if(fabsf(textField->GetFloat() - world.GetGuiParameters().GetRectSelDistanceStep()) < FLOAT_SAFE_EPSILON){
			return;
		}
		world.GetGuiParameters().SetRectSelDistanceStep(textField->GetFloat());
	}
};

class cActionAutoUpdate : public cBaseAction{
public:
	typedef deTObjectReference<cActionAutoUpdate> Ref;
	cActionAutoUpdate(meWPView &panel) :
	cBaseAction(panel, "Enable Auto Updating", nullptr, "Automatically update the screen"){}
	
	void OnAction(meWorld &world) override{
		meConfiguration &configuration = pPanel.GetWindowProperties().GetWindowMain().GetConfiguration();
		configuration.SetAutoUpdate(!configuration.GetAutoUpdate());
		world.NotifyEditingChanged();
	}
};


class cActionClassHideTags : public cBaseAction {
	igdeToggleTags::Ref &pToggleTags;
	
public:
	typedef deTObjectReference<cActionClassHideTags> Ref;
	cActionClassHideTags(meWPView &panel, igdeToggleTags::Ref &toggleTags) :
	cBaseAction(panel, "", nullptr, "Hide classes matching one or more tags"),
	pToggleTags(toggleTags){}
	
	void OnAction(meWorld &world) override{
		world.GetGuiParameters().GetTagsHideClass() = pToggleTags->GetEnabledTags();
		world.NotifyClassHideTagsChanged();
	}
};

class cActionClassPartialHideTags : public cBaseAction {
	igdeToggleTags::Ref &pToggleTags;
	
public:
	typedef deTObjectReference<cActionClassPartialHideTags> Ref;
	cActionClassPartialHideTags(meWPView &panel, igdeToggleTags::Ref &toggleTags) :
	cBaseAction(panel, "", nullptr, "Partially hide classes matching one or more tags"),
	pToggleTags(toggleTags){}
	
	void OnAction(meWorld &world) override{
		world.GetGuiParameters().GetTagsPartialHideClass() = pToggleTags->GetEnabledTags();
		world.NotifyClassHideTagsChanged();
	}
};


class cActionCameraFreeRoaming : public cBaseAction{
public:
	typedef deTObjectReference<cActionCameraFreeRoaming> Ref;
	cActionCameraFreeRoaming(meWPView &panel) :
	cBaseAction(panel, "Free Roaming", nullptr, "Select free roaming camera"){}
	
	void OnAction(meWorld &world) override{
		world.SetActiveCamera(world.GetFreeRoamingCamera());
	}
};

class cActionCameraPlayer : public cBaseAction{
public:
	typedef deTObjectReference<cActionCameraPlayer> Ref;
	cActionCameraPlayer(meWPView &panel) :
	cBaseAction(panel, "Player", nullptr, "Select player camera"){}
	
	void OnAction(meWorld &world) override{
		world.SetActiveCamera(world.GetPlayerCamera());
	}
};

class cActionCameraObject : public cBaseAction{
public:
	typedef deTObjectReference<cActionCameraObject> Ref;
	cActionCameraObject(meWPView &panel) :
	cBaseAction(panel, "Object", nullptr, "Select object camera"){}
	
	void OnAction(meWorld &world) override{
		meCamera * const camera = pPanel.GetSelectedCameraObject();
		if(camera){
			world.SetActiveCamera(camera);
		}
	}
};

class cComboCamera: public igdeComboBoxListener{
	meWPView &pPanel;
public:
	typedef deTObjectReference<cComboCamera> Ref;
	cComboCamera(meWPView &panel) : igdeComboBoxListener(), pPanel(panel){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		meWorld * const world = pPanel.GetWorld();
		if(!world){
			return;
		}
		
		meCamera * const camera = (meCamera*)comboBox->GetSelectedItemData();
		if(camera == world->GetActiveCamera()
		|| camera == world->GetFreeRoamingCamera()
		|| camera == world->GetPlayerCamera()){
			return;
		}
		
		world->SetActiveCamera(camera);
	}
};


class cActionEnableAuralization : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnableAuralization> Ref;
	cActionEnableAuralization(meWPView &panel) :
	cBaseAction(panel, "Enable Auralization", nullptr,
		"Enable auralization if supported by audio module"){ }
	
	void OnAction(meWorld &world) override{
		meConfiguration &configuration = pPanel.GetWindowProperties().GetWindowMain().GetConfiguration();
		configuration.SetEnableAuralization(!configuration.GetEnableAuralization());
		world.NotifyEditingChanged();
	}
};


class cActionSkyChanged : public cBaseAction{
public:
	typedef deTObjectReference<cActionSkyChanged> Ref;
	cActionSkyChanged(meWPView &panel) : cBaseAction(panel, "", nullptr, ""){}
	
	void OnAction(meWorld &world) override{
		world.NotifySkyChanged();
	}
};

class cEditBgObject : public cBaseAction{
public:
	typedef deTObjectReference<cEditBgObject> Ref;
	cEditBgObject(meWPView &panel) : cBaseAction(panel, "", nullptr, ""){}
	
	void OnAction(meWorld &world) override{
		world.NotifyBgObjectChanged();
	}
};

class cEditLimitBoxMinExtend : public igdeEditVectorListener{
	meWPView &pPanel;
public:
	typedef deTObjectReference<cEditLimitBoxMinExtend> Ref;
	cEditLimitBoxMinExtend(meWPView &panel) : pPanel(panel){}
	
	void OnVectorChanged(igdeEditVector *editVector) override{
		meWorld * const world = pPanel.GetWorld();
		const decVector &vector = editVector->GetVector();
		if(world && !world->GetLimitBoxMinExtend().IsEqualTo(vector)){
			world->GetUndoSystem()->Add(meUViewSetLimitBoxExtends::Ref::New(
				world, vector, world->GetLimitBoxMaxExtend()));
		}
	}
};

class cEditLimitBoxMaxExtend : public igdeEditVectorListener{
	meWPView &pPanel;
public:
	typedef deTObjectReference<cEditLimitBoxMaxExtend> Ref;
	cEditLimitBoxMaxExtend(meWPView &panel) : pPanel(panel){}
	
	void OnVectorChanged(igdeEditVector *editVector) override{
		meWorld * const world = pPanel.GetWorld();
		const decVector &vector = editVector->GetVector();
		if(world && !world->GetLimitBoxMaxExtend().IsEqualTo(vector)){
			world->GetUndoSystem()->Add(meUViewSetLimitBoxExtends::Ref::New(
				world, world->GetLimitBoxMinExtend(), vector));
		}
	}
};

class cActionLimitBoxClear : public cBaseAction{
public:
	typedef deTObjectReference<cActionLimitBoxClear> Ref;
	cActionLimitBoxClear(meWPView &panel) :
	cBaseAction(panel, "Clear", nullptr, "Clear limit box"){}
	
	void OnAction(meWorld &world) override{
		if(!world.GetLimitBoxMinExtend().IsZero() || !world.GetLimitBoxMaxExtend().IsZero()){
			world.GetUndoSystem()->Add(meUViewSetLimitBoxExtends::Ref::New(
				&world, decVector(), decVector()));
		}
	}
};

class cActionLimitBoxDefault : public cBaseAction{
public:
	typedef deTObjectReference<cActionLimitBoxDefault> Ref;
	cActionLimitBoxDefault(meWPView &panel) :
	cBaseAction(panel, "Default", nullptr, "Set limit box to 1m cube"){}
	
	void OnAction(meWorld &world) override{
		const decVector minExtend(-0.5f, -0.5f, -0.5f), maxExtend(0.5f, 0.5f, 0.5f);
		if(!world.GetLimitBoxMinExtend().IsEqualTo(minExtend)
		|| !world.GetLimitBoxMaxExtend().IsEqualTo(maxExtend)){
			world.GetUndoSystem()->Add(meUViewSetLimitBoxExtends::Ref::New(
				&world, minExtend, maxExtend));
		}
	}
};

class cActionLimitBoxDefaultGround : public cBaseAction{
public:
	typedef deTObjectReference<cActionLimitBoxDefaultGround> Ref;
	cActionLimitBoxDefaultGround(meWPView &panel) :
	cBaseAction(panel, "Default Ground", nullptr, "Set limit box to 1m cube sitting on ground"){}
	
	void OnAction(meWorld &world) override{
		const decVector minExtend(-0.5f, 0.0f, -0.5f), maxExtend(0.5f, 1.0f, 0.5f);
		if(!world.GetLimitBoxMinExtend().IsEqualTo(minExtend)
		|| !world.GetLimitBoxMaxExtend().IsEqualTo(maxExtend)){
			world.GetUndoSystem()->Add(meUViewSetLimitBoxExtends::Ref::New(
				&world, minExtend, maxExtend));
		}
	}
};

class cActionMenuLimitBox : public igdeActionContextMenu{
	meWPView &pPanel;
	
public:
	typedef deTObjectReference<cActionMenuLimitBox> Ref;
	cActionMenuLimitBox(meWPView &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "Limit box menu"),
	pPanel(panel){}
	
	void AddContextMenuEntries(igdeMenuCascade &contextMenu) override{
		meWorld * const world = pPanel.GetWorld();
		if(!world){
			return;
		}
		
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, cActionLimitBoxClear::Ref::New(pPanel));
		helper.Separator(contextMenu);
		helper.MenuCommand(contextMenu, cActionLimitBoxDefault::Ref::New(pPanel));
		helper.MenuCommand(contextMenu, cActionLimitBoxDefaultGround::Ref::New(pPanel));
	}
};

class cActionCameraChanged : public cBaseAction{
public:
	typedef deTObjectReference<cActionCameraChanged> Ref;
	cActionCameraChanged(meWPView &panel) : cBaseAction(panel, "", nullptr, ""){}
	
	void OnAction(meWorld &world) override{
		meCamera * const camera = world.GetActiveCamera();
		if(camera){
			pPanel.GetWindowProperties().GetWindowMain().GetConfiguration().SetEnableGI(camera->GetEnableGI());
			world.NotifyCameraChanged(camera);
		}
	}
};

class cActionTriggerTable : public cBaseAction{
public:
	typedef deTObjectReference<cActionTriggerTable> Ref;
	cActionTriggerTable(meWPView &panel) : cBaseAction(panel, "", nullptr, "Trigger table"){}
	
	void OnAction(meWorld &world) override{
		world.NotifyTriggerTableChanged();
	}
};

}



// Class meWPView
///////////////////

// Constructor, destructor
////////////////////////////

meWPView::meWPView(meWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties),
pPreventUpdateCamera(false)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, form, formLine;
	igdeActionContextMenu::Ref actionMenu;
	
	pListener = meWPViewListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	// grid snapping and alignment
	helper.GroupBox(content, groupBox, "Grid/Snapping:");
	
	helper.CheckBoxOnly(groupBox, pChkMoveSnap, cActionMoveSnap::Ref::New(*this));
	helper.EditFloat(groupBox, "Snap moving distance", pEditMoveStep, cTextMoveStep::Ref::New(*this));
	
	helper.CheckBoxOnly(groupBox, pChkRotSnap, cActionRotateSnap::Ref::New(*this));
	helper.EditFloat(groupBox, "Snap rotation angle", pEditRotStep, cTextRotateStep::Ref::New(*this));
	
	helper.CheckBoxOnly(groupBox, pChkScaleSnap, cActionScaleSnap::Ref::New(*this));
	helper.EditFloat(groupBox, "Snap scaling factor", pEditScaleStep, cTextScaleStep::Ref::New(*this));
	
	helper.FormLineStretchFirst(groupBox, "Sensitivity:", "Sensitivity of mouse input", formLine);
	helper.EditFloat(formLine, "Sensitivity of mouse input", pEditSensitivity, cTextSensitivity::Ref::New(*this));
	actionMenu = cActionMenuSensitivity::Ref::New(*this);
	helper.Button(formLine, pBtnSensitivity, actionMenu);
	actionMenu->SetWidget(pBtnSensitivity);
	
	
	// selection
	helper.GroupBox(content, groupBox, "Selection:");
	
	helper.EditFloat(groupBox, "Distance:", "Distance for rectangular selection",
		pEditRectSelDist, cTextRectSelDist::Ref::New(*this));
	helper.EditFloat(groupBox, "Distance Step:",
		"Mouse wheel adjustment step for distance for rectangular selection",
		pEditRectSelDistStep, cTextRectSelDistStep::Ref::New(*this));
	
	
	// auto updating
	helper.GroupBox(content, groupBox, "Auto Updating:");
	helper.CheckBox(groupBox, pChkAutoUpdate, cActionAutoUpdate::Ref::New(*this));
	
	
	// class hiding
	helper.GroupBoxFlow(content, groupBox, "Class Hide Tags:", false, true);
	helper.ToggleTags(groupBox, pEditClassHideTags, cActionClassHideTags::Ref::New(*this, pEditClassHideTags));
	
	helper.GroupBoxFlow(content, groupBox, "Class Partial Hide Tags:", false, true);
	helper.ToggleTags(groupBox, pEditClassPartialHideTags, cActionClassPartialHideTags::Ref::New(*this, pEditClassPartialHideTags));
	
	
	// camera
	helper.GroupBoxFlow(content, groupBox, "Camera:");
	
	form = igdeContainerForm::Ref::New(env);
	groupBox->AddChild(form);
	
	helper.EditString(form, "Active:", "Active camera", pEditActiveCamera, {});
	pEditActiveCamera->SetEditable(false);
	
	helper.FormLine(form, "", "", formLine);
	pActionCameraFreeRoaming = cActionCameraFreeRoaming::Ref::New(*this);
	helper.Button(formLine, pActionCameraFreeRoaming);
	pActionCameraPlayer = cActionCameraPlayer::Ref::New(*this);
	helper.Button(formLine, pActionCameraPlayer);
	pActionCameraObject = cActionCameraObject::Ref::New(*this);
	helper.Button(formLine, pActionCameraObject);
	
	helper.ComboBox(form, "Object Camera:", "Object camera", pCBCameraObjects, {});
	pCBCameraObjects->SetDefaultSorter();
	
	helper.WPCamera(groupBox, pWPCamera, cActionCameraChanged::Ref::New(*this),
		"Camera Parameters:", false, false);
	
	
	// microphone
	helper.GroupBoxFlow(content, groupBox, "Microphone:");
	helper.CheckBox(groupBox, pChkEnableAuralization, cActionEnableAuralization::Ref::New(*this));
	
	
	// property panels
	helper.WPSky(content, pWPSky, cActionSkyChanged::Ref::New(*this), "Sky:", false, false);
	helper.WPWObject(content, pWPBgObject, cEditBgObject::Ref::New(*this), "Background Object:", false, true);
	helper.WPTriggerTable(content, pWPTriggerTable, cActionTriggerTable::Ref::New(*this),
		"Trigger Table:", false, true);
	
	// limit box
	helper.GroupBox(content, groupBox, "Limit Box:");
	
	helper.FormLineStretchFirst(groupBox, "Minimum Extend:", "Minimum extend of limit box", formLine);
	helper.EditVector(formLine, "Minimum extend of limit box", pEditLimitBoxMinExtend, cEditLimitBoxMinExtend::Ref::New(*this));
	actionMenu = cActionMenuLimitBox::Ref::New(*this);
	helper.Button(formLine, pBtnLimitBoxMenu, actionMenu);
	actionMenu->SetWidget(pBtnLimitBoxMenu);
	
	helper.EditVector(groupBox, "Maximum Extend:", "Maximum extend of limit box",
		pEditLimitBoxMaxExtend, cEditLimitBoxMaxExtend::Ref::New(*this));
}

meWPView::~meWPView(){
	SetWorld(nullptr);
}



// Management
///////////////

void meWPView::SetWorld(meWorld *world){
	if(world == pWorld){
		return;
	}
	
	pWPSky->SetSky(nullptr);
	pWPBgObject->SetObject(nullptr);
	pWPBgObject->SetUndoSystem(nullptr);
	pWPCamera->SetCamera(nullptr);
	pWPTriggerTable->SetTriggerTargetList(nullptr);
	pEditClassHideTags->SetTags(decStringSet());
	pEditClassPartialHideTags->Clear();
	
	if(pWorld){
		pWorld->RemoveNotifier(pListener);
	}
	
	pWorld = world;
	
	if(world){
		world->AddNotifier(pListener);
		pWPSky->SetSky(world->GetSky());
		pWPBgObject->SetObject(world->GetBgObject());
		pWPBgObject->SetUndoSystem(world->GetUndoSystem());
		pWPTriggerTable->SetTriggerTargetList(&world->GetTriggerTable());
		
		if(world->GetGameDefinition()){
			const igdeGDClassManager &gdClasses = *pWorld->GetGameDefinition()->GetClassManager();
			pEditClassHideTags->SetTags(gdClasses.GetHideTags());
			pEditClassPartialHideTags->SetTags(gdClasses.GetPartialHideTags());
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
		(meCamera*)pCBCameraObjects->GetSelectedItem()->GetData() : nullptr;
}

void meWPView::UpdateView(){
	const meConfiguration &configuration = pWindowProperties.GetWindowMain().GetConfiguration();
	
	pChkMoveSnap->SetChecked(configuration.GetMoveSnap());
	pEditMoveStep->SetFloat(configuration.GetMoveStep());
	pChkRotSnap->SetChecked(configuration.GetRotateSnap());
	pEditRotStep->SetFloat(configuration.GetRotateStep());
	pChkScaleSnap->SetChecked(configuration.GetScaleSnap());
	pEditScaleStep->SetFloat(configuration.GetScaleStep());
	pEditSensitivity->SetFloat(configuration.GetSensitivity());
	pChkAutoUpdate->SetChecked(configuration.GetAutoUpdate());
	pChkEnableAuralization->SetChecked(configuration.GetEnableAuralization());
	
	if(pWorld){
		const meWorldGuiParameters &guiParams = pWorld->GetGuiParameters();
		pEditRectSelDist->SetFloat(guiParams.GetRectSelDistance());
		pEditRectSelDistStep->SetFloat(guiParams.GetRectSelDistanceStep());
		
	}else{
		pEditRectSelDist->ClearText();
		pEditRectSelDistStep->ClearText();
	}
	
	const bool enable = pWorld;
	pEditRectSelDist->SetEnabled(enable);
	pEditRectSelDistStep->SetEnabled(enable);
}

void meWPView::UpdateCameraList(){
	meCamera * const selectedCamera = (meCamera*)pCBCameraObjects->GetSelectedItemData();
	
	pCBCameraObjects->RemoveAllItems();
	
	if(pWorld){
		pWorld->GetObjects().Visit([&](const meObject &object){
			meCamera * const camera = object.GetCamera();
			if(camera){
				pCBCameraObjects->AddItem(camera->GetName(), nullptr, camera);
			}
		});
	}
	
	pCBCameraObjects->SortItems();
	
	pCBCameraObjects->SetSelectionWithData(selectedCamera);
	if(!pCBCameraObjects->GetSelectedItem() && pCBCameraObjects->GetItems().IsNotEmpty()){
		pCBCameraObjects->SetSelection(0);
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
		
		if(camera){
			pEditActiveCamera->SetText(camera->GetName());
			
		}else{
			pEditActiveCamera->ClearText();
		}
		
		pWPCamera->SetCamera(camera);
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
	if(pWorld){
		pEditClassHideTags->EnableTags(pWorld->GetGuiParameters().GetTagsHideClass());
		pEditClassPartialHideTags->EnableTags(pWorld->GetGuiParameters().GetTagsPartialHideClass());
		
	}else{
		pEditClassHideTags->EnableTags(decStringSet());
		pEditClassPartialHideTags->EnableTags(decStringSet());
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
