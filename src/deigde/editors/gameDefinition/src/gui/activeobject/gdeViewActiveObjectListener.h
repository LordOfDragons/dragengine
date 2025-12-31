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
	typedef deTObjectReference<gdeViewActiveObjectListener> Ref;
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	gdeViewActiveObjectListener(gdeViewActiveObject &view);
	
	/** \brief Clean up listener. */
protected:
	~gdeViewActiveObjectListener() override;
public:
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Selected object changed. */
	void SelectedObjectChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Base path changed. */
	void BasePathChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Base game definitions changed. */
	void BaseGameDefinitionsChanged(gdeGameDefinition *gameDefinition) override;
	
	
	
	/** \brief Object class changed. */
	void ObjectClassChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object class billboards changed. */
	void OCBillboardsChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object class billboard changed. */
	void OCBillboardChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCBillboard *billboard) override;
	
	/** \brief Object class cameras changed. */
	void OCCamerasChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object class camera changed. */
	void OCCameraChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCCamera *camera) override;
	
	/** \brief Object class components changed. */
	void OCComponentsChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object class component changed. */
	void OCComponentChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component) override;
	
	/** \brief Object class component active texture changed. */
	void OCComponentActiveTextureChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component) override;
	
	/** \brief Object class component texture changed. */
	void OCComponentTextureChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component,
		gdeOCComponentTexture *texture) override;
	
	/** \brief Object class component texture name changed. */
	void OCComponentTextureNameChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component, gdeOCComponentTexture *texture) override;
	
	/** \brief Object class component texture properties changed. */
	void OCComponentTexturePropertiesChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component, gdeOCComponentTexture *texture) override;
	
	/** \brief Object class environment map probes changed. */
	void OCEnvMapProbesChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object class environment map probe changed. */
	void OCEnvMapProbeChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCEnvMapProbe *envMapProbe) override;
	
	/** \brief Object class lights changed. */
	void OCLightsChanged(gdeGameDefinition *gameDefinition, gdeObjectClass *objectClass) override;
	
	/** \brief Object class light changed. */
	void OCLightChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCLight *light) override;
	
	/** \brief Object class navigation blockers changed. */
	void OCNavigationBlockersChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object class navigation blocker changed. */
	void OCNavigationBlockerChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCNavigationBlocker *navblocker) override;
	
	/** \brief Object class navigation spaces changed. */
	void OCNavigationSpacesChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object class navigation space changed. */
	void OCNavigationSpaceChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCNavigationSpace *navspace) override;
	
	/** \brief Object class particle emitters changed. */
	void OCParticleEmittersChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object class particle emitter changed. */
	void OCParticleEmitterChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCParticleEmitter *emitter) override;
	
	/** \brief Object class force fields changed. */
	void OCForceFieldsChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object class force field changed. */
	void OCForceFieldChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCForceField *field) override;
	
	/** \brief Object class snap points changed. */
	void OCSnapPointsChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object class snap point changed. */
	void OCSnapPointChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCSnapPoint *snappoint) override;
	
	/** \brief Object class speakers changed. */
	void OCSpeakersChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object class speaker changed. */
	void OCSpeakerChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCSpeaker *speaker) override;
	
	/** \brief Object class worlds changed. */
	void OCWorldsChanged(gdeGameDefinition *gameDefinition, gdeObjectClass *objectClass) override;
	
	/** \brief Object class world changed. */
	void OCWorldChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCWorld *world) override;
	
	/** \brief Active object class changed. */
	void ActiveObjectClassChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Active object class billboard changed. */
	void ActiveOCBillboardChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Active object class camera changed. */
	void ActiveOCCameraChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Active object class component changed. */
	void ActiveOCComponentChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Active object class environment map probe changed. */
	void ActiveOCEnvMapProbeChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Active object class light changed. */
	void ActiveOCLightChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Active object class navigation blocker changed. */
	void ActiveOCNavigationBlockerChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Active object class navigation space changed. */
	void ActiveOCNavigationSpaceChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Active object class particle emitter changed. */
	void ActiveOCParticleEmitterChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Active object class force field changed. */
	void ActiveOCForceFieldChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Active object class snap point changed. */
	void ActiveOCSnapPointChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Active object class speaker changed. */
	void ActiveOCSpeakerChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Active object class world changed. */
	void ActiveOCWorldChanged(gdeGameDefinition *gameDefinition) override;
	
	
	
	/** \brief Particle emitter changed. */
	void ParticleEmitterChanged(gdeGameDefinition *gameDefinition,
		gdeParticleEmitter *particleEmitter) override;
	
	/** \brief Particle emitter name changed. */
	void ParticleEmitterNameChanged(gdeGameDefinition *gameDefinition,
		gdeParticleEmitter *particleEmitter) override;
	
	/** \brief Active particle emitter changed. */
	void ActiveParticleEmitterChanged(gdeGameDefinition *gameDefinition) override;
	
	
	
	/** \brief Skin changed. */
	void SkinChanged(gdeGameDefinition *gameDefinition, gdeSkin *skin) override;
	
	/** \brief Skin name changed. */
	void SkinNameChanged(gdeGameDefinition *gameDefinition, gdeSkin *skin) override;
	
	/** \brief Active skin changed. */
	void ActiveSkinChanged(gdeGameDefinition *gameDefinition) override;
	
	
	
	/** Object class textures changed. */
	void OCTexturesChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** Object class tzexture changed. */
	void OCTextureChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponentTexture *texture) override;
	
	
	
	/** Object property changed. */
	void OCPropertyChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property) override;
	
	/** Object properties changed. */
	void OCPropertiesChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** Object property values changed. */
	void OCPropertyValuesChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	/*@}*/
	
	
	
private:
	/** \brief Get object class of selected sub object or \em nullptr. */
	gdeObjectClass *pGetSSOObjectClass(const gdeGameDefinition &gameDefinition) const;
	
	/** \brief Active sub object changed. */
	void pOCActiveSubObjectChanged(const gdeGameDefinition &gameDefinition);
};

#endif
