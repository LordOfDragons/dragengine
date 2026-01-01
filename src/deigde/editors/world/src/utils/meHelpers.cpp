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
	if(!gdclass){
		return nullptr;
	}
	
	igdeGDCComponent *component = nullptr;
	decString prefix;
	return FindFirstComponent(*gdclass, prefix, component) ? component : nullptr;
}

bool meHelpers::FindFirstComponent(const igdeGDClass &gdclass, igdeGDCComponent* &component){
	decString prefix;
	return FindFirstComponent(gdclass, prefix, component);
}

bool meHelpers::FindFirstComponent(const igdeGDClass &gdclass, decString &prefix, igdeGDCComponent* &component){
	if(gdclass.GetComponentList().IsNotEmpty()){
		component = gdclass.GetComponentList().GetAt(0);
		prefix.Empty();
		return true;
	}
	
	return gdclass.GetInheritClasses().HasMatching([&](const igdeGDClassInherit &ic){
		if(ic.GetClass() && FindFirstComponent(ic.GetClass(), prefix, component)){
			prefix = ic.GetPropertyPrefix() + prefix;
			return true;
		}
		return false;
	});
}


igdeGDCLight *meHelpers::FindFirstLight(const igdeGDClass *gdclass){
	return gdclass ? FindFirstLight(*gdclass) : nullptr;
}

igdeGDCLight *meHelpers::FindFirstLight(const igdeGDClass &gdclass){
	igdeGDCLight *light = nullptr;
	decString prefix;
	return FindFirstLight(gdclass, prefix, light) ? light : nullptr;
}

bool meHelpers::FindFirstLight(const igdeGDClass &gdclass, igdeGDCLight* &light){
	decString prefix;
	return FindFirstLight(gdclass, prefix, light);
}

bool meHelpers::FindFirstLight(const igdeGDClass &gdclass, decString &prefix, igdeGDCLight* &light){
	if(gdclass.GetLightList().IsNotEmpty()){
		light = gdclass.GetLightList().GetAt(0);
		prefix.Empty();
		return true;
	}
	
	return gdclass.GetInheritClasses().HasMatching([&](const igdeGDClassInherit &ic){
		if(ic.GetClass() && FindFirstLight(ic.GetClass(), prefix, light)){
			prefix = ic.GetPropertyPrefix() + prefix;
			return true;
		}
		return false;
	});
}


igdeGDCNavigationSpace *meHelpers::FindFirstNavigationSpace(const igdeGDClass *gdclass){
	return gdclass ? FindFirstNavigationSpace(*gdclass) : nullptr;
}

igdeGDCNavigationSpace *meHelpers::FindFirstNavigationSpace(const igdeGDClass &gdclass){
	igdeGDCNavigationSpace *navigationSpace = nullptr;
	decString prefix;
	return FindFirstNavigationSpace(gdclass, prefix, navigationSpace) ? navigationSpace : nullptr;
}

bool meHelpers::FindFirstNavigationSpace(const igdeGDClass &gdclass, igdeGDCNavigationSpace* &navigationSpace){
	decString prefix;
	return FindFirstNavigationSpace(gdclass, prefix, navigationSpace);
}

bool meHelpers::FindFirstNavigationSpace(const igdeGDClass &gdclass, decString &prefix, igdeGDCNavigationSpace* &navigationSpace){
	if(gdclass.GetNavigationSpaceList().IsNotEmpty()){
		navigationSpace = gdclass.GetNavigationSpaceList().GetAt(0);
		prefix.Empty();
		return true;
	}
	
	return gdclass.GetInheritClasses().HasMatching([&](const igdeGDClassInherit &ic){
		if(ic.GetClass() && FindFirstNavigationSpace(ic.GetClass(), prefix, navigationSpace)){
			prefix = ic.GetPropertyPrefix() + prefix;
			return true;
		}
		return false;
	});
}


igdeGDCNavigationBlocker *meHelpers::FindFirstNavigationBlocker(const igdeGDClass *gdclass){
	return gdclass ? FindFirstNavigationBlocker(*gdclass) : nullptr;
}

igdeGDCNavigationBlocker *meHelpers::FindFirstNavigationBlocker(const igdeGDClass &gdclass){
	igdeGDCNavigationBlocker *navigationBlocker = nullptr;
	decString prefix;
	return FindFirstNavigationBlocker(gdclass, prefix, navigationBlocker) ? navigationBlocker : nullptr;
}

bool meHelpers::FindFirstNavigationBlocker(const igdeGDClass &gdclass, igdeGDCNavigationBlocker* &navigationBlocker){
	decString prefix;
	return FindFirstNavigationBlocker(gdclass, prefix, navigationBlocker);
}

bool meHelpers::FindFirstNavigationBlocker(const igdeGDClass &gdclass, decString &prefix, igdeGDCNavigationBlocker* &navigationBlocker){
	if(gdclass.GetNavigationBlockerList().IsNotEmpty()){
		navigationBlocker = gdclass.GetNavigationBlockerList().GetAt(0);
		prefix.Empty();
		return true;
	}
	return gdclass.GetInheritClasses().HasMatching([&](const igdeGDClassInherit &ic){
		if(ic.GetClass() && FindFirstNavigationBlocker(ic.GetClass(), prefix, navigationBlocker)){
			prefix = ic.GetPropertyPrefix() + prefix;
			return true;
		}
		return false;
	});
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
	
	return gdclass.GetInheritClasses().HasMatching([&](const igdeGDClassInherit &ic){
		if(ic.GetClass() && FindFirstCamera(ic.GetClass(), prefix, camera)){
			prefix = ic.GetPropertyPrefix() + prefix;
			return true;
		}
		return false;
	});
}


void meHelpers::GetPatternTypePropertyNames(const igdeGDClass &gdclass,
igdeGDProperty::ePathPatternTypes patternType, decStringList &names){
	decStringSet allPropNames;
	gdclass.AddPropertyNames(allPropNames, true);
	
	allPropNames.Visit([&](const decString &name){
		const igdeGDProperty &p = *gdclass.GetPropertyNamed(name);
		if(p.GetType() == igdeGDProperty::eptPath && p.GetPathPatternType() == patternType){
			names.Add(name);
		}
	});
}


void meHelpers::CreateTexture(meObjectTexture::Ref &texture, meObject *object, const char *textureName){
	const igdeGDCComponent * const gdcomponent = meHelpers::FindFirstComponent(object->GetGDClass());
	const igdeGDCCTexture *gdctexture = nullptr;
	if(gdcomponent){
		gdctexture = gdcomponent->GetTextureList().FindOrDefault([&](const igdeGDCCTexture &t){
			return t.GetName() == textureName;
		});
	}
	if(!gdctexture && object->GetGDClass()){
		gdctexture = object->GetGDClass()->GetComponentTextures().FindOrDefault([&](const igdeGDCCTexture &t){
			return t.GetName() == textureName;
		});
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
	
	texture = meObjectTexture::Ref::New(object->GetEnvironment(), textureName);
	texture->SetSkinPath(initialSkin);
	texture->SetColorTint(initialColor);
	texture->SetTexCoordOffset(initialTexCoordOffset);
	texture->SetTexCoordScaling(initialTexCoordScale);
	texture->SetTexCoordRotation(initialTexCoordRotation);
}
