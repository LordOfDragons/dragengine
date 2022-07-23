/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "seViewSynthesizer.h"
#include "seWindowMain.h"
#include "properties/seWPSynthesizer.h"
#include "properties/seWPController.h"
#include "properties/seWPLink.h"
#include "properties/seWPSource.h"
#include "../synthesizer/seSynthesizer.h"
#include "../configuration/seConfiguration.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/theme/themeNames.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class seViewSynthesizer
////////////////////////////

// Constructor, destructor
////////////////////////////

seViewSynthesizer::seViewSynthesizer( seWindowMain &windowMain ) :
igdeContainerBorder( windowMain.GetEnvironment(), 10 ),
pWindowMain( windowMain ),
pSynthesizer( NULL ),
pWPController( NULL ),
pWPLink( NULL ),
pWPSource( NULL ),
pWPSynthesizer( NULL )
{
	igdeEnvironment &env = windowMain.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	SetWidgetGuiThemeName( igdeGuiThemeNames::properties );
	
	// content
	igdeContainerReference panels;
	panels.TakeOver( new igdeContainerBox( env, igdeContainerBox::eaX, 10 ) );
	AddChild( panels, eaCenter );
	
	pWPController = new seWPController( *this );
	panels->AddChild( pWPController );
	helper.Separator( panels, false );
	
	pWPLink = new seWPLink( *this );
	panels->AddChild( pWPLink );
	helper.Separator( panels, false );
	
	pWPSource = new seWPSource( *this );
	panels->AddChild( pWPSource );
	helper.Separator( panels, false );
	
	// right
	pWPSynthesizer = new seWPSynthesizer( *this );
	AddChild( pWPSynthesizer, eaRight );
}

seViewSynthesizer::~seViewSynthesizer(){
	SetSynthesizer( NULL );
	
	if( pWPController ){
		pWPController->FreeReference();
	}
	if( pWPLink ){
		pWPLink->FreeReference();
	}
	if( pWPSource ){
		pWPSource->FreeReference();
	}
	if( pWPSynthesizer ){
		pWPSynthesizer->FreeReference();
	}
}



// Management
///////////////

void seViewSynthesizer::SetSynthesizer( seSynthesizer *synthesizer ){
	if( synthesizer == pSynthesizer ){
		return;
	}
	
	if( pSynthesizer ){
		pSynthesizer->FreeReference();
	}
	
	pSynthesizer = synthesizer;
	
	if( synthesizer ){
		synthesizer->AddReference();
	}
	
	pWPController->SetSynthesizer( synthesizer );
	pWPLink->SetSynthesizer( synthesizer );
	pWPSource->SetSynthesizer( synthesizer );
	pWPSynthesizer->SetSynthesizer( synthesizer );
}

void seViewSynthesizer::OnSynthesizerPathChanged(){
	pWPSource->OnSynthesizerPathChanged();
}



void seViewSynthesizer::ResetView(){
}

void seViewSynthesizer::OnFrameUpdate( float elapsed ){
	if( pSynthesizer ){
		pSynthesizer->Update( elapsed );
	}
}
