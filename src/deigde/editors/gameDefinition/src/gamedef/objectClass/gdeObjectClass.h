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

#include "billboard/gdeOCBillboardList.h"
#include "camera/gdeOCCameraList.h"
#include "component/gdeOCComponentList.h"
#include "component/gdeOCComponentTextureList.h"
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
#include <dragengine/deObjectReference.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/common/string/decStringDictionary.h>

class gdeGameDefinition;
class gdeOCComponentTexture;



/**
 * Object class.
 */
class gdeObjectClass : public deObject{
public:
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
	bool pCanInstantiate;
	int pInheritSubObjects;
	
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
	
	gdeOCComponentTextureList pTextures;
	deObjectReference pActiveTexture;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create object class. */
	gdeObjectClass( const char *name = "NewClass" );
	
	/** Create copy of object class. */
	gdeObjectClass( const gdeObjectClass &objectClass );
	
	/** Clean up object class. */
	virtual ~gdeObjectClass();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent game definition. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** Set parent game definition. */
	void SetGameDefinition( gdeGameDefinition *gamedef );
	
	
	
	/** Class name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Set class name. */
	void SetName( const char *name );
	
	/** Description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** Set description. */
	void SetDescription( const char *description );
	
	
	
	/** Scale mode. */
	inline eScaleModes GetScaleMode() const{ return pScaleMode; }
	
	/** Set scale mode. */
	void SetScaleMode( eScaleModes mode );
	
	
	
	/** Cathegory. */
	inline const decString &GetCategory() const{ return pCategory; }
	
	/** Set cathegory. */
	void SetCategory( const char *category );
	
	/** Hide tags. */
	inline const decStringSet &GetHideTags() const{ return pHideTags; }
	
	/** Set hide tags. */
	void SetHideTags( const decStringSet &tags );
	
	/** Partial hide tags. */
	inline const decStringSet &GetPartialHideTags() const{ return pPartialHideTags; }
	
	/** Set partial hide tags. */
	void SetPartialHideTags( const decStringSet &tags );
	
	
	
	/** Properties. */
	inline gdePropertyList &GetProperties(){ return pProperties; }
	inline const gdePropertyList &GetProperties() const{ return pProperties; }
	
	/** Property values. */
	inline decStringDictionary &GetPropertyValues(){ return pPropertyValues; }
	inline const decStringDictionary &GetPropertyValues() const{ return pPropertyValues; }
	
	/** Notify listeners property changed. */
	void NotifyPropertyChanged( gdeProperty *property );
	
	/** Notify listeners property name changed. */
	void NotifyPropertyNameChanged( gdeProperty *property );
	
	/** Notify listeners properties changed. */
	void NotifyPropertiesChanged();
	
	/** Notify listeners property values changed. */
	void NotifyPropertyValuesChanged();
	
	
	
	/** Texture properties. */
	inline gdePropertyList &GetTextureProperties(){ return pTextureProperties; }
	inline const gdePropertyList &GetTextureProperties() const{ return pTextureProperties; }
	
	/** Notify listeners texture property changed. */
	void NotifyTexturePropertyChanged( gdeProperty *property );
	
	/** Notify listeners texture property name changed. */
	void NotifyTexturePropertyNameChanged( gdeProperty *property );
	
	/** Notify listeners texture properties changed. */
	void NotifyTexturePropertiesChanged();
	
	
	
	/** Inherit. */
	inline gdeOCInheritList &GetInherits(){ return pInherits; }
	inline const gdeOCInheritList &GetInherits() const{ return pInherits; }
	
	/** Notify listeners inherit changed. */
	void NotifyInheritChanged( gdeOCInherit *inherit );
	
	/** Notify listeners inherit structure changed. */
	void NotifyInheritsChanged();
	
	/** Default property name to propose if this object class is inherited. */
	inline const decString &GetDefaultInheritPropertyPrefix() const{ return pDefaultInheritPropertyPrefix; }
	
	/** Set default property name to propose if this object class is inherited. */
	void SetDefaultInheritPropertyPrefix( const char *propertyName );
	
	/** Object class inherits (in-)directly object class. */
	bool InheritsFrom( const gdeObjectClass *objectClass ) const;
	
	/** Object class is or inherits (in-)directly object class. */
	bool IsOrInheritsFrom( const gdeObjectClass *objectClass ) const;
	bool IsOrInheritsFrom( const char *name ) const;
	
	
	
	/** Billboards. */
	inline gdeOCBillboardList &GetBillboards(){ return pBillboards; }
	inline const gdeOCBillboardList &GetBillboards() const{ return pBillboards; }
	
	/** Notify listeners billboards changed. */
	void NotifyBillboardsChanged();
	
	/** Notify listeners billboard changed. */
	void NotifyBillboardChanged( gdeOCBillboard *billboard );
	
	
	
	/** Cameras. */
	inline gdeOCCameraList &GetCameras(){ return pCameras; }
	inline const gdeOCCameraList &GetCameras() const{ return pCameras; }
	
	/** Notify listeners cameras changed. */
	void NotifyCamerasChanged();
	
	/** Notify listeners camera changed. */
	void NotifyCameraChanged( gdeOCCamera *camera );
	
	
	
	/** Components. */
	inline gdeOCComponentList &GetComponents(){ return pComponents; }
	inline const gdeOCComponentList &GetComponents() const{ return pComponents; }
	
	/** Notify listeners components changed. */
	void NotifyComponentsChanged();
	
	/** Notify listeners component changed. */
	void NotifyComponentChanged( gdeOCComponent *component );
	
	/** Notify listeners component texture changed. */
	void NotifyComponentTextureChanged( gdeOCComponent *component,
		gdeOCComponentTexture *texture );
	
	/** Notify listeners component texture name changed. */
	void NotifyComponentTextureNameChanged( gdeOCComponent *component,
		gdeOCComponentTexture *texture );
	
	/** Notify listeners component texture properties changed. */
	void NotifyComponentTexturePropertiesChanged( gdeOCComponent *component,
		gdeOCComponentTexture *texture );
	
	
	
	/** Lights. */
	inline gdeOCLightList &GetLights(){ return pLights; }
	inline const gdeOCLightList &GetLights() const{ return pLights; }
	
	/** Notify listeners lights changed. */
	void NotifyLightsChanged();
	
	/** Notify listeners light changed. */
	void NotifyLightChanged( gdeOCLight *light );
	
	
	
	/** Environment map probes. */
	inline gdeOCEnvMapProbeList &GetEnvMapProbes(){ return pEnvMapProbes; }
	inline const gdeOCEnvMapProbeList &GetEnvMapProbes() const{ return pEnvMapProbes; }
	
	/** Notify listeners environment map probes changed. */
	void NotifyEnvMapProbesChanged();
	
	/** Notify listeners environment map probe changed. */
	void NotifyEnvMapProbeChanged( gdeOCEnvMapProbe *envMapProbe );
	
	
	
	/** Navigation blockers. */
	inline gdeOCNavigationBlockerList &GetNavigationBlockers(){ return pNavigationBlockers; }
	inline const gdeOCNavigationBlockerList &GetNavigationBlockers() const{ return pNavigationBlockers; }
	
	/** Notify listeners navigation blockers changed. */
	void NotifyNavigationBlockersChanged();
	
	/** Notify listeners navigation blocker changed. */
	void NotifyNavigationBlockerChanged( gdeOCNavigationBlocker *navblocker );
	
	
	
	/** Navigation spaces. */
	inline gdeOCNavigationSpaceList &GetNavigationSpaces(){ return pNavigationSpaces; }
	inline const gdeOCNavigationSpaceList &GetNavigationSpaces() const{ return pNavigationSpaces; }
	
	/** Notify listeners navigation spaces changed. */
	void NotifyNavigationSpacesChanged();
	
	/** Notify listeners navigation space changed. */
	void NotifyNavigationSpaceChanged( gdeOCNavigationSpace *navspace );
	
	
	
	/** Particle emitters. */
	inline gdeOCParticleEmitterList &GetParticleEmitters(){ return pParticleEmitters; }
	inline const gdeOCParticleEmitterList &GetParticleEmitters() const{ return pParticleEmitters; }
	
	/** Notify listeners particle emitters changed. */
	void NotifyParticleEmittersChanged();
	
	/** Notify listeners particle emitter changed. */
	void NotifyParticleEmitterChanged( gdeOCParticleEmitter *emitter );
	
	
	
	/** Force fields. */
	inline gdeOCForceFieldList &GetForceFields(){ return pForceFields; }
	inline const gdeOCForceFieldList &GetForceFields() const{ return pForceFields; }
	
	/** Notify listeners force fields changed. */
	void NotifyForceFieldsChanged();
	
	/** Notify listeners force fields changed. */
	void NotifyForceFieldChanged( gdeOCForceField *field );
	
	
	
	/** Snap points. */
	inline gdeOCSnapPointList &GetSnapPoints(){ return pSnapPoints; }
	inline const gdeOCSnapPointList &GetSnapPoints() const{ return pSnapPoints; }
	
	/** Notify listeners snap points changed. */
	void NotifySnapPointsChanged();
	
	/** Notify listeners snap point changed. */
	void NotifySnapPointChanged( gdeOCSnapPoint *snapoint );
	
	/** Notify listeners snap point name changed. */
	void NotifySnapPointNameChanged( gdeOCSnapPoint *snapoint );
	
	
	
	/** Speakers. */
	inline gdeOCSpeakerList &GetSpeakers(){ return pSpeakers; }
	inline const gdeOCSpeakerList &GetSpeakers() const{ return pSpeakers; }
	
	/** Notify listeners speakers changed. */
	void NotifySpeakersChanged();
	
	/** Notify listeners speaker changed. */
	void NotifySpeakerChanged( gdeOCSpeaker *speaker );
	
	
	
	/** List of textures. */
	inline gdeOCComponentTextureList &GetTextures(){ return pTextures; }
	inline const gdeOCComponentTextureList &GetTextures() const{ return pTextures; }
	
	/** Active texture or \em NULL if none. */
	inline gdeOCComponentTexture *GetActiveTexture() const{
		return ( gdeOCComponentTexture* )( deObject* )pActiveTexture;
	}
	
	/** Set active texture or \em NULL if none. */
	void SetActiveTexture( gdeOCComponentTexture *texture );
	
	/** Notify listeners textures changed. */
	void NotifyTexturesChanged();
	
	/** Notify listeners texture changed. */
	void NotifyTextureChanged( gdeOCComponentTexture *texture );
	
	
	
	/** Object is ghost not blocking other objects. */
	inline bool GetIsGhost() const{ return pIsGhost; }
	
	/** Set if object is a ghost not blocking other objects. */
	void SetIsGhost( bool isGhost );
	
	/** Object can be instantiated. */
	inline bool GetCanInstantiate() const{ return pCanInstantiate; }
	
	/** Set if object can be instantiated. */
	void SetCanInstantiate( bool canInstantiate );
	
	/** Inherit sub objects. */
	inline int GetInheritSubObjects() const{ return pInheritSubObjects; }
	
	/** Set inherit sub objects. */
	void SetInheritSubObjects( int filter );
	
	
	
	/** Named property searching inheritance. */
	bool DeepGetNamedProperty( const char *name, const gdeObjectClass* &objectClass,
		const gdeProperty* &property ) const;
	
	/** Named property default value searching inheritance. */
	bool NamedPropertyDefaultValue( const char *name, decString &value ) const;
	
	/**
	 * Add property names in this class and optionally inherited classes.
	 * \param[out] set Set to add property names to.
	 * \param[in] inherited Include inherited parameters.
	 */
	void AddPropertyNamesTo( decStringSet &set, bool inherited = false ) const;
	
	/**
	 * Add property names in this class and optionally inherited classes.
	 * \param[out] set Set to add property names to.
	 * \param[in] inherited Include inherited parameters.
	 */
	void AddTexturePropertyNamesTo( decStringSet &set, bool inherited = false ) const;
	
	/** Add all properties with inheritance if required. */
	void AddPropertiesTo( decStringDictionary &properties, bool inherited = false,
		bool includeValues = true ) const;
	
	/** Find identifiers. */
	void GetDefinedUsedIDs( decStringSet &definedIDs, decStringSet &usedIDs ) const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pAddPropertyNames( decStringSet &set, const decString &prefix ) const;
	void pAddTexturePropertyNames( decStringSet &set, const decString &prefix ) const;
	void pAddPropertiesTo( decStringDictionary &properties, const decString &prefix ) const;
};

#endif
