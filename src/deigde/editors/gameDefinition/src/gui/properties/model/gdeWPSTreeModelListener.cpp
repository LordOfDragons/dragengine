/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include "gdeWPSTreeModel.h"
#include "gdeWPSTreeModelListener.h"
#include "category/gdeWPSTIMCategories.h"
#include "category/gdeWPSTIMCategoriesObjectClass.h"
#include "category/gdeWPSTIMCategoriesSkin.h"
#include "category/gdeWPSTIMCategoriesSky.h"
#include "category/gdeWPSTIMCategoriesParticleEmitter.h"
#include "particleEmitter/gdeWPSTIMParticleEmitter.h"
#include "particleEmitter/gdeWPSTIMParticleEmitters.h"
#include "skin/gdeWPSTIMSkin.h"
#include "skin/gdeWPSTIMSkins.h"
#include "sky/gdeWPSTIMSky.h"
#include "sky/gdeWPSTIMSkies.h"
#include "objectClass/gdeWPSTIMObjectClass.h"
#include "objectClass/gdeWPSTIMObjectClasses.h"
#include "objectClass/gdeWPSTIMOCComponent.h"
#include "objectClass/gdeWPSTIMOCSnapPoint.h"
#include "../../../gamedef/gdeGameDefinition.h"

#include <dragengine/common/exceptions.h>



// Class gdeWPSTreeModelListener
//////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSTreeModelListener::gdeWPSTreeModelListener( gdeWPSTreeModel &model ) :
pModel( model ){
}

gdeWPSTreeModelListener::~gdeWPSTreeModelListener(){
}



// Management
///////////////

void gdeWPSTreeModelListener::ObjectClassCategoriesChanged( gdeGameDefinition* ){
	pModel.GetCategories().GetObjectClass().CategoriesChanged();
	pModel.GetObjectClasses().ValidateCategoryName();
	pModel.SetCurrentItemFromGameDef();
}

void gdeWPSTreeModelListener::SkinCategoriesChanged( gdeGameDefinition* ){
	pModel.GetCategories().GetSkin().CategoriesChanged();
	pModel.SetCurrentItemFromGameDef();
}

void gdeWPSTreeModelListener::SkyCategoriesChanged( gdeGameDefinition* ){
	pModel.GetCategories().GetSky().CategoriesChanged();
	pModel.SetCurrentItemFromGameDef();
}

void gdeWPSTreeModelListener::ParticleEmitterCategoriesChanged( gdeGameDefinition* ){
	pModel.GetCategories().GetParticleEmitter().CategoriesChanged();
	pModel.SetCurrentItemFromGameDef();
}

void gdeWPSTreeModelListener::ActiveCategoryChanged( gdeGameDefinition* ){
	pModel.SetCurrentItemFromGameDef();
}



void gdeWPSTreeModelListener::SelectedObjectChanged( gdeGameDefinition* ){
	pModel.SetCurrentItemFromGameDef();
}

void gdeWPSTreeModelListener::ObjectClassStructureChanged( gdeGameDefinition* ){
	pModel.GetObjectClasses().StructureChanged();
	pModel.SetCurrentItemFromGameDef();
}

void gdeWPSTreeModelListener::ObjectClassChanged( gdeGameDefinition*,
gdeObjectClass *objectClass ){
	gdeWPSTIMObjectClass * const model = pModel.GetObjectClasses().GetChildWith( objectClass );
	if( model ){
		model->Validate();
	}
}

void gdeWPSTreeModelListener::ObjectClassNameChanged( gdeGameDefinition*, gdeObjectClass *objectClass ){
	gdeWPSTIMObjectClass * const model = pModel.GetObjectClasses().GetChildWith( objectClass );
	if( model ){
		model->NameChanged();
	}
	pModel.GetObjectClasses().ValidateObjectClassName();
}

void gdeWPSTreeModelListener::OCBillboardsChanged( gdeGameDefinition*, gdeObjectClass *objectClass ){
	gdeWPSTIMObjectClass * const model = pModel.GetObjectClasses().GetChildWith( objectClass );
	if( model ){
		model->SubObjectsChanged();
		pModel.SetCurrentItemFromGameDef();
	}
}

void gdeWPSTreeModelListener::OCComponentsChanged( gdeGameDefinition*, gdeObjectClass *objectClass ){
	gdeWPSTIMObjectClass * const model = pModel.GetObjectClasses().GetChildWith( objectClass );
	if( model ){
		model->SubObjectsChanged();
		pModel.SetCurrentItemFromGameDef();
	}
}

void gdeWPSTreeModelListener::OCComponentChanged( gdeGameDefinition*,
gdeObjectClass *objectClass, gdeOCComponent *component ){
	gdeWPSTIMObjectClass * const model = pModel.GetObjectClasses().GetChildWith( objectClass );
	if( ! model ){
		return;
	}
	
	gdeWPSTIMOCComponent * const modelComponent = model->GetChildWith( component );
	if( modelComponent ){
		modelComponent->Validate();
	}
	model->Validate();
}

void gdeWPSTreeModelListener::OCEnvMapProbesChanged( gdeGameDefinition*, gdeObjectClass *objectClass ){
	gdeWPSTIMObjectClass * const model = pModel.GetObjectClasses().GetChildWith( objectClass );
	if( model ){
		model->SubObjectsChanged();
		pModel.SetCurrentItemFromGameDef();
	}
}

void gdeWPSTreeModelListener::OCLightsChanged( gdeGameDefinition*, gdeObjectClass *objectClass ){
	gdeWPSTIMObjectClass * const model = pModel.GetObjectClasses().GetChildWith( objectClass );
	if( model ){
		model->SubObjectsChanged();
		pModel.SetCurrentItemFromGameDef();
	}
}

void gdeWPSTreeModelListener::OCNavigationBlockersChanged( gdeGameDefinition*,
gdeObjectClass *objectClass ){
	gdeWPSTIMObjectClass * const model = pModel.GetObjectClasses().GetChildWith( objectClass );
	if( model ){
		model->SubObjectsChanged();
		pModel.SetCurrentItemFromGameDef();
	}
}

void gdeWPSTreeModelListener::OCNavigationSpacesChanged( gdeGameDefinition*,
gdeObjectClass *objectClass ){
	gdeWPSTIMObjectClass * const model = pModel.GetObjectClasses().GetChildWith( objectClass );
	if( model ){
		model->SubObjectsChanged();
		pModel.SetCurrentItemFromGameDef();
	}
}

void gdeWPSTreeModelListener::OCParticleEmittersChanged( gdeGameDefinition*,
gdeObjectClass *objectClass ){
	gdeWPSTIMObjectClass * const model = pModel.GetObjectClasses().GetChildWith( objectClass );
	if( model ){
		model->SubObjectsChanged();
		pModel.SetCurrentItemFromGameDef();
	}
}

void gdeWPSTreeModelListener::OCForceFieldsChanged( gdeGameDefinition*,
gdeObjectClass *objectClass ){
	gdeWPSTIMObjectClass * const model = pModel.GetObjectClasses().GetChildWith( objectClass );
	if( model ){
		model->SubObjectsChanged();
		pModel.SetCurrentItemFromGameDef();
	}
}

void gdeWPSTreeModelListener::OCSnapPointsChanged( gdeGameDefinition*, gdeObjectClass *objectClass ){
	gdeWPSTIMObjectClass * const model = pModel.GetObjectClasses().GetChildWith( objectClass );
	if( model ){
		model->SubObjectsChanged();
		pModel.SetCurrentItemFromGameDef();
	}
}

void gdeWPSTreeModelListener::OCSnapPointNameChanged( gdeGameDefinition*,
gdeObjectClass *objectClass, gdeOCSnapPoint *snappoint ){
	gdeWPSTIMObjectClass * const modelObjectClass = pModel.GetObjectClasses().GetChildWith( objectClass );
	if( ! modelObjectClass ){
		return;
	}
	
	gdeWPSTIMOCSnapPoint * const modelSnapPoint = modelObjectClass->GetChildWith( snappoint );
	if( modelSnapPoint ){
		modelSnapPoint->UpdateName();
	}
}

void gdeWPSTreeModelListener::OCSpeakersChanged( gdeGameDefinition*, gdeObjectClass *objectClass ){
	gdeWPSTIMObjectClass * const model = pModel.GetObjectClasses().GetChildWith( objectClass );
	if( model ){
		model->SubObjectsChanged();
		pModel.SetCurrentItemFromGameDef();
	}
}

void gdeWPSTreeModelListener::ActiveObjectClassChanged( gdeGameDefinition *gameDefinition ){
	if( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotObjectClass ){
		pModel.SetCurrentItemFromGameDef();
	}
}

void gdeWPSTreeModelListener::ActiveOCBillboardChanged( gdeGameDefinition *gameDefinition ){
	if( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCBillboard ){
		pModel.SetCurrentItemFromGameDef();
	}
}

void gdeWPSTreeModelListener::ActiveOCCameraChanged( gdeGameDefinition *gameDefinition ){
	if( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCCamera ){
		pModel.SetCurrentItemFromGameDef();
	}
}

void gdeWPSTreeModelListener::ActiveOCComponentChanged( gdeGameDefinition *gameDefinition ){
	if( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCComponent ){
		pModel.SetCurrentItemFromGameDef();
	}
}

void gdeWPSTreeModelListener::ActiveOCEnvMapProbeChanged( gdeGameDefinition *gameDefinition ){
	if( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCEnvMapProbe ){
		pModel.SetCurrentItemFromGameDef();
	}
}

void gdeWPSTreeModelListener::ActiveOCLightChanged( gdeGameDefinition *gameDefinition ){
	if( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCLight ){
		pModel.SetCurrentItemFromGameDef();
	}
}

void gdeWPSTreeModelListener::ActiveOCNavigationBlockerChanged( gdeGameDefinition *gameDefinition ){
	if( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCNavigationBlocker ){
		pModel.SetCurrentItemFromGameDef();
	}
}

void gdeWPSTreeModelListener::ActiveOCNavigationSpaceChanged( gdeGameDefinition *gameDefinition ){
	if( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCNavigationSpace ){
		pModel.SetCurrentItemFromGameDef();
	}
}

void gdeWPSTreeModelListener::ActiveOCParticleEmitterChanged( gdeGameDefinition *gameDefinition ){
	if( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCParticleEmitter ){
		pModel.SetCurrentItemFromGameDef();
	}
}

void gdeWPSTreeModelListener::ActiveOCForceFieldChanged( gdeGameDefinition *gameDefinition ){
	if( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCForceField ){
		pModel.SetCurrentItemFromGameDef();
	}
}

void gdeWPSTreeModelListener::ActiveOCSnapPointChanged( gdeGameDefinition *gameDefinition ){
	if( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCSnapPoint ){
		pModel.SetCurrentItemFromGameDef();
	}
}

void gdeWPSTreeModelListener::ActiveOCSpeakerChanged( gdeGameDefinition *gameDefinition ){
	if( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCSnapPoint ){
		pModel.SetCurrentItemFromGameDef();
	}
}



void gdeWPSTreeModelListener::ParticleEmitterStructureChanged( gdeGameDefinition* ){
	pModel.GetParticleEmitters().StructureChanged();
	pModel.SetCurrentItemFromGameDef();
}

void gdeWPSTreeModelListener::ParticleEmitterNameChanged( gdeGameDefinition*,
gdeParticleEmitter *particleEmitter ){
	gdeWPSTIMParticleEmitter * const model = pModel.GetParticleEmitters()
		.GetChildWith( particleEmitter );
	if( model ){
		model->NameChanged();
	}
}

void gdeWPSTreeModelListener::ActiveParticleEmitterChanged( gdeGameDefinition *gameDefinition ){
	if( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotParticleEmitter ){
		pModel.SetCurrentItemFromGameDef();
	}
}



void gdeWPSTreeModelListener::SkinStructureChanged( gdeGameDefinition* ){
	pModel.GetSkins().StructureChanged();
	pModel.SetCurrentItemFromGameDef();
}

void gdeWPSTreeModelListener::SkinNameChanged( gdeGameDefinition*, gdeSkin *skin ){
	gdeWPSTIMSkin * const model = pModel.GetSkins().GetChildWith( skin );
	if( model ){
		model->NameChanged();
	}
}

void gdeWPSTreeModelListener::ActiveSkinChanged( gdeGameDefinition *gameDefinition ){
	if( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotSkin ){
		pModel.SetCurrentItemFromGameDef();
	}
}



void gdeWPSTreeModelListener::SkyStructureChanged( gdeGameDefinition* ){
	pModel.GetSkies().StructureChanged();
	pModel.SetCurrentItemFromGameDef();
}

void gdeWPSTreeModelListener::SkyNameChanged( gdeGameDefinition*, gdeSky *sky ){
	gdeWPSTIMSky * const model = pModel.GetSkies().GetChildWith( sky );
	if( model ){
		model->NameChanged();
	}
}

void gdeWPSTreeModelListener::ActiveSkyChanged( gdeGameDefinition *gameDefinition ){
	if( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotSky ){
		pModel.SetCurrentItemFromGameDef();
	}
}
