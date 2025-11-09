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
pTextSize( 10.0f ){
}

sePropertyNodeText::sePropertyNodeText( const sePropertyNodeText &node ) :
sePropertyNode( node ),
pPath( node.pPath ),
pFont(node.pFont),
pFontSize(node.pFontSize),
pTextSize( node.pTextSize ),
pText( node.pText ),
pColor( node.pColor ){
}

sePropertyNodeText::~sePropertyNodeText(){
}



// Management
///////////////

void sePropertyNodeText::SetPath( const char *path ){
	if(pPath == path){
		return;
	}
	
	pPath = path;
	UpdateFont();
	UpdateFontSize();
	NotifyChanged();
}

void sePropertyNodeText::SetTextSize( float size ){
	size = decMath::max(size, 0.0f);
	if(fabsf(size - pTextSize) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pTextSize = size;
	UpdateFontSize();
	NotifyChanged();
}

void sePropertyNodeText::UpdateFont(){
	pFontSize = nullptr;
	
	if(!pPath.IsEmpty() && GetProperty() && GetProperty()->GetTexture()
	&& GetProperty()->GetTexture()->GetSkin()){
		const decString &basePath = GetProperty()->GetTexture()->GetSkin()->GetDirectoryPath();
		
		try{
			pFont.TakeOver(GetEngine().GetFontManager()->LoadFont(pPath, basePath));
			
		}catch(const deException &e){
			GetProperty()->GetTexture()->GetSkin()->GetLogger()->LogException("Skin Editor", e);
		}
	}
}

void sePropertyNodeText::UpdateFontSize(){
	if(!pFont){
		pFontSize = nullptr;
		return;
	}
	
	try{
		pFontSize = pFont->PrepareSize((int)(pTextSize + 0.5f));
		
	}catch(const deException &e){
		GetProperty()->GetTexture()->GetSkin()->GetLogger()->LogException("Skin Editor", e);
	}
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
