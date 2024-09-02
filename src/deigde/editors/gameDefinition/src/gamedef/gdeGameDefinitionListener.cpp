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

#include "gdeGameDefinitionListener.h"



// Class gdeGameDefinitionListener
////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeGameDefinitionListener::gdeGameDefinitionListener(){
}

gdeGameDefinitionListener::~gdeGameDefinitionListener(){
}



// Notifications
//////////////////

void gdeGameDefinitionListener::StateChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::UndoChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::ViewChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::GameDefinitionChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::BasePathChanged( gdeGameDefinition *gameDefinition ){
	GameDefinitionChanged( gameDefinition );
}

void gdeGameDefinitionListener::BaseGameDefinitionsChanged( gdeGameDefinition *gameDefinition ){
	GameDefinitionChanged( gameDefinition );
}

void gdeGameDefinitionListener::SkyChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::EnvObjectChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::CameraChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::SelectedObjectChanged( gdeGameDefinition* ){
}



void gdeGameDefinitionListener::WorldPropertiesChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::WorldPropertyChanged( gdeGameDefinition*, gdeProperty* ){
}

void gdeGameDefinitionListener::WorldPropertyNameChanged( gdeGameDefinition *gameDefinition,
gdeProperty *property ){
	WorldPropertyChanged( gameDefinition, property );
}



void gdeGameDefinitionListener::DecalPropertiesChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::DecalPropertyChanged( gdeGameDefinition*, gdeProperty* ){
}

void gdeGameDefinitionListener::DecalPropertyNameChanged( gdeGameDefinition *gameDefinition,
gdeProperty *property ){
	DecalPropertyChanged( gameDefinition, property );
}



void gdeGameDefinitionListener::AutoFindPathObjectClassesChanged( gdeGameDefinition *gameDefinition ){
	GameDefinitionChanged( gameDefinition );
}

void gdeGameDefinitionListener::AutoFindPathSkinsChanged( gdeGameDefinition *gameDefinition ){
	GameDefinitionChanged( gameDefinition );
}

void gdeGameDefinitionListener::AutoFindPathSkiesChanged( gdeGameDefinition *gameDefinition ){
	GameDefinitionChanged( gameDefinition );
}



void gdeGameDefinitionListener::ObjectClassCategoriesChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::SkinCategoriesChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::SkyCategoriesChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::ParticleEmitterCategoriesChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::ActiveCategoryChanged( gdeGameDefinition* ){
}



void gdeGameDefinitionListener::ObjectClassUsedTagsChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::DefinedUsedIDsChanged( gdeGameDefinition* ){
}



void gdeGameDefinitionListener::ObjectClassStructureChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::ObjectClassChanged( gdeGameDefinition*, gdeObjectClass* ){
}

void gdeGameDefinitionListener::ObjectClassNameChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCPropertyChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeProperty* ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCPropertyNameChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeProperty *property ){
	OCPropertyChanged( gameDefinition, objectClass, property );
}

void gdeGameDefinitionListener::OCPropertiesChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCPropertyValuesChanged( gdeGameDefinition* gameDefinition,
gdeObjectClass *objectClass ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCTexturePropertyChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeProperty* ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCTexturePropertyNameChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeProperty *property ){
	OCTexturePropertyChanged( gameDefinition, objectClass, property );
}

void gdeGameDefinitionListener::OCTexturePropertiesChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCInheritChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCInherit* ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCInheritsChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCBillboardsChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCBillboardChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCBillboard* ){
	OCBillboardsChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCCamerasChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCCameraChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCCamera* ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCComponentsChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCComponentChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCComponent* ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCComponentActiveTextureChanged( gdeGameDefinition*,
gdeObjectClass*, gdeOCComponent* ){
}

void gdeGameDefinitionListener::OCComponentTextureChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCComponent *component, gdeOCComponentTexture* ){
	OCComponentChanged( gameDefinition, objectClass, component );
}

void gdeGameDefinitionListener::OCComponentTextureNameChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCComponent *component, gdeOCComponentTexture *texture ){
	OCComponentTextureChanged( gameDefinition, objectClass, component, texture );
}

void gdeGameDefinitionListener::OCComponentTexturePropertiesChanged(
gdeGameDefinition *gameDefinition, gdeObjectClass *objectClass,
gdeOCComponent *component, gdeOCComponentTexture *texture ){
	OCComponentTextureChanged( gameDefinition, objectClass, component, texture );
}

void gdeGameDefinitionListener::OCEnvMapProbesChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCEnvMapProbeChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCEnvMapProbe* ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCLightsChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCLightChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCLight* ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCNavigationBlockersChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCNavigationBlockerChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCNavigationBlocker* ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCNavigationSpacesChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCNavigationSpaceChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCNavigationSpace* ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCParticleEmittersChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCParticleEmitterChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCParticleEmitter* ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCForceFieldsChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCForceFieldChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCForceField* ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCSnapPointsChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCSnapPointChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCSnapPoint* ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCSnapPointNameChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCSnapPoint *snappoint ){
	OCSnapPointChanged( gameDefinition, objectClass, snappoint );
}

void gdeGameDefinitionListener::OCSpeakersChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCSpeakerChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCSpeaker* ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCTexturesChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCTextureChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCComponentTexture* ){
	ObjectClassChanged( gameDefinition, objectClass );
}

void gdeGameDefinitionListener::OCActiveTextureChanged( gdeGameDefinition*, gdeObjectClass* ){
}

void gdeGameDefinitionListener::ActiveObjectClassChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::ActiveOCBillboardChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::ActiveOCCameraChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::ActiveOCComponentChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::ActiveOCEnvMapProbeChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::ActiveOCLightChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::ActiveOCNavigationBlockerChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::ActiveOCNavigationSpaceChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::ActiveOCParticleEmitterChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::ActiveOCForceFieldChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::ActiveOCSnapPointChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::ActiveOCSpeakerChanged( gdeGameDefinition* ){
}



void gdeGameDefinitionListener::ParticleEmitterStructureChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::ParticleEmitterChanged( gdeGameDefinition*, gdeParticleEmitter* ){
}

void gdeGameDefinitionListener::ParticleEmitterNameChanged( gdeGameDefinition *gameDefinition,
gdeParticleEmitter *particleEmitter ){
	ParticleEmitterChanged( gameDefinition, particleEmitter );
}

void gdeGameDefinitionListener::ActiveParticleEmitterChanged( gdeGameDefinition* ){
}



void gdeGameDefinitionListener::SkinStructureChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::SkinChanged( gdeGameDefinition*, gdeSkin* ){
}

void gdeGameDefinitionListener::SkinNameChanged( gdeGameDefinition *gameDefinition, gdeSkin *skin ){
	SkinChanged( gameDefinition, skin );
}

void gdeGameDefinitionListener::ActiveSkinChanged( gdeGameDefinition* ){
}



void gdeGameDefinitionListener::SkyStructureChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::SkyChanged( gdeGameDefinition*, gdeSky* ){
}

void gdeGameDefinitionListener::SkyNameChanged( gdeGameDefinition *gameDefinition, gdeSky *sky ){
	SkyChanged( gameDefinition, sky );
}

void gdeGameDefinitionListener::ActiveSkyChanged( gdeGameDefinition* ){
}

void gdeGameDefinitionListener::SkyControllerStructureChanged(
gdeGameDefinition *gameDefinition, gdeSky *sky ){
	SkyChanged( gameDefinition, sky );
}

void gdeGameDefinitionListener::SkyControllerChanged( gdeGameDefinition *gameDefinition,
gdeSky *sky, gdeSkyController* ){
	SkyChanged( gameDefinition, sky );
}
