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

#include "igdeDialogCurveBezierClamp.h"
#include "../igdeUIHelper.h"
#include "../igdeCheckBox.h"
#include "../igdeContainer.h"
#include "../composed/igdeEditVector2.h"
#include "../layout/igdeContainerForm.h"

#include <dragengine/common/exceptions.h>



// Class igdeDialogCurveBezierClamp
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDialogCurveBezierClamp::igdeDialogCurveBezierClamp(igdeEnvironment &environment) :
igdeDialog(environment, "Clamp Curve")
{
	pCreateContent(environment);
}

igdeDialogCurveBezierClamp::igdeDialogCurveBezierClamp(igdeEnvironment &environment,
bool clamp, const decVector2 &minimum, const decVector2 &maximum) :
igdeDialog(environment, "Clamp Curve")
{
	pCreateContent(environment);
	pChkClamp->SetChecked(clamp);
	pEditMinimum->SetVector2(minimum);
	pEditMaximum->SetVector2(maximum);
}

igdeDialogCurveBezierClamp::~igdeDialogCurveBezierClamp(){
}



// Management
///////////////

bool igdeDialogCurveBezierClamp::GetClamp() const{
	return pChkClamp->GetChecked();
}

void igdeDialogCurveBezierClamp::SetClamp(bool clamp){
	pChkClamp->SetChecked(clamp);
}

decVector2 igdeDialogCurveBezierClamp::GetClampMin() const{
	return pEditMinimum->GetVector2();
}

void igdeDialogCurveBezierClamp::SetClampMin(const decVector2 &minimum){
	pEditMinimum->SetVector2(minimum);
}

decVector2 igdeDialogCurveBezierClamp::GetClampMax() const{
	return pEditMaximum->GetVector2();
}

void igdeDialogCurveBezierClamp::SetClampMax(const decVector2 &maximum){
	pEditMaximum->SetVector2(maximum);
}



// Private Functions
//////////////////////

void igdeDialogCurveBezierClamp::pCreateContent(igdeEnvironment &environment){
	igdeUIHelper &helper = environment.GetUIHelper();
	
	igdeContainerForm::Ref content(igdeContainerForm::Ref::New(environment));
	
	helper.CheckBox(content, "Enable Clamping", "Curve points are clamped to minimum/maximum range", pChkClamp);
	helper.EditVector2(content, "Minimum:", "Minimum coordinates to clamp curve points to", pEditMinimum, NULL);
	helper.EditVector2(content, "Maximum:", "Maximum coordinates to clamp curve points to", pEditMaximum, NULL);
	
	igdeContainer::Ref buttonBar;
	CreateButtonBar(buttonBar, "Accept", "Discard");
	
	AddContent(content, buttonBar);
}
