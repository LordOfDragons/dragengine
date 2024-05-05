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

#ifndef _GDEGAMEDEFINITIONLISTENER_H_
#define _GDEGAMEDEFINITIONLISTENER_H_

#include <dragengine/deObject.h>

class gdeCategory;
class gdeGameDefinition;
class gdeOCBillboard;
class gdeOCCamera;
class gdeOCComponent;
class gdeOCComponentTexture;
class gdeOCEnvMapProbe;
class gdeOCInherit;
class gdeOCLight;
class gdeOCNavigationBlocker;
class gdeOCNavigationSpace;
class gdeOCParticleEmitter;
class gdeOCForceField;
class gdeOCSnapPoint;
class gdeOCSpeaker;
class gdeObjectClass;
class gdeParticleEmitter;
class gdeForceField;
class gdeProperty;
class gdeSkin;
class gdeSky;
class gdeSkyController;



/**
 * Game definition listener.
 */
class gdeGameDefinitionListener : public deObject{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create listener. */
	gdeGameDefinitionListener();
	
	/** Clean up listener. */
	virtual ~gdeGameDefinitionListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Changed or saved state changed. */
	virtual void StateChanged( gdeGameDefinition *gameDefinition );
	
	/** Undo system changed. */
	virtual void UndoChanged( gdeGameDefinition *gameDefinition );
	
	/** View properties changed. */
	virtual void ViewChanged( gdeGameDefinition *gameDefinition );
	
	/** Game definition changed. */
	virtual void GameDefinitionChanged( gdeGameDefinition *gameDefinition );
	
	/** Base path changed. */
	virtual void BasePathChanged( gdeGameDefinition *gameDefinition );
	
	/** Base game definitions changed. */
	virtual void BaseGameDefinitionsChanged( gdeGameDefinition *gameDefinition );
	
	/** Sky changed. */
	virtual void SkyChanged( gdeGameDefinition *gameDefinition );
	
	/** Environment object changed. */
	virtual void EnvObjectChanged( gdeGameDefinition *gameDefinition );
	
	/** Camera changed. */
	virtual void CameraChanged( gdeGameDefinition *gameDefinition );
	
	/** Selected object changed. */
	virtual void SelectedObjectChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** World properties changed. */
	virtual void WorldPropertiesChanged( gdeGameDefinition *gameDefinition );
	
	/** World changed. */
	virtual void WorldPropertyChanged( gdeGameDefinition *gameDefinition,
		gdeProperty *property );
	
	/** World name changed. */
	virtual void WorldPropertyNameChanged( gdeGameDefinition *gameDefinition,
		gdeProperty *property );
	
	
	
	/** Decal properties changed. */
	virtual void DecalPropertiesChanged( gdeGameDefinition *gameDefinition );
	
	/** Decal changed. */
	virtual void DecalPropertyChanged( gdeGameDefinition *gameDefinition,
		gdeProperty *property );
	
	/** Decal name changed. */
	virtual void DecalPropertyNameChanged( gdeGameDefinition *gameDefinition,
		gdeProperty *property );
	
	
	
	/** Auto find path object classes changed. */
	virtual void AutoFindPathObjectClassesChanged( gdeGameDefinition *gameDefinition );
	
	/** Auto find path skins changed. */
	virtual void AutoFindPathSkinsChanged( gdeGameDefinition *gameDefinition );
	
	/** Auto find path skies changed. */
	virtual void AutoFindPathSkiesChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** Object class categories changed. */
	virtual void ObjectClassCategoriesChanged( gdeGameDefinition *gameDefinition );
	
	/** Skin categories changed. */
	virtual void SkinCategoriesChanged( gdeGameDefinition *gameDefinition );
	
	/** Sky categories changed. */
	virtual void SkyCategoriesChanged( gdeGameDefinition *gameDefinition );
	
	/** Particle emitter categories changed. */
	virtual void ParticleEmitterCategoriesChanged( gdeGameDefinition *gameDefinition );
	
	/** Active category changed. */
	virtual void ActiveCategoryChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** Object class used tags changed. */
	virtual void ObjectClassUsedTagsChanged( gdeGameDefinition *gameDefinition );
	
	/** Defined and used identifiers changed. */
	virtual void DefinedUsedIDsChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** Object class count or order changed. */
	virtual void ObjectClassStructureChanged( gdeGameDefinition *gameDefinition );
	
	/** Object class changed. */
	virtual void ObjectClassChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** Object class name changed. */
	virtual void ObjectClassNameChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** Object property changed. */
	virtual void OCPropertyChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property );
	
	/** Object property name changed. */
	virtual void OCPropertyNameChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property );
	
	/** Object properties changed. */
	virtual void OCPropertiesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** Object property values changed. */
	virtual void OCPropertyValuesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** Object texture property changed. */
	virtual void OCTexturePropertyChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property );
	
	/** Object texture property name changed. */
	virtual void OCTexturePropertyNameChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property );
	
	/** Object texture properties changed. */
	virtual void OCTexturePropertiesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** Object inherit changed. */
	virtual void OCInheritChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCInherit *inherit );
	
	/** Object inherit structure changed. */
	virtual void OCInheritsChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** Object class billboards changed. */
	virtual void OCBillboardsChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** Object class billboard changed. */
	virtual void OCBillboardChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCBillboard *billboard );
	
	/** Object class cameras changed. */
	virtual void OCCamerasChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** Object class camera changed. */
	virtual void OCCameraChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCCamera *camera );
	
	/** Object class components changed. */
	virtual void OCComponentsChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** Object class component changed. */
	virtual void OCComponentChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component );
	
	/** Object class component active texture changed. */
	virtual void OCComponentActiveTextureChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component );
	
	/** Object class component texture changed. */
	virtual void OCComponentTextureChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component,
		gdeOCComponentTexture *texture );
	
	/** Object class component texture name changed. */
	virtual void OCComponentTextureNameChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component,
		gdeOCComponentTexture *texture );
	
	/** Object class component texture properties changed. */
	virtual void OCComponentTexturePropertiesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component,
		gdeOCComponentTexture *texture );
	
	/** Object class environment map probes changed. */
	virtual void OCEnvMapProbesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** Object class environment map probe changed. */
	virtual void OCEnvMapProbeChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCEnvMapProbe *envMapProbe );
	
	/** Object class lights changed. */
	virtual void OCLightsChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** Object class light changed. */
	virtual void OCLightChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCLight *light );
	
	/** Object class navigation blockers changed. */
	virtual void OCNavigationBlockersChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** Object class navigation blocker changed. */
	virtual void OCNavigationBlockerChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCNavigationBlocker *navblocker );
	
	/** Object class navigation spaces changed. */
	virtual void OCNavigationSpacesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** Object class navigation space changed. */
	virtual void OCNavigationSpaceChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCNavigationSpace *navspace );
	
	/** Object class particle emitters changed. */
	virtual void OCParticleEmittersChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** Object class particle emitter changed. */
	virtual void OCParticleEmitterChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCParticleEmitter *emitter );
	
	/** Object class force fields changed. */
	virtual void OCForceFieldsChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** Object class force field changed. */
	virtual void OCForceFieldChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCForceField *field );
	
	/** Object class snap points changed. */
	virtual void OCSnapPointsChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** Object class snap point changed. */
	virtual void OCSnapPointChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCSnapPoint *snappoint );
	
	/** Object class snap point name changed. */
	virtual void OCSnapPointNameChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCSnapPoint *snappoint );
	
	/** Object class speakers changed. */
	virtual void OCSpeakersChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** Object class speaker changed. */
	virtual void OCSpeakerChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCSpeaker *speaker );
	
	/** Object class textures changed. */
	virtual void OCTexturesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** Object class tzexture changed. */
	virtual void OCTextureChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponentTexture *texture );
	
	/** Object class active texture changed. */
	virtual void OCActiveTextureChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** Active object class changed. */
	virtual void ActiveObjectClassChanged( gdeGameDefinition *gameDefinition );
	
	/** Active object class billboard changed. */
	virtual void ActiveOCBillboardChanged( gdeGameDefinition *gameDefinition );
	
	/** Active object class camera changed. */
	virtual void ActiveOCCameraChanged( gdeGameDefinition *gameDefinition );
	
	/** Active object class component changed. */
	virtual void ActiveOCComponentChanged( gdeGameDefinition *gameDefinition );
	
	/** Active object class environment map probe changed. */
	virtual void ActiveOCEnvMapProbeChanged( gdeGameDefinition *gameDefinition );
	
	/** Active object class light changed. */
	virtual void ActiveOCLightChanged( gdeGameDefinition *gameDefinition );
	
	/** Active object class navigation blocker changed. */
	virtual void ActiveOCNavigationBlockerChanged( gdeGameDefinition *gameDefinition );
	
	/** Active object class navigation space changed. */
	virtual void ActiveOCNavigationSpaceChanged( gdeGameDefinition *gameDefinition );
	
	/** Active object class particle emitter changed. */
	virtual void ActiveOCParticleEmitterChanged( gdeGameDefinition *gameDefinition );
	
	/** Active object class force field changed. */
	virtual void ActiveOCForceFieldChanged( gdeGameDefinition *gameDefinition );
	
	/** Active object class snap point changed. */
	virtual void ActiveOCSnapPointChanged( gdeGameDefinition *gameDefinition );
	
	/** Active object class speaker changed. */
	virtual void ActiveOCSpeakerChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** Particle emitter count or order changed. */
	virtual void ParticleEmitterStructureChanged( gdeGameDefinition *gameDefinition );
	
	/** Particle emitter changed. */
	virtual void ParticleEmitterChanged( gdeGameDefinition *gameDefinition,
		gdeParticleEmitter *particleEmitter );
	
	/** Particle emitter name changed. */
	virtual void ParticleEmitterNameChanged( gdeGameDefinition *gameDefinition,
		gdeParticleEmitter *particleEmitter );
	
	/** Active particle emitter changed. */
	virtual void ActiveParticleEmitterChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** Skin count or order changed. */
	virtual void SkinStructureChanged( gdeGameDefinition *gameDefinition );
	
	/** Skin changed. */
	virtual void SkinChanged( gdeGameDefinition *gameDefinition, gdeSkin *skin );
	
	/** Skin name changed. */
	virtual void SkinNameChanged( gdeGameDefinition *gameDefinition, gdeSkin *skin );
	
	/** Active skin changed. */
	virtual void ActiveSkinChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** Sky count or order changed. */
	virtual void SkyStructureChanged( gdeGameDefinition *gameDefinition );
	
	/** Sky changed. */
	virtual void SkyChanged( gdeGameDefinition *gameDefinition, gdeSky *sky );
	
	/** Sky name changed. */
	virtual void SkyNameChanged( gdeGameDefinition *gameDefinition, gdeSky *sky );
	
	/** Active sky changed. */
	virtual void ActiveSkyChanged( gdeGameDefinition *gameDefinition );
	
	/** Sky controller count or order changed. */
	virtual void SkyControllerStructureChanged( gdeGameDefinition *gameDefinition, gdeSky *sky );
	
	/** Sky changed. */
	virtual void SkyControllerChanged( gdeGameDefinition *gameDefinition,
		gdeSky *sky, gdeSkyController *controller );
	/*@}*/
};

#endif
