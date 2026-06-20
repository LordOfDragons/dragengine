/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "igdeDialogEditCurveBezier.h"
#include "../igdeApplication.h"
#include "../igdeUIHelper.h"
#include "../igdeCheckBox.h"
#include "../igdeContainer.h"
#include "../composed/igdeEditVector2.h"
#include "../composed/igdeEditVector2Listener.h"
#include "../layout/igdeContainerForm.h"

#include <dragengine/common/exceptions.h>


// Class igdeDialogEditCurveBezier
////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDialogEditCurveBezier::igdeDialogEditCurveBezier(igdeEnvironment &environment) :
igdeDialog(environment, "@Igde.DialogEditCurveBezier.Title")
{
	SetSize(igdeApplication::app().DisplayScaled(decPoint(1000, 600)));
	pCreateContent(environment);
}

igdeDialogEditCurveBezier::igdeDialogEditCurveBezier(igdeEnvironment &environment,
const decCurveBezier &curve) :
igdeDialogEditCurveBezier(environment)
{
	SetCurve(curve);
}

igdeDialogEditCurveBezier::~igdeDialogEditCurveBezier() = default;


// Management
///////////////

const decCurveBezier &igdeDialogEditCurveBezier::GetCurve() const{
	return pViewCurveBezier->GetCurve();
}

void igdeDialogEditCurveBezier::SetCurve(const decCurveBezier &curve){
	pViewCurveBezier->SetCurve(curve);
}


// Private Functions
//////////////////////

void igdeDialogEditCurveBezier::pCreateContent(igdeEnvironment &environment){
	igdeUIHelper &helper = environment.GetUIHelper();
	
	helper.ViewCurveBezier(pViewCurveBezier, {});
	
	igdeContainer::Ref buttonBar;
	CreateButtonBar(buttonBar, "@Igde.Accept", "@Igde.Discard");
	
	AddContent(pViewCurveBezier, buttonBar);
}
