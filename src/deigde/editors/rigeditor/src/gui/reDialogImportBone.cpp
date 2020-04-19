/* 
 * Drag[en]gine IGDE Rig Editor
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

#include "reDialogImportBone.h"
#include "reWindowMain.h"
#include "../rigeditor.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeContainerReference.h>
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
	cActionSelectRigPath( reDialogImportBone &dialog, igdeTextField &textField ) :
	igdeActionSelectFile( dialog.GetEnvironment(), igdeEnvironment::efpltRig, textField ),
	pDialog( dialog ){ }
};

}



// Class reDialogImportBone
/////////////////////////////

// Constructor, destructor
////////////////////////////

reDialogImportBone::reDialogImportBone( reWindowMain &windowMain ) :
igdeDialog( windowMain.GetEnvironment(), "Import Bones" ),
pWindowMain( windowMain )
{
	igdeEnvironment &env = windowMain.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	
	
	// content
	igdeContainerReference content, frameLine;
	content.TakeOver( new igdeContainerForm( env ) );
	
	const char *description = "File to import selected bones from.";
	helper.FormLineStretchFirst( content, "Path:", description, frameLine );
	helper.EditString( frameLine, description, pEditPath, NULL );
	helper.Button( frameLine, pBtnPath, new igdeActionSelectFile(
		env, igdeEnvironment::efpltRig, pEditPath ), true );
	
	helper.EditFloat( content, "Scaling:", "Scaling to apply to the imported objects.",
		pEditScaling, NULL );
	pEditScaling->SetFloat( 1.0f );
	
	helper.CheckBox( content, "Import bone properties",
		"Import bone properties (central mass point, mass, dynamic, ...)",
		pChkImportBoneProperties );
	
	helper.CheckBox( content, "Import shapes", "Import shapes", pChkImportShapes );
	
	helper.CheckBox( content, "Import constraints", "Import constraints", pChkImportConstraints );
	
	
	// button line
	igdeContainerReference buttonBar;
	CreateButtonBar( buttonBar, "Import", "Cancel" );
	
	AddContent( content, buttonBar );
}

reDialogImportBone::~reDialogImportBone(){
}



// Management
///////////////

const decString &reDialogImportBone::GetPath() const{
	return pEditPath->GetText();
}

void reDialogImportBone::SetPath( const char *path ){
	pEditPath->SetText( path );
}

float reDialogImportBone::GetScaling() const{
	return pEditScaling->GetFloat();
}

void reDialogImportBone::SetScaling( float scaling ){
	pEditScaling->SetFloat( scaling );
}

bool reDialogImportBone::GetImportBoneProperties() const{
	return pChkImportBoneProperties->GetChecked();
}

void reDialogImportBone::SetImportBoneProperties( bool import ){
	pChkImportBoneProperties->SetChecked( import );
}

bool reDialogImportBone::GetImportShapes() const{
	return pChkImportShapes->GetChecked();
}

void reDialogImportBone::SetImportShapes( bool import ){
	pChkImportShapes->SetChecked( import );
}

bool reDialogImportBone::GetImportConstraints() const{
	return pChkImportConstraints->GetChecked();
}

void reDialogImportBone::SetImportConstraints( bool import ){
	pChkImportConstraints->SetChecked( import );
}



bool reDialogImportBone::Accept(){
	if( GetPath().IsEmpty() ){
		igdeCommonDialogs::Error( this, "Invalid value", "No path set." );
		return false;
	}
	
	const float scale = GetScaling();
	if( scale <= FLOAT_SAFE_EPSILON ){
		igdeCommonDialogs::Error( this, "Invalid value", "Scaling has to be 0 or higher." );
		return false;
	}
	
	return true;
}
