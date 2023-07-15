/* 
 * Drag[en]gine IGDE Skin Editor
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

#include "sePropertyNodeText.h"
#include "../seProperty.h"
#include "../../seSkin.h"
#include "../../texture/seTexture.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontManager.h>



// Class sePropertyNodeText
/////////////////////////////

// Constructor, destructor
////////////////////////////

sePropertyNodeText::sePropertyNodeText( deEngine &engine ) :
sePropertyNode( entText, engine, TextMappedCount ),
pFont( NULL ),
pFontSize( 10.0f ){
}

sePropertyNodeText::sePropertyNodeText( const sePropertyNodeText &node ) :
sePropertyNode( node ),
pPath( node.pPath ),
pFont( NULL ),
pFontSize( node.pFontSize ),
pText( node.pText ),
pColor( node.pColor )
{
	pFont = node.pFont;
	if( pFont ){
		pFont->AddReference();
	}
}

sePropertyNodeText::~sePropertyNodeText(){
	if( pFont ){
		pFont->FreeReference();
	}
}



// Management
///////////////

void sePropertyNodeText::SetPath( const char *path ){
	if( pPath.Equals( path ) ){
		return;
	}
	
	pPath = path;
	UpdateFont();
	NotifyChanged();
}

void sePropertyNodeText::SetFontSize( float size ){
	size = decMath::max( size, 0.0f );
	if( fabsf( size - pFontSize ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pFontSize = size;
	NotifyChanged();
}

void sePropertyNodeText::UpdateFont(){
	deFont *font = NULL;
	
	if( ! pPath.IsEmpty() && GetProperty() && GetProperty()->GetTexture() && GetProperty()->GetTexture()->GetSkin() ){
		const decString &basePath = GetProperty()->GetTexture()->GetSkin()->GetDirectoryPath();
		
		try{
			font = GetEngine().GetFontManager()->LoadFont( pPath, basePath );
			
		}catch( const deException &e ){
			GetProperty()->GetTexture()->GetSkin()->GetLogger()->LogException( "Skin Editor", e );
		}
	}
	
	if( font == pFont ){
		if( font ){
			font->FreeReference();
		}
		return;
	}
	
	if( pFont ){
		pFont->FreeReference();
	}
	pFont = font;
}

void sePropertyNodeText::SetText( const char *text ){
	if( pText == text ){
		return;
	}
	
	pText = text;
	NotifyChanged();
}

void sePropertyNodeText::SetColor( const decColor &color ){
	if( color.IsEqualTo( pColor ) ){
		return;
	}
	
	pColor = color;
	NotifyChanged();
}



sePropertyNode *sePropertyNodeText::Copy() const{
	return new sePropertyNodeText( *this );
}

void sePropertyNodeText::UpdateResources(){
	sePropertyNode::UpdateResources();
	UpdateFont();
}
