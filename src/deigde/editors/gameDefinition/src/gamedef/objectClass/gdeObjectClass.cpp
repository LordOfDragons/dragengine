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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gdeObjectClass.h"
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
#include "../gdeGameDefinition.h"
#include "../property/gdeProperty.h"

#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>



// Class gdeObjectClass
/////////////////////////

// Constructor, destructor
////////////////////////////

gdeObjectClass::gdeObjectClass( const char *name ) :
pGameDefinition( NULL ),
pName( name ),
pScaleMode( esmUniform ),

pIsGhost( false ),
pCanInstantiate( true ){
}

gdeObjectClass::gdeObjectClass( const gdeObjectClass &objectClass ) :
pGameDefinition( NULL ),
pName( objectClass.pName ),
pDescription( objectClass.pDescription ),
pScaleMode( objectClass.pScaleMode ),

pPropertyValues( objectClass.pPropertyValues ),

pCategory( objectClass.pCategory ),
pHideTags( objectClass.pHideTags ),
pPartialHideTags( objectClass.pPartialHideTags ),

pIsGhost( objectClass.pIsGhost ),
pCanInstantiate( objectClass.pCanInstantiate )
{
	deObjectReference objRef;
	int i, count;
	
	try{
		count = objectClass.pProperties.GetCount();
		for( i=0; i<count; i++ ){
			objRef.TakeOver( new gdeProperty( *objectClass.pProperties.GetAt( i ) ) );
			pProperties.Add( ( gdeProperty* )( deObject* )objRef );
		}
		
		count = objectClass.pTextureProperties.GetCount();
		for( i=0; i<count; i++ ){
			objRef.TakeOver( new gdeProperty( *objectClass.pTextureProperties.GetAt( i ) ) );
			pTextureProperties.Add( ( gdeProperty* )( deObject* )objRef );
		}
		
		count = objectClass.pInherits.GetCount();
		for( i=0; i<count; i++ ){
			objRef.TakeOver( new gdeOCInherit( *objectClass.pInherits.GetAt( i ) ) );
			pInherits.Add( ( gdeOCInherit* )( deObject* )objRef );
		}
		
		count = objectClass.pBillboards.GetCount();
		for( i=0; i<count; i++ ){
			objRef.TakeOver( new gdeOCBillboard( *objectClass.pBillboards.GetAt( i ) ) );
			pBillboards.Add( ( gdeOCBillboard* )( deObject* )objRef );
		}
		
		count = objectClass.pCameras.GetCount();
		for( i=0; i<count; i++ ){
			objRef.TakeOver( new gdeOCCamera( *objectClass.pCameras.GetAt( i ) ) );
			pCameras.Add( ( gdeOCCamera* )( deObject* )objRef );
		}
		
		count = objectClass.pComponents.GetCount();
		for( i=0; i<count; i++ ){
			objRef.TakeOver( new gdeOCComponent( *objectClass.pComponents.GetAt( i ) ) );
			pComponents.Add( ( gdeOCComponent* )( deObject* )objRef );
		}
		
		count = objectClass.pEnvMapProbes.GetCount();
		for( i=0; i<count; i++ ){
			objRef.TakeOver( new gdeOCEnvMapProbe( *objectClass.pEnvMapProbes.GetAt( i ) ) );
			pEnvMapProbes.Add( ( gdeOCEnvMapProbe* )( deObject* )objRef );
		}
		
		count = objectClass.pLights.GetCount();
		for( i=0; i<count; i++ ){
			objRef.TakeOver( new gdeOCLight( *objectClass.pLights.GetAt( i ) ) );
			pLights.Add( ( gdeOCLight* )( deObject* )objRef );
		}
		
		count = objectClass.pNavigationBlockers.GetCount();
		for( i=0; i<count; i++ ){
			objRef.TakeOver( new gdeOCNavigationBlocker( *objectClass.pNavigationBlockers.GetAt( i ) ) );
			pNavigationBlockers.Add( ( gdeOCNavigationBlocker* )( deObject* )objRef );
		}
		
		count = objectClass.pNavigationSpaces.GetCount();
		for( i=0; i<count; i++ ){
			objRef.TakeOver( new gdeOCNavigationSpace( *objectClass.pNavigationSpaces.GetAt( i ) ) );
			pNavigationSpaces.Add( ( gdeOCNavigationSpace* )( deObject* )objRef );
		}
		
		count = objectClass.pParticleEmitters.GetCount();
		for( i=0; i<count; i++ ){
			objRef.TakeOver( new gdeOCParticleEmitter( *objectClass.pParticleEmitters.GetAt( i ) ) );
			pParticleEmitters.Add( ( gdeOCParticleEmitter* )( deObject* )objRef );
		}
		
		count = objectClass.pForceFields.GetCount();
		for( i=0; i<count; i++ ){
			objRef.TakeOver( new gdeOCForceField( *objectClass.pForceFields.GetAt( i ) ) );
			pForceFields.Add( ( gdeOCForceField* )( deObject* )objRef );
		}
		
		count = objectClass.pSnapPoints.GetCount();
		for( i=0; i<count; i++ ){
			objRef.TakeOver( new gdeOCSnapPoint( *objectClass.pSnapPoints.GetAt( i ) ) );
			pSnapPoints.Add( ( gdeOCSnapPoint* )( deObject* )objRef );
		}
		
		count = objectClass.pSpeakers.GetCount();
		for( i=0; i<count; i++ ){
			objRef.TakeOver( new gdeOCSpeaker( *objectClass.pSpeakers.GetAt( i ) ) );
			pSpeakers.Add( ( gdeOCSpeaker* )( deObject* )objRef );
		}
		
		count = objectClass.pTextures.GetCount();
		for( i=0; i<count; i++ ){
			objRef.TakeOver( new gdeOCComponentTexture( *objectClass.pTextures.GetAt( i ) ) );
			pTextures.Add( ( gdeOCComponentTexture* )( deObject* )objRef );
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

gdeObjectClass::~gdeObjectClass(){
	pCleanUp();
}



// Management
///////////////

void gdeObjectClass::SetGameDefinition( gdeGameDefinition *gamedef ){
	pGameDefinition = gamedef;
}



void gdeObjectClass::SetName( const char *name ){
	if( pName == name ){
		return;
	}
	
	pName = name;
	
	if( pGameDefinition ){
		pGameDefinition->NotifyObjectClassNameChanged( this );
	}
}

void gdeObjectClass::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	
	if( pGameDefinition ){
		pGameDefinition->NotifyObjectClassChanged( this );
	}
}



void gdeObjectClass::SetScaleMode( eScaleModes mode ){
	if( mode < esmFixed || mode > esmFree ){
		DETHROW( deeInvalidParam );
	}
	
	if( pScaleMode == mode ){
		return;
	}
	
	pScaleMode = mode;
	
	if( pGameDefinition ){
		pGameDefinition->NotifyObjectClassChanged( this );
	}
}



void gdeObjectClass::SetCategory( const char *category ){
	if( pCategory == category ){
		return;
	}
	
	pCategory = category;
	
	if( pGameDefinition ){
		pGameDefinition->NotifyObjectClassChanged( this );
	}
}

void gdeObjectClass::SetHideTags( const decStringSet &tags ){
	if( pHideTags == tags ){
		return;
	}
	
	pHideTags = tags;
	
	if( pGameDefinition ){
		pGameDefinition->NotifyObjectClassChanged( this );
	}
}

void gdeObjectClass::SetPartialHideTags( const decStringSet &tags ){
	if( pPartialHideTags == tags ){
		return;
	}
	
	pPartialHideTags = tags;
	
	if( pGameDefinition ){
		pGameDefinition->NotifyObjectClassChanged( this );
	}
}




void gdeObjectClass::NotifyPropertyChanged( gdeProperty *property ){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCPropertyChanged( this, property );
	}
}

void gdeObjectClass::NotifyPropertyNameChanged( gdeProperty *property ){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCPropertyNameChanged( this, property );
	}
}

void gdeObjectClass::NotifyPropertiesChanged(){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCPropertiesChanged( this );
	}
}

void gdeObjectClass::NotifyPropertyValuesChanged(){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCPropertyValuesChanged( this );
	}
}

void gdeObjectClass::NotifyTexturePropertyChanged( gdeProperty *property ){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCTexturePropertyChanged( this, property );
	}
}

void gdeObjectClass::NotifyTexturePropertyNameChanged( gdeProperty *property ){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCTexturePropertyNameChanged( this, property );
	}
}

void gdeObjectClass::NotifyTexturePropertiesChanged(){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCTexturePropertiesChanged( this );
	}
}

void gdeObjectClass::NotifyInheritChanged( gdeOCInherit *inherit ){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCInheritChanged( this, inherit );
	}
}

void gdeObjectClass::NotifyInheritsChanged(){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCInheritsChanged( this );
	}
}

void gdeObjectClass::SetDefaultInheritPropertyPrefix( const char *propertyName ){
	if( pDefaultInheritPropertyPrefix == propertyName ){
		return;
	}
	
	pDefaultInheritPropertyPrefix = propertyName;
	
	if( pGameDefinition ){
		pGameDefinition->NotifyObjectClassChanged( this );
	}
}

bool gdeObjectClass::InheritsFrom( const gdeObjectClass *objectClass ) const{
	const int count = pInherits.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		const gdeOCInherit &inherit = *pInherits.GetAt( i );
		const gdeObjectClass * const inheritOC = pGameDefinition->FindObjectClass( inherit.GetName() );
		if( inheritOC && inheritOC->IsOrInheritsFrom( objectClass ) ){
			return true;
		}
	}
	return false;
}

bool gdeObjectClass::IsOrInheritsFrom( const gdeObjectClass *objectClass ) const{
	return this == objectClass || InheritsFrom( objectClass );
}

bool gdeObjectClass::IsOrInheritsFrom( const char *name ) const{
	const gdeObjectClass * const objectClass = pGameDefinition->FindObjectClass( name );
	return objectClass && IsOrInheritsFrom( objectClass );
}


void gdeObjectClass::NotifyBillboardsChanged(){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCBillboardsChanged( this );
	}
}

void gdeObjectClass::NotifyBillboardChanged( gdeOCBillboard *billboard ){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCBillboardChanged( this, billboard );
	}
}

void gdeObjectClass::NotifyCamerasChanged(){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCCamerasChanged( this );
	}
}

void gdeObjectClass::NotifyCameraChanged( gdeOCCamera *camera ){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCCameraChanged( this, camera );
	}
}

void gdeObjectClass::NotifyComponentsChanged(){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCComponentsChanged( this );
	}
}

void gdeObjectClass::NotifyComponentChanged( gdeOCComponent *component ){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCComponentChanged( this, component );
	}
}

void gdeObjectClass::NotifyComponentTextureChanged( gdeOCComponent *component,
gdeOCComponentTexture *texture ){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCComponentTextureChanged( this, component, texture );
	}
}

void gdeObjectClass::NotifyComponentTextureNameChanged( gdeOCComponent *component,
gdeOCComponentTexture *texture ){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCComponentTextureNameChanged( this, component, texture );
	}
}

void gdeObjectClass::NotifyComponentTexturePropertiesChanged( gdeOCComponent *component,
gdeOCComponentTexture *texture ){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCComponentTexturePropertiesChanged( this, component, texture );
	}
}

void gdeObjectClass::NotifyEnvMapProbesChanged(){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCEnvMapProbesChanged( this );
	}
}

void gdeObjectClass::NotifyEnvMapProbeChanged( gdeOCEnvMapProbe *envMapProbe ){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCEnvMapProbeChanged( this, envMapProbe );
	}
}

void gdeObjectClass::NotifyLightsChanged(){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCLightsChanged( this );
	}
}

void gdeObjectClass::NotifyLightChanged( gdeOCLight *light ){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCLightChanged( this, light );
	}
}

void gdeObjectClass::NotifyNavigationBlockersChanged(){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCNavigationBlockersChanged( this );
	}
}

void gdeObjectClass::NotifyNavigationBlockerChanged( gdeOCNavigationBlocker *navblocker ){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCNavigationBlockerChanged( this, navblocker );
	}
}

void gdeObjectClass::NotifyNavigationSpacesChanged(){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCNavigationSpacesChanged( this );
	}
}

void gdeObjectClass::NotifyNavigationSpaceChanged( gdeOCNavigationSpace *navspace ){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCNavigationSpaceChanged( this, navspace );
	}
}

void gdeObjectClass::NotifyParticleEmittersChanged(){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCParticleEmittersChanged( this );
	}
}

void gdeObjectClass::NotifyParticleEmitterChanged( gdeOCParticleEmitter *emitter ){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCParticleEmitterChanged( this, emitter );
	}
}

void gdeObjectClass::NotifyForceFieldsChanged(){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCForceFieldsChanged( this );
	}
}

void gdeObjectClass::NotifyForceFieldChanged( gdeOCForceField *field ){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCForceFieldChanged( this, field );
	}
}

void gdeObjectClass::NotifySnapPointsChanged(){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCSnapPointsChanged( this );
	}
}

void gdeObjectClass::NotifySnapPointChanged( gdeOCSnapPoint *snappoint ){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCSnapPointChanged( this, snappoint );
	}
}

void gdeObjectClass::NotifySnapPointNameChanged( gdeOCSnapPoint *snappoint ){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCSnapPointNameChanged( this, snappoint );
	}
}

void gdeObjectClass::NotifySpeakersChanged(){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCSpeakersChanged( this );
	}
}

void gdeObjectClass::NotifySpeakerChanged( gdeOCSpeaker *speaker ){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCSpeakerChanged( this, speaker );
	}
}



void gdeObjectClass::SetActiveTexture( gdeOCComponentTexture *texture ){
	pActiveTexture = texture;
}

void gdeObjectClass::NotifyTexturesChanged(){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCTexturesChanged( this );
	}
}

void gdeObjectClass::NotifyTextureChanged( gdeOCComponentTexture *texture ){
	if( pGameDefinition ){
		pGameDefinition->NotifyOCTextureChanged( this, texture );
	}
}



void gdeObjectClass::SetIsGhost( bool isGhost ){
	if( pIsGhost == isGhost ){
		return;
	}
	
	pIsGhost = isGhost;
	
	if( pGameDefinition ){
		pGameDefinition->NotifyObjectClassChanged( this );
	}
}

void gdeObjectClass::SetCanInstantiate( bool canInstantiate ){
	if( pCanInstantiate == canInstantiate ){
		return;
	}
	
	pCanInstantiate = canInstantiate;
	
	if( pGameDefinition ){
		pGameDefinition->NotifyObjectClassChanged( this );
	}
}



bool gdeObjectClass::DeepGetNamedProperty( const char *name, const gdeObjectClass* &objectClass,
const gdeProperty* &property ) const{
	property = pProperties.GetNamed( name );
	if( property ){
		objectClass = this;
		return true;
	}
	
	if( ! pGameDefinition ){
		return false;
	}
	
	const int nameLen = strlen( name );
	const int count = pInherits.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		const gdeOCInherit &inherit = *pInherits.GetAt( i );
		const gdeObjectClass * const ioc = pGameDefinition->FindObjectClass( inherit.GetName() );
		if( ! ioc ){
			continue;
		}
		
		const decString &prefix = inherit.GetPropertyPrefix();
		const int prefixLen = prefix.GetLength();
		if( nameLen < prefixLen || strncmp( name, prefix, prefixLen ) != 0 ){
			continue;
		}
		
		if( ioc->DeepGetNamedProperty( name + prefixLen, objectClass, property ) ){
			return true;
		}
	}
	
	return false;
}

bool gdeObjectClass::NamedPropertyDefaultValue( const char *name, decString &value ) const{
	const decString *propertyValue = NULL;
	if( pPropertyValues.GetAt( name, &propertyValue ) ){
		value = *propertyValue;
		return true;
	}
	
	const gdeProperty * const property = pProperties.GetNamed( name );
	if( property ){
		value = property->GetDefaultValue();
		return true;
	}
	
	const int nameLen = strlen( name );
	const int count = pInherits.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		const gdeOCInherit &inherit = *pInherits.GetAt( i );
		const gdeObjectClass * const ioc = pGameDefinition->FindObjectClass( inherit.GetName() );
		if( ! ioc ){
			continue;
		}
		
		const decString &prefix = inherit.GetPropertyPrefix();
		const int prefixLen = prefix.GetLength();
		if( nameLen < prefixLen || strncmp( name, prefix, prefixLen ) != 0 ){
			continue;
		}
		
		if( ioc->NamedPropertyDefaultValue( name + prefixLen, value ) ){
			return true;
		}
	}
	
	return false;
}

void gdeObjectClass::AddPropertyNamesTo( decStringSet &set, bool inherited ) const{
	const int propertyCount = pProperties.GetCount();
	int i;
	for( i=0; i<propertyCount; i++ ){
		set.Add( pProperties.GetAt( i )->GetName() );
	}
	
	if( ! pGameDefinition || ! inherited ){
		return;
	}
	
	const int inheritCount = pInherits.GetCount();
	for( i=0; i<inheritCount; i++ ){
		const gdeOCInherit &inherit = *pInherits.GetAt( i );
		const gdeObjectClass * const objectClass = pGameDefinition->FindObjectClass( inherit.GetName() );
		if( objectClass ){
			objectClass->pAddPropertyNames( set, inherit.GetPropertyPrefix() );
		}
	}
}

void gdeObjectClass::AddTexturePropertyNamesTo( decStringSet &set, bool inherited ) const{
	const int propertyCount = pTextureProperties.GetCount();
	int i;
	for( i=0; i<propertyCount; i++ ){
		set.Add( pTextureProperties.GetAt( i )->GetName() );
	}
	
	if( ! pGameDefinition || ! inherited ){
		return;
	}
	
	const int inheritCount = pInherits.GetCount();
	for( i=0; i<inheritCount; i++ ){
		const gdeOCInherit &inherit = *pInherits.GetAt( i );
		const gdeObjectClass * const objectClass = pGameDefinition->FindObjectClass( inherit.GetName() );
		if( objectClass ){
			objectClass->pAddTexturePropertyNames( set, inherit.GetPropertyPrefix() );
		}
	}
}

void gdeObjectClass::AddPropertiesTo( decStringDictionary &properties, bool inherited, bool includeValues ) const{
	if( inherited ){
		const int inheritCount = pInherits.GetCount();
		int i;
		for( i=0; i<inheritCount; i++ ){
			const gdeOCInherit &inherit = *pInherits.GetAt( i );
			const gdeObjectClass * const objectClass = pGameDefinition->FindObjectClass( inherit.GetName() );
			if( objectClass ){
				objectClass->pAddPropertiesTo( properties, inherit.GetPropertyPrefix() );
			}
		}
	}
	
	const int propertyCount = pProperties.GetCount();
	int i;
	for( i=0; i<propertyCount; i++ ){
		const gdeProperty &property = *pProperties.GetAt( i );
		properties.SetAt( property.GetName(), property.GetDefaultValue() );
	}
	
	if( includeValues ){
		properties += pPropertyValues;
	}
}

void gdeObjectClass::GetDefinedUsedIDs( decStringSet &definedIDs, decStringSet &usedIDs ) const{
	const int inheritCount = pInherits.GetCount();
	int i;
	for( i=0; i<inheritCount; i++ ){
		const gdeOCInherit &inherit = *pInherits.GetAt( i );
		const gdeObjectClass * const objectClass = pGameDefinition->FindObjectClass( inherit.GetName() );
		if( objectClass ){
			objectClass->GetDefinedUsedIDs( definedIDs, usedIDs );
		}
	}
	
	const int propertyCount = pProperties.GetCount();
	for( i=0; i<propertyCount; i++ ){
		const gdeProperty &property = *pProperties.GetAt( i );
		if( property.GetType() != gdeProperty::eptIdentifier || property.GetIdentifierGroup().IsEmpty() ){
			continue;
		}
		
		if( property.GetIdentifierUsage() ){
			definedIDs.Add( property.GetIdentifierGroup() );
			
		}else{
			usedIDs.Add( property.GetIdentifierGroup() );
		}
	}
}



// Private functions
//////////////////////

void gdeObjectClass::pCleanUp(){
// 	gdePropertyList pProperties;
// 	gdePropertyList pTextureProperties;
	
	SetGameDefinition( NULL );
}

void gdeObjectClass::pAddPropertyNames( decStringSet &set, const decString &prefix ) const{
	const int propertyCount = pProperties.GetCount();
	int i;
	for( i=0; i<propertyCount; i++ ){
		set.Add( prefix + pProperties.GetAt( i )->GetName() );
	}
	
	const int inheritCount = pInherits.GetCount();
	for( i=0; i<inheritCount; i++ ){
		const gdeOCInherit &inherit = *pInherits.GetAt( i );
		const gdeObjectClass * const objectClass = pGameDefinition->FindObjectClass( inherit.GetName() );
		if( objectClass ){
			objectClass->pAddPropertyNames( set, prefix + inherit.GetPropertyPrefix() );
		}
	}
}

void gdeObjectClass::pAddTexturePropertyNames( decStringSet &set, const decString &prefix ) const{
	const int propertyCount = pTextureProperties.GetCount();
	int i;
	for( i=0; i<propertyCount; i++ ){
		set.Add( prefix + pTextureProperties.GetAt( i )->GetName() );
	}
	
	const int inheritCount = pInherits.GetCount();
	for( i=0; i<inheritCount; i++ ){
		const gdeOCInherit &inherit = *pInherits.GetAt( i );
		const gdeObjectClass * const objectClass = pGameDefinition->FindObjectClass( inherit.GetName() );
		if( objectClass ){
			objectClass->pAddTexturePropertyNames( set, prefix + inherit.GetPropertyPrefix() );
		}
	}
}

void gdeObjectClass::pAddPropertiesTo( decStringDictionary &properties, const decString &prefix ) const{
	const int inheritCount = pInherits.GetCount();
	int i;
	for( i=0; i<inheritCount; i++ ){
		const gdeOCInherit &inherit = *pInherits.GetAt( i );
		const gdeObjectClass * const objectClass = pGameDefinition->FindObjectClass( inherit.GetName() );
		if( objectClass ){
			objectClass->pAddPropertiesTo( properties, prefix + inherit.GetPropertyPrefix() );
		}
	}
	
	const int propertyCount = pProperties.GetCount();
	for( i=0; i<propertyCount; i++ ){
		const gdeProperty &property = *pProperties.GetAt( i );
		properties.SetAt( prefix + property.GetName(), property.GetDefaultValue() );
	}
	
	const decStringList keys( pPropertyValues.GetKeys() );
	const int keyCount = keys.GetCount();
	for( i=0; i<keyCount; i++ ){
		const decString &key = keys.GetAt( i );
		properties.SetAt( prefix + key, pPropertyValues.GetAt( key ) );
	}
}
