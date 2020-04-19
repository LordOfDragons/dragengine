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

#ifndef _GDEOBJECTCLASS_H_
#define _GDEOBJECTCLASS_H_

#include "billboard/gdeOCBillboardList.h"
#include "camera/gdeOCCameraList.h"
#include "component/gdeOCComponentList.h"
#include "envmapprobe/gdeOCEnvMapProbeList.h"
#include "inherit/gdeOCInheritList.h"
#include "light/gdeOCLightList.h"
#include "navblocker/gdeOCNavigationBlockerList.h"
#include "navspace/gdeOCNavigationSpaceList.h"
#include "particleemitter/gdeOCParticleEmitterList.h"
#include "forceField/gdeOCForceFieldList.h"
#include "snappoint/gdeOCSnapPointList.h"
#include "speaker/gdeOCSpeakerList.h"
#include "../property/gdePropertyList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/common/string/decStringDictionary.h>

class gdeGameDefinition;
class gdeOCComponentTexture;



/**
 * \brief Object class.
 */
class gdeObjectClass : public deObject{
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
	gdeGameDefinition *pGameDefinition;
	
	decString pName;
	decString pDescription;
	eScaleModes pScaleMode;
	
	gdePropertyList pProperties;
	gdePropertyList pTextureProperties;
	decStringDictionary pPropertyValues;
	
	decString pCategory;
	decStringSet pHideTags;
	decStringSet pPartialHideTags;
	
	gdeOCInheritList pInherits;
	decString pDefaultInheritPropertyPrefix;
	
	bool pIsGhost;
	bool pCanInstanciate;
	
	gdeOCBillboardList pBillboards;
	gdeOCCameraList pCameras;
	gdeOCComponentList pComponents;
	gdeOCLightList pLights;
	gdeOCEnvMapProbeList pEnvMapProbes;
	gdeOCSnapPointList pSnapPoints;
	gdeOCParticleEmitterList pParticleEmitters;
	gdeOCForceFieldList pForceFields;
	gdeOCSpeakerList pSpeakers;
	gdeOCNavigationBlockerList pNavigationBlockers;
	gdeOCNavigationSpaceList pNavigationSpaces;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object class. */
	gdeObjectClass();
	
	/** \brief Create object class. */
	gdeObjectClass( const char *name );
	
	/** \brief Create copy of object class. */
	gdeObjectClass( const gdeObjectClass &objectClass );
	
	/** \brief Clean up object class. */
	virtual ~gdeObjectClass();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent game definition. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set parent game definition. */
	void SetGameDefinition( gdeGameDefinition *gamedef );
	
	
	
	/** \brief Class name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set class name. */
	void SetName( const char *name );
	
	/** \brief Description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description. */
	void SetDescription( const char *description );
	
	
	
	/** \brief Scale mode. */
	inline eScaleModes GetScaleMode() const{ return pScaleMode; }
	
	/** \brief Set scale mode. */
	void SetScaleMode( eScaleModes mode );
	
	
	
	/** \brief Cathegory. */
	inline const decString &GetCategory() const{ return pCategory; }
	
	/** \brief Set cathegory. */
	void SetCategory( const char *category );
	
	/** \brief Hide tags. */
	inline const decStringSet &GetHideTags() const{ return pHideTags; }
	
	/** \brief Set hide tags. */
	void SetHideTags( const decStringSet &tags );
	
	/** \brief Partial hide tags. */
	inline const decStringSet &GetPartialHideTags() const{ return pPartialHideTags; }
	
	/** \brief Set partial hide tags. */
	void SetPartialHideTags( const decStringSet &tags );
	
	
	
	/** \brief Properties. */
	inline gdePropertyList &GetProperties(){ return pProperties; }
	inline const gdePropertyList &GetProperties() const{ return pProperties; }
	
	/** \brief Property values. */
	inline decStringDictionary &GetPropertyValues(){ return pPropertyValues; }
	inline const decStringDictionary &GetPropertyValues() const{ return pPropertyValues; }
	
	/** \brief Notify listeners property changed. */
	void NotifyPropertyChanged( gdeProperty *property );
	
	/** \brief Notify listeners property name changed. */
	void NotifyPropertyNameChanged( gdeProperty *property );
	
	/** \brief Notify listeners properties changed. */
	void NotifyPropertiesChanged();
	
	/** \brief Notify listeners property values changed. */
	void NotifyPropertyValuesChanged();
	
	
	
	/** \brief Texture properties. */
	inline gdePropertyList &GetTextureProperties(){ return pTextureProperties; }
	inline const gdePropertyList &GetTextureProperties() const{ return pTextureProperties; }
	
	/** \brief Notify listeners texture property changed. */
	void NotifyTexturePropertyChanged( gdeProperty *property );
	
	/** \brief Notify listeners texture property name changed. */
	void NotifyTexturePropertyNameChanged( gdeProperty *property );
	
	/** \brief Notify listeners texture properties changed. */
	void NotifyTexturePropertiesChanged();
	
	
	
	/** \brief Inherit. */
	inline gdeOCInheritList &GetInherits(){ return pInherits; }
	inline const gdeOCInheritList &GetInherits() const{ return pInherits; }
	
	/** \brief Notify listeners inherit changed. */
	void NotifyInheritChanged( gdeOCInherit *inherit );
	
	/** \brief Notify listeners inherit structure changed. */
	void NotifyInheritsChanged();
	
	/** \brief Default property name to propose if this object class is inherited. */
	inline const decString &GetDefaultInheritPropertyPrefix() const{ return pDefaultInheritPropertyPrefix; }
	
	/** \brief Set default property name to propose if this object class is inherited. */
	void SetDefaultInheritPropertyPrefix( const char *propertyName );
	
	
	/** \brief Billboards. */
	inline gdeOCBillboardList &GetBillboards(){ return pBillboards; }
	inline const gdeOCBillboardList &GetBillboards() const{ return pBillboards; }
	
	/** \brief Notify listeners billboards changed. */
	void NotifyBillboardsChanged();
	
	/** \brief Notify listeners billboard changed. */
	void NotifyBillboardChanged( gdeOCBillboard *billboard );
	
	
	
	/** \brief Cameras. */
	inline gdeOCCameraList &GetCameras(){ return pCameras; }
	inline const gdeOCCameraList &GetCameras() const{ return pCameras; }
	
	/** \brief Notify listeners cameras changed. */
	void NotifyCamerasChanged();
	
	/** \brief Notify listeners camera changed. */
	void NotifyCameraChanged( gdeOCCamera *camera );
	
	
	
	/** \brief Components. */
	inline gdeOCComponentList &GetComponents(){ return pComponents; }
	inline const gdeOCComponentList &GetComponents() const{ return pComponents; }
	
	/** \brief Notify listeners components changed. */
	void NotifyComponentsChanged();
	
	/** \brief Notify listeners component changed. */
	void NotifyComponentChanged( gdeOCComponent *component );
	
	/** \brief Notify listeners component texture changed. */
	void NotifyComponentTextureChanged( gdeOCComponent *component,
		gdeOCComponentTexture *texture );
	
	/** \brief Notify listeners component texture name changed. */
	void NotifyComponentTextureNameChanged( gdeOCComponent *component,
		gdeOCComponentTexture *texture );
	
	/** \brief Notify listeners component texture properties changed. */
	void NotifyComponentTexturePropertiesChanged( gdeOCComponent *component,
		gdeOCComponentTexture *texture );
	
	
	
	/** \brief Lights. */
	inline gdeOCLightList &GetLights(){ return pLights; }
	inline const gdeOCLightList &GetLights() const{ return pLights; }
	
	/** \brief Notify listeners lights changed. */
	void NotifyLightsChanged();
	
	/** \brief Notify listeners light changed. */
	void NotifyLightChanged( gdeOCLight *light );
	
	
	
	/** \brief Environment map probes. */
	inline gdeOCEnvMapProbeList &GetEnvMapProbes(){ return pEnvMapProbes; }
	inline const gdeOCEnvMapProbeList &GetEnvMapProbes() const{ return pEnvMapProbes; }
	
	/** \brief Notify listeners environment map probes changed. */
	void NotifyEnvMapProbesChanged();
	
	/** \brief Notify listeners environment map probe changed. */
	void NotifyEnvMapProbeChanged( gdeOCEnvMapProbe *envMapProbe );
	
	
	
	/** \brief Navigation blockers. */
	inline gdeOCNavigationBlockerList &GetNavigationBlockers(){ return pNavigationBlockers; }
	inline const gdeOCNavigationBlockerList &GetNavigationBlockers() const{ return pNavigationBlockers; }
	
	/** \brief Notify listeners navigation blockers changed. */
	void NotifyNavigationBlockersChanged();
	
	/** \brief Notify listeners navigation blocker changed. */
	void NotifyNavigationBlockerChanged( gdeOCNavigationBlocker *navblocker );
	
	
	
	/** \brief Navigation spaces. */
	inline gdeOCNavigationSpaceList &GetNavigationSpaces(){ return pNavigationSpaces; }
	inline const gdeOCNavigationSpaceList &GetNavigationSpaces() const{ return pNavigationSpaces; }
	
	/** \brief Notify listeners navigation spaces changed. */
	void NotifyNavigationSpacesChanged();
	
	/** \brief Notify listeners navigation space changed. */
	void NotifyNavigationSpaceChanged( gdeOCNavigationSpace *navspace );
	
	
	
	/** \brief Particle emitters. */
	inline gdeOCParticleEmitterList &GetParticleEmitters(){ return pParticleEmitters; }
	inline const gdeOCParticleEmitterList &GetParticleEmitters() const{ return pParticleEmitters; }
	
	/** \brief Notify listeners particle emitters changed. */
	void NotifyParticleEmittersChanged();
	
	/** \brief Notify listeners particle emitter changed. */
	void NotifyParticleEmitterChanged( gdeOCParticleEmitter *emitter );
	
	
	
	/** \brief Force fields. */
	inline gdeOCForceFieldList &GetForceFields(){ return pForceFields; }
	inline const gdeOCForceFieldList &GetForceFields() const{ return pForceFields; }
	
	/** \brief Notify listeners force fields changed. */
	void NotifyForceFieldsChanged();
	
	/** \brief Notify listeners force fields changed. */
	void NotifyForceFieldChanged( gdeOCForceField *field );
	
	
	
	/** \brief Snap points. */
	inline gdeOCSnapPointList &GetSnapPoints(){ return pSnapPoints; }
	inline const gdeOCSnapPointList &GetSnapPoints() const{ return pSnapPoints; }
	
	/** \brief Notify listeners snap points changed. */
	void NotifySnapPointsChanged();
	
	/** \brief Notify listeners snap point changed. */
	void NotifySnapPointChanged( gdeOCSnapPoint *snapoint );
	
	/** \brief Notify listeners snap point name changed. */
	void NotifySnapPointNameChanged( gdeOCSnapPoint *snapoint );
	
	
	
	/** \brief Speakers. */
	inline gdeOCSpeakerList &GetSpeakers(){ return pSpeakers; }
	inline const gdeOCSpeakerList &GetSpeakers() const{ return pSpeakers; }
	
	/** \brief Notify listeners speakers changed. */
	void NotifySpeakersChanged();
	
	/** \brief Notify listeners speaker changed. */
	void NotifySpeakerChanged( gdeOCSpeaker *speaker );
	
	
	
	/** \brief Object is ghost not blocking other objects. */
	inline bool GetIsGhost() const{ return pIsGhost; }
	
	/** \brief Set if object is a ghost not blocking other objects. */
	void SetIsGhost( bool isGhost );
	
	/** \brief Object can be instantiated. */
	inline bool GetCanInstanciate() const{ return pCanInstanciate; }
	
	/** \brief Set if object can be instantiated. */
	void SetCanInstanciate( bool canInstanciate );
	
	
	
	/** \brief Named property searching inheritance. */
	bool DeepGetNamedProperty( const char *name, const gdeObjectClass* &objectClass,
		const gdeProperty* &property ) const;
	
	/** \brief Named property default value searching inheritance. */
	bool NamedPropertyDefaultValue( const char *name, decString &value ) const;
	
	/**
	 * \brief Add property names in this class and optionally inherited classes.
	 * \param[out] set Set to add property names to.
	 * \param[in] inherited Include inherited parameters.
	 */
	void AddPropertyNamesTo( decStringSet &set, bool inherited = false ) const;
	
	/**
	 * \brief Add property names in this class and optionally inherited classes.
	 * \param[out] set Set to add property names to.
	 * \param[in] inherited Include inherited parameters.
	 */
	void AddTexturePropertyNamesTo( decStringSet &set, bool inherited = false ) const;
	
	/** \brief Add all properties with inheritance if required. */
	void AddPropertiesTo( decStringDictionary &properties, bool inherited = false,
		bool includeValues = true ) const;
	
	/** \brief Find identifiers. */
	void GetDefinedUsedIDs( decStringSet &definedIDs, decStringSet &usedIDs ) const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pAddPropertyNames( decStringSet &set, const decString &prefix ) const;
	void pAddTexturePropertyNames( decStringSet &set, const decString &prefix ) const;
	void pAddPropertiesTo( decStringDictionary &properties, const decString &prefix ) const;
};

#endif
