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

meHTVegetationLayer::meHTVegetationLayer(deEngine *engine, const char *name){
	if(!engine) DETHROW(deeInvalidParam);
	
	pEngine = engine;
	pHeightTerrain = nullptr;
	pName = name;
}

meHTVegetationLayer::~meHTVegetationLayer(){
	pCleanUp();
}



// Management
///////////////

void meHTVegetationLayer::SetHeightTerrain(meHeightTerrain *heightTerrain){
	pHeightTerrain = heightTerrain;
}

void meHTVegetationLayer::SetViewCenter(const decVector2 &center){
	if(!center.IsEqualTo(pViewCenter)){
		pViewCenter = center;
		
		if(pHeightTerrain){
			pHeightTerrain->SetChanged(true);
		}
	}
}

void meHTVegetationLayer::SetName(const char *name){
	if(!name) DETHROW(deeInvalidParam);
	
	if(!pName.Equals(name)){
		pName = name;
		
		if(pHeightTerrain){
			         pHeightTerrain->SetChanged(true);
		}
	}
}



// Variations
///////////////

void meHTVegetationLayer::AddVariation(meHTVVariation *variation){
	DEASSERT_NOTNULL(variation)
	
	pVariations.Add(variation);
	variation->SetVLayer(this);
	
	if(!pActiveVariation){
		SetActiveVariation(variation);
	}
	
	if(pHeightTerrain){
		pHeightTerrain->GetWorld().NotifyHTVLVariationCountChanged(this);
		pHeightTerrain->SetChanged(true);
		pHeightTerrain->InvalidateAllPropFields();
	}
}

void meHTVegetationLayer::InsertVariation(int before, meHTVVariation *variation){
	DEASSERT_NOTNULL(variation)
	
	pVariations.Insert(variation, before);
	variation->SetVLayer(this);
	
	if(!pActiveVariation){
		SetActiveVariation(variation);
	}
	
	if(pHeightTerrain){
		pHeightTerrain->GetWorld().NotifyHTVLVariationCountChanged(this);
		pHeightTerrain->SetChanged(true);
		pHeightTerrain->InvalidateAllPropFields();
	}
}

void meHTVegetationLayer::MoveVariation(meHTVVariation *variation, int moveTo){
	pVariations.Move(variation, moveTo);
	
	if(pHeightTerrain){
		pHeightTerrain->GetWorld().NotifyHTVLVariationCountChanged(this);
		pHeightTerrain->SetChanged(true);
		pHeightTerrain->InvalidateAllPropFields();
	}
}

void meHTVegetationLayer::RemoveVariation(meHTVVariation *variation){
	const meHTVVariation::Ref guard(variation);
	
	if(variation == pActiveVariation){
		SetActiveVariation(nullptr);
	}
	
	pVariations.Remove(variation);
	variation->SetVLayer(nullptr);
	
	if(pHeightTerrain){
		pHeightTerrain->GetWorld().NotifyHTVLVariationCountChanged(this);
		pHeightTerrain->SetChanged(true);
		pHeightTerrain->InvalidateAllPropFields();
	}
}

void meHTVegetationLayer::RemoveAllVariations(){
	SetActiveVariation(nullptr);
	
	pVariations.Visit([](meHTVVariation &variation){
		variation.SetVLayer(nullptr);
	});
	
	if(pHeightTerrain){
		pHeightTerrain->GetWorld().NotifyHTVLVariationCountChanged(this);
		pHeightTerrain->SetChanged(true);
		pHeightTerrain->InvalidateAllPropFields();
	}
}

void meHTVegetationLayer::SetActiveVariation(meHTVVariation *variation){
	if(variation != pActiveVariation){
		pActiveVariation = variation;
		
		if(pHeightTerrain){
			pHeightTerrain->GetWorld().NotifyHTVLActiveVariationChanged(this);
		}
	}
}



// Rules
//////////

void meHTVegetationLayer::AddRule(meHTVRule *rule){
	DEASSERT_NOTNULL(rule)
	
	pRules.Add(rule);
	
	if(pHeightTerrain){
		pHeightTerrain->GetWorld().NotifyHTVLRuleCountChanged(this);
		pHeightTerrain->SetChanged(true);
		pHeightTerrain->InvalidateAllPropFields();
	}
	
	if(!pActiveRule){
		SetActiveRule(rule);
	}
}

void meHTVegetationLayer::RemoveRule(meHTVRule *rule){
	const meHTVRule::Ref guard(rule);
	
	if(rule == pActiveRule){
		SetActiveRule(nullptr);
	}
	
	pRules.Remove(rule);
	
	if(pHeightTerrain){
		pHeightTerrain->GetWorld().NotifyHTVLRuleCountChanged(this);
		pHeightTerrain->SetChanged(true);
		pHeightTerrain->InvalidateAllPropFields();
	}
}

void meHTVegetationLayer::RemoveAllRules(){
	SetActiveRule(nullptr);
	
	pRules.RemoveAll();
	
	if(pHeightTerrain){
		pHeightTerrain->GetWorld().NotifyHTVLRuleCountChanged(this);
		pHeightTerrain->SetChanged(true);
		pHeightTerrain->InvalidateAllPropFields();
	}
}

void meHTVegetationLayer::SetActiveRule(meHTVRule *rule){
	if(rule != pActiveRule){
		pActiveRule = rule;
		
		if(pHeightTerrain){
			pHeightTerrain->GetWorld().NotifyHTVLActiveRuleChanged(this);
		}
	}
}

void meHTVegetationLayer::NotifyRuleChanged(meHTVRule *rule){
	if(pHeightTerrain){
		pHeightTerrain->GetWorld().NotifyHTVLRuleChanged(this, rule);
		pHeightTerrain->SetChanged(true);
		pHeightTerrain->InvalidateAllPropFields();
	}
}

void meHTVegetationLayer::NotifyRuleMoved(meHTVRule *rule){
	if(pHeightTerrain){
		pHeightTerrain->GetWorld().NotifyHTVLRuleMoved(this, rule);
		pHeightTerrain->SetChanged(true);
	}
}



void meHTVegetationLayer::EvaluateRules(meHTVEvaluationEnvironment &evalEnv){
	// reset rule states
	pRules.Visit([](meHTVRule &rule){
		rule.Reset();
	});
	
	// evaluate sink type rules
	pRules.Visit([&](meHTVRule &rule){
		rule.Evaluate(evalEnv);
	});
}



// Links
//////////

void meHTVegetationLayer::AddLink(meHTVRLink *link){
	DEASSERT_NOTNULL(link)
	
	pLinks.Add(link);
	
	if(pHeightTerrain){
		pHeightTerrain->GetWorld().NotifyHTVLLinkCountChanged(this);
		pHeightTerrain->SetChanged(true);
		pHeightTerrain->InvalidateAllPropFields();
	}
}

void meHTVegetationLayer::RemoveLink(meHTVRLink *link){
	const meHTVRLink::Ref guard(link);
	
	pLinks.Remove(link);
	
	if(pHeightTerrain){
		pHeightTerrain->GetWorld().NotifyHTVLLinkCountChanged(this);
		pHeightTerrain->SetChanged(true);
		pHeightTerrain->InvalidateAllPropFields();
	}
}

void meHTVegetationLayer::RemoveAllLinks(){
	pLinks.RemoveAll();
	
	if(pHeightTerrain){
		pHeightTerrain->GetWorld().NotifyHTVLLinkCountChanged(this);
		pHeightTerrain->SetChanged(true);
		pHeightTerrain->InvalidateAllPropFields();
	}
}

bool meHTVegetationLayer::LinkProducesLoop(meHTVRule *sourceRule, int sourceSlot, meHTVRule *destinationRule, int destinationSlot){
	if(!sourceRule || sourceSlot < 0 || sourceSlot >= sourceRule->GetSlots().GetCount()){
		DETHROW(deeInvalidParam);
	}
	if(!destinationRule || destinationSlot < 0 || destinationSlot >= destinationRule->GetSlots().GetCount()){
		DETHROW(deeInvalidParam);
	}
	
	return sourceRule->DependsOn(destinationRule);
}



// Private Functions
//////////////////////

void meHTVegetationLayer::pCleanUp(){
	RemoveAllLinks();
	RemoveAllRules();
	RemoveAllVariations();
}
