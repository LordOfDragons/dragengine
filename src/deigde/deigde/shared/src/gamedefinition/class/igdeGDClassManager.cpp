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
#include "igdeGDClassReference.h"
#include "igdeGDClassManager.h"
#include "component/igdeGDCComponent.h"
#include "light/igdeGDCLight.h"
#include "particleemitter/igdeGDCParticleEmitter.h"
#include "forcefield/igdeGDCForceField.h"
#include "envmapprobe/igdeGDCEnvMapProbe.h"
#include "speaker/igdeGDCSpeaker.h"
#include "navspace/igdeGDCNavigationSpace.h"
#include "navblocker/igdeGDCNavigationBlocker.h"
#include "../igdeGDCategory.h"
#include "../igdeTagManager.h"
#include "../property/igdeGDProperty.h"
#include "../property/igdeGDPropertyList.h"
#include "../visitor/igdeGDVisitor.h"
#include "../../codec/igdeCodecPropertyString.h"

#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/logger/deLogger.h>



// Class igdeGDClassManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDClassManager::igdeGDClassManager(){
	pCategories.TakeOver( new igdeGDCategory( "Classes" ) );
}

igdeGDClassManager::~igdeGDClassManager(){
}



// Management
///////////////

int igdeGDClassManager::GetCount() const{
	return pClasses.GetCount();
}

bool igdeGDClassManager::Has( igdeGDClass *gdClass ) const{
	return pClasses.Has( gdClass );
}

bool igdeGDClassManager::HasNamed( const char *name ) const{
	return IndexOfNamed( name ) != -1;
}

int igdeGDClassManager::IndexOf( igdeGDClass *gdClass ) const{
	return pClasses.IndexOf( gdClass );
}

int igdeGDClassManager::IndexOfNamed( const char *name ) const{
	const int count = pClasses.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( igdeGDClass* )pClasses.GetAt( i ) )->GetName() == name ){
			return i;
		}
	}
	
	return -1;
}

igdeGDClass *igdeGDClassManager::GetAt( int index ) const{
	return ( igdeGDClass* )pClasses.GetAt( index );
}

igdeGDClass *igdeGDClassManager::GetNamed( const char *name ) const{
	const int index = IndexOfNamed( name );
	if( index != -1 ){
		return ( igdeGDClass* )pClasses.GetAt( index );
		
	}else{
		return NULL;
	}
}

void igdeGDClassManager::Add( igdeGDClass *gdClass ){
	if( ! gdClass || HasNamed( gdClass->GetName() ) ){
		DETHROW( deeInvalidParam );
	}
	pClasses.Add( gdClass );
}

void igdeGDClassManager::Remove( igdeGDClass *gdClass ){
	pClasses.Remove( gdClass );
}

void igdeGDClassManager::RemoveAll(){
	pClasses.RemoveAll();
}



void igdeGDClassManager::UpdateTags(){
	const int count = pClasses.GetCount();
	int i;
	
	pHideTags.RemoveAllTags();
	pPartialHideTags.RemoveAllTags();
	
	for( i=0; i<count; i++ ){
		const igdeGDClass &gdclass = *( ( igdeGDClass* )pClasses.GetAt( i ) );
		pHideTags.AddTagsFrom( gdclass.GetHideTags() );
		pPartialHideTags.AddTagsFrom( gdclass.GetPartialHideTags() );
	}
}

void igdeGDClassManager::SetDefaultClassName( const char *defaultClassName ){
	pDefaultClassName = defaultClassName;
}



void igdeGDClassManager::ResolveInheritClasses(){
	const int count = pClasses.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeGDClass* )pClasses.GetAt( i ) )->ResolveInheritClasses( *this );
	}
}



void igdeGDClassManager::VisitClassesMatchingCategory( igdeGDVisitor &visitor, const igdeGDCategory *category ) const{
	decPath pathCat;
	if( category ){
		category->GetFullPath( pathCat );
	}
	
	const decString strPathCat( pathCat.GetPathUnix() );
	const int count = pClasses.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		igdeGDClass * const gdClass = ( igdeGDClass* )pClasses.GetAt( i );
		if( gdClass->GetCategory() == strPathCat ){
			visitor.VisitObjectClass( gdClass );
		}
	}
}

void igdeGDClassManager::VisitMatchingFilter( igdeGDVisitor &visitor, const decString &filter ) const{
	if( filter.GetLength() < 2 ){
		return;
	}
	
	const decString realFilter( filter.GetLower() );
	const int count = pClasses.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		igdeGDClass * const gdClass = ( igdeGDClass* )pClasses.GetAt( i );
		if( gdClass->GetName().GetLower().FindString( realFilter ) != -1 ){
			visitor.VisitObjectClass( gdClass );
		}
	}
}



void igdeGDClassManager::UpdateWith( const igdeGDClassManager &classManager ){
	const int count = classManager.GetCount();
	igdeGDClassReference gdClass;
	int i;
	
	for( i=0; i<count; i++ ){
		gdClass.TakeOver( new igdeGDClass( *classManager.GetAt( i ) ) );
		igdeGDClass * const gdclassCheck = GetNamed( gdClass->GetName() );
		if( gdclassCheck ){
			Remove( gdclassCheck );
		}
		Add( gdClass );
	}
	
	pCategories->UpdateWith( classManager.pCategories );
	pAutoFindPath = classManager.pAutoFindPath;
	
	if( ! classManager.pDefaultClassName.IsEmpty() ){
		pDefaultClassName = classManager.pDefaultClassName;
	}
}

static bool fFindFirstComponent( const igdeGDClass &gdclass, decString &prefix, igdeGDCComponent* &component ){
	if( gdclass.GetComponentList().GetCount() > 0 ){
		component = gdclass.GetComponentList().GetAt( 0 );
		return true;
	}
	const int count = gdclass.GetInheritClassCount();
	for( int i=0; i<count; i++ ){
		const igdeGDClassInherit &inherit = *gdclass.GetInheritClassAt( i );
		if( inherit.GetClass() && fFindFirstComponent( *inherit.GetClass(), prefix, component ) ){
			prefix = inherit.GetPropertyPrefix() + prefix;
			return true;
		}
	}
	return false;
}

void igdeGDClassManager::UpdateWithElementClasses( const igdeGDClassManager &classManager ){
	const int count = classManager.GetCount();
	igdeCodecPropertyString codec;
	igdeGDClassReference gdClass;
	deObjectReference refCopy;
	decString propertyValue;
	int i, j, k, h;
	
	for( i=0; i<count; i++ ){
		const igdeGDClass &eclass = *classManager.GetAt( i );
		
		decPath basePath( decPath::CreatePathUnix( eclass.GetPathEClass() ) );
		basePath.RemoveLastComponent();
		const decString basePathStr( basePath.GetPathUnix() );
		
		igdeGDClass * const gdclassExisting = GetNamed( eclass.GetName() );
		
		if( gdclassExisting ){
			gdclassExisting->SetPathEClass( eclass.GetPathEClass() );
			gdclassExisting->SetPropertyValues( eclass.GetPropertyValues() );
			
			gdclassExisting->RemoveAllInheritClasses();
			const int inheritClassCount = eclass.GetInheritClassCount();
			for( j=0; j<inheritClassCount; j++ ){
				refCopy.TakeOver( new igdeGDClassInherit( *eclass.GetInheritClassAt( j ) ) );
				gdclassExisting->AddInheritClass( ( igdeGDClassInherit* )( deObject* )refCopy );
			}
			gdclassExisting->ResolveInheritClasses( *this );
			
			/*
			const igdeGDCComponentList &components = eclass.GetComponentList();
			const int componentCount = components.GetCount();
			if( componentCount > 0 ){
				gdclassExisting->RemoveAllComponents();
				for( j=0; j<componentCount; j++ ){
					refCopy.TakeOver( new igdeGDCComponent( *components.GetAt( j ) ) );
					gdclassExisting->AddComponent( ( igdeGDCComponent* )( deObject* )refCopy );
				}
			}
			*/
			
			gdClass = gdclassExisting;
			
		}else{
			gdClass.TakeOver( new igdeGDClass( eclass ) );
			Add( gdClass );
			
			gdClass->ResolveInheritClasses( *this );
			
			const igdeGDClass *firstInheritClass = NULL;
			const int inheritClassCount = gdClass->GetInheritClassCount();
			for( j=0; j<inheritClassCount; j++ ){
				firstInheritClass = gdClass->GetInheritClassAt( j )->GetClass();
				if( firstInheritClass ){
					break;
				}
			}
			
			if( firstInheritClass ){
				gdClass->SetDescription( firstInheritClass->GetDescription() );
				gdClass->SetHasCamera( firstInheritClass->GetHasCamera() );
				gdClass->SetIsGhost( firstInheritClass->GetIsGhost() );
				gdClass->SetCanInstantiate( firstInheritClass->GetCanInstantiate() );
				gdClass->SetScaleMode( firstInheritClass->GetScaleMode() );
				
				gdClass->SetCategory( firstInheritClass->GetCategory() );
				
				// certain element classes (like triggers) do not define any component but the
				// element class loader always adds one component. if the inheritClass class has a
				// component without any linked parameter then we consider the element class
				// having no componet and we copy the inheritClass component
				if( firstInheritClass->GetComponentList().GetCount() > 0 ){
					const igdeGDCComponent &component = *firstInheritClass->GetComponentList().GetAt( 0 );
					bool hasComponent = false;
					
					for( j=0; j<=igdeGDCComponent::epAttachRotation; j++ ){
						if( component.IsPropertySet( j ) ){
							hasComponent = true;
							break;
						}
					}
					
					if( ! hasComponent ){
						gdClass->RemoveAllComponents();
						refCopy.TakeOver( new igdeGDCComponent( component ) );
						gdClass->AddComponent( ( igdeGDCComponent* )( deObject* )refCopy );
					}
				}
			}
		}
		
		// check all property values if they are a path. if so prepend the base path
		decStringDictionary propertyValues( gdClass->GetPropertyValues() );
		const decStringList keys( propertyValues.GetKeys() );
		const int keyCount = keys.GetCount();
		
		for( j=0; j<keyCount; j++ ){
			const decString &key = keys.GetAt( j );
			const igdeGDProperty * const gdproperty = gdClass->GetPropertyNamed( key );
			if( gdproperty && gdproperty->GetType() == igdeGDProperty::eptPath ){
				const decString &value = propertyValues.GetAt( key );
				if( ! value.IsEmpty() ){
					propertyValues.SetAt( key, decPath::AbsolutePathUnix( value, basePathStr ).GetPathUnix() );
				}
			}
		}
		
		// removed unknown property values but only if there are inherits
		const int inheritClassCount = gdClass->GetInheritClassCount();
		if(inheritClassCount > 0 ){
			decStringList removeKeys;
			
			for( j=0; j<keyCount; j++ ){
				const decString &key = keys.GetAt( j );
				if( ! gdClass->HasDefaultPropertyValue( key ) ){
					removeKeys.Add( key );
				}
			}
			
			const int removeCount = removeKeys.GetCount();
			for( j=0; j<removeCount; j++ ){
				propertyValues.Remove( removeKeys.GetAt( j ) );
			}
		}
		
		gdClass->SetPropertyValues( propertyValues );
		
		// now we can work with the updated property values
		for( h=0; h<inheritClassCount; h++ ){
			const igdeGDClass * const inheritClass = gdClass->GetInheritClassAt( h )->GetClass();
			if( ! inheritClass ){
				continue;
			}
			
			// link components
			igdeGDCComponent *inheritClassComponent = NULL;
			decString inheritComponentPrefix;
			fFindFirstComponent( *inheritClass, inheritComponentPrefix, inheritClassComponent );
			
			const igdeGDCComponentList &components = gdClass->GetComponentList();
			const int componentCount = components.GetCount();
			const int copyCount = decMath::min( componentCount, inheritClassComponent ? 1 : 0 );
			
			for( j=0; j<copyCount; j++ ){
				igdeGDCComponent &component = *components.GetAt( j );
				
				component.SetDoNotScale( inheritClassComponent->GetDoNotScale() );
				component.SetStatic( inheritClassComponent->GetStatic() );
				component.SetPartialHide( inheritClassComponent->GetPartialHide() );
				component.SetAttachTarget( inheritClassComponent->GetAttachTarget() );
				component.SetColliderResponseType( inheritClassComponent->GetColliderResponseType() );
				component.SetPosition( inheritClassComponent->GetPosition() );
				component.SetOrientation( inheritClassComponent->GetOrientation() );
				component.SetBoneName( inheritClassComponent->GetBoneName() );
				
				for( k=0; k<=igdeGDCComponent::epAttachRotation; k++ ){
					component.SetPropertyName( k, inheritComponentPrefix + inheritClassComponent->GetPropertyName( k ) );
				}
				
				const decString &nameModel = component.GetPropertyName( igdeGDCComponent::epModel );
				if( ! nameModel.IsEmpty() && gdClass->GetDefaultPropertyValue( nameModel, propertyValue ) ){
					component.SetModelPath( propertyValue );
				}
				
				const decString &nameSkin = component.GetPropertyName( igdeGDCComponent::epSkin );
				if( ! nameSkin.IsEmpty() && gdClass->GetDefaultPropertyValue( nameSkin, propertyValue ) ){
					component.SetSkinPath( propertyValue );
				}
				
				const decString &nameRig = component.GetPropertyName( igdeGDCComponent::epRig );
				if( ! nameRig.IsEmpty() && gdClass->GetDefaultPropertyValue( nameRig, propertyValue ) ){
					component.SetRigPath( propertyValue );
				}
				
				const decString &nameAnim = component.GetPropertyName( igdeGDCComponent::epAnimator );
				if( ! nameAnim.IsEmpty() && gdClass->GetDefaultPropertyValue( nameAnim, propertyValue ) ){
					component.SetAnimatorPath( propertyValue );
				}
				
				const decString &namePlaybackController = component.GetPropertyName( igdeGDCComponent::epPlaybackController );
				if( ! namePlaybackController.IsEmpty() && gdClass->GetDefaultPropertyValue( namePlaybackController, propertyValue ) ){
					component.SetPlaybackController( propertyValue );
				}
				
				const decString &nameOccMesh = component.GetPropertyName( igdeGDCComponent::epOcclusionMesh );
				if( ! nameOccMesh.IsEmpty() && gdClass->GetDefaultPropertyValue( nameOccMesh, propertyValue ) ){
					component.SetOcclusionMeshPath( propertyValue );
				}
				
				const decString &nameAudioModel = component.GetPropertyName( igdeGDCComponent::epAudioModel );
				if( ! nameAudioModel.IsEmpty() && gdClass->GetDefaultPropertyValue( nameAudioModel, propertyValue ) ){
					component.SetAudioModelPath( propertyValue );
				}
				
				const decString &nameRenderEnvMap = component.GetPropertyName( igdeGDCComponent::epRenderEnvMap );
				if( ! nameRenderEnvMap.IsEmpty() && gdClass->GetDefaultPropertyValue( nameRenderEnvMap, propertyValue ) ){
					component.SetRenderEnvMap( propertyValue == "1" );
				}
				
				const decString &nameAffectsAudio = component.GetPropertyName( igdeGDCComponent::epAffectsAudio );
				if( ! nameAffectsAudio.IsEmpty() && gdClass->GetDefaultPropertyValue( nameAffectsAudio, propertyValue ) ){
					component.SetAffectsAudio( propertyValue == "1" );
				}
			}
		}
		
		if(inheritClassCount > 0 ){
			// removed unknown property values
			const decStringList keys( propertyValues.GetKeys() );
			const int keyCount = keys.GetCount();
			decStringList removeKeys;
			
			for( j=0; j<keyCount; j++ ){
				const decString &key = keys.GetAt( j );
				for( h=0; h<inheritClassCount; h++ ){
					const igdeGDClass * const inheritClass = gdClass->GetInheritClassAt( h )->GetClass();
					if( inheritClass && inheritClass->HasDefaultPropertyValue( key ) ){
						break;
					}
				}
				if( h == inheritClassCount ){
					removeKeys.Add( key );
				}
			}
			
			const int removeCount = removeKeys.GetCount();
			for( j=0; j<removeCount; j++ ){
				propertyValues.Remove( removeKeys.GetAt( j ) );
			}
		}
		
		gdClass->SetPropertyValues( propertyValues );
		
		// auto categorize
		igdeGDCategory * const autoCategory = pCategories->AutoCategorize( gdClass->GetPathEClass() );
		if( autoCategory ){
			gdClass->SetCategory( autoCategory->GetFullPathString() );
		}
	}
}
