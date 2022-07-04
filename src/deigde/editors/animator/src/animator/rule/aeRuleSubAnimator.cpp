/* 
 * Drag[en]gine IGDE Animator Editor
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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "aeRuleSubAnimator.h"
#include "../aeAnimator.h"
#include "../controller/aeController.h"
#include "../link/aeLink.h"
#include "../../gui/aeWindowMain.h"
#include "../../loadsave/aeLoadSaveSystem.h"

#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/curve/decCurveBezierPoint.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorManager.h>
#include "dragengine/resources/animator/deAnimatorLink.h"
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleSubAnimator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>



// Definitions
////////////////

#define LOGSOURCE "Animator Editor"



// Class aeRuleSubAnimator
////////////////////////////

// Constructor, destructor
////////////////////////////

aeRuleSubAnimator::aeRuleSubAnimator() :
aeRule( deAnimatorRuleVisitorIdentify::ertSubAnimator ),
pSubAnimator( NULL ),
pEnablePosition( true ),
pEnableOrientation( true ),
pEnableSize( true )
{
	SetName( "Sub Animator" );
}

aeRuleSubAnimator::aeRuleSubAnimator( const aeRuleSubAnimator &copy ) :
aeRule( copy ),
pPathSubAnimator( copy.pPathSubAnimator ),
pSubAnimator( NULL ),
pEnablePosition( copy.pEnablePosition ),
pEnableOrientation( copy.pEnableOrientation ),
pEnableSize( copy.pEnableSize ),
pConnections( copy.pConnections )
{
	pSubAnimator = copy.pSubAnimator;
	if( pSubAnimator ){
		pSubAnimator->AddReference();
	}
}

aeRuleSubAnimator::~aeRuleSubAnimator(){
	if( pSubAnimator ) pSubAnimator->FreeReference();
}



// Management
///////////////

void aeRuleSubAnimator::SetPathSubAnimator( const char *path ){
	if( pPathSubAnimator == path ){
		return;
	}
	
	pPathSubAnimator = path;
	NotifyRuleChanged();
}

void aeRuleSubAnimator::LoadSubAnimator(){
	// clear connections only if not manually set
	int i, count = pConnections.GetCount();
	for( i=0; i<count; i++ ){
		if( pConnections.GetAt( i ) ){
			break;
		}
	}
	
	const bool autoConnections = i == count;
	if( autoConnections ){
		pConnections.RemoveAll();
	}
	
	// release the sub animator
	if( pSubAnimator ){
		pSubAnimator->FreeReference();
		pSubAnimator = NULL;
	}
	
	// if there is no parent animator no loading can be done
	aeAnimator * const parentAnimator = GetAnimator();
	if( ! parentAnimator ){
		return;
	}
	
	// otherwise try loading the animator
	deAnimatorRuleSubAnimator *rule = ( deAnimatorRuleSubAnimator* )GetEngineRule();
	deEngine *engine = parentAnimator->GetEngine();
	deAnimatorController *engController = NULL;
	deAnimatorLink *engLink = NULL;
	deAnimatorRule *engRule = NULL;
	aeAnimator *animator = NULL;
	int c, controllerCount;
	int l, linkCount;
	int r, ruleCount;
	
	// try to load the animator
	if( ! pPathSubAnimator.IsEmpty() ){
		parentAnimator->GetLogger()->LogInfoFormat( LOGSOURCE,
			"Rule Sub Animator: Loading animator %s...", pPathSubAnimator.GetString() );
		
		try{
			// load from file
			animator = parentAnimator->GetWindowMain().GetLoadSaveSystem().LoadAnimator(
				decPath::AbsolutePathUnix( pPathSubAnimator, parentAnimator->GetDirectoryPath() ).GetPathUnix() );
			
			controllerCount = animator->GetControllers().GetCount();
			linkCount = animator->GetLinks().GetCount();
			ruleCount = animator->GetRules().GetCount();
			
			// create animator
			pSubAnimator = engine->GetAnimatorManager()->CreateAnimator();
			pSubAnimator->SetRig( animator->GetEngineAnimator()->GetRig() );
			pSubAnimator->SetAnimation( animator->GetEngineAnimator()->GetAnimation() );
			
			// add controllers
			for( c=0; c<controllerCount; c++ ){
				const aeController &controller = *animator->GetControllers().GetAt( c );
				
				engController = new deAnimatorController;
				engController->SetName( controller.GetName() );
				engController->SetValueRange( controller.GetMinimumValue(), controller.GetMaximumValue() );
				engController->SetFrozen( controller.GetFrozen() );
				engController->SetClamp( controller.GetClamp() );
				engController->SetVector( controller.GetVector() );
				
				pSubAnimator->AddController( engController );
				engController = NULL;
			}
			
			// add links
			for( l=0; l<linkCount; l++ ){
				const aeLink &link = *animator->GetLinks().GetAt( l );
				
				engLink = new deAnimatorLink;
				
				if( link.GetController() ){
					engLink->SetController( animator->GetControllers().IndexOf( link.GetController() ) );
					
				}else{
					engLink->SetController( -1 );
				}
				
				engLink->SetCurve( link.GetCurve() );
				engLink->SetRepeat( link.GetRepeat() );
				
				pSubAnimator->AddLink( engLink );
				engLink = NULL;
			}
			
			// add rules
			for( r=0; r<ruleCount; r++ ){
				engRule = animator->GetRules().GetAt( r )->CreateEngineRule();
				if( ! engRule ) DETHROW( deeOutOfMemory );
				
				pSubAnimator->AddRule( engRule );
				engRule->FreeReference();
				engRule = NULL;
			}
			
			// free the loaded animator as it is no more needed
			animator->FreeReference();
			
		}catch( const deException &e ){
			parentAnimator->GetLogger()->LogException( "Animator Editor", e );
			
			if( engRule ){
				engRule->FreeReference();
			}
			if( engLink ){
				delete engLink;
			}
			if( engController ){
				delete engController;
			}
			if( animator ){
				animator->FreeReference();
			}
		}
	}
	
	if( pSubAnimator && autoConnections ){
		while( pConnections.GetCount() < pSubAnimator->GetControllerCount() ){
			pConnections.Add( NULL );
		}
	}
	
	// if the engine rule exists assign sub animator
	if( ! rule ){
		return;
	}
	
	rule->SetSubAnimator( pSubAnimator );
	
	pUpdateConnections( *rule );
	NotifyRuleChanged();
}



int aeRuleSubAnimator::GetConnectionCount() const{
	return pConnections.GetCount();
}

aeController *aeRuleSubAnimator::GetControllerAt( int position ) const{
	return ( aeController* )pConnections.GetAt( position );
}

void aeRuleSubAnimator::SetControllerAt( int position, aeController *controller ){
	if( controller == ( aeController* )pConnections.GetAt( position ) ){
		return;
	}
	
	pConnections.SetAt( position, controller );
	
	deAnimatorRuleSubAnimator * const rule = ( deAnimatorRuleSubAnimator* )GetEngineRule();
	if( rule ){
		pUpdateConnections( *rule );
	}
	
	NotifyRuleChanged();
}



void aeRuleSubAnimator::SetEnablePosition( bool enabled ){
	if( enabled != pEnablePosition ){
		pEnablePosition = enabled;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleSubAnimator* )GetEngineRule() )->SetEnablePosition( enabled );
			NotifyRuleChanged();
		}
	}
}

void aeRuleSubAnimator::SetEnableOrientation( bool enabled ){
	if( enabled != pEnableOrientation ){
		pEnableOrientation = enabled;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleSubAnimator* )GetEngineRule() )->SetEnableOrientation( enabled );
			NotifyRuleChanged();
		}
	}
}

void aeRuleSubAnimator::SetEnableSize( bool enabled ){
	if( enabled != pEnableSize ){
		pEnableSize = enabled;
		
		if( GetEngineRule() ){
			( ( deAnimatorRuleSubAnimator* )GetEngineRule() )->SetEnableSize( enabled );
			NotifyRuleChanged();
		}
	}
}




deAnimatorRule *aeRuleSubAnimator::CreateEngineRule(){
	deAnimatorRuleSubAnimator *engRule = NULL;
	
	try{
		engRule = new deAnimatorRuleSubAnimator;
		
		InitEngineRule( engRule );
		
		engRule->SetSubAnimator( pSubAnimator );
		
		pUpdateConnections( *engRule );
		
		engRule->SetEnablePosition( pEnablePosition );
		engRule->SetEnableOrientation( pEnableOrientation );
		engRule->SetEnableSize( pEnableSize );
		
	}catch( const deException & ){
		if( engRule ){
			engRule->FreeReference();
		}
		throw;
	}
	
	// finished
	return engRule;
}



void aeRuleSubAnimator::UpdateCompAnim(){
	deAnimatorRuleSubAnimator * const rule = ( deAnimatorRuleSubAnimator* )GetEngineRule();
	if( rule ){
		rule->SetSubAnimator( pSubAnimator );
		NotifyRuleChanged();
	}
}



// Operators
//////////////

aeRuleSubAnimator &aeRuleSubAnimator::operator=( const aeRuleSubAnimator &copy ){
	SetPathSubAnimator( copy.pPathSubAnimator );
	
	SetEnablePosition( copy.pEnablePosition );
	SetEnableOrientation( copy.pEnableOrientation );
	SetEnableSize( copy.pEnableSize );
	
	if( pSubAnimator ){
		pSubAnimator->FreeReference();
		pSubAnimator = NULL;
	}
	pSubAnimator = copy.pSubAnimator;
	if( pSubAnimator ){
		pSubAnimator->AddReference();
	}
	
	pConnections = copy.pConnections;
	
	deAnimatorRuleSubAnimator * const rule = ( deAnimatorRuleSubAnimator* )GetEngineRule();
	if( rule ){
		pUpdateConnections( *rule );
	}
	
	aeRule::operator=( copy );
	return *this;
}



aeRule *aeRuleSubAnimator::CreateCopy() const{
	return new aeRuleSubAnimator( *this );
}

void aeRuleSubAnimator::ListLinks( aeLinkList &list ){
	aeRule::ListLinks( list );
}

void aeRuleSubAnimator::OnParentAnimatorChanged(){
	LoadSubAnimator();
}



// Private Functions
//////////////////////

void aeRuleSubAnimator::pUpdateConnections( deAnimatorRuleSubAnimator &rule ) const{
	const int count = pConnections.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( pConnections.GetAt( i ) ){
			break;
		}
	}
	
	if( i == count ){
		// auto connections
		if( GetAnimator() ){
			const aeControllerList &controllers = GetAnimator()->GetControllers();
			for( i=0; i<count; i++ ){
				rule.SetConnectionAt( i, controllers.IndexOfNamed( pSubAnimator->GetControllerAt( i )->GetName() ) );
			}
			
		}else{
			rule.ClearConnections();
		}
		
	}else{
		// custom connections
		for( i=0; i<count; i++ ){
			int engController = -1;
			if( pConnections[ i ] ){
				engController = ( ( aeController* )pConnections.GetAt( i ) )->GetEngineControllerIndex();
			}
			rule.SetConnectionAt( i, engController );
		}
	}
}
