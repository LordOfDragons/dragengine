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

#include "igdeWPSky.h"
#include "../igdeUIHelper.h"
#include "../igdeCommonDialogs.h"
#include "../igdeTextField.h"
#include "../igdeButton.h"
#include "../igdeLabel.h"
#include "../igdeGroupBox.h"
#include "../igdeSlider.h"
#include "../browse/igdeDialogBrowserSky.h"
#include "../composed/igdeEditSliderText.h"
#include "../composed/igdeEditSliderTextListener.h"
#include "../event/igdeAction.h"
#include "../event/igdeTextFieldListener.h"
#include "../layout/igdeContainerForm.h"
#include "../layout/igdeContainerFlow.h"
#include "../wrapper/igdeWSky.h"
#include "../../environment/igdeEnvironment.h"
#include "../../gamedefinition/sky/igdeGDSky.h"
#include "../../gamedefinition/sky/igdeGDSkyManager.h"
#include "../../gamedefinition/igdeGameDefinition.h"
#include "../../gameproject/igdeGameProject.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyController.h>



// Actions
////////////

namespace{

class cTextPathSky : public igdeTextFieldListener{
	igdeWPSky &pPanel;
	
public:
	cTextPathSky( igdeWPSky &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		if( ! pPanel.GetSky() ){
			return;
		}
		
		pPanel.GetSky()->SetPath( textField->GetText() );
		pPanel.UpdateSky();
		pPanel.OnAction();
	}
};

class cActionFromGDSky : public igdeAction{
	igdeWPSky &pPanel;
	igdeTextField &pTextField;
	
public:
	cActionFromGDSky( igdeWPSky &panel, igdeTextField &textField ) :
	igdeAction( "GDef", "Show dialog to select sky from game definition" ),
	pPanel( panel ), pTextField( textField ){ }
	
	virtual void OnAction(){
		if( ! pPanel.GetSky() ){
			return;
		}
		
		decString sky( pTextField.GetText() );
		if( ! igdeDialogBrowserSky::SelectSky( &pPanel, sky ) ){
			return;
		}
		
		pPanel.GetSky()->SetPath( sky );
		pPanel.UpdateSky();
		pPanel.OnAction();
	}
};

class cActionPathSky : public igdeAction{
	igdeWPSky &pPanel;
	igdeTextField &pTextField;
	
public:
	cActionPathSky( igdeWPSky &panel, igdeTextField &textField ) :
	igdeAction( "...", "Show dialog to select the sky from file" ),
	pPanel( panel ), pTextField( textField ){ }
	
	virtual void OnAction(){
		if( ! pPanel.GetSky() ){
			return;
		}
		
		decString filename( pTextField.GetText().IsEmpty() ? "/igde/skies" : pTextField.GetText() );
		if( ! igdeCommonDialogs::GetFileOpen( &pPanel, "Select Sky",
		*pPanel.GetEnvironment().GetFileSystemGame(),
		*pPanel.GetEnvironment().GetOpenFilePatternList( igdeEnvironment::efpltSky ), filename ) ){
			return;
		}
		
		pPanel.GetSky()->SetPath( filename );
		pPanel.UpdateSky();
		pPanel.OnAction();
	}
};

class cEditControllerValue : public igdeEditSliderTextListener{
protected:
	igdeWPSky &pPanel;
	int pIndex;
	
public:
	cEditControllerValue( igdeWPSky &panel, int index ) :
	pPanel( panel ), pIndex( index ){ }
	
	virtual void OnSliderTextValueChanging( igdeEditSliderText *sliderText ){
		OnSliderTextValueChanged( sliderText );
	}
	
	virtual void OnSliderTextValueChanged( igdeEditSliderText *sliderText ){
		if( ! pPanel.GetSky() ){
			return;
		}
		pPanel.GetSky()->SetControllerValue( pIndex, sliderText->GetValue() );
	}
};

}



// Class igdeWPSky
////////////////////

// Constructor, destructor
////////////////////////////

igdeWPSky::igdeWPSky( igdeEnvironment &environment ) :
igdeContainerFlow( environment, igdeContainerFlow::eaY ),
pSky( NULL ),
pControllers( NULL ),
pControllerCount( 0 )
{
	pCreateContent();
}

igdeWPSky::igdeWPSky( igdeEnvironment &environment, igdeAction *action ) :
igdeContainerFlow( environment, igdeContainerFlow::eaY ),
pSky( NULL ),
pControllers( NULL ),
pControllerCount( 0 )
{
	pCreateContent();
	SetAction( action );
}

igdeWPSky::~igdeWPSky(){
	DestroyNativeWidget();
	SetAction( NULL );
	
	if( pControllers ){
		delete [] pControllers;
	}
}



// Management
///////////////

void igdeWPSky::SetSky( igdeWSky *sky ){
	if( sky == pSky ){
		return;
	}
	
	pSky = sky;
	UpdateSky();
}

void igdeWPSky::UpdateSky(){
	if( pSky ){
		pEditSkyPath->SetText( pSky->GetPath() );
		
	}else{
		pEditSkyPath->ClearText();
	}
	
	pEditSkyPath->SetEnabled( pSky != NULL );
	
	RebuildControllers();
}

void igdeWPSky::RebuildControllers(){
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pFraControllers->RemoveAllChildren();
	
	// create array holding controller widgets ( even if not all are used in the end )
	if( pControllers ){
		delete [] pControllers;
		pControllers = NULL;
	}
	pControllerCount = 0;
	
	int controllerCount = 0;
	if( pSky ){
		controllerCount = pSky->GetControllerCount();
	}
	
	if( controllerCount > 0 ){
		pControllers = new sController[ controllerCount ];
	}
	
	// create widgets
	decString name, description;
	int i;
	
	for( i=0; i<controllerCount; i++ ){
		sController &sctrl = pControllers[ pControllerCount ];
		
		if( i < controllerCount ){
			name = pSky->GetControllerAt( i ).GetName();
			
		}else{
			name.Format( "Controller #%d", i );
		}
		
		description.Format( "Current value of the controller %s", name.GetString() );
		
		sctrl.controller = i;
		helper.EditSliderText( pFraControllers, name, description, 0.0f, 1.0f, 6, 3, 0.1f,
			sctrl.slider, new cEditControllerValue( *this, i ) );
		
		pControllerCount++;
		
		UpdateController( sctrl.controller );
	}
}

void igdeWPSky::UpdateController( int controller ){
	const deSkyController &octrl = pSky->GetControllerAt( controller );
	sController &sctrl = pControllers[ controller ];
	
	const float minimum = octrl.GetMinimumValue();
	const float maximum = octrl.GetMaximumValue();
	const float value = octrl.GetCurrentValue();
	
	if( maximum > minimum ){
		sctrl.slider->SetRange( minimum, maximum );
		sctrl.slider->SetTickSpacing( ( maximum - minimum ) * 0.1f );
		sctrl.slider->SetValue( value );
		sctrl.slider->SetEnabled( true );
		
	}else{
		sctrl.slider->SetRange( 0.0f, 1.0f );
		sctrl.slider->SetTickSpacing( 0.1f );
		sctrl.slider->SetValue( 0.0f );
		sctrl.slider->SetEnabled( false );
	}
}

void igdeWPSky::UpdateControllerValue( int controller ){
	const deSkyController &octrl = pSky->GetControllerAt( controller );
	sController &sctrl = pControllers[ controller ];
	
	sctrl.slider->SetValue( octrl.GetCurrentValue() );
}



void igdeWPSky::SetAction( igdeAction *action ){
	if( pAction == action ){
		return;
	}
	
	if( pAction ){
		pAction->RemoveListener( this );
	}
	
	pAction = action;
	
	if( action ){
		action->AddListener( this );
		OnParameterChanged( action );
	}
}

void igdeWPSky::OnAction(){
	if( pAction ){
		pAction->OnAction();
	}
}

void igdeWPSky::OnParameterChanged( igdeAction *action ){
	//SetEnabled( action->GetEnabled() );
}

void igdeWPSky::OnDestroyed( igdeAction *action ){
	GetLogger()->LogWarnFormat( "IGDE", "igdeWPSky::OnDestroyed: "
		"Action(%s) destroyed while still listening on it", action->GetText().GetString() );
	pAction = NULL;
}



// Private Functions
//////////////////////

void igdeWPSky::pCreateContent(){
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference form, frameLine;
	
	form.TakeOver( new igdeContainerForm( env ) );
	AddChild( form );
	
	helper.FormLineStretchFirst( form, "Sky Path:", "", frameLine );
	helper.EditString( frameLine, "Path to the sky to use.",
		pEditSkyPath, new cTextPathSky( *this ) );
	helper.Button( frameLine, pBtnSkyPath, new cActionPathSky( *this, pEditSkyPath ), true );
	helper.Button( frameLine, pBtnFromGDSky, new cActionFromGDSky( *this, pEditSkyPath ), true );
	
	helper.GroupBox( *this, pFraControllers, "Controllers:" );
}
