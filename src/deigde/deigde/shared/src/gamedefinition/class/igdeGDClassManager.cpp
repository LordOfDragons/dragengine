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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeGDClass.h"
#include "igdeGDClassInherit.h"
#include "igdeGDClassReference.h"
#include "igdeGDClassManager.h"
#include "component/igdeGDCComponent.h"
#include "component/igdeGDCCTexture.h"
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
		bool reused = false;
		
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
			
			const igdeGDCCTextureList &eclassCompTextures = eclass.GetComponentTextures();
			igdeGDCCTextureList &compTextures = gdclassExisting->GetComponentTextures();
			const int textureCount = eclassCompTextures.GetCount();
			for( j=0; j<textureCount; j++ ){
				igdeGDCCTexture * const texture = eclassCompTextures.GetAt( j );
				igdeGDCCTexture * const existingTexture = compTextures.GetNamed( texture->GetName() );
				if( existingTexture ){
					compTextures.Remove( existingTexture );
				}
				compTextures.Add( texture );
			}
			
			gdClass = gdclassExisting;
			reused = true;
			
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
				if(!gdClass->HasDefaultPropertyValue(key)){
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
		if( ! reused ){
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
					
					for( k=0; k<=igdeGDCComponent::epMove; k++ ){
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
					
					const decString &nameAnimator = component.GetPropertyName( igdeGDCComponent::epAnimator );
					if( ! nameAnimator.IsEmpty() && gdClass->GetDefaultPropertyValue( nameAnimator, propertyValue ) ){
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
					
					const decString &nameLightShadowIgnore = component.GetPropertyName( igdeGDCComponent::epLightShadowIgnore );
					if( ! nameLightShadowIgnore.IsEmpty() && gdClass->GetDefaultPropertyValue( nameLightShadowIgnore, propertyValue ) ){
						component.SetLightShadowIgnore( propertyValue == "1" );
					}
					
					const decString &nameAnimation = component.GetPropertyName( igdeGDCComponent::epAnimation );
					if( ! nameAnimation.IsEmpty() && gdClass->GetDefaultPropertyValue( nameAnimation, propertyValue ) ){
						component.SetAnimationPath( propertyValue );
					}
					
					const decString &nameMove = component.GetPropertyName( igdeGDCComponent::epMove );
					if( ! nameMove.IsEmpty() && gdClass->GetDefaultPropertyValue( nameMove, propertyValue ) ){
						component.SetMove( propertyValue );
					}
				}
			}
		}
		
		// auto categorize
		igdeGDCategory * const autoCategory = pCategories->AutoCategorize( gdClass->GetPathEClass() );
		if( autoCategory ){
			gdClass->SetCategory( autoCategory->GetFullPathString() );
		}
	}
}
