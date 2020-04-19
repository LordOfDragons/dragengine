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

#include "igdeDialogCurveBezierCoord.h"
#include "../igdeUIHelper.h"
#include "../igdeCheckBox.h"
#include "../igdeContainerReference.h"
#include "../composed/igdeEditVector2.h"
#include "../composed/igdeEditVector2Listener.h"
#include "../layout/igdeContainerForm.h"

#include <dragengine/common/exceptions.h>



// Actions
////////////

class igdeDialogCurveBezierCoordEditPoint : public igdeEditVector2Listener {
	igdeDialogCurveBezierCoord &pDialog;
	
public:
	igdeDialogCurveBezierCoordEditPoint( igdeDialogCurveBezierCoord &dialog ) :
	pDialog( dialog ){ }
	
	virtual void OnVector2Changed( igdeEditVector2 *editVector2 ){
		const decCurveBezierPoint &point = pDialog.GetPoint();
		const decVector2 diff( editVector2->GetVector2() - point.GetPoint() );
		pDialog.SetPoint( decCurveBezierPoint( editVector2->GetVector2(),
			point.GetHandle1() + diff, point.GetHandle2() + diff ) );
	}
};

class igdeDialogCurveBezierCoordEditHandle1 : public igdeEditVector2Listener {
	igdeDialogCurveBezierCoord &pDialog;
	
public:
	igdeDialogCurveBezierCoordEditHandle1( igdeDialogCurveBezierCoord &dialog ) :
	pDialog( dialog ){ }
	
	virtual void OnVector2Changed( igdeEditVector2 *editVector2 ){
		const decCurveBezierPoint &point = pDialog.GetPoint();
		pDialog.SetPoint( decCurveBezierPoint( point.GetPoint(),
			editVector2->GetVector2(), point.GetHandle2() ) );
	}
};

class igdeDialogCurveBezierCoordEditHandle2 : public igdeEditVector2Listener {
	igdeDialogCurveBezierCoord &pDialog;
	
public:
	igdeDialogCurveBezierCoordEditHandle2( igdeDialogCurveBezierCoord &dialog ) :
	pDialog( dialog ){ }
	
	virtual void OnVector2Changed( igdeEditVector2 *editVector2 ){
		const decCurveBezierPoint &point = pDialog.GetPoint();
		pDialog.SetPoint( decCurveBezierPoint( point.GetPoint(),
			point.GetHandle1(), editVector2->GetVector2() ) );
	}
};



// Class igdeDialogCurveBezierCoord
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDialogCurveBezierCoord::igdeDialogCurveBezierCoord( igdeEnvironment &environment ) :
igdeDialog( environment, "Point Coordinates" )
{
	pCreateContent( environment );
}

igdeDialogCurveBezierCoord::igdeDialogCurveBezierCoord( igdeEnvironment &environment,
const decCurveBezierPoint &point ) :
igdeDialog( environment, "Point Coordinates" )
{
	pCreateContent( environment );
	SetPoint( point );
}

igdeDialogCurveBezierCoord::~igdeDialogCurveBezierCoord(){
}



// Management
///////////////

void igdeDialogCurveBezierCoord::SetPoint( const decCurveBezierPoint &point ){
	if( point.IsEqualTo( pPoint ) ){
		return;
	}
	
	pPoint = point;
	pEditPoint->SetVector2( point.GetPoint() );
	pEditHandle1->SetVector2( point.GetHandle1() );
	pEditHandle2->SetVector2( point.GetHandle2() );
}



// Private Functions
//////////////////////

void igdeDialogCurveBezierCoord::pCreateContent( igdeEnvironment &environment ){
	igdeUIHelper &helper = environment.GetUIHelper();
	
	igdeContainerReference content;
	content.TakeOver( new igdeContainerForm( environment ) );
	
	helper.EditVector2( content, "Point:", "Point coordinates",
		pEditPoint, new igdeDialogCurveBezierCoordEditPoint( *this ) );
	helper.EditVector2( content, "Left Handle:", "Left handle coordinates",
		pEditHandle1, new igdeDialogCurveBezierCoordEditHandle1( *this ) );
	helper.EditVector2( content, "Right Handle:", "Right handle coordinates",
		pEditHandle2, new igdeDialogCurveBezierCoordEditHandle2( *this ) );
	
	igdeContainerReference buttonBar;
	CreateButtonBar( buttonBar, "Accept", "Discard" );
	
	AddContent( content, buttonBar );
}
