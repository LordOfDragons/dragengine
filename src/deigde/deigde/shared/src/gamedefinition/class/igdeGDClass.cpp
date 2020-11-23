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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeGDClass.h"
#include "igdeGDClassInherit.h"
#include "igdeGDClassManager.h"
#include "igdeGDCamera.h"
#include "billboard/igdeGDCBillboard.h"
#include "component/igdeGDCComponent.h"
#include "snappoint/igdeGDCSnapPoint.h"
#include "particleemitter/igdeGDCParticleEmitter.h"
#include "forcefield/igdeGDCForceField.h"
#include "envmapprobe/igdeGDCEnvMapProbe.h"
#include "light/igdeGDCLight.h"
#include "speaker/igdeGDCSpeaker.h"
#include "navspace/igdeGDCNavigationSpace.h"
#include "navblocker/igdeGDCNavigationBlocker.h"
#include "../igdeTagManager.h"
#include "../property/igdeGDProperty.h"

#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decIntList.h>
#include <dragengine/resources/image/deImage.h>



// Class igdeGDClass
//////////////////////

// Constructor, destructor
////////////////////////////

igdeGDClass::igdeGDClass( const char *name ){
	if( ! name || strlen( name ) == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pName = name;
	pScaleMode = esmUniform;
	
	pCamera = NULL;
	pHasCamera = false;
	pIsGhost = false;
	pCanInstantiate = true;
	
	pPreviewImage = NULL;
	
	try{
		pCamera = new igdeGDCamera;
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

igdeGDClass::igdeGDClass( const igdeGDClass &gdclass ){
	igdeGDCSnapPoint *snappoint = NULL;
	deObjectReference objRef;
	int i, count;
	
	pCamera = NULL;
	
	pPreviewImage = NULL;
	
	try{
		pName = gdclass.pName;
		pDescription = gdclass.pDescription;
		pScaleMode = gdclass.pScaleMode;
		
		if( gdclass.pCamera ){
			pCamera = new igdeGDCamera( *gdclass.pCamera );
		}
		pHasCamera = gdclass.pHasCamera;
		pIsGhost = gdclass.pIsGhost;
		pCanInstantiate = gdclass.pCanInstantiate;
		
		pCategory = gdclass.pCategory;
		
		pListProperties.SetToDeepCopyFrom( gdclass.pListProperties );
		pPropertyValues = gdclass.pPropertyValues;
		pTextureProperties.SetToDeepCopyFrom( gdclass.pTextureProperties );
		pListBillboards.SetToDeepCopyFrom( gdclass.pListBillboards );
		pListComponents.SetToDeepCopyFrom( gdclass.pListComponents );
		pListLights.SetToDeepCopyFrom( gdclass.pListLights );
		
		count = gdclass.pSnapPoints.GetCount();
		for( i=0; i<count; i++ ){
			snappoint = new igdeGDCSnapPoint( *gdclass.pSnapPoints.GetAt( i ) );
			pSnapPoints.Add( snappoint );
			snappoint->FreeReference();
			snappoint = NULL;
		}
		
		pListParticleEmitters.SetToDeepCopyFrom( gdclass.pListParticleEmitters );
		pListForceFields.SetToDeepCopyFrom( gdclass.pListForceFields );
		pListEnvMapProbes.SetToDeepCopyFrom( gdclass.pListEnvMapProbes );
		pListSpeakers.SetToDeepCopyFrom( gdclass.pListSpeakers );
		pListNavigationSpaces.SetToDeepCopyFrom( gdclass.pListNavigationSpaces );
		pListNavigationBlockers.SetToDeepCopyFrom( gdclass.pListNavigationBlockers );
		
		pHideTags = gdclass.pHideTags;
		pPartialHideTags = gdclass.pPartialHideTags;
		
		count = gdclass.pInheritClasses.GetCount();
		for( i=0; i<count; i++ ){
			objRef.TakeOver( new igdeGDClassInherit(
				*( ( igdeGDClassInherit* )gdclass.pInheritClasses.GetAt( i ) ) ) );
			pInheritClasses.Add( objRef );
		}
		
		pPathEClass = gdclass.pPathEClass;
		
	}catch( const deException & ){
		if( snappoint ){
			snappoint->FreeReference();
		}
		
		pCleanUp();
		throw;
	}
}

igdeGDClass::~igdeGDClass(){
	pCleanUp();
}



// Management
///////////////

void igdeGDClass::SetDescription( const char *description ){
	pDescription = description;
}

void igdeGDClass::SetScaleMode( eScaleModes mode ){
	pScaleMode = mode;
}

void igdeGDClass::SetHasCamera( bool hasCamera ){
	pHasCamera = hasCamera;
}

void igdeGDClass::SetIsGhost( bool isGhost ){
	pIsGhost = isGhost;
}

void igdeGDClass::SetCanInstantiate( bool canInstantiate ){
	pCanInstantiate = canInstantiate;
}

void igdeGDClass::SetCategory( const char *category ){
	if( ! category ){
		DETHROW( deeInvalidParam );
	}
	
	pCategory = category;
}



int igdeGDClass::GetInheritClassCount() const{
	return pInheritClasses.GetCount();
}

igdeGDClassInherit *igdeGDClass::GetInheritClassAt( int index ) const{
	return ( igdeGDClassInherit* )pInheritClasses.GetAt( index );
}

void igdeGDClass::AddInheritClass( igdeGDClassInherit *inheritClass ){
	if( ! inheritClass ){
		DETHROW( deeInvalidParam );
	}
	pInheritClasses.Add( inheritClass );
}

void igdeGDClass::ResolveInheritClasses( const igdeGDClassManager &classManager ){
	const int count = pInheritClasses.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( igdeGDClassInherit* )pInheritClasses.GetAt( i ) )->ResolveClass( classManager );
	}
}

void igdeGDClass::RemoveAllInheritClasses(){
	pInheritClasses.RemoveAll();
}

bool igdeGDClass::Inherits( igdeGDClass *gdclass ) const{
	if( ! gdclass || gdclass == this ){
		return false;
	}
	
	const int count = pInheritClasses.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		igdeGDClass * const inheritClass = ( ( igdeGDClassInherit* )pInheritClasses.GetAt( i ) )->GetClass();
		if( inheritClass == gdclass || inheritClass->Inherits( gdclass ) ){
			return true;
		}
	}
	
	return false;
}



void igdeGDClass::Check(){
}



void igdeGDClass::SetPreviewImage( deImage *image ){
	if( image == pPreviewImage ){
		return;
	}
	
	if( pPreviewImage ){
		pPreviewImage->FreeReference();
	}
	
	pPreviewImage = image;
	
	if( image ){
		image->AddReference();
	}
}

void igdeGDClass::SetPathEClass( const decString &pathEClass ){
	pPathEClass = pathEClass;
}



// Properties
///////////////

void igdeGDClass::AddProperty( igdeGDProperty *property ){
	pListProperties.Add( property );
}

void igdeGDClass::RemoveProperty( igdeGDProperty *property ){
	pListProperties.Remove( property );
}

void igdeGDClass::RemoveAllProperties(){
	pListProperties.RemoveAll();
}

void igdeGDClass::SetPropertyValues( const decStringDictionary &values ){
	pPropertyValues = values;
}

igdeGDProperty *igdeGDClass::GetPropertyNamed( const char *name ) const{
	const igdeGDClass *gdclass = this;
	igdeGDProperty *property = gdclass->pListProperties.GetNamed( name );
	if( property ){
		return property;
	}
	
	const int count = pInheritClasses.GetCount();
	const int nameLen = strlen( name );
	int i;
	
	for( i=0; i<count; i++ ){
		const igdeGDClassInherit &inherit = *( ( igdeGDClassInherit* )pInheritClasses.GetAt( i ) );
		if( ! inherit.GetClass() ){
			continue;
		}
		
		const decString &prefix = inherit.GetPropertyPrefix();
		const int prefixLen = prefix.GetLength();
		if( nameLen < prefixLen || strncmp( name, prefix, prefixLen ) != 0 ){
			continue;
		}
		
		property = inherit.GetClass()->GetPropertyNamed( name + prefixLen );
		if( property ){
			return property;
		}
	}
	
	return NULL;
}

bool igdeGDClass::GetDefaultPropertyValue( const char *name, decString &value) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const decString *checkValue;
	if( pPropertyValues.GetAt( name, &checkValue ) ){
		value = *checkValue;
		return true;
	}
	
	igdeGDProperty * const property = pListProperties.GetNamed( name );
	if( property ){
		value = property->GetDefaultValue();
		return true;
	}
	
	const int count = pInheritClasses.GetCount();
	const int nameLen = strlen( name );
	int i;
	
	for( i=0; i<count; i++ ){
		const igdeGDClassInherit &inherit = *( ( igdeGDClassInherit* )pInheritClasses.GetAt( i ) );
		if( ! inherit.GetClass() ){
			continue;
		}
		
		const decString &prefix = inherit.GetPropertyPrefix();
		const int prefixLen = prefix.GetLength();
		if( nameLen < prefixLen || strncmp( name, prefix, prefixLen ) != 0 ){
			continue;
		}
		
		if( inherit.GetClass()->GetDefaultPropertyValue( name + prefixLen, value ) ){
			return true;
		}
	}
	
	return false;
}

bool igdeGDClass::HasDefaultPropertyValue( const char *name ) const{
	decString value;
	return GetDefaultPropertyValue( name, value );
}

void igdeGDClass::AddPropertyNames( decStringSet &set, bool inherited ) const{
	const int propertyCount = pListProperties.GetCount();
	int i;
	for( i=0; i<propertyCount; i++ ){
		set.Add( pListProperties.GetAt( i )->GetName() );
	}
	
	if( ! inherited ){
		return;
	}
	
	const int inheritCount = pInheritClasses.GetCount();
	for( i=0; i<inheritCount; i++ ){
		const igdeGDClassInherit &inherit = *( ( igdeGDClassInherit* )pInheritClasses.GetAt( i ) );
		if( inherit.GetClass() ){
			inherit.GetClass()->pAddPropertyNames( set, inherit.GetPropertyPrefix() );
		}
	}
}



// Texture properties
///////////////////////

void igdeGDClass::SetTextureProperties( const igdeGDPropertyList &properties ){
	pTextureProperties = properties;
}

void igdeGDClass::AddTextureProperty( igdeGDProperty *property ){
	pTextureProperties.Add( property );
}

void igdeGDClass::RemoveTextureProperty( igdeGDProperty *property ){
	pTextureProperties.Remove( property );
}

void igdeGDClass::RemoveAllTextureProperties(){
	pTextureProperties.RemoveAll();
}

igdeGDProperty *igdeGDClass::GetTexturePropertyNamed( const char *name ) const{
	const igdeGDClass *gdclass = this;
	igdeGDProperty *property = gdclass->pTextureProperties.GetNamed( name );
	if( property ){
		return property;
	}
	
	const int count = pInheritClasses.GetCount();
	const int nameLen = strlen( name );
	int i;
	
	for( i=0; i<count; i++ ){
		const igdeGDClassInherit &inherit = *( ( igdeGDClassInherit* )pInheritClasses.GetAt( i ) );
		if( ! inherit.GetClass() ){
			continue;
		}
		
		const decString &prefix = inherit.GetPropertyPrefix();
		const int prefixLen = prefix.GetLength();
		if( nameLen < prefixLen || strncmp( name, prefix, prefixLen ) != 0 ){
			continue;
		}
		
		property = inherit.GetClass()->GetTexturePropertyNamed( name + prefixLen );
		if( property ){
			return property;
		}
	}
	
	return NULL;
}

bool igdeGDClass::GetDefaultTexturePropertyValue( const char *name, decString &value ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	igdeGDProperty * const property = pTextureProperties.GetNamed( name );
	if( property ){
		value = property->GetDefaultValue();
		return true;
	}
	
	const int count = pInheritClasses.GetCount();
	const int nameLen = strlen( name );
	int i;
	
	for( i=0; i<count; i++ ){
		const igdeGDClassInherit &inherit = *( ( igdeGDClassInherit* )pInheritClasses.GetAt( i ) );
		if( ! inherit.GetClass() ){
			continue;
		}
		
		const decString &prefix = inherit.GetPropertyPrefix();
		const int prefixLen = prefix.GetLength();
		if( nameLen < prefixLen || strncmp( name, prefix, prefixLen ) != 0 ){
			continue;
		}
		
		if( inherit.GetClass()->GetDefaultTexturePropertyValue( name + prefixLen, value ) ){
			return true;
		}
	}
	
	return false;
}

bool igdeGDClass::HasDefaultTexturePropertyValue( const char *name ) const{
	decString value;
	return GetDefaultTexturePropertyValue( name, value );
}

void igdeGDClass::AddTexturePropertyNames( decStringSet &set, bool inherited ) const{
	const int propertyCount = pTextureProperties.GetCount();
	int i;
	for( i=0; i<propertyCount; i++ ){
		set.Add( pTextureProperties.GetAt( i )->GetName() );
	}
	
	if( ! inherited ){
		return;
	}
	
	const int inheritCount = pInheritClasses.GetCount();
	for( i=0; i<inheritCount; i++ ){
		const igdeGDClassInherit &inherit = *( ( igdeGDClassInherit* )pInheritClasses.GetAt( i ) );
		if( inherit.GetClass() ){
			inherit.GetClass()->pAddTexturePropertyNames( set, inherit.GetPropertyPrefix() );
		}
	}
}



// Components
///////////////

void igdeGDClass::AddComponent( igdeGDCComponent *component ){
	pListComponents.Add( component );
}

void igdeGDClass::RemoveComponent( igdeGDCComponent *component ){
	pListComponents.Remove( component );
}

void igdeGDClass::RemoveAllComponents(){
	pListComponents.RemoveAll();
}

bool igdeGDClass::HasComponentLinkedProperty( const char *name ) const{
	const int count = pListComponents.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( pListComponents.GetAt( i )->HasPropertyWithName( name ) ){
			return true;
		}
	}
	
	return false;
}

const igdeGDCComponentList igdeGDClass::GetComponentsWithLinkedProperty( const char *name ) const{
	const int count = pListComponents.GetCount();
	igdeGDCComponentList list;
	int i;
	
	for( i=0; i<count; i++ ){
		if( pListComponents.GetAt( i )->HasPropertyWithName( name ) ){
			list.Add( pListComponents.GetAt( i ) );
		}
	}
	
	return list;
}

const decIntList igdeGDClass::GetComponentIndicesWithLinkedProperty( const char *name ) const{
	const int count = pListComponents.GetCount();
	decIntList list;
	int i;
	
	for( i=0; i<count; i++ ){
		if( pListComponents.GetAt( i )->HasPropertyWithName( name ) ){
			list.Add( i );
		}
	}
	
	return list;
}



// Lights
///////////

void igdeGDClass::AddLight( igdeGDCLight *light ){
	pListLights.Add( light );
}

void igdeGDClass::RemoveLight( igdeGDCLight *light ){
	pListLights.Remove( light );
	// notify;
}

void igdeGDClass::RemoveAllLights(){
	pListLights.RemoveAll();
	// notify;
}

bool igdeGDClass::HasLightLinkedProperty( const char *name ) const{
	const int count = pListLights.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( pListLights.GetAt( i )->HasPropertyWithName( name ) ){
			return true;
		}
	}
	
	return false;
}

const igdeGDCLightList igdeGDClass::GetLightWithLinkedProperty( const char *name ) const{
	const int count = pListLights.GetCount();
	igdeGDCLightList list;
	int i;
	
	for( i=0; i<count; i++ ){
		if( pListLights.GetAt( i )->HasPropertyWithName( name ) ){
			list.Add( pListLights.GetAt( i ) );
		}
	}
	
	return list;
}

const decIntList igdeGDClass::GetLightIndicesWithLinkedProperty( const char *name ) const{
	const int count = pListLights.GetCount();
	decIntList list;
	int i;
	
	for( i=0; i<count; i++ ){
		if( pListLights.GetAt( i )->HasPropertyWithName( name ) ){
			list.Add( i );
		}
	}
	
	return list;
}



// Billboards
///////////

void igdeGDClass::AddBillboard( igdeGDCBillboard *billboard ){
	pListBillboards.Add( billboard );
}

void igdeGDClass::RemoveBillboard( igdeGDCBillboard *billboard ){
	pListBillboards.Remove( billboard );
	// notify;
}

void igdeGDClass::RemoveAllBillboards(){
	pListBillboards.RemoveAll();
	// notify;
}

bool igdeGDClass::HasBillboardLinkedProperty( const char *name ) const{
	const int count = pListBillboards.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( pListBillboards.GetAt( i )->HasPropertyWithName( name ) ){
			return true;
		}
	}
	
	return false;
}

const igdeGDCBillboardList igdeGDClass::GetBillboardWithLinkedProperty( const char *name ) const{
	const int count = pListBillboards.GetCount();
	igdeGDCBillboardList list;
	int i;
	
	for( i=0; i<count; i++ ){
		if( pListBillboards.GetAt( i )->HasPropertyWithName( name ) ){
			list.Add( pListBillboards.GetAt( i ) );
		}
	}
	
	return list;
}

const decIntList igdeGDClass::GetBillboardIndicesWithLinkedProperty( const char *name ) const{
	const int count = pListBillboards.GetCount();
	decIntList list;
	int i;
	
	for( i=0; i<count; i++ ){
		if( pListBillboards.GetAt( i )->HasPropertyWithName( name ) ){
			list.Add( i );
		}
	}
	
	return list;
}



// Speakers
/////////////

void igdeGDClass::AddSpeaker( igdeGDCSpeaker *speaker ){
	pListSpeakers.Add( speaker );
}

void igdeGDClass::RemoveSpeaker( igdeGDCSpeaker *speaker ){
	pListSpeakers.Remove( speaker );
}

void igdeGDClass::RemoveAllSpeakers(){
	pListSpeakers.RemoveAll();
}

bool igdeGDClass::HasSpeakerLinkedProperty( const char *name ) const{
	const int count = pListSpeakers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( pListSpeakers.GetAt( i )->HasPropertyWithName( name ) ){
			return true;
		}
	}
	
	return false;
}

const igdeGDCSpeakerList igdeGDClass::GetSpeakerWithLinkedProperty( const char *name ) const{
	const int count = pListSpeakers.GetCount();
	igdeGDCSpeakerList list;
	int i;
	
	for( i=0; i<count; i++ ){
		if( pListSpeakers.GetAt( i )->HasPropertyWithName( name ) ){
			list.Add( pListSpeakers.GetAt( i ) );
		}
	}
	
	return list;
}

const decIntList igdeGDClass::GetSpeakerIndicesWithLinkedProperty( const char *name ) const{
	const int count = pListSpeakers.GetCount();
	decIntList list;
	int i;
	
	for( i=0; i<count; i++ ){
		if( pListSpeakers.GetAt( i )->HasPropertyWithName( name ) ){
			list.Add( i );
		}
	}
	
	return list;
}



// Navigation spaces
//////////////////////

void igdeGDClass::AddNavigationSpace( igdeGDCNavigationSpace *navSpace ){
	pListNavigationSpaces.Add( navSpace );
}

void igdeGDClass::RemoveNavigationSpace( igdeGDCNavigationSpace *navSpace ){
	pListNavigationSpaces.Remove( navSpace );
}

void igdeGDClass::RemoveAllNavigationSpaces(){
	pListNavigationSpaces.RemoveAll();
}

bool igdeGDClass::HasNavSpaceLinkedProperty( const char *name ) const{
	const int count = pListNavigationSpaces.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( pListNavigationSpaces.GetAt( i )->HasPropertyWithName( name ) ){
			return true;
		}
	}
	
	return false;
}

const igdeGDCNavigationSpaceList igdeGDClass::GetNavSpaceWithLinkedProperty( const char *name ) const{
	const int count = pListNavigationSpaces.GetCount();
	igdeGDCNavigationSpaceList list;
	int i;
	
	for( i=0; i<count; i++ ){
		if( pListNavigationSpaces.GetAt( i )->HasPropertyWithName( name ) ){
			list.Add( pListNavigationSpaces.GetAt( i ) );
		}
	}
	
	return list;
}

const decIntList igdeGDClass::GetNavSpaceIndicesWithLinkedProperty( const char *name ) const{
	const int count = pListNavigationSpaces.GetCount();
	decIntList list;
	int i;
	
	for( i=0; i<count; i++ ){
		if( pListNavigationSpaces.GetAt( i )->HasPropertyWithName( name ) ){
			list.Add( i );
		}
	}
	
	return list;
}



// Navigation blockers
//////////////////////

void igdeGDClass::AddNavigationBlocker( igdeGDCNavigationBlocker *blocker ){
	pListNavigationBlockers.Add( blocker );
}

void igdeGDClass::RemoveNavigationBlocker( igdeGDCNavigationBlocker *blocker ){
	pListNavigationBlockers.Remove( blocker );
}

void igdeGDClass::RemoveAllNavigationBlockers(){
	pListNavigationBlockers.RemoveAll();
}

bool igdeGDClass::HasNavBlockerLinkedProperty( const char *name ) const{
	const int count = pListNavigationBlockers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( pListNavigationBlockers.GetAt( i )->HasPropertyWithName( name ) ){
			return true;
		}
	}
	
	return false;
}

const igdeGDCNavigationBlockerList igdeGDClass::GetNavBlockerWithLinkedProperty( const char *name ) const{
	const int count = pListNavigationBlockers.GetCount();
	igdeGDCNavigationBlockerList list;
	int i;
	
	for( i=0; i<count; i++ ){
		if( pListNavigationBlockers.GetAt( i )->HasPropertyWithName( name ) ){
			list.Add( pListNavigationBlockers.GetAt( i ) );
		}
	}
	
	return list;
}

const decIntList igdeGDClass::GetNavBlockerIndicesWithLinkedProperty( const char *name ) const{
	const int count = pListNavigationBlockers.GetCount();
	decIntList list;
	int i;
	
	for( i=0; i<count; i++ ){
		if( pListNavigationBlockers.GetAt( i )->HasPropertyWithName( name ) ){
			list.Add( i );
		}
	}
	
	return list;
}




// SnapPoints
////////////

void igdeGDClass::AddSnapPoint( igdeGDCSnapPoint *snappoint ){
	pSnapPoints.Add( snappoint );
}

void igdeGDClass::RemoveSnapPoint( igdeGDCSnapPoint *snappoint ){
	pSnapPoints.Remove( snappoint );
}

void igdeGDClass::RemoveAllSnapPoints(){
	pSnapPoints.RemoveAll();
}



// Particle Emitters
//////////////////////

void igdeGDClass::AddParticleEmitter( igdeGDCParticleEmitter *emitter){
	pListParticleEmitters.Add( emitter );
}

void igdeGDClass::RemoveParticleEmitter( igdeGDCParticleEmitter *emitter ){
	pListParticleEmitters.Remove( emitter );
	// notify;
}

void igdeGDClass::RemoveAllParticleEmitters(){
	pListParticleEmitters.RemoveAll();
	// notify;
}



// Force Fields
/////////////////

void igdeGDClass::AddForceField( igdeGDCForceField *field ){
	pListForceFields.Add( field );
}

void igdeGDClass::RemoveForceField( igdeGDCForceField *field ){
	pListForceFields.Remove( field );
	// notify;
}

void igdeGDClass::RemoveAllForceFields(){
	pListForceFields.RemoveAll();
	// notify;
}



// Environment Map Probes
///////////////////////////

void igdeGDClass::AddEnvironmentMapProbe( igdeGDCEnvMapProbe *probe ){
	pListEnvMapProbes.Add( probe );
}

void igdeGDClass::RemoveEnvironmentMapProbe( igdeGDCEnvMapProbe *probe ){
	pListEnvMapProbes.Remove( probe );
	// notify;
}

void igdeGDClass::RemoveAllEnvironmentMapProbes(){
	pListEnvMapProbes.RemoveAll();
	// notify;
}

bool igdeGDClass::HasEnvMapProbesLinkedProperty( const char *name ) const{
	const int count = pListEnvMapProbes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( pListEnvMapProbes.GetAt( i )->HasPropertyWithName( name ) ){
			return true;
		}
	}
	
	return false;
}

const igdeGDCEnvMapProbeList igdeGDClass::GetEnvMapProbesWithLinkedProperty( const char *name ) const{
	const int count = pListEnvMapProbes.GetCount();
	igdeGDCEnvMapProbeList list;
	int i;
	
	for( i=0; i<count; i++ ){
		if( pListEnvMapProbes.GetAt( i )->HasPropertyWithName( name ) ){
			list.Add( pListEnvMapProbes.GetAt( i ) );
		}
	}
	
	return list;
}

const decIntList igdeGDClass::GetEnvMapProbesIndicesWithLinkedProperty( const char *name ) const{
	const int count = pListEnvMapProbes.GetCount();
	decIntList list;
	int i;
	
	for( i=0; i<count; i++ ){
		if( pListEnvMapProbes.GetAt( i )->HasPropertyWithName( name ) ){
			list.Add( i );
		}
	}
	
	return list;
}



// Private Functions
//////////////////////

void igdeGDClass::pCleanUp(){
	if( pPreviewImage ){
		pPreviewImage->FreeReference();
	}
	
	RemoveAllSpeakers();
	RemoveAllNavigationBlockers();
	RemoveAllNavigationSpaces();
	RemoveAllEnvironmentMapProbes();
	RemoveAllParticleEmitters();
	RemoveAllForceFields();
	RemoveAllSnapPoints();
	RemoveAllLights();
	RemoveAllBillboards();
	RemoveAllComponents();
	if( pCamera ){
		delete pCamera;
	}
}

void igdeGDClass::pAddPropertyNames( decStringSet &set, const decString &prefix ) const{
	const int propertyCount = pListProperties.GetCount();
	int i;
	for( i=0; i<propertyCount; i++ ){
		set.Add( prefix + pListProperties.GetAt( i )->GetName() );
	}
	
	const int inheritCount = pInheritClasses.GetCount();
	for( i=0; i<inheritCount; i++ ){
		const igdeGDClassInherit &inherit = *( ( igdeGDClassInherit* )pInheritClasses.GetAt( i ) );
		if( inherit.GetClass() ){
			inherit.GetClass()->pAddPropertyNames( set, prefix + inherit.GetPropertyPrefix() );
		}
	}
}

void igdeGDClass::pAddTexturePropertyNames( decStringSet &set, const decString &prefix ) const{
	const int propertyCount = pTextureProperties.GetCount();
	int i;
	for( i=0; i<propertyCount; i++ ){
		set.Add( prefix + pTextureProperties.GetAt( i )->GetName() );
	}
	
	const int inheritCount = pInheritClasses.GetCount();
	for( i=0; i<inheritCount; i++ ){
		const igdeGDClassInherit &inherit = *( ( igdeGDClassInherit* )pInheritClasses.GetAt( i ) );
		if( inherit.GetClass() ){
			inherit.GetClass()->pAddTexturePropertyNames( set, prefix + inherit.GetPropertyPrefix() );
		}
	}
}
