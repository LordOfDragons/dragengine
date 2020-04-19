/* 
 * Drag[en]gine IGDE
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

#include <stdio.h>
#include <stdlib.h>

#include "igdeDialogCurveBezierClamp.h"
#include "../igdeUIHelper.h"
#include "../igdeCheckBox.h"
#include "../igdeContainerReference.h"
#include "../composed/igdeEditVector2.h"
#include "../layout/igdeContainerForm.h"

#include <dragengine/common/exceptions.h>



// Class igdeDialogCurveBezierClamp
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDialogCurveBezierClamp::igdeDialogCurveBezierClamp( igdeEnvironment &environment ) :
igdeDialog( environment, "Clamp Curve" )
{
	pCreateContent( environment );
}

igdeDialogCurveBezierClamp::igdeDialogCurveBezierClamp( igdeEnvironment &environment,
bool clamp, const decVector2 &minimum, const decVector2 &maximum ) :
igdeDialog( environment, "Clamp Curve" )
{
	pCreateContent( environment );
	pChkClamp->SetChecked( clamp );
	pEditMinimum->SetVector2( minimum );
	pEditMaximum->SetVector2( maximum );
}

igdeDialogCurveBezierClamp::~igdeDialogCurveBezierClamp(){
}



// Management
///////////////

bool igdeDialogCurveBezierClamp::GetClamp() const{
	return pChkClamp->GetChecked();
}

void igdeDialogCurveBezierClamp::SetClamp( bool clamp ){
	pChkClamp->SetChecked( clamp );
}

decVector2 igdeDialogCurveBezierClamp::GetClampMin() const{
	return pEditMinimum->GetVector2();
}

void igdeDialogCurveBezierClamp::SetClampMin( const decVector2 &minimum ){
	pEditMinimum->SetVector2( minimum );
}

decVector2 igdeDialogCurveBezierClamp::GetClampMax() const{
	return pEditMaximum->GetVector2();
}

void igdeDialogCurveBezierClamp::SetClampMax( const decVector2 &maximum ){
	pEditMaximum->SetVector2( maximum );
}



// Private Functions
//////////////////////

void igdeDialogCurveBezierClamp::pCreateContent( igdeEnvironment &environment ){
	igdeUIHelper &helper = environment.GetUIHelper();
	
	igdeContainerReference content;
	content.TakeOver( new igdeContainerForm( environment ) );
	
	helper.CheckBox( content, "Enable Clamping", "Curve points are clamped to minimum/maximum range", pChkClamp );
	helper.EditVector2( content, "Minimum:", "Minimum coordinates to clamp curve points to", pEditMinimum, NULL );
	helper.EditVector2( content, "Maximum:", "Maximum coordinates to clamp curve points to", pEditMaximum, NULL );
	
	igdeContainerReference buttonBar;
	CreateButtonBar( buttonBar, "Accept", "Discard" );
	
	AddContent( content, buttonBar );
}
