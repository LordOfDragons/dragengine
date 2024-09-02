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

#include "deoglSkinStateCNText.h"
#include "../deoglSkinState.h"
#include "../deoglSkinStateMapped.h"
#include "../../../font/deoglFont.h"
#include "../../../rendering/deoglRenderCanvasContext.h"
#include "../../../rendering/deoglRenderConstructed.h"
#include "../../../renderthread/deoglRenderThread.h"
#include "../../../renderthread/deoglRTRenderers.h"

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
	if( pText.IsEmpty() || ! pFont || pFontSize <= 0.01f ){
		return;
	}
	
	deoglSkinStateConstructedNode::Update( state );
	
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

void deoglSkinStateCNText::Render( deoglSkinState &state, const deoglRenderCanvasContext &context ){
	if( pText.IsEmpty() || ! pFont || pFontSize <= 0.01f ){
		return;
	}
	
	deoglSkinStateConstructedNode::Render( state, context );
	
	const deoglRenderCanvasContext textContext( context, *this );
	state.GetRenderThread().GetRenderers().GetConstructed().DrawNodeText( textContext, *this );
}

deoglSkinStateConstructedNode::Ref deoglSkinStateCNText::Copy() const{
	return deoglSkinStateConstructedNode::Ref::New( new deoglSkinStateCNText( *this ) );
}
