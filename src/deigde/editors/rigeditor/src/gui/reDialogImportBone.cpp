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

#include "reDialogImportBone.h"
#include "reWindowMain.h"
#include "../rigeditor.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionSelectFile.h>
#include <deigde/gui/layout/igdeContainerForm.h>

#include <dragengine/common/exceptions.h>



// Events
///////////

namespace {

class cActionSelectRigPath : public igdeActionSelectFile{
	reDialogImportBone &pDialog;
public:
	cActionSelectRigPath(reDialogImportBone &dialog, igdeTextField &textField) :
	igdeActionSelectFile(dialog.GetEnvironment(), igdeEnvironment::efpltRig, textField),
	pDialog(dialog){}
};

}



// Class reDialogImportBone
/////////////////////////////

// Constructor, destructor
////////////////////////////

reDialogImportBone::reDialogImportBone(reWindowMain &windowMain) :
igdeDialog(windowMain.GetEnvironment(), "@Rig.DialogImportBone.Title"),
pWindowMain(windowMain)
{
	igdeEnvironment &env = windowMain.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	
	
	// content
	igdeContainer::Ref content, frameLine;
	content = igdeContainerForm::Ref::New(env);
	
	const char *description = "@Rig.DialogImportBone.Path.ToolTip";
	helper.FormLineStretchFirst(content, "@Rig.DialogImportBone.Path.Label", description, frameLine);
	helper.EditString(frameLine, description, pEditPath, {});
	helper.Button(frameLine, pBtnPath, igdeActionSelectFile::Ref::New(
		env, igdeEnvironment::efpltRig, pEditPath));
	
	helper.EditFloat(content, "@Rig.DialogImportBone.Scaling.Label", "@Rig.DialogImportBone.Scaling.ToolTip",
		pEditScaling, {});
	pEditScaling->SetFloat(1.0f);
	
	helper.CheckBox(content, "@Rig.DialogImportBone.ImportBoneProperties",
		"@Rig.DialogImportBone.ImportBoneProperties.ToolTip",
		pChkImportBoneProperties);
	
	helper.CheckBox(content, "@Rig.DialogImportBone.ImportShapes", "@Rig.DialogImportBone.ImportShapes.ToolTip", pChkImportShapes);
	
	helper.CheckBox(content, "@Rig.DialogImportBone.ImportConstraints", "@Rig.DialogImportBone.ImportConstraints.ToolTip", pChkImportConstraints);
	
	
	// button line
	igdeContainer::Ref buttonBar;
	CreateButtonBar(buttonBar, "@Rig.DialogImportBone.Button.Import", "@Igde.Cancel");
	
	AddContent(content, buttonBar);
}

reDialogImportBone::~reDialogImportBone(){
}



// Management
///////////////

const decString &reDialogImportBone::GetPath() const{
	return pEditPath->GetText();
}

void reDialogImportBone::SetPath(const char *path){
	pEditPath->SetText(path);
}

float reDialogImportBone::GetScaling() const{
	return pEditScaling->GetFloat();
}

void reDialogImportBone::SetScaling(float scaling){
	pEditScaling->SetFloat(scaling);
}

bool reDialogImportBone::GetImportBoneProperties() const{
	return pChkImportBoneProperties->GetChecked();
}

void reDialogImportBone::SetImportBoneProperties(bool import){
	pChkImportBoneProperties->SetChecked(import);
}

bool reDialogImportBone::GetImportShapes() const{
	return pChkImportShapes->GetChecked();
}

void reDialogImportBone::SetImportShapes(bool import){
	pChkImportShapes->SetChecked(import);
}

bool reDialogImportBone::GetImportConstraints() const{
	return pChkImportConstraints->GetChecked();
}

void reDialogImportBone::SetImportConstraints(bool import){
	pChkImportConstraints->SetChecked(import);
}



bool reDialogImportBone::Accept(){
	if(GetPath().IsEmpty()){
		igdeCommonDialogs::Error(*this, "@Rig.DialogImportBone.Error.NoPath.Title", "@Rig.DialogImportBone.Error.NoPath.Message");
		return false;
	}
	
	const float scale = GetScaling();
	if(scale <= FLOAT_SAFE_EPSILON){
		igdeCommonDialogs::Error(*this, "@Rig.DialogImportBone.Error.InvalidScaling.Title", "@Rig.DialogImportBone.Error.InvalidScaling.Message");
		return false;
	}
	
	return igdeDialog::Accept();
}
