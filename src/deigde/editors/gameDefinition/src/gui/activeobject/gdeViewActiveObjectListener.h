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

#ifndef _GDEVIEWACTIVEOBJECTLISTENER_H_
#define _GDEVIEWACTIVEOBJECTLISTENER_H_

#include "../../gamedef/gdeGameDefinitionListener.h"

class gdeViewActiveObject;



/**
 * \brief Active object view listener.
 */
class gdeViewActiveObjectListener : public gdeGameDefinitionListener{
private:
	gdeViewActiveObject &pView;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	gdeViewActiveObjectListener( gdeViewActiveObject &view );
	
	/** \brief Clean up listener. */
	virtual ~gdeViewActiveObjectListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Selected object changed. */
	virtual void SelectedObjectChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Base path changed. */
	virtual void BasePathChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Base game definitions changed. */
	virtual void BaseGameDefinitionsChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** \brief Object class changed. */
	virtual void ObjectClassChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class billboards changed. */
	virtual void OCBillboardsChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class billboard changed. */
	virtual void OCBillboardChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCBillboard *billboard );
	
	/** \brief Object class cameras changed. */
	virtual void OCCamerasChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class camera changed. */
	virtual void OCCameraChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCCamera *camera );
	
	/** \brief Object class components changed. */
	virtual void OCComponentsChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class component changed. */
	virtual void OCComponentChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component );
	
	/** \brief Object class component active texture changed. */
	virtual void OCComponentActiveTextureChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component );
	
	/** \brief Object class component texture changed. */
	virtual void OCComponentTextureChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component,
		gdeOCComponentTexture *texture );
	
	/** \brief Object class component texture name changed. */
	virtual void OCComponentTextureNameChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component, gdeOCComponentTexture *texture );
	
	/** \brief Object class component texture properties changed. */
	virtual void OCComponentTexturePropertiesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component, gdeOCComponentTexture *texture );
	
	/** \brief Object class environment map probes changed. */
	virtual void OCEnvMapProbesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class environment map probe changed. */
	virtual void OCEnvMapProbeChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCEnvMapProbe *envMapProbe );
	
	/** \brief Object class lights changed. */
	virtual void OCLightsChanged( gdeGameDefinition *gameDefinition, gdeObjectClass *objectClass );
	
	/** \brief Object class light changed. */
	virtual void OCLightChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCLight *light );
	
	/** \brief Object class navigation blockers changed. */
	virtual void OCNavigationBlockersChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class navigation blocker changed. */
	virtual void OCNavigationBlockerChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCNavigationBlocker *navblocker );
	
	/** \brief Object class navigation spaces changed. */
	virtual void OCNavigationSpacesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class navigation space changed. */
	virtual void OCNavigationSpaceChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCNavigationSpace *navspace );
	
	/** \brief Object class particle emitters changed. */
	virtual void OCParticleEmittersChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class particle emitter changed. */
	virtual void OCParticleEmitterChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCParticleEmitter *emitter );
	
	/** \brief Object class force fields changed. */
	virtual void OCForceFieldsChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class force field changed. */
	virtual void OCForceFieldChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCForceField *field );
	
	/** \brief Object class snap points changed. */
	virtual void OCSnapPointsChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class snap point changed. */
	virtual void OCSnapPointChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCSnapPoint *snappoint );
	
	/** \brief Object class speakers changed. */
	virtual void OCSpeakersChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class speaker changed. */
	virtual void OCSpeakerChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCSpeaker *speaker );
	
	/** \brief Object class worlds changed. */
	virtual void OCWorldsChanged(gdeGameDefinition *gameDefinition, gdeObjectClass *objectClass);
	
	/** \brief Object class world changed. */
	virtual void OCWorldChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCWorld *world);
	
	/** \brief Active object class changed. */
	virtual void ActiveObjectClassChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Active object class billboard changed. */
	virtual void ActiveOCBillboardChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Active object class camera changed. */
	virtual void ActiveOCCameraChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Active object class component changed. */
	virtual void ActiveOCComponentChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Active object class environment map probe changed. */
	virtual void ActiveOCEnvMapProbeChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Active object class light changed. */
	virtual void ActiveOCLightChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Active object class navigation blocker changed. */
	virtual void ActiveOCNavigationBlockerChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Active object class navigation space changed. */
	virtual void ActiveOCNavigationSpaceChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Active object class particle emitter changed. */
	virtual void ActiveOCParticleEmitterChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Active object class force field changed. */
	virtual void ActiveOCForceFieldChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Active object class snap point changed. */
	virtual void ActiveOCSnapPointChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Active object class speaker changed. */
	virtual void ActiveOCSpeakerChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Active object class world changed. */
	virtual void ActiveOCWorldChanged(gdeGameDefinition *gameDefinition);
	
	
	
	/** \brief Particle emitter changed. */
	virtual void ParticleEmitterChanged( gdeGameDefinition *gameDefinition,
		gdeParticleEmitter *particleEmitter );
	
	/** \brief Particle emitter name changed. */
	virtual void ParticleEmitterNameChanged( gdeGameDefinition *gameDefinition,
		gdeParticleEmitter *particleEmitter );
	
	/** \brief Active particle emitter changed. */
	virtual void ActiveParticleEmitterChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** \brief Skin changed. */
	virtual void SkinChanged( gdeGameDefinition *gameDefinition, gdeSkin *skin );
	
	/** \brief Skin name changed. */
	virtual void SkinNameChanged( gdeGameDefinition *gameDefinition, gdeSkin *skin );
	
	/** \brief Active skin changed. */
	virtual void ActiveSkinChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** Object class textures changed. */
	virtual void OCTexturesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** Object class tzexture changed. */
	virtual void OCTextureChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponentTexture *texture );
	
	
	
	/** Object property changed. */
	virtual void OCPropertyChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property );
	
	/** Object properties changed. */
	virtual void OCPropertiesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** Object property values changed. */
	virtual void OCPropertyValuesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	/*@}*/
	
	
	
private:
	/** \brief Get object class of selected sub object or \em NULL. */
	gdeObjectClass *pGetSSOObjectClass( const gdeGameDefinition &gameDefinition ) const;
	
	/** \brief Active sub object changed. */
	void pOCActiveSubObjectChanged( const gdeGameDefinition &gameDefinition );
};

#endif
