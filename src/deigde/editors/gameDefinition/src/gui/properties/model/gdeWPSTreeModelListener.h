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

#ifndef _GDEWPSTREEMODELLISTENER_H_
#define _GDEWPSTREEMODELLISTENER_H_

#include "../../../gamedef/gdeGameDefinitionListener.h"

class gdeWPSTreeModel;



/**
 * \brief Tree model listener.
 */
class gdeWPSTreeModelListener : public gdeGameDefinitionListener{
private:
	gdeWPSTreeModel &pModel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	gdeWPSTreeModelListener( gdeWPSTreeModel &model );
	
	/** \brief Clean up listener. */
	virtual ~gdeWPSTreeModelListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	
	/** \brief Object class categories changed. */
	virtual void ObjectClassCategoriesChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Skin categories changed. */
	virtual void SkinCategoriesChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Sky categories changed. */
	virtual void SkyCategoriesChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Particle emitter categories changed. */
	virtual void ParticleEmitterCategoriesChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Active category changed. */
	virtual void ActiveCategoryChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** \brief Selected object changed. */
	virtual void SelectedObjectChanged( gdeGameDefinition *gameDefinition );
	
	/** Object class count or order changed. */
	virtual void ObjectClassStructureChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Object class changed. */
	virtual void ObjectClassChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class name changed. */
	virtual void ObjectClassNameChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class billboards changed. */
	virtual void OCBillboardsChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class component changed. */
	virtual void OCComponentsChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class component changed. */
	virtual void OCComponentChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component );
	
	/** \brief Object class environment map probes changed. */
	virtual void OCEnvMapProbesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class lights changed. */
	virtual void OCLightsChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class navigation blockers changed. */
	virtual void OCNavigationBlockersChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class navigation spaces changed. */
	virtual void OCNavigationSpacesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class particle emitters changed. */
	virtual void OCParticleEmittersChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class force fields changed. */
	virtual void OCForceFieldsChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class snap points changed. */
	virtual void OCSnapPointsChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class snap point name changed. */
	virtual void OCSnapPointNameChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCSnapPoint *snappoint );
	
	/** \brief Object class speakers changed. */
	virtual void OCSpeakersChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
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
	
	
	
	/** \brief Particle emitter count or order changed. */
	virtual void ParticleEmitterStructureChanged( gdeGameDefinition *gameDefinitiSon );
	
	/** \brief Particle emitter name changed. */
	virtual void ParticleEmitterNameChanged( gdeGameDefinition *gameDefinition,
		gdeParticleEmitter *particleEmitter );
	
	/** \brief Active particle emitter changed. */
	virtual void ActiveParticleEmitterChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** \brief Skin count or order changed. */
	virtual void SkinStructureChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Skin name changed. */
	virtual void SkinNameChanged( gdeGameDefinition *gameDefinition, gdeSkin *skin );
	
	/** \brief Active skin changed. */
	virtual void ActiveSkinChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** \brief Sky count or order changed. */
	virtual void SkyStructureChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Sky name changed. */
	virtual void SkyNameChanged( gdeGameDefinition *gameDefinition, gdeSky *sky );
	
	/** \brief Active sky changed. */
	virtual void ActiveSkyChanged( gdeGameDefinition *gameDefinition );
	/*@}*/
};

#endif
