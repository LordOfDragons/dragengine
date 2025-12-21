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

#ifndef _GDEOBJECTCLASS_H_
#define _GDEOBJECTCLASS_H_

#include "billboard/gdeOCBillboard.h"
#include "camera/gdeOCCamera.h"
#include "component/gdeOCComponent.h"
#include "component/gdeOCComponentTexture.h"
#include "envmapprobe/gdeOCEnvMapProbe.h"
#include "inherit/gdeOCInherit.h"
#include "light/gdeOCLight.h"
#include "navblocker/gdeOCNavigationBlocker.h"
#include "navspace/gdeOCNavigationSpace.h"
#include "particleemitter/gdeOCParticleEmitter.h"
#include "forceField/gdeOCForceField.h"
#include "snappoint/gdeOCSnapPoint.h"
#include "speaker/gdeOCSpeaker.h"
#include "world/gdeOCWorld.h"
#include "../property/gdeProperty.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/common/string/decStringDictionary.h>

class gdeGameDefinition;



/**
 * Object class.
 */
class gdeObjectClass : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<gdeObjectClass> Ref;

	/** \brief Type holding list. */
	typedef decTObjectOrderedSet<gdeObjectClass> List;
	
	
	/** Scale modes. */
	enum eScaleModes{
		/** Fixed size. */
		esmFixed,
		
		/** Scalable uniformly. */
		esmUniform,
		
		/** Scalable freely. */
		esmFree
	};
	
	
	
private:
	gdeGameDefinition *pGameDefinition;
	
	decString pName, pDescription;
	eScaleModes pScaleMode;
	
	gdeProperty::List pProperties, pTextureProperties;
	decStringDictionary pPropertyValues;
	
	decString pCategory;
	decStringSet pHideTags, pPartialHideTags;
	
	gdeOCInherit::List pInherits;
	decString pDefaultInheritPropertyPrefix;
	
	bool pIsGhost, pCanInstantiate, pIsAttachableBehavior;
	int pInheritSubObjects;
	
	gdeOCBillboard::List pBillboards;
	gdeOCCamera::List pCameras;
	gdeOCComponent::List pComponents;
	gdeOCLight::List pLights;
	gdeOCEnvMapProbe::List pEnvMapProbes;
	gdeOCSnapPoint::List pSnapPoints;
	gdeOCParticleEmitter::List pParticleEmitters;
	gdeOCForceField::List pForceFields;
	gdeOCSpeaker::List pSpeakers;
	gdeOCNavigationBlocker::List pNavigationBlockers;
	gdeOCNavigationSpace::List pNavigationSpaces;
	gdeOCWorld::List pWorlds;
	
	gdeOCComponentTexture::List pTextures;
	gdeOCComponentTexture::Ref pActiveTexture;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create object class. */
	gdeObjectClass(const char *name = "NewClass");
	
	/** Create copy of object class. */
	gdeObjectClass(const gdeObjectClass &objectClass);
	
	/** Clean up object class. */
	virtual ~gdeObjectClass();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent game definition. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** Set parent game definition. */
	void SetGameDefinition(gdeGameDefinition *gamedef);
	
	
	
	/** Class name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Set class name. */
	void SetName(const char *name);
	
	/** Description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** Set description. */
	void SetDescription(const char *description);
	
	
	
	/** Scale mode. */
	inline eScaleModes GetScaleMode() const{ return pScaleMode; }
	
	/** Set scale mode. */
	void SetScaleMode(eScaleModes mode);
	
	
	
	/** Cathegory. */
	inline const decString &GetCategory() const{ return pCategory; }
	
	/** Set cathegory. */
	void SetCategory(const char *category);
	
	/** Hide tags. */
	inline const decStringSet &GetHideTags() const{ return pHideTags; }
	
	/** Set hide tags. */
	void SetHideTags(const decStringSet &tags);
	
	/** Partial hide tags. */
	inline const decStringSet &GetPartialHideTags() const{ return pPartialHideTags; }
	
	/** Set partial hide tags. */
	void SetPartialHideTags(const decStringSet &tags);
	
	
	
	/** Properties. */
	inline gdeProperty::List &GetProperties(){ return pProperties; }
	inline const gdeProperty::List &GetProperties() const{ return pProperties; }
	
	/** Property values. */
	inline decStringDictionary &GetPropertyValues(){ return pPropertyValues; }
	inline const decStringDictionary &GetPropertyValues() const{ return pPropertyValues; }
	
	/** Notify listeners property changed. */
	void NotifyPropertyChanged(gdeProperty *property);
	
	/** Notify listeners property name changed. */
	void NotifyPropertyNameChanged(gdeProperty *property);
	
	/** Notify listeners properties changed. */
	void NotifyPropertiesChanged();
	
	/** Notify listeners property values changed. */
	void NotifyPropertyValuesChanged();
	
	
	
	/** Texture properties. */
	inline gdeProperty::List &GetTextureProperties(){ return pTextureProperties; }
	inline const gdeProperty::List &GetTextureProperties() const{ return pTextureProperties; }
	
	/** Notify listeners texture property changed. */
	void NotifyTexturePropertyChanged(gdeProperty *property);
	
	/** Notify listeners texture property name changed. */
	void NotifyTexturePropertyNameChanged(gdeProperty *property);
	
	/** Notify listeners texture properties changed. */
	void NotifyTexturePropertiesChanged();
	
	
	
	/** Inherit. */
	inline gdeOCInherit::List &GetInherits(){ return pInherits; }
	inline const gdeOCInherit::List &GetInherits() const{ return pInherits; }
	
	/** Notify listeners inherit changed. */
	void NotifyInheritChanged(gdeOCInherit *inherit);
	
	/** Notify listeners inherit structure changed. */
	void NotifyInheritsChanged();
	
	/** Default property name to propose if this object class is inherited. */
	inline const decString &GetDefaultInheritPropertyPrefix() const{ return pDefaultInheritPropertyPrefix; }
	
	/** Set default property name to propose if this object class is inherited. */
	void SetDefaultInheritPropertyPrefix(const char *propertyName);
	
	/** Object class inherits (in-)directly object class. */
	bool InheritsFrom(const gdeObjectClass *objectClass) const;
	
	/** Object class is or inherits (in-)directly object class. */
	bool IsOrInheritsFrom(const gdeObjectClass *objectClass) const;
	bool IsOrInheritsFrom(const char *name) const;
	
	
	
	/** Billboards. */
	inline gdeOCBillboard::List &GetBillboards(){ return pBillboards; }
	inline const gdeOCBillboard::List &GetBillboards() const{ return pBillboards; }
	
	/** Notify listeners billboards changed. */
	void NotifyBillboardsChanged();
	
	/** Notify listeners billboard changed. */
	void NotifyBillboardChanged(gdeOCBillboard *billboard);
	
	
	
	/** Cameras. */
	inline gdeOCCamera::List &GetCameras(){ return pCameras; }
	inline const gdeOCCamera::List &GetCameras() const{ return pCameras; }
	
	/** Notify listeners cameras changed. */
	void NotifyCamerasChanged();
	
	/** Notify listeners camera changed. */
	void NotifyCameraChanged(gdeOCCamera *camera);
	
	
	
	/** Components. */
	inline gdeOCComponent::List &GetComponents(){ return pComponents; }
	inline const gdeOCComponent::List &GetComponents() const{ return pComponents; }
	
	/** Notify listeners components changed. */
	void NotifyComponentsChanged();
	
	/** Notify listeners component changed. */
	void NotifyComponentChanged(gdeOCComponent *component);
	
	/** Notify listeners component texture changed. */
	void NotifyComponentTextureChanged(gdeOCComponent *component,
		gdeOCComponentTexture *texture);
	
	/** Notify listeners component texture name changed. */
	void NotifyComponentTextureNameChanged(gdeOCComponent *component,
		gdeOCComponentTexture *texture);
	
	/** Notify listeners component texture properties changed. */
	void NotifyComponentTexturePropertiesChanged(gdeOCComponent *component,
		gdeOCComponentTexture *texture);
	
	
	
	/** Lights. */
	inline gdeOCLight::List &GetLights(){ return pLights; }
	inline const gdeOCLight::List &GetLights() const{ return pLights; }
	
	/** Notify listeners lights changed. */
	void NotifyLightsChanged();
	
	/** Notify listeners light changed. */
	void NotifyLightChanged(gdeOCLight *light);
	
	
	
	/** Environment map probes. */
	inline gdeOCEnvMapProbe::List &GetEnvMapProbes(){ return pEnvMapProbes; }
	inline const gdeOCEnvMapProbe::List &GetEnvMapProbes() const{ return pEnvMapProbes; }
	
	/** Notify listeners environment map probes changed. */
	void NotifyEnvMapProbesChanged();
	
	/** Notify listeners environment map probe changed. */
	void NotifyEnvMapProbeChanged(gdeOCEnvMapProbe *envMapProbe);
	
	
	
	/** Navigation blockers. */
	inline gdeOCNavigationBlocker::List &GetNavigationBlockers(){ return pNavigationBlockers; }
	inline const gdeOCNavigationBlocker::List &GetNavigationBlockers() const{ return pNavigationBlockers; }
	
	/** Notify listeners navigation blockers changed. */
	void NotifyNavigationBlockersChanged();
	
	/** Notify listeners navigation blocker changed. */
	void NotifyNavigationBlockerChanged(gdeOCNavigationBlocker *navblocker);
	
	
	
	/** Navigation spaces. */
	inline gdeOCNavigationSpace::List &GetNavigationSpaces(){ return pNavigationSpaces; }
	inline const gdeOCNavigationSpace::List &GetNavigationSpaces() const{ return pNavigationSpaces; }
	
	/** Notify listeners navigation spaces changed. */
	void NotifyNavigationSpacesChanged();
	
	/** Notify listeners navigation space changed. */
	void NotifyNavigationSpaceChanged(gdeOCNavigationSpace *navspace);
	
	
	
	/** Particle emitters. */
	inline gdeOCParticleEmitter::List &GetParticleEmitters(){ return pParticleEmitters; }
	inline const gdeOCParticleEmitter::List &GetParticleEmitters() const{ return pParticleEmitters; }
	
	/** Notify listeners particle emitters changed. */
	void NotifyParticleEmittersChanged();
	
	/** Notify listeners particle emitter changed. */
	void NotifyParticleEmitterChanged(gdeOCParticleEmitter *emitter);
	
	
	
	/** Force fields. */
	inline gdeOCForceField::List &GetForceFields(){ return pForceFields; }
	inline const gdeOCForceField::List &GetForceFields() const{ return pForceFields; }
	
	/** Notify listeners force fields changed. */
	void NotifyForceFieldsChanged();
	
	/** Notify listeners force fields changed. */
	void NotifyForceFieldChanged(gdeOCForceField *field);
	
	
	
	/** Snap points. */
	inline gdeOCSnapPoint::List &GetSnapPoints(){ return pSnapPoints; }
	inline const gdeOCSnapPoint::List &GetSnapPoints() const{ return pSnapPoints; }
	
	/** Notify listeners snap points changed. */
	void NotifySnapPointsChanged();
	
	/** Notify listeners snap point changed. */
	void NotifySnapPointChanged(gdeOCSnapPoint *snapoint);
	
	/** Notify listeners snap point name changed. */
	void NotifySnapPointNameChanged(gdeOCSnapPoint *snapoint);
	
	
	
	/** Speakers. */
	inline gdeOCSpeaker::List &GetSpeakers(){ return pSpeakers; }
	inline const gdeOCSpeaker::List &GetSpeakers() const{ return pSpeakers; }
	
	/** Notify listeners speakers changed. */
	void NotifySpeakersChanged();
	
	/** Notify listeners speaker changed. */
	void NotifySpeakerChanged(gdeOCSpeaker *speaker);
	
	
	
	/** Worlds. */
	inline gdeOCWorld::List &GetWorlds(){ return pWorlds; }
	inline const gdeOCWorld::List &GetWorlds() const{ return pWorlds; }
	
	/** Notify listeners world changed. */
	void NotifyWorldsChanged();
	
	/** Notify listeners world changed. */
	void NotifyWorldChanged(gdeOCWorld *world);
	
	
	
	/** List of textures. */
	inline gdeOCComponentTexture::List &GetTextures(){ return pTextures; }
	inline const gdeOCComponentTexture::List &GetTextures() const{ return pTextures; }
	
	/** Active texture or \em nullptr if none. */
	inline const gdeOCComponentTexture::Ref &GetActiveTexture() const{ return pActiveTexture; }
	
	/** Set active texture or \em nullptr if none. */
	void SetActiveTexture(gdeOCComponentTexture *texture);
	
	/** Notify listeners textures changed. */
	void NotifyTexturesChanged();
	
	/** Notify listeners texture changed. */
	void NotifyTextureChanged(gdeOCComponentTexture *texture);
	
	
	
	/** Object is ghost not blocking other objects. */
	inline bool GetIsGhost() const{ return pIsGhost; }
	
	/** Set if object is a ghost not blocking other objects. */
	void SetIsGhost(bool isGhost);
	
	/** Object can be instantiated. */
	inline bool GetCanInstantiate() const{ return pCanInstantiate; }
	
	/** Set if object can be instantiated. */
	void SetCanInstantiate(bool canInstantiate);
	
	/** Inherit sub objects. */
	inline int GetInheritSubObjects() const{ return pInheritSubObjects; }
	
	/** Set inherit sub objects. */
	void SetInheritSubObjects(int filter);
	
	/** Object class is an attachable behavior. */
	inline bool GetIsAttachableBehavior() const{ return pIsAttachableBehavior; }
	
	/** Set if object class is an attachable behavior. */
	void SetIsAttachableBehavior(bool isAttachableBehavior);
	
	
	/** Named property searching inheritance. */
	bool DeepGetNamedProperty(const char *name, const gdeObjectClass* &objectClass,
		const gdeProperty* &property) const;
	
	/** Named property default value searching inheritance. */
	bool NamedPropertyDefaultValue(const char *name, decString &value) const;
	
	/**
	 * Add property names in this class and optionally inherited classes.
	 * \param[out] set Set to add property names to.
	 * \param[in] inherited Include inherited parameters.
	 */
	void AddPropertyNamesTo(decStringSet &set, bool inherited = false) const;
	
	/**
	 * Add property names in this class and optionally inherited classes.
	 * \param[out] set Set to add property names to.
	 * \param[in] inherited Include inherited parameters.
	 */
	void AddTexturePropertyNamesTo(decStringSet &set, bool inherited = false) const;
	
	/** Add all properties with inheritance if required. */
	void AddPropertiesTo(decStringDictionary &properties, bool inherited = false,
		bool includeValues = true) const;
	
	/** Find identifiers. */
	void GetDefinedUsedIDs(decStringSet &definedIDs, decStringSet &usedIDs) const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pAddPropertyNames(decStringSet &set, const decString &prefix) const;
	void pAddTexturePropertyNames(decStringSet &set, const decString &prefix) const;
	void pAddPropertiesTo(decStringDictionary &properties, const decString &prefix) const;
};

#endif
