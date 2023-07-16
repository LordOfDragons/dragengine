/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "deoglSkinStateCNText.h"
#include "../deoglSkinState.h"
#include "../deoglSkinStateMapped.h"
#include "../../../font/deoglFont.h"

#include <dragengine/common/exceptions.h>


// Class deoglSkinStateCNText
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinStateCNText::deoglSkinStateCNText( deSkinPropertyNodeText &node ) :
deoglSkinStateConstructedNode( node, etText ),
pFont( node.GetFont() && node.GetFont()->GetPeerGraphic()
	? ( ( deoglFont* )node.GetFont()->GetPeerGraphic() )->GetRFont() : nullptr ),
pFontSize( node.GetFontSize() ),
pText( node.GetText() ),
pColor( node.GetColor() )
{
	int i;
	for( i=0; i<deSkinPropertyNodeText::TextMappedCount; i++ ){
		pTextMapped[ i ] = node.GetTextMappedFor( ( deSkinPropertyNodeText::eTextMapped )i );
	}
}

deoglSkinStateCNText::deoglSkinStateCNText( const deoglSkinStateCNText &node ) :
deoglSkinStateConstructedNode( node ),
pFont( node.pFont ),
pFontSize( node.pFontSize ),
pText( node.pText ),
pColor( node.pColor )
{
	int i;
	for( i=0; i<deSkinPropertyNodeText::TextMappedCount; i++ ){
		pTextMapped[ i ] = node.pTextMapped[ i ];
	}
}

deoglSkinStateCNText::~deoglSkinStateCNText(){
}



// Management
///////////////

int deoglSkinStateCNText::GetTextMappedFor( deSkinPropertyNodeText::eTextMapped type ) const{
	return pTextMapped[ type ];
}



void deoglSkinStateCNText::Update( deoglSkinState &state ){
	if( pTextMapped[ deSkinPropertyNodeText::etmFontSize ] != -1 ){
		pFontSize = state.GetMappedAt( pTextMapped[ deSkinPropertyNodeText::etmFontSize ] ).GetValue();
	}
	if( pTextMapped[ deSkinPropertyNodeText::etmColorRed ] != -1 ){
		pColor.r = state.GetMappedAt( pTextMapped[ deSkinPropertyNodeText::etmColorRed ] ).GetValue();
	}
	if( pTextMapped[ deSkinPropertyNodeText::etmColorGreen ] != -1 ){
		pColor.g = state.GetMappedAt( pTextMapped[ deSkinPropertyNodeText::etmColorGreen ] ).GetValue();
	}
	if( pTextMapped[ deSkinPropertyNodeText::etmColorBlue ] != -1 ){
		pColor.b = state.GetMappedAt( pTextMapped[ deSkinPropertyNodeText::etmColorBlue ] ).GetValue();
	}
}

deoglSkinStateConstructedNode::Ref deoglSkinStateCNText::Copy() const{
	return deoglSkinStateConstructedNode::Ref::New( new deoglSkinStateCNText( *this ) );
}
