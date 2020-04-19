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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "meHTVVariation.h"
#include "meHTVegetationLayer.h"
#include "meHTVEvaluationEnvironment.h"
#include "rules/meHTVRLink.h"
#include "rules/meHTVRule.h"

#include "../terrain/meHeightTerrain.h"
#include "../meWorld.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/propfield/dePropFieldType.h>
#include <dragengine/resources/propfield/dePropFieldInstance.h>
#include <dragengine/common/exceptions.h>



// Class meHTVegetationLayer
////////////////////////////////////

// Constructor, destructor
////////////////////////////

meHTVegetationLayer::meHTVegetationLayer( deEngine *engine, const char *name ){
	if( ! engine ) DETHROW( deeInvalidParam );
	
	pEngine = engine;
	pHeightTerrain = NULL;
	pName = name;
	
	pVariations = NULL;
	pVariationCount = 0;
	pVariationSize = 0;
	pActiveVariation = NULL;
	
	pRules = NULL;
	pRuleCount = 0;
	pRuleSize = 0;
	pActiveRule = NULL;
	
	pLinks = NULL;
	pLinkCount = 0;
	pLinkSize = 0;
}

meHTVegetationLayer::~meHTVegetationLayer(){
	pCleanUp();
}



// Management
///////////////

void meHTVegetationLayer::SetHeightTerrain( meHeightTerrain *heightTerrain ){
	pHeightTerrain = heightTerrain;
}

void meHTVegetationLayer::SetViewCenter( const decVector2 &center ){
	if( ! center.IsEqualTo( pViewCenter ) ){
		pViewCenter = center;
		
		if( pHeightTerrain ){
			pHeightTerrain->SetChanged( true );
		}
	}
}

void meHTVegetationLayer::SetName( const char *name ){
	if( ! name ) DETHROW( deeInvalidParam );
	
	if( ! pName.Equals( name ) ){
		pName = name;
		
		if( pHeightTerrain ){
			         pHeightTerrain->SetChanged( true );
		}
	}
}



// Variations
///////////////

meHTVVariation *meHTVegetationLayer::GetVariationAt( int index ) const{
	if( index < 0 || index >= pVariationCount ) DETHROW( deeInvalidParam );
	
	return pVariations[ index ];
}

int meHTVegetationLayer::IndexOfVariation( meHTVVariation *variation ) const{
	if( ! variation ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pVariationCount; i++ ){
		if( variation == pVariations[ i ] ) return i;
	}
	
	return -1;
}

bool meHTVegetationLayer::HasVariation( meHTVVariation *variation ) const{
	if( ! variation ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pVariationCount; i++ ){
		if( variation == pVariations[ i ] ) return true;
	}
	
	return false;
}

void meHTVegetationLayer::AddVariation( meHTVVariation *variation ){
	if( ! variation ) DETHROW( deeInvalidParam );
	
	if( pVariationCount == pVariationSize ){
		int newSize = pVariationCount * 3 / 2 + 1;
		meHTVVariation **newArray = new meHTVVariation*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pVariations ){
			memcpy( newArray, pVariations, sizeof( meHTVVariation* ) * pVariationSize );
			delete [] pVariations;
		}
		pVariations = newArray;
		pVariationSize = newSize;
	}
	
	pVariations[ pVariationCount ] = variation;
	pVariationCount++;
	
	variation->AddReference();
	variation->SetVLayer( this );
	
	if( ! pActiveVariation ) SetActiveVariation( variation );
	
	if( pHeightTerrain ){
		pHeightTerrain->GetWorld().NotifyHTVLVariationCountChanged( this );
		pHeightTerrain->SetChanged( true );
		pHeightTerrain->InvalidateAllPropFields();
	}
}

void meHTVegetationLayer::InsertVariation( int before, meHTVVariation *variation ){
	if( before < 0 || before > pVariationCount || ! variation ) DETHROW( deeInvalidParam );
	
	int i;
	
	if( pVariationCount == pVariationSize ){
		int newSize = pVariationCount * 3 / 2 + 1;
		meHTVVariation **newArray = new meHTVVariation*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pVariations ){
			memcpy( newArray, pVariations, sizeof( meHTVVariation* ) * pVariationSize );
			delete [] pVariations;
		}
		pVariations = newArray;
		pVariationSize = newSize;
	}
	
	for( i=pVariationCount; i>before; i-- ){
		pVariations[ i ] = pVariations[ i - 1 ];
	}
	pVariations[ before ] = variation;
	pVariationCount++;
	
	variation->AddReference();
	variation->SetVLayer( this );
	
	if( ! pActiveVariation ) SetActiveVariation( variation );
	
	if( pHeightTerrain ){
		pHeightTerrain->GetWorld().NotifyHTVLVariationCountChanged( this );
		pHeightTerrain->SetChanged( true );
		pHeightTerrain->InvalidateAllPropFields();
	}
}

void meHTVegetationLayer::MoveVariation( meHTVVariation *variation, int moveTo ){
	if( moveTo < 0 || moveTo > pVariationCount ) DETHROW( deeInvalidParam );
	
	int i, moveFrom = IndexOfVariation( variation );
	
	if( moveFrom == -1 ) DETHROW( deeInvalidParam );
	
	if( moveTo > moveFrom ){
		for( i=moveFrom; i<moveTo; i++ ){
			pVariations[ i ] = pVariations[ i + 1 ];
		}
		pVariations[ moveTo ] = variation;
		
	}else if( moveTo < moveFrom ){
		for( i=moveFrom; i>moveTo; i-- ){
			pVariations[ i ] = pVariations[ i - 1 ];
		}
		pVariations[ moveTo ] = variation;
	}
	
	if( pHeightTerrain ){
		pHeightTerrain->GetWorld().NotifyHTVLVariationCountChanged( this );
		pHeightTerrain->SetChanged( true );
		pHeightTerrain->InvalidateAllPropFields();
	}
}

void meHTVegetationLayer::RemoveVariation( meHTVVariation *variation ){
	int i, index = IndexOfVariation( variation );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	if( variation == pActiveVariation ) SetActiveVariation( NULL );
	
	for( i=index+1; i<pVariationCount; i++ ){
		pVariations[ i - 1 ] = pVariations[ i ];
	}
	pVariationCount--;
	
	variation->SetVLayer( NULL );
	variation->FreeReference();
	
	if( pHeightTerrain ){
		pHeightTerrain->GetWorld().NotifyHTVLVariationCountChanged( this );
		pHeightTerrain->SetChanged( true );
		pHeightTerrain->InvalidateAllPropFields();
	}
}

void meHTVegetationLayer::RemoveAllVariations(){
	SetActiveVariation( NULL );
	
	while( pVariationCount > 0 ){
		pVariationCount--;
		pVariations[ pVariationCount ]->SetVLayer( NULL );
		pVariations[ pVariationCount ]->FreeReference();
	}
	
	if( pHeightTerrain ){
		pHeightTerrain->GetWorld().NotifyHTVLVariationCountChanged( this );
		pHeightTerrain->SetChanged( true );
		pHeightTerrain->InvalidateAllPropFields();
	}
}

void meHTVegetationLayer::SetActiveVariation( meHTVVariation *variation ){
	if( variation != pActiveVariation ){
		pActiveVariation = variation;
		
		if( pHeightTerrain ){
			pHeightTerrain->GetWorld().NotifyHTVLActiveVariationChanged( this );
		}
	}
}



// Rules
//////////

meHTVRule *meHTVegetationLayer::GetRuleAt( int index ) const{
	if( index < 0 || index >= pRuleCount ) DETHROW( deeInvalidParam );
	
	return pRules[ index ];
}

int meHTVegetationLayer::IndexOfRule( meHTVRule *rule ) const{
	if( ! rule ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pRuleCount; i++ ){
		if( rule == pRules[ i ] ) return i;
	}
	
	return -1;
}

bool meHTVegetationLayer::HasRule( meHTVRule *rule ) const{
	if( ! rule ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pRuleCount; i++ ){
		if( rule == pRules[ i ] ) return true;
	}
	
	return false;
}

void meHTVegetationLayer::AddRule( meHTVRule *rule ){
	if( ! rule ) DETHROW( deeInvalidParam );
	
	if( pRuleCount == pRuleSize ){
		int newSize = pRuleCount * 3 / 2 + 1;
		meHTVRule **newArray = new meHTVRule*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pRules ){
			memcpy( newArray, pRules, sizeof( meHTVRule* ) * pRuleSize );
			delete [] pRules;
		}
		pRules = newArray;
		pRuleSize = newSize;
	}
	
	pRules[ pRuleCount ] = rule;
	pRuleCount++;
	
	rule->AddReference();
	
	if( pHeightTerrain ){
		pHeightTerrain->GetWorld().NotifyHTVLRuleCountChanged( this );
		pHeightTerrain->SetChanged( true );
		pHeightTerrain->InvalidateAllPropFields();
	}
	
	if( ! pActiveRule ) SetActiveRule( rule );
}

void meHTVegetationLayer::RemoveRule( meHTVRule *rule ){
	int i, index = IndexOfRule( rule );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	if( rule == pActiveRule ) SetActiveRule( NULL );
	
	for( i=index+1; i<pRuleCount; i++ ){
		pRules[ i - 1 ] = pRules[ i ];
	}
	pRuleCount--;
	
	rule->FreeReference();
	
	if( pHeightTerrain ){
		pHeightTerrain->GetWorld().NotifyHTVLRuleCountChanged( this );
		pHeightTerrain->SetChanged( true );
		pHeightTerrain->InvalidateAllPropFields();
	}
}

void meHTVegetationLayer::RemoveAllRules(){
	SetActiveRule( NULL );
	
	while( pRuleCount > 0 ){
		pRuleCount--;
		pRules[ pRuleCount ]->FreeReference();
	}
	
	if( pHeightTerrain ){
		pHeightTerrain->GetWorld().NotifyHTVLRuleCountChanged( this );
		pHeightTerrain->SetChanged( true );
		pHeightTerrain->InvalidateAllPropFields();
	}
}

void meHTVegetationLayer::SetActiveRule( meHTVRule *rule ){
	if( rule != pActiveRule ){
		pActiveRule = rule;
		
		if( pHeightTerrain ){
			pHeightTerrain->GetWorld().NotifyHTVLActiveRuleChanged( this );
		}
	}
}

void meHTVegetationLayer::NotifyRuleChanged( meHTVRule *rule ){
	if( pHeightTerrain ){
		pHeightTerrain->GetWorld().NotifyHTVLRuleChanged( this, rule );
		pHeightTerrain->SetChanged( true );
		pHeightTerrain->InvalidateAllPropFields();
	}
}

void meHTVegetationLayer::NotifyRuleMoved( meHTVRule *rule ){
	if( pHeightTerrain ){
		pHeightTerrain->GetWorld().NotifyHTVLRuleMoved( this, rule );
		pHeightTerrain->SetChanged( true );
	}
}



void meHTVegetationLayer::EvaluateRules( meHTVEvaluationEnvironment &evalEnv ){
	int r;
	
	// reset rule states
	for( r=0; r<pRuleCount; r++ ){
		pRules[ r ]->Reset();
	}
	
	// evaluate sink type rules
	for( r=0; r<pRuleCount; r++ ){
		pRules[ r ]->Evaluate( evalEnv );
	}
}



// Links
//////////

meHTVRLink *meHTVegetationLayer::GetLinkAt( int index ) const{
	if( index < 0 || index >= pLinkCount ) DETHROW( deeInvalidParam );
	
	return pLinks[ index ];
}

int meHTVegetationLayer::IndexOfLink( meHTVRLink *link ) const{
	if( ! link ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pLinkCount; i++ ){
		if( link == pLinks[ i ] ) return i;
	}
	
	return -1;
}

bool meHTVegetationLayer::HasLink( meHTVRLink *link ) const{
	if( ! link ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pLinkCount; i++ ){
		if( link == pLinks[ i ] ) return true;
	}
	
	return false;
}

void meHTVegetationLayer::AddLink( meHTVRLink *link ){
	if( ! link ) DETHROW( deeInvalidParam );
	
	if( pLinkCount == pLinkSize ){
		int newSize = pLinkCount * 3 / 2 + 1;
		meHTVRLink **newArray = new meHTVRLink*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pLinks ){
			memcpy( newArray, pLinks, sizeof( meHTVRLink* ) * pLinkSize );
			delete [] pLinks;
		}
		pLinks = newArray;
		pLinkSize = newSize;
	}
	
	pLinks[ pLinkCount ] = link;
	pLinkCount++;
	
	link->AddReference();
	
	if( pHeightTerrain ){
		pHeightTerrain->GetWorld().NotifyHTVLLinkCountChanged( this );
		pHeightTerrain->SetChanged( true );
		pHeightTerrain->InvalidateAllPropFields();
	}
}

void meHTVegetationLayer::RemoveLink( meHTVRLink *link ){
	int i, index = IndexOfLink( link );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	for( i=index+1; i<pLinkCount; i++ ){
		pLinks[ i - 1 ] = pLinks[ i ];
	}
	pLinkCount--;
	
	link->FreeReference();
	
	if( pHeightTerrain ){
		pHeightTerrain->GetWorld().NotifyHTVLLinkCountChanged( this );
		pHeightTerrain->SetChanged( true );
		pHeightTerrain->InvalidateAllPropFields();
	}
}

void meHTVegetationLayer::RemoveAllLinks(){
	while( pLinkCount > 0 ){
		pLinkCount--;
		pLinks[ pLinkCount ]->FreeReference();
	}
	
	if( pHeightTerrain ){
		pHeightTerrain->GetWorld().NotifyHTVLLinkCountChanged( this );
		pHeightTerrain->SetChanged( true );
		pHeightTerrain->InvalidateAllPropFields();
	}
}

bool meHTVegetationLayer::LinkProducesLoop( meHTVRule *sourceRule, int sourceSlot, meHTVRule *destinationRule, int destinationSlot ){
	if( ! sourceRule || sourceSlot < 0 || sourceSlot >= sourceRule->GetSlotCount() ){
		DETHROW( deeInvalidParam );
	}
	if( ! destinationRule || destinationSlot < 0 || destinationSlot >= destinationRule->GetSlotCount() ){
		DETHROW( deeInvalidParam );
	}
	
	return sourceRule->DependsOn( destinationRule );
}



// Private Functions
//////////////////////

void meHTVegetationLayer::pCleanUp(){
	RemoveAllLinks();
	if( pLinks ) delete [] pLinks;
	
	RemoveAllRules();
	if( pRules ) delete [] pRules;
	
	RemoveAllVariations();
	if( pVariations ) delete [] pVariations;
}
