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

#ifndef _IGDEGDCLASS_H_
#define _IGDEGDCLASS_H_

#include "billboard/igdeGDCBillboard.h"
#include "component/igdeGDCComponent.h"
#include "component/igdeGDCCTexture.h"
#include "envmapprobe/igdeGDCEnvMapProbe.h"
#include "light/igdeGDCLight.h"
#include "snappoint/igdeGDCSnapPoint.h"
#include "navspace/igdeGDCNavigationSpace.h"
#include "navblocker/igdeGDCNavigationBlocker.h"
#include "particleemitter/igdeGDCParticleEmitter.h"
#include "forcefield/igdeGDCForceField.h"
#include "speaker/igdeGDCSpeaker.h"
#include "world/igdeGDCWorld.h"
#include "../igdeTagManager.h"
#include "../property/igdeGDProperty.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTList.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/string/decStringDictionary.h>
#include <dragengine/resources/image/deImage.h>

class deglIntList;
class igdeGDClassInherit;
class igdeGDCamera;
class igdeGDClassManager;



/**
 * \brief Game Definition Class.
 *
 * Stores the definition of a class in a game definition.
 */
class DE_DLL_EXPORT igdeGDClass : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeGDClass> Ref;
	
	typedef decTList<int> IndexList;
	
	
	/** \brief Scale modes. */
	enum eScaleModes{
		/** \brief Fixed size. */
		esmFixed,
		
		/** \brief Scalable uniformly. */
		esmUniform,
		
		/** \brief Scalable freely. */
		esmFree
	};
	
	/** \brief Sub objects filter. */
	enum eFilterSubObjects{
		efsoBillboards = 0x1,
		efsoComponents = 0x2,
		efsoLights = 0x4,
		efsoSnapPoints = 0x8,
		efsoParticleEmitters = 0x10,
		efsoForceFields = 0x20,
		efsoEnvMapProbes = 0x40,
		efsoSpeakers = 0x80,
		efsoNavigationSpaces = 0x100,
		efsoNavigationBlockers = 0x200,
		efsoWorlds = 0x400
	};
	
	static const int FilterSubObjectsAll = 0x7ff;
	
	
	
private:
	decString pName;
	decString pDescription;
	eScaleModes pScaleMode;
	igdeGDCamera *pCamera;
	bool pHasCamera, pIsGhost, pCanInstantiate, pIsAttachableBehavior;
	decString pCategory;
	igdeGDProperty::List pListProperties;
	decStringDictionary pPropertyValues;
	igdeGDProperty::List pTextureProperties;
	igdeGDCBillboard::List pListBillboards;
	igdeGDCComponent::List pListComponents;
	igdeGDCLight::List pListLights;
	igdeGDCSnapPoint::List pSnapPoints;
	igdeGDCParticleEmitter::List pListParticleEmitters;
	igdeGDCForceField::List pListForceFields;
	igdeGDCEnvMapProbe::List pListEnvMapProbes;
	igdeGDCSpeaker::List pListSpeakers;
	igdeGDCNavigationSpace::List pListNavigationSpaces;
	igdeGDCNavigationBlocker::List pListNavigationBlockers;
	igdeGDCWorld::List pListWorlds;
	int pInheritSubObjects;
	
	igdeTagManager pHideTags;
	igdeTagManager pPartialHideTags;
	
	decObjectOrderedSet pInheritClasses;
	decString pDefaultInheritPropertyPrefix;
	
	decString pPathEClass;
	igdeGDCCTexture::List pComponentTextures;
	
	deImage::Ref pPreviewImage;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create named game definition class. */
	igdeGDClass(const char *name);
	
	/** \brief Create game definition class as a copy of another game definition class. */
	igdeGDClass(const igdeGDClass &gdclass);
	
	
	
protected:
	/** \brief Clean up game definition class. */
	virtual ~igdeGDClass();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name of the class. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Description of the class. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description of the class. */
	void SetDescription(const char *description);
	
	/** \brief Scale mode. */
	inline eScaleModes GetScaleMode() const{ return pScaleMode; }
	
	/** \brief Set scale mode. */
	void SetScaleMode(eScaleModes mode);
	
	/** \brief Cathegory. */
	inline const decString &GetCategory() const{ return pCategory; }
	
	/** \brief Set cathegory. */
	void SetCategory(const char *category);
	
	/** \brief Class camera. */
	inline igdeGDCamera *GetCamera() const{ return pCamera; }
	
	/** \brief Determines if the class has a camera. */
	inline bool GetHasCamera() const{ return pHasCamera; }
	
	/** \brief Sets if the class has a camera. */
	void SetHasCamera(bool hasCamera);
	
	/** \brief Determines if this is a ghost object not blocking other objects. */
	inline bool GetIsGhost() const{ return pIsGhost; }
	
	/** \brief Sets if this is a ghost object not blocking other objects. */
	void SetIsGhost(bool isGhost);
	
	/** \brief Object can be instantiated. */
	inline bool GetCanInstantiate() const{ return pCanInstantiate; }
	
	/** \brief Set if object can be instantiated. */
	void SetCanInstantiate(bool canInstantiate);
	
	/** Object class is an attachable behavior. */
	inline bool GetIsAttachableBehavior() const{ return pIsAttachableBehavior; }
	
	/** Set if object class is an attachable behavior. */
	void SetIsAttachableBehavior(bool isAttachableBehavior);
	
	/** \brief Hide tags. */
	inline igdeTagManager &GetHideTags(){ return pHideTags; }
	inline const igdeTagManager &GetHideTags() const{ return pHideTags; }
	
	/** \brief Partial hide tags. */
	inline igdeTagManager &GetPartialHideTags(){ return pPartialHideTags; }
	inline const igdeTagManager &GetPartialHideTags() const{ return pPartialHideTags; }
	
	/** \brief Inherit sub objects filter. */
	inline int GetInheritSubObjects() const{ return pInheritSubObjects; }
	
	/** \brief Set inherit sub objects filter. */
	void SetInheritSubObjects(int filter);
	
	
	
	/** \brief Count of inherit classes. */
	int GetInheritClassCount() const;
	
	/** \brief Inherit class at index. */
	igdeGDClassInherit *GetInheritClassAt(int index) const;
	
	/** \brief Add inherit class. */
	void AddInheritClass(igdeGDClassInherit *inheritClass);
	
	/** \brief Find inherited classes from their names if present. */
	void ResolveInheritClasses(const igdeGDClassManager &classManager);
	
	/** \brief Remove all inherit classes. */
	void RemoveAllInheritClasses();
	
	/** \brief Class directly or indirectly inherits another class. */
	bool Inherits(igdeGDClass *gdclass) const;
	
	
	
	/** \brief Checks the class for errors. */
	void Check();
	
	/** \brief Preview image or nullptr if not created yet. */
	inline const deImage::Ref &GetPreviewImage() const{ return pPreviewImage; }
	
	/** \brief Set preview image or nullptr if not created yet. */
	void SetPreviewImage(deImage *image);
	
	/** \brief Default inherit property prefix. */
	inline const decString &GetDefaultInheritPropertyPrefix() const{ return pDefaultInheritPropertyPrefix; }
	
	/** \brief Set default inherit property prefix. */
	void SetDefaultInheritPropertyPrefix(const decString &prefix);
	
	/** \brief Element class path or empty string if defined manually. */
	inline const decString &GetPathEClass() const{ return pPathEClass; }
	
	/** \brief Set element class path or empty string if defined manually. */
	void SetPathEClass(const decString &pathEClass);
	
	/** \brief Component texture replacements. */
	inline igdeGDCCTexture::List &GetComponentTextures(){ return pComponentTextures; }
	inline const igdeGDCCTexture::List &GetComponentTextures() const{ return pComponentTextures; }
	
	/** \brief Build flattened component texture replacements list. */
	void GetDeepComponentTextures(igdeGDCCTexture::List &list) const;
	/*@}*/
	
	
	
	/** \name Properties */
	/*@{*/
	/** \brief List of class properties. */
	inline const igdeGDProperty::List &GetListProperties() const{ return pListProperties; }
	
	/** \brief Adds a class property. */
	void AddProperty(igdeGDProperty *property);
	
	/** \brief Removes a class property. */
	void RemoveProperty(igdeGDProperty *property);
	
	/** \brief Removes all class properties. */
	void RemoveAllProperties();
	
	/** \brief Property values. */
	inline const decStringDictionary &GetPropertyValues() const{ return pPropertyValues; }
	
	/** \brief Set property values. */
	void SetPropertyValues(const decStringDictionary &values);
	
	/** \brief First named property in class and inherited or nullptr if not found. */
	igdeGDProperty *GetPropertyNamed(const char *name) const;
	
	/**
	 * \brief Get default property value including parent classes.
	 * 
	 * In each class checks first GetPropertyValues() for a match then GetListProperties()
	 * for a match. If property from GetListProperties() matches its default value is used.
	 * 
	 * \param[in] name Name of property to find default value for.
	 * \param[out] value Variable to write found default value to.
	 * \retval true Default value has been found and has been written to \em value.
	 * \retval false Default value has not been found. \em value is left untouched.
	 */
	bool GetDefaultPropertyValue(const char *name, decString &value) const;
	
	/**
	 * \brief Check if default property exists value including parent classes.
	 * 
	 * In each class checks first GetPropertyValues() for a match then GetListProperties()
	 * for a match. If property from GetListProperties() matches its default value is used.
	 * 
	 * \param[in] name Name of property to check.
	 * \retval true Default value is present.
	 * \retval false Default value is absent.
	 */
	bool HasDefaultPropertyValue(const char *name) const;
	
	/**
	 * \brief Add property names in this class and optionally inherited classes.
	 * \param[out] set Set to add property names to.
	 * \param[in] inherited Include inherited parameters.
	 */
	void AddPropertyNames(decStringSet &set, bool inherited = false) const;
	/*@}*/
	
	
	
	/** \name Texture properties */
	/*@{*/
	/** \brief Texture properties. */
	inline const igdeGDProperty::List &GetTextureProperties() const{ return pTextureProperties; }
	
	/** \brief Set texture properties. */
	void SetTextureProperties(const igdeGDProperty::List &properties);
	
	/** \brief Add texture property. */
	void AddTextureProperty(igdeGDProperty *property);
	
	/** \brief Remove texture property. */
	void RemoveTextureProperty(igdeGDProperty *property);
	
	/** \brief Remove all texture properties. */
	void RemoveAllTextureProperties();
	
	/** \brief First named texture property in class and inherited or nullptr if not found. */
	igdeGDProperty *GetTexturePropertyNamed(const char *name) const;
	
	/**
	 * \brief Get default texture property value including parent classes.
	 * 
	 * In each class checks first GetTexturePropertyValues() for a match then GetListProperties()
	 * for a match. If texture property from GetListProperties() matches its default value is used.
	 * 
	 * \param[in] name Name of texture property to find default value for.
	 * \param[out] value Variable to write found default value to.
	 * \retval true Default value has been found and has been written to \em value.
	 * \retval false Default value has not been found. \em value is left untouched.
	 */
	bool GetDefaultTexturePropertyValue(const char *name, decString &value) const;
	
	/**
	 * \brief Check if default texture property exists value including parent classes.
	 * 
	 * In each class checks first GetTexturePropertyValues() for a match then GetListProperties()
	 * for a match. If texture property from GetListProperties() matches its default value is used.
	 * 
	 * \param[in] name Name of texture property to check.
	 * \retval true Default value is present.
	 * \retval false Default value is absent.
	 */
	bool HasDefaultTexturePropertyValue(const char *name) const;
	
	/**
	 * \brief Add texture property names in this class and optionally inherited classes.
	 * \param[out] set Set to add texture property names to.
	 * \param[in] inherited Include inherited parameters.
	 */
	void AddTexturePropertyNames(decStringSet &set, bool inherited = false) const;
	/*@}*/
	
	
	
	/** \name Components */
	/*@{*/
	/** \brief List of components. */
	inline const igdeGDCComponent::List &GetComponentList() const{ return pListComponents; }
	
	/** \brief Adds a component. */
	void AddComponent(igdeGDCComponent *component);
	
	/** \brief Removes a component. */
	void RemoveComponent(igdeGDCComponent *component);
	
	/** \brief Removes all components. */
	void RemoveAllComponents();
	
	/** \brief Determines if a component has a named linked property. */
	bool HasComponentLinkedProperty(const char *name) const;
	
	/** \brief Retrieves a list of components with a named linked property. */
	const igdeGDCComponent::List GetComponentsWithLinkedProperty(const char *name) const;
	
	/** \brief Retrieves a list of indices of components with a named linked property. */
	const IndexList GetComponentIndicesWithLinkedProperty(const char *name) const;
	/*@}*/
	
	
	
	/** \name Lights */
	/*@{*/
	/** \brief List of lights. */
	inline const igdeGDCLight::List &GetLightList() const{ return pListLights; }
	
	/** \brief Adds a light. */
	void AddLight(igdeGDCLight *light);
	
	/** \brief Removes a light. */
	void RemoveLight(igdeGDCLight *light);
	
	/** \brief Removes all lights. */
	void RemoveAllLights();
	
	/** \brief Determines if a light has a named linked property. */
	bool HasLightLinkedProperty(const char *name) const;
	
	/** \brief Retrieves a list of lights with a named linked property. */
	const igdeGDCLight::List GetLightWithLinkedProperty(const char *name) const;
	
	/** \brief Retrieves a list of indices of lights with a named linked property. */
	const IndexList GetLightIndicesWithLinkedProperty(const char *name) const;
	/*@}*/
	
	
	
	/** \name Billboards */
	/*@{*/
	/** \brief List of billboards. */
	inline const igdeGDCBillboard::List &GetBillboardList() const{ return pListBillboards; }
	
	/** \brief Adds a billboard. */
	void AddBillboard(igdeGDCBillboard *billboard);
	
	/** \brief Removes a billboard. */
	void RemoveBillboard(igdeGDCBillboard *billboard);
	
	/** \brief Removes all billboards. */
	void RemoveAllBillboards();
	
	/** \brief Determines if a billboard has a named linked property. */
	bool HasBillboardLinkedProperty(const char *name) const;
	
	/** \brief Retrieves a list of billboards with a named linked property. */
	const igdeGDCBillboard::List GetBillboardWithLinkedProperty(const char *name) const;
	
	/** \brief Retrieves a list of indices of billboards with a named linked property. */
	const IndexList GetBillboardIndicesWithLinkedProperty(const char *name) const;
	/*@}*/
	
	
	
	/** \name SnapPoints */
	/*@{*/
	/** \brief List of snap points. */
	inline const igdeGDCSnapPoint::List &GetSnapPointList() const{ return pSnapPoints; }
	
	/** \brief Add snap point. */
	void AddSnapPoint(igdeGDCSnapPoint *snappoint);
	
	/** \brief Remove snap point. */
	void RemoveSnapPoint(igdeGDCSnapPoint *snapppoint);
	
	/** \brief Remove all snap points. */
	void RemoveAllSnapPoints();
	/*@}*/
	
	
	
	/** \name Particle Emitters */
	/*@{*/
	/** \brief List of particle emitters. */
	inline const igdeGDCParticleEmitter::List &GetParticleEmitterList() const{ return pListParticleEmitters; }
	
	/** \brief Adds a particle emitter. */
	void AddParticleEmitter(igdeGDCParticleEmitter *emitter);
	
	/** \brief Removes a particle emitter. */
	void RemoveParticleEmitter(igdeGDCParticleEmitter *emitter);
	
	/** \brief Removes all particle emitters. */
	void RemoveAllParticleEmitters();
	/*@}*/
	
	
	
	/** \name Force Fields */
	/*@{*/
	/** \brief List of force fields. */
	inline const igdeGDCForceField::List &GetForceFieldList() const{ return pListForceFields; }
	
	/** \brief Adds a force field. */
	void AddForceField(igdeGDCForceField *field);
	
	/** \brief Removes a force field. */
	void RemoveForceField(igdeGDCForceField *field);
	
	/** \brief Removes all force fields. */
	void RemoveAllForceFields();
	/*@}*/
	
	
	
	/** \name Environment Map Probes */
	/*@{*/
	/** \brief List of environment map probes. */
	inline const igdeGDCEnvMapProbe::List &GetEnvironmentMapProbeList() const{ return pListEnvMapProbes; }
	
	/** \brief Adds an environment map probe. */
	void AddEnvironmentMapProbe(igdeGDCEnvMapProbe *probe);
	
	/** \brief Removes an environment map probe. */
	void RemoveEnvironmentMapProbe(igdeGDCEnvMapProbe *probe);
	
	/** \brief Removes all environment map probes. */
	void RemoveAllEnvironmentMapProbes();
	
	/** \brief Determines if an environment map probes has a named linked property. */
	bool HasEnvMapProbesLinkedProperty(const char *name) const;
	
	/** \brief Retrieves a list of environment map probes with a named linked property. */
	const igdeGDCEnvMapProbe::List GetEnvMapProbesWithLinkedProperty(const char *name) const;
	
	/** \brief Retrieves a list of indices of environment map probes with a named linked property. */
	const IndexList GetEnvMapProbesIndicesWithLinkedProperty(const char *name) const;
	/*@}*/
	
	
	
	/** \name Speakers */
	/*@{*/
	/** \brief List of speakers. */
	inline const igdeGDCSpeaker::List &GetSpeakerList() const{ return pListSpeakers; }
	
	/** \brief Adds a speaker. */
	void AddSpeaker(igdeGDCSpeaker *speaker);
	
	/** \brief Removes a speaker. */
	void RemoveSpeaker(igdeGDCSpeaker *speaker);
	
	/** \brief Removes all speakers. */
	void RemoveAllSpeakers();
	
	/** \brief Determines if a speaker has a named linked property. */
	bool HasSpeakerLinkedProperty(const char *name) const;
	
	/** \brief Retrieves a list of speakers with a named linked property. */
	const igdeGDCSpeaker::List GetSpeakerWithLinkedProperty(const char *name) const;
	
	/** \brief Retrieves a list of indices of speakers with a named linked property. */
	const IndexList GetSpeakerIndicesWithLinkedProperty(const char *name) const;
	/*@}*/
	
	
	
	/** \name Navigation spaces */
	/*@{*/
	/** \brief List of navigation spaces. */
	inline const igdeGDCNavigationSpace::List &GetNavigationSpaceList() const{ return pListNavigationSpaces; }
	
	/** \brief Adds a navigation space. */
	void AddNavigationSpace(igdeGDCNavigationSpace *navSpace);
	
	/** \brief Removes a navigation space. */
	void RemoveNavigationSpace(igdeGDCNavigationSpace *navSpace);
	
	/** \brief Removes all navigation spaces. */
	void RemoveAllNavigationSpaces();
	
	/** \brief Determines if a navigation space has a named linked property. */
	bool HasNavSpaceLinkedProperty(const char *name) const;
	
	/** \brief Retrieves a list of navigation spaces with a named linked property. */
	const igdeGDCNavigationSpace::List GetNavSpaceWithLinkedProperty(const char *name) const;
	
	/** \brief Retrieves a list of indices of navigation spaces with a named linked property. */
	const IndexList GetNavSpaceIndicesWithLinkedProperty(const char *name) const;
	/*@}*/
	
	
	
	/** \name Navigation blockers */
	/*@{*/
	/** \brief List of navigation blockers. */
	inline const igdeGDCNavigationBlocker::List &GetNavigationBlockerList() const{ return pListNavigationBlockers; }
	
	/** \brief Adds a navigation blocker. */
	void AddNavigationBlocker(igdeGDCNavigationBlocker *blocker);
	
	/** \brief Removes a navigation blocker. */
	void RemoveNavigationBlocker(igdeGDCNavigationBlocker *blocker);
	
	/** \brief Removes all navigation blockers. */
	void RemoveAllNavigationBlockers();
	
	/** \brief Determines if a navigation blocker has a named linked property. */
	bool HasNavBlockerLinkedProperty(const char *name) const;
	
	/** \brief Retrieves a list of navigation blockers with a named linked property. */
	const igdeGDCNavigationBlocker::List GetNavBlockerWithLinkedProperty(const char *name) const;
	
	/** \brief Retrieves a list of indices of navigation blockers with a named linked property. */
	const IndexList GetNavBlockerIndicesWithLinkedProperty(const char *name) const;
	/*@}*/
	
	
	
	/** \name Worlds */
	/*@{*/
	/** \brief List of worlds. */
	inline const igdeGDCWorld::List &GetWorldList() const{ return pListWorlds; }
	
	/** \brief Add world. */
	void AddWorld(igdeGDCWorld *world);
	
	/** \brief Remove world. */
	void RemoveWorld(igdeGDCWorld *world);
	
	/** \brief Removes all worlds. */
	void RemoveAllWorlds();
	
	/** \brief Any world has named linked property. */
	bool HasWorldLinkedProperty(const char *name) const;
	
	/** \brief List of worlds with named linked property. */
	const igdeGDCWorld::List GetWorldsWithLinkedProperty(const char *name) const;
	
	/** \brief List of indices of worlds with named linked property. */
	const IndexList GetWorldIndicesWithLinkedProperty(const char *name) const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pAddPropertyNames(decStringSet &set, const decString &prefix) const;
	void pAddTexturePropertyNames(decStringSet &set, const decString &prefix) const;
};

#endif
