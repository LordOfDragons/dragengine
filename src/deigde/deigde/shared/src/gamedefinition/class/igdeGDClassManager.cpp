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
#include "igdeGDClassManager.h"
#include "component/igdeGDCComponent.h"
#include "component/igdeGDCCTexture.h"
#include "../igdeGDCategory.h"
#include "../igdeTagManager.h"
#include "../property/igdeGDProperty.h"
#include "../visitor/igdeGDVisitor.h"
#include "../../codec/igdeCodecPropertyString.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/logger/deLogger.h>



// Class igdeGDClassManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDClassManager::igdeGDClassManager(){
	pCategories = igdeGDCategory::Ref::New("Classes");
}

igdeGDClassManager::~igdeGDClassManager(){
}



// Management
///////////////

bool igdeGDClassManager::HasNamed(const char *name) const{
	return IndexOfNamed(name) != -1;
}

int igdeGDClassManager::IndexOfNamed(const char *name) const{
	return pClasses.IndexOfMatching([&](const igdeGDClass &c){
		return c.GetName() == name;
	});
}

igdeGDClass *igdeGDClassManager::GetNamed(const char *name) const{
	return pClasses.FindOrDefault([&](const igdeGDClass &c){
		return c.GetName() == name;
	});
}

void igdeGDClassManager::Add(igdeGDClass *gdClass){
	if(!gdClass || HasNamed(gdClass->GetName())){
		DETHROW(deeInvalidParam);
	}
	pClasses.Add(gdClass);
}

void igdeGDClassManager::Remove(igdeGDClass *gdClass){
	pClasses.Remove(gdClass);
}

void igdeGDClassManager::RemoveAll(){
	pClasses.RemoveAll();
}



void igdeGDClassManager::UpdateTags(){
	pHideTags.RemoveAllTags();
	pPartialHideTags.RemoveAllTags();
	
	pClasses.Visit([&](const igdeGDClass &c){
		pHideTags.AddTagsFrom(c.GetHideTags());
		pPartialHideTags.AddTagsFrom(c.GetPartialHideTags());
	});
}

void igdeGDClassManager::SetDefaultClassName(const char *defaultClassName){
	pDefaultClassName = defaultClassName;
}



void igdeGDClassManager::ResolveInheritClasses(){
	pClasses.Visit([&](igdeGDClass &c){
		c.ResolveInheritClasses(*this);
	});
}



void igdeGDClassManager::VisitClassesMatchingCategory(igdeGDVisitor &visitor, const igdeGDCategory *category) const{
	decPath pathCat;
	if(category){
		category->GetFullPath(pathCat);
	}
	
	const decString strPathCat(pathCat.GetPathUnix());
	
	pClasses.Visit([&](igdeGDClass &c){
		if(c.GetCategory() == strPathCat){
			visitor.VisitObjectClass(&c);
		}
	});
}

void igdeGDClassManager::VisitMatchingFilter(igdeGDVisitor &visitor, const decString &filter) const{
	if(filter.GetLength() < 2){
		return;
	}
	
	const decString realFilter(filter.GetLower());
	
	pClasses.Visit([&](igdeGDClass &c){
		if(c.GetName().GetLower().FindString(realFilter) != -1){
			visitor.VisitObjectClass(&c);
		}
	});
}



void igdeGDClassManager::UpdateWith(const igdeGDClassManager &classManager){
	classManager.pClasses.Visit([&](const igdeGDClass &c){
		igdeGDClass::Ref gdclass(igdeGDClass::Ref::New(c));
		igdeGDClass * const gdclassCheck = GetNamed(gdclass->GetName());
		if(gdclassCheck){
			Remove(gdclassCheck);
		}
		Add(gdclass);
	});
	
	pCategories->UpdateWith(classManager.pCategories);
	pAutoFindPath = classManager.pAutoFindPath;
	
	if(!classManager.pDefaultClassName.IsEmpty()){
		pDefaultClassName = classManager.pDefaultClassName;
	}
}

static bool fFindFirstComponent(const igdeGDClass &gdclass, decString &prefix, igdeGDCComponent* &component){
	if(gdclass.GetComponentList().GetCount() > 0){
		component = gdclass.GetComponentList().GetAt(0);
		return true;
	}
	return gdclass.GetInheritClasses().HasMatching([&](const igdeGDClassInherit &inherit){
		if(inherit.GetClass() && fFindFirstComponent(*inherit.GetClass(), prefix, component)){
			prefix = inherit.GetPropertyPrefix() + prefix;
			return true;
		}
		return false;
	});
}

void igdeGDClassManager::UpdateWithElementClasses(const igdeGDClassManager &classManager){
	ClassesList pendingClasses(classManager.pClasses), retryClasses;
	igdeCodecPropertyString codec;
	decString propertyValue;
	bool detectRetry = true;
	
	while(pendingClasses.IsNotEmpty()){
		pendingClasses.Visit([&](igdeGDClass *eclass){
			decPath basePath(decPath::CreatePathUnix(eclass->GetPathEClass()));
			basePath.RemoveLastComponent();
			const decString basePathStr(basePath.GetPathUnix());
			
			igdeGDClass * const gdclassExisting = GetNamed(eclass->GetName());
			igdeGDClass::Ref gdclass;
			bool reused = false;
			
			if(gdclassExisting){
				gdclassExisting->SetPathEClass(eclass->GetPathEClass());
				gdclassExisting->SetPropertyValues(eclass->GetPropertyValues());
				
				gdclassExisting->RemoveAllInheritClasses();
				eclass->GetInheritClasses().Visit([&](const igdeGDClassInherit &inherit){
					gdclassExisting->AddInheritClass(igdeGDClassInherit::Ref::New(inherit));
				});
				gdclassExisting->ResolveInheritClasses(*this);
				
				const igdeGDCCTexture::List &eclassCompTextures = eclass->GetComponentTextures();
				igdeGDCCTexture::List &compTextures = gdclassExisting->GetComponentTextures();
				eclassCompTextures.Visit([&](igdeGDCCTexture *texture){
					const decString &textureName = texture->GetName();
					igdeGDCCTexture * const existingTexture = compTextures.FindOrDefault([&](const igdeGDCCTexture &t){
						return t.GetName() == textureName;
					});
					if(existingTexture){
						compTextures.Remove(existingTexture);
					}
					compTextures.Add(texture);
				});
				
				gdclass = gdclassExisting;
				reused = true;
				
			}else{
				gdclass = igdeGDClass::Ref::New(*eclass);
				gdclass->ResolveInheritClasses(*this);
				
				if(detectRetry){
					if(gdclass->GetInheritClasses().HasMatching([&](const igdeGDClassInherit &inherit){
						if(inherit.GetClass()){
							return false;
						}
						
						const decString &cname = inherit.GetName();
						return pendingClasses.HasMatching([&](const igdeGDClass &c){
							if(c.GetName() == cname){
								// XML element depends on another XML element not processed yet.
								// postpone processing until the required class is ready
								retryClasses.Add(eclass);
								return true;
							}
							return false;
						});
					})){
						return;
					}
				}
				
				Add(gdclass);
				
				const igdeGDClass *firstInheritClass = nullptr;
				gdclass->GetInheritClasses().HasMatching([&](const igdeGDClassInherit &inherit){
					firstInheritClass = inherit.GetClass();
					return firstInheritClass != nullptr;
				});
				
				if(firstInheritClass){
					gdclass->SetDescription(firstInheritClass->GetDescription());
					gdclass->SetHasCamera(firstInheritClass->GetHasCamera());
					gdclass->SetIsGhost(firstInheritClass->GetIsGhost());
					gdclass->SetCanInstantiate(firstInheritClass->GetCanInstantiate());
					gdclass->SetScaleMode(firstInheritClass->GetScaleMode());
					gdclass->SetCategory(firstInheritClass->GetCategory());
				}
			}
			
			// check all property values if they are a path. if so prepend the base path
			decStringDictionary propertyValues(gdclass->GetPropertyValues());
			const decStringList keys(propertyValues.GetKeys());
			
			keys.Visit([&](const decString &key){
				const igdeGDProperty * const gdproperty = gdclass->GetPropertyNamed(key);
				if(gdproperty && gdproperty->GetType() == igdeGDProperty::eptPath){
					const decString &value = propertyValues.GetAt(key);
					if(!value.IsEmpty()){
						propertyValues.SetAt(key, decPath::AbsolutePathUnix(value, basePathStr).GetPathUnix());
					}
				}
			});
			
			// removed unknown property values but only if there are inherits
			if(gdclass->GetInheritClasses().IsNotEmpty()){
				keys.Visit([&](const decString &key){
					if(!gdclass->HasDefaultPropertyValue(key)){
						propertyValues.Remove(key);
					}
				});
			}
			
			gdclass->SetPropertyValues(propertyValues);
			
			// now we can work with the updated property values
			if(!reused){
				gdclass->GetInheritClasses().Visit([&](const igdeGDClassInherit &inherit){
					if(!inherit.GetClass()){
						return;
					}
					
					// link components
					igdeGDCComponent *inheritClassComponent = nullptr;
					decString inheritComponentPrefix;
					fFindFirstComponent(inherit.GetClass(), inheritComponentPrefix, inheritClassComponent);
					
					const igdeGDCComponent::List &components = gdclass->GetComponentList();
					
					components.Visit([&](igdeGDCComponent &component){
						component.SetDoNotScale(inheritClassComponent->GetDoNotScale());
						component.SetStatic(inheritClassComponent->GetStatic());
						component.SetPartialHide(inheritClassComponent->GetPartialHide());
						component.SetAttachTarget(inheritClassComponent->GetAttachTarget());
						component.SetColliderResponseType(inheritClassComponent->GetColliderResponseType());
						component.SetPosition(inheritClassComponent->GetPosition());
						component.SetOrientation(inheritClassComponent->GetOrientation());
						component.SetBoneName(inheritClassComponent->GetBoneName());
						
						int i;
						for(i=0; i<=igdeGDCComponent::epMove; i++){
							component.SetPropertyName(i, inheritComponentPrefix + inheritClassComponent->GetPropertyName(i));
						}
						
						const decString &nameModel = component.GetPropertyName(igdeGDCComponent::epModel);
						if(!nameModel.IsEmpty() && gdclass->GetDefaultPropertyValue(nameModel, propertyValue)){
							component.SetModelPath(propertyValue);
						}
						
						const decString &nameSkin = component.GetPropertyName(igdeGDCComponent::epSkin);
						if(!nameSkin.IsEmpty() && gdclass->GetDefaultPropertyValue(nameSkin, propertyValue)){
							component.SetSkinPath(propertyValue);
						}
						
						const decString &nameRig = component.GetPropertyName(igdeGDCComponent::epRig);
						if(!nameRig.IsEmpty() && gdclass->GetDefaultPropertyValue(nameRig, propertyValue)){
							component.SetRigPath(propertyValue);
						}
						
						const decString &nameAnimator = component.GetPropertyName(igdeGDCComponent::epAnimator);
						if(!nameAnimator.IsEmpty() && gdclass->GetDefaultPropertyValue(nameAnimator, propertyValue)){
							component.SetAnimatorPath(propertyValue);
						}
						
						const decString &namePlaybackController = component.GetPropertyName(igdeGDCComponent::epPlaybackController);
						if(!namePlaybackController.IsEmpty() && gdclass->GetDefaultPropertyValue(namePlaybackController, propertyValue)){
							component.SetPlaybackController(propertyValue);
						}
						
						const decString &nameOccMesh = component.GetPropertyName(igdeGDCComponent::epOcclusionMesh);
						if(!nameOccMesh.IsEmpty() && gdclass->GetDefaultPropertyValue(nameOccMesh, propertyValue)){
							component.SetOcclusionMeshPath(propertyValue);
						}
						
						const decString &nameAudioModel = component.GetPropertyName(igdeGDCComponent::epAudioModel);
						if(!nameAudioModel.IsEmpty() && gdclass->GetDefaultPropertyValue(nameAudioModel, propertyValue)){
							component.SetAudioModelPath(propertyValue);
						}
						
						const decString &nameRenderEnvMap = component.GetPropertyName(igdeGDCComponent::epRenderEnvMap);
						if(!nameRenderEnvMap.IsEmpty() && gdclass->GetDefaultPropertyValue(nameRenderEnvMap, propertyValue)){
							component.SetRenderEnvMap(propertyValue == "1");
						}
						
						const decString &nameAffectsAudio = component.GetPropertyName(igdeGDCComponent::epAffectsAudio);
						if(!nameAffectsAudio.IsEmpty() && gdclass->GetDefaultPropertyValue(nameAffectsAudio, propertyValue)){
							component.SetAffectsAudio(propertyValue == "1");
						}
						
						const decString &nameLightShadowIgnore = component.GetPropertyName(igdeGDCComponent::epLightShadowIgnore);
						if(!nameLightShadowIgnore.IsEmpty() && gdclass->GetDefaultPropertyValue(nameLightShadowIgnore, propertyValue)){
							component.SetLightShadowIgnore(propertyValue == "1");
						}
						
						const decString &nameAnimation = component.GetPropertyName(igdeGDCComponent::epAnimation);
						if(!nameAnimation.IsEmpty() && gdclass->GetDefaultPropertyValue(nameAnimation, propertyValue)){
							component.SetAnimationPath(propertyValue);
						}
						
						const decString &nameMove = component.GetPropertyName(igdeGDCComponent::epMove);
						if(!nameMove.IsEmpty() && gdclass->GetDefaultPropertyValue(nameMove, propertyValue)){
							component.SetMove(propertyValue);
						}
					}, 0, decMath::min(components.GetCount(), inheritClassComponent ? 1 : 0));
				});
			}
			
			// auto categorize
			igdeGDCategory * const autoCategory = pCategories->AutoCategorize(gdclass->GetPathEClass());
			if(autoCategory){
				gdclass->SetCategory(autoCategory->GetFullPathString());
			}
		});
		
		// if the count of postponed classes does not change a reference loop cycle exists.
		// if this is the case process the classes anyway
		detectRetry = retryClasses.GetCount() < pendingClasses.GetCount();
		pendingClasses = retryClasses;
		retryClasses.RemoveAll();
	}
}
