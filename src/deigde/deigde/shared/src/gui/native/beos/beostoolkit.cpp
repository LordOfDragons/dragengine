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

#ifdef IGDE_TOOLKIT_BEOS

#include "beostoolkit.h"
#include "../../igdeContainer.h"

#include <dragengine/common/exceptions.h>



// Class igdeUIBeOSHelper
///////////////////////////

igdeUIBeOSHelper::igdeUIBeOSHelper(){}



// Management
///////////////

void igdeUIBeOSHelper::AddView( BView *view, igdeContainer *parent ){
	if( ! view ){
		DETHROW( deeInvalidParam );
	}
	if( ! parent ){
		delete view;
		DETHROW( deeInvalidParam );
	}
	
	AddView( view, ( BView* )parent->GetNativeContainer() );
}

void igdeUIBeOSHelper::AddView( BView *view, BView *parent ){
	if( ! view ){
		DETHROW( deeInvalidParam );
	}
	if( ! parent || parent->Perform( epcAddView, view ) != B_OK ){
		delete view;
		DETHROW( deeInvalidParam );
	}
}

void igdeUIBeOSHelper::DestroyView( BView *view ){
	if( ! view || ! view->RemoveSelf() ){
		DETHROW( deeInvalidParam );
	}
	delete view;
}

rgb_color igdeUIBeOSHelper::BlendColor( rgb_color color1, rgb_color color2, float factor ){
	const float c1r = ( float )color1.red / 255.f;
	const float c1g = ( float )color1.green / 255.f;
	const float c1b = ( float )color1.blue / 255.f;
	
	const float c2r = ( float )color2.red / 255.f;
	const float c2g = ( float )color2.green / 255.f;
	const float c2b = ( float )color2.blue / 255.f;
	
	const uint8 c3r = ( uint8 )decMath::clamp( ( int )( decMath::mix( c1r, c2r, factor ) * 255.0f ), 0, 255 );
	const uint8 c3g = ( uint8 )decMath::clamp( ( int )( decMath::mix( c1g, c2g, factor ) * 255.0f ), 0, 255 );
	const uint8 c3b = ( uint8 )decMath::clamp( ( int )( decMath::mix( c1b, c2b, factor ) * 255.0f ), 0, 255 );
	
	return rgb_color{ c3r, c3g, c3b, 255 };
}

rgb_color igdeUIBeOSHelper::ConvertColor( const decColor &color ){
	const uint8 r = ( uint8 )decMath::clamp( ( int )( color.r * 255.0f ), 0, 255 );
	const uint8 g = ( uint8 )decMath::clamp( ( int )( color.g * 255.0f ), 0, 255 );
	const uint8 b = ( uint8 )decMath::clamp( ( int )( color.b * 255.0f ), 0, 255 );
	return rgb_color{ r, g, b, 255 };
}

decColor igdeUIBeOSHelper::ConvertColor( rgb_color color ){
	return decColor( ( float )color.red / 255.f, ( float )color.green / 255.f, ( float )color.blue / 255.f );
}

rgb_color igdeUIBeOSHelper::BrighterColor( rgb_color color, int amount ){
	const uint8 r = ( uint8 )decMath::min( ( int )color.red + amount, 255 );
	const uint8 g = ( uint8 )decMath::min( ( int )color.green + amount, 255 );
	const uint8 b = ( uint8 )decMath::min( ( int )color.blue + amount, 255 );
	return rgb_color{ r, g, b };
}

#endif
