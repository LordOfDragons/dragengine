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

#include "igdeDialogCurveBezierCoord.h"
#include "../igdeUIHelper.h"
#include "../igdeCheckBox.h"
#include "../igdeContainer.h"
#include "../composed/igdeEditVector2.h"
#include "../composed/igdeEditVector2Listener.h"
#include "../layout/igdeContainerForm.h"

#include <dragengine/common/exceptions.h>



// Actions
////////////

class igdeDialogCurveBezierCoordEditPoint : public igdeEditVector2Listener {
	igdeDialogCurveBezierCoord &pDialog;
	
public:
	igdeDialogCurveBezierCoordEditPoint(igdeDialogCurveBezierCoord &dialog) :
	pDialog(dialog){}
	
	virtual void OnVector2Changed(igdeEditVector2 *editVector2){
		const decCurveBezierPoint &point = pDialog.GetPoint();
		const decVector2 diff(editVector2->GetVector2() - point.GetPoint());
		pDialog.SetPoint(decCurveBezierPoint(editVector2->GetVector2(),
			point.GetHandle1() + diff, point.GetHandle2() + diff));
	}
};

class igdeDialogCurveBezierCoordEditHandle1 : public igdeEditVector2Listener {
	igdeDialogCurveBezierCoord &pDialog;
	
public:
	igdeDialogCurveBezierCoordEditHandle1(igdeDialogCurveBezierCoord &dialog) :
	pDialog(dialog){}
	
	virtual void OnVector2Changed(igdeEditVector2 *editVector2){
		const decCurveBezierPoint &point = pDialog.GetPoint();
		pDialog.SetPoint(decCurveBezierPoint(point.GetPoint(),
			editVector2->GetVector2(), point.GetHandle2()));
	}
};

class igdeDialogCurveBezierCoordEditHandle2 : public igdeEditVector2Listener {
	igdeDialogCurveBezierCoord &pDialog;
	
public:
	igdeDialogCurveBezierCoordEditHandle2(igdeDialogCurveBezierCoord &dialog) :
	pDialog(dialog){}
	
	virtual void OnVector2Changed(igdeEditVector2 *editVector2){
		const decCurveBezierPoint &point = pDialog.GetPoint();
		pDialog.SetPoint(decCurveBezierPoint(point.GetPoint(),
			point.GetHandle1(), editVector2->GetVector2()));
	}
};



// Class igdeDialogCurveBezierCoord
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDialogCurveBezierCoord::igdeDialogCurveBezierCoord(igdeEnvironment &environment) :
igdeDialog(environment, "Point Coordinates")
{
	pCreateContent(environment);
}

igdeDialogCurveBezierCoord::igdeDialogCurveBezierCoord(igdeEnvironment &environment,
const decCurveBezierPoint &point) :
igdeDialog(environment, "Point Coordinates")
{
	pCreateContent(environment);
	SetPoint(point);
}

igdeDialogCurveBezierCoord::~igdeDialogCurveBezierCoord(){
}



// Management
///////////////

void igdeDialogCurveBezierCoord::SetPoint(const decCurveBezierPoint &point){
	if(point.IsEqualTo(pPoint)){
		return;
	}
	
	pPoint = point;
	pEditPoint->SetVector2(point.GetPoint());
	pEditHandle1->SetVector2(point.GetHandle1());
	pEditHandle2->SetVector2(point.GetHandle2());
}



// Private Functions
//////////////////////

void igdeDialogCurveBezierCoord::pCreateContent(igdeEnvironment &environment){
	igdeUIHelper &helper = environment.GetUIHelper();
	
	igdeContainerForm::Ref content(igdeContainerForm::Ref::New(environment));
	
	helper.EditVector2(content, "Point:", "Point coordinates",
		pEditPoint, new igdeDialogCurveBezierCoordEditPoint(*this));
	helper.EditVector2(content, "Left Handle:", "Left handle coordinates",
		pEditHandle1, new igdeDialogCurveBezierCoordEditHandle1(*this));
	helper.EditVector2(content, "Right Handle:", "Right handle coordinates",
		pEditHandle2, new igdeDialogCurveBezierCoordEditHandle2(*this));
	
	igdeContainer::Ref buttonBar;
	CreateButtonBar(buttonBar, "Accept", "Discard");
	
	AddContent(content, buttonBar);
}
