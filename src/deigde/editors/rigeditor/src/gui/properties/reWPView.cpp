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
#include <string.h>
#include <stdlib.h>

#include "reWPView.h"
#include "reWPViewListener.h"
#include "reWindowProperties.h"
#include "../reWindowMain.h"
#include "../../configuration/reConfiguration.h"
#include "../../rig/reRig.h"
#include "../../rig/reCamera.h"
#include "../../rig/reRigTexture.h"
#include "../../rig/bone/reRigBone.h"
#include "../../loadsave/reLoadSaveSystem.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/igdeSlider.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/composed/igdeEditSliderText.h>
#include <deigde/gui/composed/igdeEditSliderTextListener.h>
#include <deigde/gui/composed/igdeEditDVector.h>
#include <deigde/gui/composed/igdeEditDVectorListener.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionSelectFile.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/event/igdeSliderListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/properties/igdeWPSky.h>
#include <deigde/gui/properties/igdeWPWObject.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/rig/deRig.h>



// Actions
///////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	reWPView &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(reWPView &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		reRig * const rig = pPanel.GetRig();
		if(rig){
			OnChanged(textField, *rig);
		}
	}
	
	virtual void OnChanged(igdeTextField *textField, reRig &rig) = 0;
};

class cBaseAction : public igdeAction{
protected:
	reWPView &pPanel;
	
public:
	typedef deTObjectReference<cBaseAction> Ref;
	cBaseAction(reWPView &panel, const char *text, const char *description) :
	igdeAction(text, description),
	pPanel(panel){}
	
	void OnAction() override{
		reRig * const rig = pPanel.GetRig();
		if(rig){
			OnAction(*rig);
		}
	}
	
	virtual void OnAction(reRig &rig) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	reWPView &pPanel;
	
public:
	typedef deTObjectReference<cBaseComboBoxListener> Ref;
	cBaseComboBoxListener(reWPView &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		reRig * const rig = pPanel.GetRig();
		if(rig){
			OnTextChanged(comboBox, *rig);
		}
	}
	
	virtual void OnTextChanged(igdeComboBox *comboBox, reRig &rig) = 0;
};

class cBaseSliderListener : public igdeEditSliderTextListener{
protected:
	reWPView &pPanel;
	
public:
	typedef deTObjectReference<cBaseSliderListener> Ref;
	cBaseSliderListener(reWPView &panel) : pPanel(panel){}
	
	virtual void OnSliderTextValueChanging(igdeEditSliderText *sliderText){
		reRig * const rig = pPanel.GetRig();
		if(rig){
			OnChanging(sliderText->GetValue(), *rig);
		}
	}
	
	virtual void OnSliderTextValueChanged(igdeEditSliderText *sliderText){
		reRig * const rig = pPanel.GetRig();
		if(rig){
			OnChanged(sliderText->GetValue(), *rig);
		}
	}
	
	virtual void OnChanging(float value, reRig &rig) = 0;
	virtual void OnChanged(float value, reRig &rig) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	reWPView &pPanel;
	
public:
	typedef deTObjectReference<cBaseEditVectorListener> Ref;
	cBaseEditVectorListener(reWPView &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		reRig * const rig = pPanel.GetRig();
		if(rig){
			OnChanged(editVector->GetVector(), *rig);
		}
	}
	
	virtual void OnChanged(const decVector &vector, reRig &rig) = 0;
};

class cBaseEditDVectorListener : public igdeEditDVectorListener{
protected:
	reWPView &pPanel;
	
public:
	typedef deTObjectReference<cBaseEditDVectorListener> Ref;
	cBaseEditDVectorListener(reWPView &panel) : pPanel(panel){}
	
	virtual void OnDVectorChanged(igdeEditDVector *editDVector){
		reRig * const rig = pPanel.GetRig();
		if(rig){
			OnChanged(editDVector->GetDVector(), *rig);
		}
	}
	
	virtual void OnChanged(const decDVector &vector, reRig &rig) = 0;
};

class cBaseEditPathListener : public igdeEditPathListener{
protected:
	reWPView &pPanel;
	
public:
	typedef deTObjectReference<cBaseEditPathListener> Ref;
	cBaseEditPathListener(reWPView &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		reRig * const rig = pPanel.GetRig();
		if(rig){
			OnChanged(editPath->GetPath(), *rig);
		}
	}
	
	virtual void OnChanged(const decString &path, reRig &rig) = 0;
};



class cEditModelPath : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cEditModelPath> Ref;
	cEditModelPath(reWPView &panel) : cBaseEditPathListener(panel){}
	
	virtual void OnChanged(const decString &path, reRig &rig){
		rig.SetModelPath(path);
	}
};

class cEditSkinPath : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cEditSkinPath> Ref;
	cEditSkinPath(reWPView &panel) : cBaseEditPathListener(panel){}
	
	virtual void OnChanged(const decString &path, reRig &rig){
		rig.SetSkinPath(path);
	}
};

class cEditAnimationPath : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cEditAnimationPath> Ref;
	cEditAnimationPath(reWPView &panel) : cBaseEditPathListener(panel){}
	
	virtual void OnChanged(const decString &path, reRig &rig){
		rig.SetAnimationPath(path);
	}
};


class cComboTexture : public igdeComboBoxListener{
	reWPView &pPanel;
	
public:
	typedef deTObjectReference<cComboTexture> Ref;
	cComboTexture(reWPView &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox*){
		pPanel.UpdateTexture();
	}
};

class cEditTextureSkinPath : public cBaseEditPathListener{
public:
	typedef deTObjectReference<cEditTextureSkinPath> Ref;
	cEditTextureSkinPath(reWPView &panel) : cBaseEditPathListener(panel){}
	
	virtual void OnChanged(const decString &path, reRig &rig){
		reRigTexture * const texture = pPanel.GetSelectedTexture();
		if(texture){
			texture->SetSkinPath(path);
			rig.UpdateComponentTextures();
		}
	}
};


class cComboAnimationMove : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboAnimationMove> Ref;
	cComboAnimationMove(reWPView &panel) : cBaseComboBoxListener(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox, reRig &rig){
		rig.SetMoveName(comboBox->GetText());
	}
};

class cSliderMoveTime : public cBaseSliderListener{
public:
	typedef deTObjectReference<cSliderMoveTime> Ref;
	cSliderMoveTime(reWPView &panel) : cBaseSliderListener(panel){}
	
	void OnChanging(float value, reRig &rig) override{
		rig.SetMoveTime(value);
	}
	
	void OnChanged(float value, reRig &rig) override{
		rig.SetMoveTime(value);
	}
};

class cCheckUseRestPose : public cBaseAction{
public:
	typedef deTObjectReference<cCheckUseRestPose> Ref;
	cCheckUseRestPose(reWPView &panel) : cBaseAction(panel,
		"@Rig.PanelView.Action.UseRestPose", "@Rig.PanelView.Action.UseRestPose.Description"){}
	
	void OnAction(reRig &rig) override{
		rig.SetUseRestPose(!rig.GetUseRestPose());
	}
};

class cCheckPlaybackMove : public cBaseAction{
public:
	typedef deTObjectReference<cCheckPlaybackMove> Ref;
	cCheckPlaybackMove(reWPView &panel) : cBaseAction(panel,
		"@Rig.PanelView.Action.Playback", "@Rig.PanelView.Action.Playback.Description"){ }
	
	void OnAction(reRig &rig) override{
		rig.SetPlaybackMove(!rig.GetPlaybackMove());
	}
};


class cEditGravity : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditGravity> Ref;
	cEditGravity(reWPView &panel) : cBaseEditVectorListener(panel){}
	
	virtual void OnChanged(const decVector &vector, reRig &rig){
		rig.SetGravity(vector);
	}
};

class cEditLocalGravity : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditLocalGravity> Ref;
	cEditLocalGravity(reWPView &panel) : cBaseEditVectorListener(panel){}
	
	virtual void OnChanged(const decVector &vector, reRig &rig){
		rig.SetLocalGravity(vector);
	}
};

class cSliderSlowMotion : public cBaseSliderListener{
public:
	typedef deTObjectReference<cSliderSlowMotion> Ref;
	cSliderSlowMotion(reWPView &panel) : cBaseSliderListener(panel){}
	
	void OnChanging(float value, reRig &rig) override{
		rig.SetSlowmotion(value);
	}
	
	void OnChanged(float value, reRig &rig) override{
		rig.SetSlowmotion(value);
	}
};


class cTextGridSize : public igdeTextFieldListener{
	reWPView &pPanel;
public:
	typedef deTObjectReference<cTextGridSize> Ref;
	cTextGridSize(reWPView &panel) : pPanel(panel){}
	
	virtual void OnChanged(igdeTextField *textField){
		pPanel.GetWindowProperties().GetWindowMain().GetConfiguration()
			.SetGridSize(textField->GetFloat());
		if(pPanel.GetRig()){
			pPanel.GetRig()->NotifyViewChanged();
		}
	}
};

class cCheckSnapToGrid : public cBaseAction{
public:
	typedef deTObjectReference<cCheckSnapToGrid> Ref;
	cCheckSnapToGrid(reWPView &panel) : cBaseAction(panel, "@Rig.PanelView.Action.Snap", "@Rig.PanelView.Action.Snap.Description"){}
	
	void OnAction(reRig &rig) override{
		reConfiguration &config = pPanel.GetWindowProperties().GetWindowMain().GetConfiguration();
		config.SetSnapToGrid(!config.GetSnapToGrid());
		rig.NotifyViewChanged();
	}
};

class cTextSensitivity : public igdeTextFieldListener{
	reWPView &pPanel;
public:
	typedef deTObjectReference<cTextSensitivity> Ref;
	cTextSensitivity(reWPView &panel) : pPanel(panel){}
	
	virtual void OnChanged(igdeTextField *textField){
		pPanel.GetWindowProperties().GetWindowMain().GetConfiguration()
			.SetSensitivity(textField->GetFloat());
		if(pPanel.GetRig()){
			pPanel.GetRig()->NotifyViewChanged();
		}
	}
};


class cEditCameraPosition : public cBaseEditDVectorListener{
public:
	typedef deTObjectReference<cEditCameraPosition> Ref;
	cEditCameraPosition(reWPView &panel) : cBaseEditDVectorListener(panel){}
	
	virtual void OnChanged(const decDVector &vector, reRig &rig){
		rig.GetCamera()->SetFreePosition(vector);
	}
};

class cEditCameraRotation : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditCameraRotation> Ref;
	cEditCameraRotation(reWPView &panel) : cBaseEditVectorListener(panel){}
	
	virtual void OnChanged(const decVector &vector, reRig &rig){
		rig.GetCamera()->SetFreeOrientation(vector);
	}
};

class cTextCameraFov : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextCameraFov> Ref;
	cTextCameraFov(reWPView &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField *textField, reRig &rig){
		rig.GetCamera()->SetFov(textField->GetFloat());
	}
};

class cTextCameraFovRatio : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextCameraFovRatio> Ref;
	cTextCameraFovRatio(reWPView &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField *textField, reRig &rig){
		rig.GetCamera()->SetFovRatio(textField->GetFloat());
	}
};

class cTextCameraImageDistance : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextCameraImageDistance> Ref;
	cTextCameraImageDistance(reWPView &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField *textField, reRig &rig){
		rig.GetCamera()->SetImageDistance(textField->GetFloat());
	}
};

class cTextCameraViewDistance : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextCameraViewDistance> Ref;
	cTextCameraViewDistance(reWPView &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField *textField, reRig &rig){
		rig.GetCamera()->SetViewDistance(textField->GetFloat());
	}
};

class cTextCameraExposure : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextCameraExposure> Ref;
	cTextCameraExposure(reWPView &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField *textField, reRig &rig){
		rig.GetCamera()->SetExposure(textField->GetFloat());
	}
};

class cTextCameraAdaptionTime : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextCameraAdaptionTime> Ref;
	cTextCameraAdaptionTime(reWPView &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField *textField, reRig &rig){
		rig.GetCamera()->SetAdaptionTime(textField->GetFloat());
	}
};

class cTextCameraLowIntensity : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextCameraLowIntensity> Ref;
	cTextCameraLowIntensity(reWPView &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField *textField, reRig &rig){
		rig.GetCamera()->SetLowestIntensity(textField->GetFloat());
	}
};

class cTextCameraHighIntensity : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextCameraHighIntensity> Ref;
	cTextCameraHighIntensity(reWPView &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField *textField, reRig &rig){
		rig.GetCamera()->SetHighestIntensity(textField->GetFloat());
	}
};


class cCheckCameraAttach : public cBaseAction{
public:
	typedef deTObjectReference<cCheckCameraAttach> Ref;
	cCheckCameraAttach(reWPView &panel) : cBaseAction(panel, "@Rig.PanelView.Action.AttachCameraToBone",
		"@Rig.PanelView.Action.AttachCameraToBone.Description"){}
	
	void OnAction(reRig &rig) override{
		reCamera &camera = *rig.GetCamera();
		camera.SetAttachToBone(!camera.GetAttachToBone());
	}
};

class cComboCameraBone : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboCameraBone> Ref;
	cComboCameraBone(reWPView &panel) : cBaseComboBoxListener(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox, reRig &rig){
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		reRigBone *bone = nullptr;
		if(selection){
			bone = (reRigBone*)selection->GetData();
		}
		rig.GetCamera()->SetBone(bone);
	}
};

class cEditCameraRelativePosition : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditCameraRelativePosition> Ref;
	cEditCameraRelativePosition(reWPView &panel) : cBaseEditVectorListener(panel){}
	
	virtual void OnChanged(const decVector &vector, reRig &rig){
		rig.GetCamera()->SetRelativePosition(vector);
	}
};

class cEditCameraRelativeRotation : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditCameraRelativeRotation> Ref;
	cEditCameraRelativeRotation(reWPView &panel) : cBaseEditVectorListener(panel){}
	
	virtual void OnChanged(const decVector &vector, reRig &rig){
		rig.GetCamera()->SetRelativeOrientation(vector);
	}
};


class cActionSkyChanged : public cBaseAction{
public:
	typedef deTObjectReference<cActionSkyChanged> Ref;
	cActionSkyChanged(reWPView &panel) : cBaseAction(panel, "", ""){}
	
	void OnAction(reRig &rig) override{
		rig.NotifySkyChanged();
	}
};

class cActionEnvObjChanged : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnvObjChanged> Ref;
	cActionEnvObjChanged(reWPView &panel) : cBaseAction(panel, "", ""){}
	
	void OnAction(reRig &rig) override{
		rig.NotifyEnvObjectChanged();
	}
};

}



// Class reWPView
///////////////////

// Constructor, destructor
////////////////////////////

reWPView::reWPView(reWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainer::Ref content, groupBox, groupBox2, frameLine, form;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeAction::Ref action;
	
	pListener = reWPViewListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	// resources
	helper.GroupBoxFlow(content, groupBox, "@Rig.PanelView.GroupBox.Resources");
	form = igdeContainerForm::Ref::New(env);
	groupBox->AddChild(form);
	
	helper.EditPath(form, "@Rig.PanelView.Resources.Model", "@Rig.PanelView.Resources.Model.ToolTip",
		igdeEnvironment::efpltModel, pEditModelPath, cEditModelPath::Ref::New(*this));
	helper.EditPath(form, "@Rig.PanelView.Resources.Skin", "@Rig.PanelView.Resources.Skin.ToolTip",
		igdeEnvironment::efpltSkin, pEditSkinPath, cEditSkinPath::Ref::New(*this));
	helper.EditPath(form, "@Rig.PanelView.Resources.Animation", "@Rig.PanelView.Resources.Animation.ToolTip",
		igdeEnvironment::efpltAnimation, pEditAnimPath, cEditAnimationPath::Ref::New(*this));
	
		// textures
		helper.GroupBox(groupBox, groupBox2, "@Rig.PanelView.GroupBox.Textures");
		
		helper.ComboBox(groupBox2, "@Rig.PanelView.Textures.Texture", "@Rig.PanelView.Textures.Texture.ToolTip",
			pCBTexture, cComboTexture::Ref::New(*this));
		pCBTexture->SetDefaultSorter();
		
		helper.EditPath(groupBox2, "@Rig.PanelView.Textures.Skin", "@Rig.PanelView.Textures.Skin.ToolTip",
			igdeEnvironment::efpltSkin, pEditTexSkin, cEditTextureSkinPath::Ref::New(*this));
	
	// animation move
	helper.GroupBox(content, groupBox, "@Rig.PanelView.GroupBox.AnimationMove");
	
	helper.ComboBox(groupBox, "@Rig.PanelView.AnimationMove.Move", true, "@Rig.PanelView.AnimationMove.Move.ToolTip",
		pCBAnimMoves, cComboAnimationMove::Ref::New(*this));
	pCBAnimMoves->SetDefaultSorter();
	
	helper.EditSliderText(groupBox, "@Rig.PanelView.AnimationMove.Time", "@Rig.PanelView.AnimationMove.Time.ToolTip",
		0.0f, 0.0f, 5, 2, 0.5f, pSldMoveTime, cSliderMoveTime::Ref::New(*this));
	
	helper.CheckBox(groupBox, pChkUseRestPose, cCheckUseRestPose::Ref::New(*this));
	helper.CheckBox(groupBox, pChkPlaybackMove, cCheckPlaybackMove::Ref::New(*this));
	
	// physics
	helper.GroupBox(content, groupBox, "@Rig.PanelView.GroupBox.Physics");
	
	helper.EditVector(groupBox, "@Rig.PanelView.Physics.Gravity", "@Rig.PanelView.Physics.Gravity.ToolTip",
		pEditGravity, cEditGravity::Ref::New(*this));
	helper.EditVector(groupBox, "@Rig.PanelView.Physics.LocalGravity", "@Rig.PanelView.Physics.LocalGravity.ToolTip",
		pEditLocalGravity, cEditLocalGravity::Ref::New(*this));
	helper.EditSliderText(groupBox, "@Rig.PanelView.Physics.Slowmotion", "@Rig.PanelView.Physics.Slowmotion.ToolTip",
		0.0f, 1.0f, 5, 2, 0.1f, pSldSlowmotion, cSliderSlowMotion::Ref::New(*this));
	pSldSlowmotion->SetValue(1.0f);
	
	// editing
	helper.GroupBox(content, groupBox, "@Rig.PanelView.GroupBox.Editing");
	
	helper.FormLineStretchFirst(groupBox, "@Rig.PanelView.Editing.Grid", "@Rig.PanelView.Editing.Grid.SnapToGrid.ToolTip", frameLine);
	helper.EditFloat(frameLine, "@Rig.PanelView.Editing.Grid.Spacing.ToolTip", pEditGridSize, cTextGridSize::Ref::New(*this));
	helper.CheckBox(frameLine, pChkSnapToGrid, cCheckSnapToGrid::Ref::New(*this));
	
	helper.EditFloat(groupBox, "@Rig.PanelView.Editing.Sensitivity",
		"@Rig.PanelView.Editing.Sensitivity.ToolTip",
		pEditSensitivity, cTextSensitivity::Ref::New(*this));
	
	// property panels
	action = cActionSkyChanged::Ref::New(*this);
	helper.WPSky(content, pWPSky, action, "@Rig.PanelView.Editing.Sky", false, true);
	
	action = cActionEnvObjChanged::Ref::New(*this);
	helper.WPWObject(content, pWPEnvObject, action, "@Rig.PanelView.Editing.EnvObject", false, true);
	
	// camera
	helper.GroupBox(content, groupBox, "@Rig.PanelView.GroupBox.Camera", true);
	
	helper.EditDVector(groupBox, "@Rig.PanelView.Camera.Position", "@Rig.PanelView.Camera.Position.ToolTip",
		pEditCamPosition, cEditCameraPosition::Ref::New(*this));
	helper.EditVector(groupBox, "@Rig.PanelView.Camera.Rotation", "@Rig.PanelView.Camera.Rotation.ToolTip",
		pEditCamRotation, cEditCameraRotation::Ref::New(*this));
	
	helper.EditVector(groupBox, "@Rig.PanelView.Camera.View", "@Rig.PanelView.Camera.View.ToolTip", pEditCamViewDir, {});
	pEditCamViewDir->SetEditable(false);
	
	helper.FormLine(groupBox, "@Rig.PanelView.Camera.Fov", "@Rig.PanelView.Camera.Fov.ToolTip", frameLine);
	helper.EditFloat(frameLine, "@Rig.PanelView.Camera.FovDegrees.ToolTip", pEditCamFov, cTextCameraFov::Ref::New(*this));
	helper.EditFloat(frameLine, "@Rig.PanelView.Camera.FovRatio.ToolTip", pEditCamFovRatio, cTextCameraFovRatio::Ref::New(*this));
	
	helper.FormLine(groupBox, "@Rig.PanelView.Camera.Distance", "@Rig.PanelView.Camera.Distance.ToolTip", frameLine);
	helper.EditFloat(frameLine, "@Rig.PanelView.Camera.ImageDistance.ToolTip", pEditCamImageDist, cTextCameraImageDistance::Ref::New(*this));
	helper.EditFloat(frameLine, "@Rig.PanelView.Camera.ViewDistance.ToolTip", pEditCamViewDist, cTextCameraViewDistance::Ref::New(*this));
	
	helper.FormLine(groupBox, "@Rig.PanelView.Camera.Exposure",
		"@Rig.PanelView.Camera.Exposure.ToolTip", frameLine);
	helper.EditFloat(frameLine, "@Rig.PanelView.Camera.ExposureMultiplier.ToolTip", pEditCamExposure, cTextCameraExposure::Ref::New(*this));
	helper.EditFloat(frameLine, "@Rig.PanelView.Camera.ExposureTime.ToolTip", pEditCamAdaptTime, cTextCameraAdaptionTime::Ref::New(*this));
	
	helper.FormLine(groupBox, "@Rig.PanelView.Camera.Adaption",
		"@Rig.PanelView.Camera.Adaption.ToolTip", frameLine);
	helper.EditFloat(frameLine, "@Rig.PanelView.Camera.AdaptionLower.ToolTip", pEditCamLowInt, cTextCameraLowIntensity::Ref::New(*this));
	helper.EditFloat(frameLine, "@Rig.PanelView.Camera.AdaptionUpper.ToolTip", pEditCamHiInt, cTextCameraHighIntensity::Ref::New(*this));
	
	// camera attaching
	helper.GroupBox(content, groupBox, "@Rig.PanelView.GroupBox.CameraAttaching", true);
	
	helper.CheckBox(groupBox, pChkCamAttach, cCheckCameraAttach::Ref::New(*this));
	
	helper.ComboBox(groupBox, "@Rig.PanelView.CameraAttaching.Bone", true, "@Rig.PanelView.CameraAttaching.Bone.ToolTip",
		pCBCamBone, cComboCameraBone::Ref::New(*this));
	pCBCamBone->SetDefaultSorter();
	
	helper.EditVector(groupBox, "@Rig.PanelView.CameraAttaching.Position",
		"@Rig.PanelView.CameraAttaching.Position.ToolTip",
		pEditCamRelPosition, cEditCameraRelativePosition::Ref::New(*this));
	helper.EditVector(groupBox, "@Rig.PanelView.CameraAttaching.Rotation",
		"@Rig.PanelView.CameraAttaching.Rotation.ToolTip",
		pEditCamRelRotation, cEditCameraRelativeRotation::Ref::New(*this));
}

reWPView::~reWPView(){
	SetRig(nullptr);
}



// Management
///////////////

void reWPView::SetRig(reRig *rig){
	if(rig == pRig){
		return;
	}
	
	pWPEnvObject->SetObject(nullptr);
	pWPSky->SetSky(nullptr);
	
	if(pRig){
		pRig->RemoveNotifier(pListener);
		pRig = nullptr;
	}
	
	pRig = rig;
	
	decString defaultPath;
	
	if(rig){
		rig->AddNotifier(pListener);
		pWPSky->SetSky(rig->GetSky());
		pWPEnvObject->SetObject(rig->GetEnvObject());
		
		defaultPath = rig->GetDirectoryPath();
	}
	
	pEditModelPath->SetDefaultPath(defaultPath);
	pEditSkinPath->SetDefaultPath(defaultPath);
	pEditAnimPath->SetDefaultPath(defaultPath);
	pEditTexSkin->SetDefaultPath(defaultPath);
	
	UpdateBoneList();
	UpdateMoveList();
	UpdateResources();
	UpdateCamera();
	UpdateCameraView();
	UpdateSky();
	UpdateEnvObject();
	UpdateView();
}



void reWPView::UpdateResources(){
	if(pRig){
		pEditModelPath->SetPath(pRig->GetModelPath());
		pEditSkinPath->SetPath(pRig->GetSkinPath());
		pEditAnimPath->SetPath(pRig->GetAnimationPath());
		
	}else{
		pEditModelPath->ClearPath();
		pEditSkinPath->ClearPath();
		pEditAnimPath->ClearPath();
	}
	
	const bool enabled = pRig.IsNotNull();
	pEditModelPath->SetEnabled(enabled);
	pEditSkinPath->SetEnabled(enabled);
	pEditAnimPath->SetEnabled(enabled);
	
	UpdateTextureList();
	UpdateTexture();
}

void reWPView::UpdateCamera(){
	reCamera * const camera = pRig ? pRig->GetCamera() : nullptr;
	const bool enableAttach = camera ? camera->GetAttachToBone() : false;
	
	if(camera){
		pEditCamPosition->SetDVector(camera->GetFreePosition());
		pEditCamRotation->SetVector(camera->GetFreeOrientation());
		pEditCamFov->SetFloat(camera->GetFov());
		pEditCamFovRatio->SetFloat(camera->GetFovRatio());
		pEditCamImageDist->SetFloat(camera->GetImageDistance());
		pEditCamViewDist->SetFloat(camera->GetViewDistance());
		pEditCamExposure->SetFloat(camera->GetExposure());
		pEditCamAdaptTime->SetFloat(camera->GetAdaptionTime());
		pEditCamLowInt->SetFloat(camera->GetLowestIntensity());
		pEditCamHiInt->SetFloat(camera->GetHighestIntensity());
		
		pChkCamAttach->SetChecked(camera->GetAttachToBone());
		pCBCamBone->SetText(camera->GetBone() ? camera->GetBone()->GetName() : decString());
		pEditCamRelPosition->SetVector(camera->GetRelativePosition());
		pEditCamRelRotation->SetVector(camera->GetRelativeOrientation());
		
	}else{
		pEditCamPosition->SetDVector(decDVector());
		pEditCamRotation->SetVector(decVector());
		pEditCamFov->ClearText();
		pEditCamFovRatio->ClearText();
		pEditCamImageDist->ClearText();
		pEditCamViewDist->ClearText();
		pEditCamExposure->ClearText();
		pEditCamAdaptTime->ClearText();
		pEditCamLowInt->ClearText();
		pEditCamHiInt->ClearText();
		
		pChkCamAttach->SetChecked(false);
		pCBCamBone->SetText("");
		pEditCamRelPosition->SetVector(decVector());
		pEditCamRelRotation->SetVector(decVector());
	}
	
	const bool enabled = camera != nullptr;
	
	pEditCamPosition->SetEnabled(!enableAttach);
	pEditCamRotation->SetEnabled(!enableAttach);
	pEditCamFov->SetEnabled(enabled);
	pEditCamFovRatio->SetEnabled(enabled);
	pEditCamImageDist->SetEnabled(enabled);
	pEditCamViewDist->SetEnabled(enabled);
	pEditCamExposure->SetEnabled(enabled);
	pEditCamAdaptTime->SetEnabled(enabled);
	pEditCamLowInt->SetEnabled(enabled);
	pEditCamHiInt->SetEnabled(enabled);
	
	pChkCamAttach->SetEnabled(enabled);
	pCBCamBone->SetEnabled(enableAttach);
	pEditCamRelPosition->SetEnabled(enableAttach);
	pEditCamRelRotation->SetEnabled(enableAttach);
}

void reWPView::UpdateCameraView(){
	reCamera * const camera = pRig ? pRig->GetCamera() : nullptr;
	
	if(camera){
		pEditCamViewDir->SetVector(camera->GetViewMatrix().TransformView());
		
	}else{
		pEditCamViewDir->SetVector(decVector());
	}
	
	pEditCamViewDir->SetEnabled(camera != nullptr);
}

void reWPView::UpdateSky(){
	pWPSky->UpdateSky();
}

void reWPView::UpdateEnvObject(){
	pWPEnvObject->UpdateObject();
}

void reWPView::UpdateView(){
	if(pRig){
		pCBAnimMoves->SetText(pRig->GetMoveName());
		
		pSldMoveTime->SetRange(0.0f, pRig->GetPlayTime());
		pSldMoveTime->SetValue(pRig->GetMoveTime());
		
		pChkUseRestPose->SetChecked(pRig->GetUseRestPose());
		pChkPlaybackMove->SetChecked(pRig->GetPlaybackMove());
		
		// physics
		pEditGravity->SetVector(pRig->GetGravity());
		pEditLocalGravity->SetVector(pRig->GetLocalGravity());
		
		pSldSlowmotion->SetValue(pRig->GetSlowmotion());
		
	}else{
		pCBAnimMoves->SetText("");
		
		pSldMoveTime->SetRange(0.0f, 0.0f);
		pSldMoveTime->SetValue(0.0f);
		
		pChkUseRestPose->SetChecked(false);
		pChkPlaybackMove->SetChecked(false);
		
		pEditGravity->SetVector(decVector());
		pEditLocalGravity->SetVector(decVector());
	}
	
	const bool enabled = pRig.IsNotNull();
	
	pCBAnimMoves->SetEnabled(enabled);
	pSldMoveTime->SetEnabled(enabled);
	pChkUseRestPose->SetEnabled(enabled);
	pChkPlaybackMove->SetEnabled(enabled);
	pEditGravity->SetEnabled(enabled);
	pEditLocalGravity->SetEnabled(enabled);
	
	// editing
	const reConfiguration &configuration = pWindowProperties.GetWindowMain().GetConfiguration();
	pChkSnapToGrid->SetChecked(configuration.GetSnapToGrid());
	pEditGridSize->SetFloat(configuration.GetGridSize());
	pEditSensitivity->SetFloat(configuration.GetSensitivity());
}

void reWPView::UpdateMoveList(){
	const deAnimator * const engAnimator = pRig ? pRig->GetEngineAnimator().Pointer() : nullptr;
	const deAnimation * const animation = engAnimator ? engAnimator->GetAnimation().Pointer() : nullptr;
	
	pCBAnimMoves->RemoveAllItems();
	
	if(animation){
		const int count = animation->GetMoveCount();
		int i;
		
		for(i=0; i<count; i++){
			pCBAnimMoves->AddItem(animation->GetMove(i)->GetName());
		}
	}
	
	pCBAnimMoves->SortItems();
}

void reWPView::UpdateBoneList(){
	pCBCamBone->RemoveAllItems();
	
	if(pRig){
		pRig->GetBones().Visit([&](reRigBone *bone){
			pCBCamBone->AddItem(bone->GetName(), nullptr, bone);
		});
	}
	
	pCBCamBone->SortItems();
}

void reWPView::UpdateTextureList(){
	const decString oldSelection(pCBTexture->GetText());
	
	pCBTexture->RemoveAllItems();
	
	if(pRig){
		pRig->GetComponentTextures().Visit([&](reRigTexture *texture){
			pCBTexture->AddItem(texture->GetName(), nullptr, texture);
		});
	}
	
	pCBTexture->SortItems();
	pCBTexture->SetText(oldSelection);
}

void reWPView::UpdateTexture(){
	reRigTexture * const texture = GetSelectedTexture();
	
	if(texture){
		pEditTexSkin->SetPath(texture->GetSkinPath());
		
	}else{
		pEditTexSkin->ClearPath();
	}
	
	pEditTexSkin->SetEnabled(texture);
}



reRigTexture* reWPView::GetSelectedTexture() const{
	if(!pRig){
		return nullptr;
	}
	
	const igdeListItem * const selection = pCBTexture->GetSelectedItem();
	if(!selection){
		return nullptr;
	}
	
	return (reRigTexture*)selection->GetData();
}
