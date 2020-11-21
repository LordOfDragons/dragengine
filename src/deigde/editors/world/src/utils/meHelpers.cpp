/* 
 * Drag[en]gine IGDE World Editor
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

#include "meHelpers.h"
#include "../world/object/meObject.h"
#include "../world/object/texture/meObjectTexture.h"
#include "../undosys/properties/object/texture/meUObjectAddTexture.h"

#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gamedefinition/class/igdeGDClassInherit.h>
#include <deigde/gamedefinition/class/component/igdeGDCComponent.h>
#include <deigde/gamedefinition/class/component/igdeGDCCTexture.h>
#include <deigde/gamedefinition/class/light/igdeGDCLight.h>
#include <deigde/gamedefinition/class/navblocker/igdeGDCNavigationBlocker.h>
#include <deigde/gamedefinition/class/navspace/igdeGDCNavigationSpace.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringSet.h>



igdeGDCComponent *meHelpers::FindFirstComponent( const igdeGDClass *gdclass ){
	return gdclass ? FindFirstComponent( *gdclass ) : NULL;
}

igdeGDCComponent *meHelpers::FindFirstComponent( const igdeGDClass &gdclass ){
	igdeGDCComponent *component = NULL;
	decString prefix;
	return FindFirstComponent( gdclass, prefix, component ) ? component : NULL;
}

bool meHelpers::FindFirstComponent( const igdeGDClass &gdclass, igdeGDCComponent* &component ){
	decString prefix;
	return FindFirstComponent( gdclass, prefix, component );
}

bool meHelpers::FindFirstComponent( const igdeGDClass &gdclass, decString &prefix, igdeGDCComponent* &component ){
	if( gdclass.GetComponentList().GetCount() > 0 ){
		component = gdclass.GetComponentList().GetAt( 0 );
		prefix.Empty();
		return true;
	}
	const int count = gdclass.GetInheritClassCount();
	for( int i=0; i<count; i++ ){
		const igdeGDClassInherit &inherit = *gdclass.GetInheritClassAt( i );
		if( inherit.GetClass() && meHelpers::FindFirstComponent( *inherit.GetClass(), prefix, component ) ){
			prefix = inherit.GetPropertyPrefix() + prefix;
			return true;
		}
	}
	return false;
}



igdeGDCLight *meHelpers::FindFirstLight( const igdeGDClass *gdclass ){
	return gdclass ? FindFirstLight( *gdclass ) : NULL;
}

igdeGDCLight *meHelpers::FindFirstLight( const igdeGDClass &gdclass ){
	igdeGDCLight *light = NULL;
	decString prefix;
	return FindFirstLight( gdclass, prefix, light ) ? light : NULL;
}

bool meHelpers::FindFirstLight( const igdeGDClass &gdclass, igdeGDCLight* &light ){
	decString prefix;
	return FindFirstLight( gdclass, prefix, light );
}

bool meHelpers::FindFirstLight( const igdeGDClass &gdclass, decString &prefix, igdeGDCLight* &light ){
	if( gdclass.GetLightList().GetCount() > 0 ){
		light = gdclass.GetLightList().GetAt( 0 );
		prefix.Empty();
		return true;
	}
	const int count = gdclass.GetInheritClassCount();
	for( int i=0; i<count; i++ ){
		const igdeGDClassInherit &inherit = *gdclass.GetInheritClassAt( i );
		if( inherit.GetClass() && meHelpers::FindFirstLight( *inherit.GetClass(), prefix, light ) ){
			prefix = inherit.GetPropertyPrefix() + prefix;
			return true;
		}
	}
	return false;
}



igdeGDCNavigationSpace *meHelpers::FindFirstNavigationSpace( const igdeGDClass *gdclass ){
	return gdclass ? FindFirstNavigationSpace( *gdclass ) : NULL;
}

igdeGDCNavigationSpace *meHelpers::FindFirstNavigationSpace( const igdeGDClass &gdclass ){
	igdeGDCNavigationSpace *navigationSpace = NULL;
	decString prefix;
	return FindFirstNavigationSpace( gdclass, prefix, navigationSpace ) ? navigationSpace : NULL;
}

bool meHelpers::FindFirstNavigationSpace( const igdeGDClass &gdclass, igdeGDCNavigationSpace* &navigationSpace ){
	decString prefix;
	return FindFirstNavigationSpace( gdclass, prefix, navigationSpace );
}

bool meHelpers::FindFirstNavigationSpace( const igdeGDClass &gdclass, decString &prefix, igdeGDCNavigationSpace* &navigationSpace ){
	if( gdclass.GetNavigationSpaceList().GetCount() > 0 ){
		navigationSpace = gdclass.GetNavigationSpaceList().GetAt( 0 );
		prefix.Empty();
		return true;
	}
	const int count = gdclass.GetInheritClassCount();
	for( int i=0; i<count; i++ ){
		const igdeGDClassInherit &inherit = *gdclass.GetInheritClassAt( i );
		if( inherit.GetClass() && meHelpers::FindFirstNavigationSpace( *inherit.GetClass(), prefix, navigationSpace ) ){
			prefix = inherit.GetPropertyPrefix() + prefix;
			return true;
		}
	}
	return false;
}



igdeGDCNavigationBlocker *meHelpers::FindFirstNavigationBlocker( const igdeGDClass *gdclass ){
	return gdclass ? FindFirstNavigationBlocker( *gdclass ) : NULL;
}

igdeGDCNavigationBlocker *meHelpers::FindFirstNavigationBlocker( const igdeGDClass &gdclass ){
	igdeGDCNavigationBlocker *navigationBlocker = NULL;
	decString prefix;
	return FindFirstNavigationBlocker( gdclass, prefix, navigationBlocker ) ? navigationBlocker : NULL;
}

bool meHelpers::FindFirstNavigationBlocker( const igdeGDClass &gdclass, igdeGDCNavigationBlocker* &navigationBlocker ){
	decString prefix;
	return FindFirstNavigationBlocker( gdclass, prefix, navigationBlocker );
}

bool meHelpers::FindFirstNavigationBlocker( const igdeGDClass &gdclass, decString &prefix, igdeGDCNavigationBlocker* &navigationBlocker ){
	if( gdclass.GetNavigationBlockerList().GetCount() > 0 ){
		navigationBlocker = gdclass.GetNavigationBlockerList().GetAt( 0 );
		prefix.Empty();
		return true;
	}
	const int count = gdclass.GetInheritClassCount();
	for( int i=0; i<count; i++ ){
		const igdeGDClassInherit &inherit = *gdclass.GetInheritClassAt( i );
		if( inherit.GetClass() && meHelpers::FindFirstNavigationBlocker( *inherit.GetClass(), prefix, navigationBlocker ) ){
			prefix = inherit.GetPropertyPrefix() + prefix;
			return true;
		}
	}
	return false;
}



void meHelpers::GetPatternTypePropertyNames( const igdeGDClass &gdclass,
igdeGDProperty::ePathPatternTypes patternType, decStringList &names ){
	decStringSet allPropNames;
	gdclass.AddPropertyNames( allPropNames, true );
	
	const int allPropNameCount = allPropNames.GetCount();
	int i;
	
	for( i=0; i<allPropNameCount; i++ ){
		const igdeGDProperty &gdprop = *gdclass.GetPropertyNamed( allPropNames.GetAt( i ) );
		if( gdprop.GetType() == igdeGDProperty::eptPath && gdprop.GetPathPatternType() == patternType ){
			names.Add( allPropNames.GetAt( i ) );
		}
	}
}



void meHelpers::CreateTexture( deObjectReference &texture, meObject *object, const char *textureName ){
	const igdeGDCComponent * const gdcomponent = meHelpers::FindFirstComponent( object->GetGDClass() );
	const igdeGDCCTexture * gdctexture = gdcomponent ? gdcomponent->GetTextureList().GetNamed( textureName ) : NULL;
	meHelpers::CreateTexture( texture, object, textureName, gdctexture );
}

void meHelpers::CreateTexture( deObjectReference &texture, meObject *object,
const char *textureName, const igdeGDCCTexture *gdctexture ){
	decColor initialColor( 1.0f, 1.0f, 1.0f );
	decVector2 initialTexCoordOffset( 0.0f, 0.0f );
	decVector2 initialTexCoordScale( 1.0f, 1.0f );
	float initialTexCoordRotation( 0.0f );
	decString initialSkin;
	
	if( gdctexture ){
		initialSkin = gdctexture->GetPathSkin();
		initialColor = gdctexture->GetColorTint();
		initialTexCoordOffset = gdctexture->GetOffset();
		initialTexCoordScale = gdctexture->GetScale();
		initialTexCoordRotation = gdctexture->GetRotation();
	}
	
	texture.TakeOver( new meObjectTexture( object->GetEnvironment(), textureName ) );
	meObjectTexture &realTexture = ( meObjectTexture& )( deObject& )texture;
	realTexture.SetSkinPath( initialSkin );
	realTexture.SetColorTint( initialColor );
	realTexture.SetTexCoordOffset( initialTexCoordOffset );
	realTexture.SetTexCoordScaling( initialTexCoordScale );
	realTexture.SetTexCoordRotation( initialTexCoordRotation );
}
