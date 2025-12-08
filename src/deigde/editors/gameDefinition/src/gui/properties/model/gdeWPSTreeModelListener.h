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

#ifndef _GDEWPSTREEMODELLISTENER_H_
#define _GDEWPSTREEMODELLISTENER_H_

#include "../../../gamedef/gdeGameDefinitionListener.h"

class gdeWPSTreeModel;



/**
 * \brief Tree model listener.
 */
class gdeWPSTreeModelListener : public gdeGameDefinitionListener{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<gdeWPSTreeModelListener> Ref;
	
	

private:
	gdeWPSTreeModel &pModel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	gdeWPSTreeModelListener(gdeWPSTreeModel &model);
	
	/** \brief Clean up listener. */
	~gdeWPSTreeModelListener() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	
	/** \brief Object class categories changed. */
	void ObjectClassCategoriesChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Skin categories changed. */
	void SkinCategoriesChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Sky categories changed. */
	void SkyCategoriesChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Particle emitter categories changed. */
	void ParticleEmitterCategoriesChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Active category changed. */
	void ActiveCategoryChanged(gdeGameDefinition *gameDefinition) override;
	
	
	
	/** \brief Selected object changed. */
	void SelectedObjectChanged(gdeGameDefinition *gameDefinition) override;
	
	/** Object class count or order changed. */
	void ObjectClassStructureChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Object class changed. */
	void ObjectClassChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object class name changed. */
	void ObjectClassNameChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object class billboards changed. */
	void OCBillboardsChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object class component changed. */
	void OCComponentsChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object class component changed. */
	void OCComponentChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component) override;
	
	/** Object class cameras changed. */
	void OCCamerasChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object class environment map probes changed. */
	void OCEnvMapProbesChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object class lights changed. */
	void OCLightsChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object class navigation blockers changed. */
	void OCNavigationBlockersChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object class navigation spaces changed. */
	void OCNavigationSpacesChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object class particle emitters changed. */
	void OCParticleEmittersChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object class force fields changed. */
	void OCForceFieldsChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object class snap points changed. */
	void OCSnapPointsChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object class snap point name changed. */
	void OCSnapPointNameChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCSnapPoint *snappoint) override;
	
	/** \brief Object class speakers changed. */
	void OCSpeakersChanged(gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass) override;
	
	/** \brief Object class worlds changed. */
	void OCWorldsChanged(gdeGameDefinition *gameDefinition, gdeObjectClass *objectClass) override;
	
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
	
	
	
	/** \brief Particle emitter count or order changed. */
	void ParticleEmitterStructureChanged(gdeGameDefinition *gameDefinitiSon) override;
	
	/** \brief Particle emitter name changed. */
	void ParticleEmitterNameChanged(gdeGameDefinition *gameDefinition,
		gdeParticleEmitter *particleEmitter) override;
	
	/** \brief Active particle emitter changed. */
	void ActiveParticleEmitterChanged(gdeGameDefinition *gameDefinition) override;
	
	
	
	/** \brief Skin count or order changed. */
	void SkinStructureChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Skin name changed. */
	void SkinNameChanged(gdeGameDefinition *gameDefinition, gdeSkin *skin) override;
	
	/** \brief Active skin changed. */
	void ActiveSkinChanged(gdeGameDefinition *gameDefinition) override;
	
	
	
	/** \brief Sky count or order changed. */
	void SkyStructureChanged(gdeGameDefinition *gameDefinition) override;
	
	/** \brief Sky name changed. */
	void SkyNameChanged(gdeGameDefinition *gameDefinition, gdeSky *sky) override;
	
	/** \brief Active sky changed. */
	void ActiveSkyChanged(gdeGameDefinition *gameDefinition) override;
	/*@}*/
};

#endif
