/* 
 * Drag[en]gine IGDE World Editor
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

#include "meWPSensors.h"
#include "meWindowProperties.h"
#include "../meWindowMain.h"
#include "../../configuration/meConfiguration.h"
#include "../../world/meWorld.h"
#include "../../world/meLumimeter.h"
#include "../../worldedit.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace{

class cActionLMTrackCam : public igdeAction{
	meWPSensors &pPanel;
	
public:
	cActionLMTrackCam( meWPSensors &panel ) : igdeAction( "Track Camera", "Track camera." ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		meWorld * const world = pPanel.GetWindowProperties().GetWindowMain().GetWorld();
		meLumimeter * const lumimeter = world ? world->GetLumimeter() : NULL;
		if( lumimeter ){
			lumimeter->SetTrackCamera( ! lumimeter->GetTrackCamera() );
		}
	}
};

class cEditLMPos : public igdeEditVectorListener{
	meWPSensors &pPanel;
	
public:
	cEditLMPos( meWPSensors &panel ) : pPanel( panel ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		meWorld * const world = pPanel.GetWindowProperties().GetWindowMain().GetWorld();
		meLumimeter * const lumimeter = world ? world->GetLumimeter() : NULL;
		if( lumimeter ){
			lumimeter->SetPosition( editVector->GetVector() );
		}
		
	}
};

class cEditLMDir : public igdeEditVectorListener{
	meWPSensors &pPanel;
	
public:
	cEditLMDir( meWPSensors &panel ) : pPanel( panel ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		meWorld * const world = pPanel.GetWindowProperties().GetWindowMain().GetWorld();
		meLumimeter * const lumimeter = world ? world->GetLumimeter() : NULL;
		if( lumimeter ){
			lumimeter->SetDirection( editVector->GetVector() );
		}
		
	}
};

class cTextLMConeIA : public igdeTextFieldListener{
	meWPSensors &pPanel;
	
public:
	cTextLMConeIA( meWPSensors &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		meWorld * const world = pPanel.GetWindowProperties().GetWindowMain().GetWorld();
		meLumimeter * const lumimeter = world ? world->GetLumimeter() : NULL;
		if( lumimeter ){
			lumimeter->SetConeInnerAngle( textField->GetFloat() );
		}
	}
};

class cTextLMConeOA : public igdeTextFieldListener{
	meWPSensors &pPanel;
	
public:
	cTextLMConeOA( meWPSensors &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		meWorld * const world = pPanel.GetWindowProperties().GetWindowMain().GetWorld();
		meLumimeter * const lumimeter = world ? world->GetLumimeter() : NULL;
		if( lumimeter ){
			lumimeter->SetConeOuterAngle( textField->GetFloat() );
		}
	}
};

class cTextLMConeExp : public igdeTextFieldListener{
	meWPSensors &pPanel;
	
public:
	cTextLMConeExp( meWPSensors &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		meWorld * const world = pPanel.GetWindowProperties().GetWindowMain().GetWorld();
		meLumimeter * const lumimeter = world ? world->GetLumimeter() : NULL;
		if( lumimeter ){
			lumimeter->SetConeExponent( textField->GetFloat() );
		}
	}
};

}



// Class meWPSensors
//////////////////////

// Constructor, destructor
////////////////////////////

meWPSensors::meWPSensors( meWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference content, groupBox;
	
// 	pListener = new meWPViewListener( *this );
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	
	// lumimeter
	helper.GroupBox( content, groupBox, "Lumimeter:" );
	
	helper.CheckBox( groupBox, pChkLMTrackCam, new cActionLMTrackCam( *this ), true );
	helper.EditVector( groupBox, "Position:", "Position", pEditLMPos, new cEditLMPos( *this ) );
	helper.EditVector( groupBox, "Direction:", "Direction", pEditLMDir, new cEditLMDir( *this ) );
	helper.EditFloat( groupBox, "Inner Angle:", "Cone inner angle in degrees",
		pEditLMConeIA, new cTextLMConeIA( *this ) );
	helper.EditFloat( groupBox, "Outer Angle:", "Cone outer angle in degrees",
		pEditLMConeOA, new cTextLMConeOA( *this ) );
	helper.EditFloat( groupBox, "Exponent:",
		"Cone exponent smoothing between inner and outer angle",
		pEditLMConeExp, new cTextLMConeExp( *this ) );
	
	helper.EditFloat( groupBox, "Measured Luminance:", "Measured luminance", pEditLMLumi, NULL );
	pEditLMLumi->SetEditable( false );
	helper.ColorBox( groupBox, "Measured Color:", "Measured color", pEditLMColor, NULL );
	pEditLMColor->SetEnabled( false );
}

meWPSensors::~meWPSensors(){
}



// Management
///////////////

void meWPSensors::UpdateSensors(){
	UpdateLumimeter();
}

void meWPSensors::UpdateLumimeter(){
	meWorld * const world = pWindowProperties.GetWindowMain().GetWorld();
	meLumimeter * lumimeter = world ? world->GetLumimeter() : NULL;
	
	if( lumimeter ){
		pChkLMTrackCam->SetChecked( lumimeter->GetTrackCamera() );
		pEditLMPos->SetVector( lumimeter->GetPosition() );
		pEditLMDir->SetVector( lumimeter->GetDirection() );
		pEditLMConeIA->SetFloat( lumimeter->GetConeInnerAngle() );
		pEditLMConeOA->SetFloat( lumimeter->GetConeOuterAngle() );
		pEditLMConeExp->SetFloat( lumimeter->GetConeExponent() );
		
		pEditLMLumi->SetFloat( lumimeter->MeasureLuminance() );
		pEditLMColor->SetColor( lumimeter->MeasureColor() );
		
	}else{
		pChkLMTrackCam->SetChecked( false );
		pEditLMPos->SetVector( decVector() );
		pEditLMDir->SetVector( decVector() );
		pEditLMConeIA->ClearText();
		pEditLMConeOA->ClearText();
		pEditLMConeExp->ClearText();
		pEditLMLumi->ClearText();
		pEditLMColor->SetColor( decColor() );
	}
	
	const bool enable = lumimeter;
	pChkLMTrackCam->SetEnabled( enable );
	pEditLMPos->SetEnabled( enable );
	pEditLMDir->SetEnabled( enable );
	pEditLMConeIA->SetEnabled( enable );
	pEditLMConeOA->SetEnabled( enable );
	pEditLMConeExp->SetEnabled( enable );
}
