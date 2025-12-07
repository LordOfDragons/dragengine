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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "meHelpers.h"
#include "../world/object/meObject.h"
#include "../world/object/texture/meObjectTexture.h"
#include "../undosys/properties/object/texture/meUObjectAddTexture.h"

#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gamedefinition/class/igdeGDClassInherit.h>
#include <deigde/gamedefinition/class/component/igdeGDCComponent.h>
#include <deigde/gamedefinition/class/component/igdeGDCCTexture.h>
#include <deigde/gamedefinition/class/igdeGDCamera.h>
#include <deigde/gamedefinition/class/light/igdeGDCLight.h>
#include <deigde/gamedefinition/class/navblocker/igdeGDCNavigationBlocker.h>
#include <deigde/gamedefinition/class/navspace/igdeGDCNavigationSpace.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringSet.h>



igdeGDCComponent *meHelpers::FindFirstComponent(const igdeGDClass *gdclass){
	return gdclass ? FindFirstComponent(*gdclass) : NULL;
}

igdeGDCComponent *meHelpers::FindFirstComponent(const igdeGDClass &gdclass){
	igdeGDCComponent *component = NULL;
	decString prefix;
	return FindFirstComponent(gdclass, prefix, component) ? component : NULL;
}

bool meHelpers::FindFirstComponent(const igdeGDClass &gdclass, igdeGDCComponent* &component){
	decString prefix;
	return FindFirstComponent(gdclass, prefix, component);
}

bool meHelpers::FindFirstComponent(const igdeGDClass &gdclass, decString &prefix, igdeGDCComponent* &component){
	if(gdclass.GetComponentList().GetCount() > 0){
		component = gdclass.GetComponentList().GetAt(0);
		prefix.Empty();
		return true;
	}
	const int count = gdclass.GetInheritClassCount();
	for(int i=0; i<count; i++){
		const igdeGDClassInherit &inherit = *gdclass.GetInheritClassAt(i);
		if(inherit.GetClass() && meHelpers::FindFirstComponent(*inherit.GetClass(), prefix, component)){
			prefix = inherit.GetPropertyPrefix() + prefix;
			return true;
		}
	}
	return false;
}



igdeGDCLight *meHelpers::FindFirstLight(const igdeGDClass *gdclass){
	return gdclass ? FindFirstLight(*gdclass) : NULL;
}

igdeGDCLight *meHelpers::FindFirstLight(const igdeGDClass &gdclass){
	igdeGDCLight *light = NULL;
	decString prefix;
	return FindFirstLight(gdclass, prefix, light) ? light : NULL;
}

bool meHelpers::FindFirstLight(const igdeGDClass &gdclass, igdeGDCLight* &light){
	decString prefix;
	return FindFirstLight(gdclass, prefix, light);
}

bool meHelpers::FindFirstLight(const igdeGDClass &gdclass, decString &prefix, igdeGDCLight* &light){
	if(gdclass.GetLightList().GetCount() > 0){
		light = gdclass.GetLightList().GetAt(0);
		prefix.Empty();
		return true;
	}
	const int count = gdclass.GetInheritClassCount();
	for(int i=0; i<count; i++){
		const igdeGDClassInherit &inherit = *gdclass.GetInheritClassAt(i);
		if(inherit.GetClass() && meHelpers::FindFirstLight(*inherit.GetClass(), prefix, light)){
			prefix = inherit.GetPropertyPrefix() + prefix;
			return true;
		}
	}
	return false;
}



igdeGDCNavigationSpace *meHelpers::FindFirstNavigationSpace(const igdeGDClass *gdclass){
	return gdclass ? FindFirstNavigationSpace(*gdclass) : NULL;
}

igdeGDCNavigationSpace *meHelpers::FindFirstNavigationSpace(const igdeGDClass &gdclass){
	igdeGDCNavigationSpace *navigationSpace = NULL;
	decString prefix;
	return FindFirstNavigationSpace(gdclass, prefix, navigationSpace) ? navigationSpace : NULL;
}

bool meHelpers::FindFirstNavigationSpace(const igdeGDClass &gdclass, igdeGDCNavigationSpace* &navigationSpace){
	decString prefix;
	return FindFirstNavigationSpace(gdclass, prefix, navigationSpace);
}

bool meHelpers::FindFirstNavigationSpace(const igdeGDClass &gdclass, decString &prefix, igdeGDCNavigationSpace* &navigationSpace){
	if(gdclass.GetNavigationSpaceList().GetCount() > 0){
		navigationSpace = gdclass.GetNavigationSpaceList().GetAt(0);
		prefix.Empty();
		return true;
	}
	const int count = gdclass.GetInheritClassCount();
	for(int i=0; i<count; i++){
		const igdeGDClassInherit &inherit = *gdclass.GetInheritClassAt(i);
		if(inherit.GetClass() && meHelpers::FindFirstNavigationSpace(*inherit.GetClass(), prefix, navigationSpace)){
			prefix = inherit.GetPropertyPrefix() + prefix;
			return true;
		}
	}
	return false;
}



igdeGDCNavigationBlocker *meHelpers::FindFirstNavigationBlocker(const igdeGDClass *gdclass){
	return gdclass ? FindFirstNavigationBlocker(*gdclass) : NULL;
}

igdeGDCNavigationBlocker *meHelpers::FindFirstNavigationBlocker(const igdeGDClass &gdclass){
	igdeGDCNavigationBlocker *navigationBlocker = NULL;
	decString prefix;
	return FindFirstNavigationBlocker(gdclass, prefix, navigationBlocker) ? navigationBlocker : NULL;
}

bool meHelpers::FindFirstNavigationBlocker(const igdeGDClass &gdclass, igdeGDCNavigationBlocker* &navigationBlocker){
	decString prefix;
	return FindFirstNavigationBlocker(gdclass, prefix, navigationBlocker);
}

bool meHelpers::FindFirstNavigationBlocker(const igdeGDClass &gdclass, decString &prefix, igdeGDCNavigationBlocker* &navigationBlocker){
	if(gdclass.GetNavigationBlockerList().GetCount() > 0){
		navigationBlocker = gdclass.GetNavigationBlockerList().GetAt(0);
		prefix.Empty();
		return true;
	}
	const int count = gdclass.GetInheritClassCount();
	for(int i=0; i<count; i++){
		const igdeGDClassInherit &inherit = *gdclass.GetInheritClassAt(i);
		if(inherit.GetClass() && meHelpers::FindFirstNavigationBlocker(*inherit.GetClass(), prefix, navigationBlocker)){
			prefix = inherit.GetPropertyPrefix() + prefix;
			return true;
		}
	}
	return false;
}



igdeGDCamera *meHelpers::FindFirstCamera(const igdeGDClass *gdclass){
	return gdclass ? FindFirstCamera(*gdclass) : nullptr;
}

igdeGDCamera *meHelpers::FindFirstCamera(const igdeGDClass &gdclass){
	igdeGDCamera *camera = nullptr;
	decString prefix;
	return FindFirstCamera(gdclass, prefix, camera) ? camera : nullptr;
}

bool meHelpers::FindFirstCamera(const igdeGDClass &gdclass, igdeGDCamera* &camera){
	decString prefix;
	return FindFirstCamera(gdclass, prefix, camera);
}

bool meHelpers::FindFirstCamera(const igdeGDClass &gdclass, decString &prefix, igdeGDCamera* &camera){
	if(gdclass.GetHasCamera()){
		camera = gdclass.GetCamera();
		prefix.Empty();
		return true;
	}
	const int count = gdclass.GetInheritClassCount();
	int i;
	for(i=0; i<count; i++){
		const igdeGDClassInherit &inherit = *gdclass.GetInheritClassAt(i);
		if(inherit.GetClass() && meHelpers::FindFirstCamera(*inherit.GetClass(), prefix, camera)){
			prefix = inherit.GetPropertyPrefix() + prefix;
			return true;
		}
	}
	return false;
}



void meHelpers::GetPatternTypePropertyNames(const igdeGDClass &gdclass,
igdeGDProperty::ePathPatternTypes patternType, decStringList &names){
	decStringSet allPropNames;
	gdclass.AddPropertyNames(allPropNames, true);
	
	const int allPropNameCount = allPropNames.GetCount();
	int i;
	
	for(i=0; i<allPropNameCount; i++){
		const igdeGDProperty &gdprop = *gdclass.GetPropertyNamed(allPropNames.GetAt(i));
		if(gdprop.GetType() == igdeGDProperty::eptPath && gdprop.GetPathPatternType() == patternType){
			names.Add(allPropNames.GetAt(i));
		}
	}
}



void meHelpers::CreateTexture(meObjectTexture::Ref &texture, meObject *object, const char *textureName){
	const igdeGDCComponent * const gdcomponent = meHelpers::FindFirstComponent(object->GetGDClass());
	const igdeGDCCTexture *gdctexture = NULL;
	if(gdcomponent){
		gdctexture = gdcomponent->GetTextureList().GetNamed(textureName);
	}
	if(! gdctexture && object->GetGDClass()){
		gdctexture = object->GetGDClass()->GetComponentTextures().GetNamed(textureName);
	}
	meHelpers::CreateTexture(texture, object, textureName, gdctexture);
}

void meHelpers::CreateTexture(meObjectTexture::Ref &texture, meObject *object,
const char *textureName, const igdeGDCCTexture *gdctexture){
	decColor initialColor(1.0f, 1.0f, 1.0f);
	decVector2 initialTexCoordOffset(0.0f, 0.0f);
	decVector2 initialTexCoordScale(1.0f, 1.0f);
	float initialTexCoordRotation(0.0f);
	decString initialSkin;
	
	if(gdctexture){
		initialSkin = gdctexture->GetPathSkin();
		initialColor = gdctexture->GetColorTint();
		initialTexCoordOffset = gdctexture->GetOffset();
		initialTexCoordScale = gdctexture->GetScale();
		initialTexCoordRotation = gdctexture->GetRotation();
	}
	
	texture.TakeOverWith(object->GetEnvironment(), textureName);
	texture->SetSkinPath(initialSkin);
	texture->SetColorTint(initialColor);
	texture->SetTexCoordOffset(initialTexCoordOffset);
	texture->SetTexCoordScaling(initialTexCoordScale);
	texture->SetTexCoordRotation(initialTexCoordRotation);
}
