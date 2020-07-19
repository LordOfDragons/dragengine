/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEGDCLASS_H_
#define _IGDEGDCLASS_H_

#include "billboard/igdeGDCBillboardList.h"
#include "component/igdeGDCComponentList.h"
#include "envmapprobe/igdeGDCEnvMapProbeList.h"
#include "light/igdeGDCLightList.h"
#include "snappoint/igdeGDCSnapPointList.h"
#include "navspace/igdeGDCNavigationSpaceList.h"
#include "navblocker/igdeGDCNavigationBlockerList.h"
#include "particleemitter/igdeGDCParticleEmitterList.h"
#include "forcefield/igdeGDCForceFieldList.h"
#include "speaker/igdeGDCSpeakerList.h"
#include "../igdeTagManager.h"
#include "../property/igdeGDPropertyList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decIntList.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/string/decStringDictionary.h>

class deglIntList;
class igdeGDClassInherit;
class igdeGDCamera;
class igdeGDClassManager;
class deImage;



/**
 * \brief Game Definition Class.
 *
 * Stores the definition of a class in a game definition.
 */
class igdeGDClass : public deObject{
public:
	/** \brief Scale modes. */
	enum eScaleModes{
		/** \brief Fixed size. */
		esmFixed,
		
		/** \brief Scalable uniformly. */
		esmUniform,
		
		/** \brief Scalable freely. */
		esmFree
	};
	
	
	
private:
	decString pName;
	decString pDescription;
	eScaleModes pScaleMode;
	igdeGDCamera *pCamera;
	bool pHasCamera;
	bool pIsGhost;
	bool pCanInstantiate;
	decString pCategory;
	igdeGDPropertyList pListProperties;
	decStringDictionary pPropertyValues;
	igdeGDPropertyList pTextureProperties;
	igdeGDCBillboardList pListBillboards;
	igdeGDCComponentList pListComponents;
	igdeGDCLightList pListLights;
	igdeGDCSnapPointList pSnapPoints;
	igdeGDCParticleEmitterList pListParticleEmitters;
	igdeGDCForceFieldList pListForceFields;
	igdeGDCEnvMapProbeList pListEnvMapProbes;
	igdeGDCSpeakerList pListSpeakers;
	igdeGDCNavigationSpaceList pListNavigationSpaces;
	igdeGDCNavigationBlockerList pListNavigationBlockers;
	
	igdeTagManager pHideTags;
	igdeTagManager pPartialHideTags;
	
	decObjectOrderedSet pInheritClasses;
	
	decString pPathEClass;
	
	deImage *pPreviewImage;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create named game definition class. */
	igdeGDClass( const char *name );
	
	/** \brief Create game definition class as a copy of another game definition class. */
	igdeGDClass( const igdeGDClass &gdclass );
	
	
	
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
	void SetDescription( const char *description );
	
	/** \brief Scale mode. */
	inline eScaleModes GetScaleMode() const{ return pScaleMode; }
	
	/** \brief Set scale mode. */
	void SetScaleMode( eScaleModes mode );
	
	/** \brief Cathegory. */
	inline const decString &GetCategory() const{ return pCategory; }
	
	/** \brief Set cathegory. */
	void SetCategory( const char *category );
	
	/** \brief Class camera. */
	inline igdeGDCamera *GetCamera() const{ return pCamera; }
	
	/** \brief Determines if the class has a camera. */
	inline bool GetHasCamera() const{ return pHasCamera; }
	
	/** \brief Sets if the class has a camera. */
	void SetHasCamera( bool hasCamera );
	
	/** \brief Determines if this is a ghost object not blocking other objects. */
	inline bool GetIsGhost() const{ return pIsGhost; }
	
	/** \brief Sets if this is a ghost object not blocking other objects. */
	void SetIsGhost( bool isGhost );
	
	/** \brief Object can be instantiated. */
	inline bool GetCanInstantiate() const{ return pCanInstantiate; }
	
	/** \brief Set if object can be instantiated. */
	void SetCanInstantiate( bool canInstantiate );
	
	/** \brief Hide tags. */
	inline igdeTagManager &GetHideTags(){ return pHideTags; }
	inline const igdeTagManager &GetHideTags() const{ return pHideTags; }
	
	/** \brief Partial hide tags. */
	inline igdeTagManager &GetPartialHideTags(){ return pPartialHideTags; }
	inline const igdeTagManager &GetPartialHideTags() const{ return pPartialHideTags; }
	
	
	
	/** \brief Count of inherit classes. */
	int GetInheritClassCount() const;
	
	/** \brief Inherit class at index. */
	igdeGDClassInherit *GetInheritClassAt( int index ) const;
	
	/** \brief Add inherit class. */
	void AddInheritClass( igdeGDClassInherit *inheritClass );
	
	/** \brief Find inherited classes from their names if present. */
	void ResolveInheritClasses( const igdeGDClassManager &classManager );
	
	/** \brief Remove all inherit classes. */
	void RemoveAllInheritClasses();
	
	/** \brief Class directly or indirectly inherits another class. */
	bool Inherits( igdeGDClass *gdclass ) const;
	
	
	
	/** \brief Checks the class for errors. */
	void Check();
	
	/** \brief Preview image or NULL if not created yet. */
	inline deImage *GetPreviewImage() const{ return pPreviewImage; }
	
	/** \brief Set preview image or NULL if not created yet. */
	void SetPreviewImage( deImage *image );
	
	/** \brief Element class path or empty string if defined manually. */
	inline const decString &GetPathEClass() const{ return pPathEClass; }
	
	/** \brief Set element class path or empty string if defined manually. */
	void SetPathEClass( const decString &pathEClass );
	/*@}*/
	
	
	
	/** \name Properties */
	/*@{*/
	/** \brief List of class properties. */
	inline const igdeGDPropertyList &GetListProperties() const{ return pListProperties; }
	
	/** \brief Adds a class property. */
	void AddProperty( igdeGDProperty *property );
	
	/** \brief Removes a class property. */
	void RemoveProperty( igdeGDProperty *property );
	
	/** \brief Removes all class properties. */
	void RemoveAllProperties();
	
	/** \brief Property values. */
	inline const decStringDictionary &GetPropertyValues() const{ return pPropertyValues; }
	
	/** \brief Set property values. */
	void SetPropertyValues( const decStringDictionary &values );
	
	/** \brief First named property in class and inherited or NULL if not found. */
	igdeGDProperty *GetPropertyNamed( const char *name ) const;
	
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
	bool GetDefaultPropertyValue( const char *name, decString &value ) const;
	
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
	bool HasDefaultPropertyValue( const char *name ) const;
	
	/**
	 * \brief Add property names in this class and optionally inherited classes.
	 * \param[out] set Set to add property names to.
	 * \param[in] inherited Include inherited parameters.
	 */
	void AddPropertyNames( decStringSet &set, bool inherited = false ) const;
	/*@}*/
	
	
	
	/** \name Texture properties */
	/*@{*/
	/** \brief Texture properties. */
	inline const igdeGDPropertyList &GetTextureProperties() const{ return pTextureProperties; }
	
	/** \brief Set texture properties. */
	void SetTextureProperties( const igdeGDPropertyList &properties );
	
	/** \brief Add texture property. */
	void AddTextureProperty( igdeGDProperty *property );
	
	/** \brief Remove texture property. */
	void RemoveTextureProperty( igdeGDProperty *property );
	
	/** \brief Remove all texture properties. */
	void RemoveAllTextureProperties();
	
	/** \brief First named texture property in class and inherited or NULL if not found. */
	igdeGDProperty *GetTexturePropertyNamed( const char *name ) const;
	
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
	bool GetDefaultTexturePropertyValue( const char *name, decString &value ) const;
	
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
	bool HasDefaultTexturePropertyValue( const char *name ) const;
	
	/**
	 * \brief Add texture property names in this class and optionally inherited classes.
	 * \param[out] set Set to add texture property names to.
	 * \param[in] inherited Include inherited parameters.
	 */
	void AddTexturePropertyNames( decStringSet &set, bool inherited = false ) const;
	/*@}*/
	
	
	
	/** \name Components */
	/*@{*/
	/** \brief List of components. */
	inline const igdeGDCComponentList &GetComponentList() const{ return pListComponents; }
	
	/** \brief Adds a component. */
	void AddComponent( igdeGDCComponent *component );
	
	/** \brief Removes a component. */
	void RemoveComponent( igdeGDCComponent *component );
	
	/** \brief Removes all components. */
	void RemoveAllComponents();
	
	/** \brief Determines if a component has a named linked property. */
	bool HasComponentLinkedProperty( const char *name ) const;
	
	/** \brief Retrieves a list of components with a named linked property. */
	const igdeGDCComponentList GetComponentsWithLinkedProperty( const char *name ) const;
	
	/** \brief Retrieves a list of indices of components with a named linked property. */
	const decIntList GetComponentIndicesWithLinkedProperty( const char *name ) const;
	/*@}*/
	
	
	
	/** \name Lights */
	/*@{*/
	/** \brief List of lights. */
	inline const igdeGDCLightList &GetLightList() const{ return pListLights; }
	
	/** \brief Adds a light. */
	void AddLight( igdeGDCLight *light );
	
	/** \brief Removes a light. */
	void RemoveLight( igdeGDCLight *light );
	
	/** \brief Removes all lights. */
	void RemoveAllLights();
	
	/** \brief Determines if a light has a named linked property. */
	bool HasLightLinkedProperty( const char *name ) const;
	
	/** \brief Retrieves a list of lights with a named linked property. */
	const igdeGDCLightList GetLightWithLinkedProperty( const char *name ) const;
	
	/** \brief Retrieves a list of indices of lights with a named linked property. */
	const decIntList GetLightIndicesWithLinkedProperty( const char *name ) const;
	/*@}*/
	
	
	
	/** \name Billboards */
	/*@{*/
	/** \brief List of billboards. */
	inline const igdeGDCBillboardList &GetBillboardList() const{ return pListBillboards; }
	
	/** \brief Adds a billboard. */
	void AddBillboard( igdeGDCBillboard *billboard );
	
	/** \brief Removes a billboard. */
	void RemoveBillboard( igdeGDCBillboard *billboard );
	
	/** \brief Removes all billboards. */
	void RemoveAllBillboards();
	
	/** \brief Determines if a billboard has a named linked property. */
	bool HasBillboardLinkedProperty( const char *name ) const;
	
	/** \brief Retrieves a list of billboards with a named linked property. */
	const igdeGDCBillboardList GetBillboardWithLinkedProperty( const char *name ) const;
	
	/** \brief Retrieves a list of indices of billboards with a named linked property. */
	const decIntList GetBillboardIndicesWithLinkedProperty( const char *name ) const;
	/*@}*/
	
	
	
	/** \name SnapPoints */
	/*@{*/
	/** \brief List of snap points. */
	inline const igdeGDCSnapPointList &GetSnapPointList() const{ return pSnapPoints; }
	
	/** \brief Add snap point. */
	void AddSnapPoint( igdeGDCSnapPoint *snappoint );
	
	/** \brief Remove snap point. */
	void RemoveSnapPoint( igdeGDCSnapPoint *snapppoint );
	
	/** \brief Remove all snap points. */
	void RemoveAllSnapPoints();
	/*@}*/
	
	
	
	/** \name Particle Emitters */
	/*@{*/
	/** \brief List of particle emitters. */
	inline const igdeGDCParticleEmitterList &GetParticleEmitterList() const{ return pListParticleEmitters; }
	
	/** \brief Adds a particle emitter. */
	void AddParticleEmitter( igdeGDCParticleEmitter *emitter );
	
	/** \brief Removes a particle emitter. */
	void RemoveParticleEmitter( igdeGDCParticleEmitter *emitter );
	
	/** \brief Removes all particle emitters. */
	void RemoveAllParticleEmitters();
	/*@}*/
	
	
	
	/** \name Force Fields */
	/*@{*/
	/** \brief List of force fields. */
	inline const igdeGDCForceFieldList &GetForceFieldList() const{ return pListForceFields; }
	
	/** \brief Adds a force field. */
	void AddForceField( igdeGDCForceField *field );
	
	/** \brief Removes a force field. */
	void RemoveForceField( igdeGDCForceField *field );
	
	/** \brief Removes all force fields. */
	void RemoveAllForceFields();
	/*@}*/
	
	
	
	/** \name Environment Map Probes */
	/*@{*/
	/** \brief List of environment map probes. */
	inline const igdeGDCEnvMapProbeList &GetEnvironmentMapProbeList() const{ return pListEnvMapProbes; }
	
	/** \brief Adds an environment map probe. */
	void AddEnvironmentMapProbe( igdeGDCEnvMapProbe *probe );
	
	/** \brief Removes an environment map probe. */
	void RemoveEnvironmentMapProbe( igdeGDCEnvMapProbe *probe );
	
	/** \brief Removes all environment map probes. */
	void RemoveAllEnvironmentMapProbes();
	
	/** \brief Determines if an environment map probes has a named linked property. */
	bool HasEnvMapProbesLinkedProperty( const char *name ) const;
	
	/** \brief Retrieves a list of environment map probes with a named linked property. */
	const igdeGDCEnvMapProbeList GetEnvMapProbesWithLinkedProperty( const char *name ) const;
	
	/** \brief Retrieves a list of indices of environment map probes with a named linked property. */
	const decIntList GetEnvMapProbesIndicesWithLinkedProperty( const char *name ) const;
	/*@}*/
	
	
	
	/** \name Speakers */
	/*@{*/
	/** \brief List of speakers. */
	inline const igdeGDCSpeakerList &GetSpeakerList() const{ return pListSpeakers; }
	
	/** \brief Adds a speaker. */
	void AddSpeaker( igdeGDCSpeaker *speaker );
	
	/** \brief Removes a speaker. */
	void RemoveSpeaker( igdeGDCSpeaker *speaker );
	
	/** \brief Removes all speakers. */
	void RemoveAllSpeakers();
	
	/** \brief Determines if a speaker has a named linked property. */
	bool HasSpeakerLinkedProperty( const char *name ) const;
	
	/** \brief Retrieves a list of speakers with a named linked property. */
	const igdeGDCSpeakerList GetSpeakerWithLinkedProperty( const char *name ) const;
	
	/** \brief Retrieves a list of indices of speakers with a named linked property. */
	const decIntList GetSpeakerIndicesWithLinkedProperty( const char *name ) const;
	/*@}*/
	
	
	
	/** \name Navigation spaces */
	/*@{*/
	/** \brief List of navigation spaces. */
	inline const igdeGDCNavigationSpaceList &GetNavigationSpaceList() const{ return pListNavigationSpaces; }
	
	/** \brief Adds a navigation space. */
	void AddNavigationSpace( igdeGDCNavigationSpace *navSpace );
	
	/** \brief Removes a navigation space. */
	void RemoveNavigationSpace( igdeGDCNavigationSpace *navSpace );
	
	/** \brief Removes all navigation spaces. */
	void RemoveAllNavigationSpaces();
	
	/** \brief Determines if a navigation space has a named linked property. */
	bool HasNavSpaceLinkedProperty( const char *name ) const;
	
	/** \brief Retrieves a list of navigation spaces with a named linked property. */
	const igdeGDCNavigationSpaceList GetNavSpaceWithLinkedProperty( const char *name ) const;
	
	/** \brief Retrieves a list of indices of navigation spaces with a named linked property. */
	const decIntList GetNavSpaceIndicesWithLinkedProperty( const char *name ) const;
	/*@}*/
	
	
	
	/** \name Navigation blockers */
	/*@{*/
	/** \brief List of navigation blockers. */
	inline const igdeGDCNavigationBlockerList &GetNavigationBlockerList() const{ return pListNavigationBlockers; }
	
	/** \brief Adds a navigation blocker. */
	void AddNavigationBlocker( igdeGDCNavigationBlocker *blocker );
	
	/** \brief Removes a navigation blocker. */
	void RemoveNavigationBlocker( igdeGDCNavigationBlocker *blocker );
	
	/** \brief Removes all navigation blockers. */
	void RemoveAllNavigationBlockers();
	
	/** \brief Determines if a navigation blocker has a named linked property. */
	bool HasNavBlockerLinkedProperty( const char *name ) const;
	
	/** \brief Retrieves a list of navigation blockers with a named linked property. */
	const igdeGDCNavigationBlockerList GetNavBlockerWithLinkedProperty( const char *name ) const;
	
	/** \brief Retrieves a list of indices of navigation blockers with a named linked property. */
	const decIntList GetNavBlockerIndicesWithLinkedProperty( const char *name ) const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pAddPropertyNames( decStringSet &set, const decString &prefix ) const;
	void pAddTexturePropertyNames( decStringSet &set, const decString &prefix ) const;
};

#endif
