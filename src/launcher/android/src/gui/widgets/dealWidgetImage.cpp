/* 
 * Drag[en]gine Android Launcher
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

#include "dealWidgetImage.h"
#include "../dealDisplay.h"
#include "../dealImage.h"
#include "../dealShader.h"
#include "../../dealLauncher.h"
#include "../../common/exceptions.h"
#include "../../logger/deLogger.h"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealWidgetImage
//////////////////////////

// Constructors, destructors
//////////////////////////////

dealWidgetImage::dealWidgetImage( dealDisplay &display, dealImage *image ) :
dealWidget( display ),
pImage( NULL ),
pColorize( 1.0f, 1.0f, 1.0f ),
pScaling( 1.0f ){
	SetImage( image );
}

dealWidgetImage::~dealWidgetImage(){
	if( pImage ){
		pImage->FreeReference();
	}
}



// Management
///////////////

void dealWidgetImage::SetImage( dealImage *image ){
	if( image == pImage ){
		return;
	}
	
	if( pImage ){
		pImage->FreeReference();
	}
	
	pImage = image;
	
	if( image ){
		image->AddReference();
	}
	
	DirtyParentLayout();
}

void dealWidgetImage::SetColorize( const decColor &colorize ){
	pColorize = colorize;
}

void dealWidgetImage::SetScaling( float scaling ){
	scaling = decMath::max( scaling, 1.0f );
	if( fabsf( scaling - pScaling ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pScaling = scaling;
	
	DirtyParentLayout();
}



decPoint dealWidgetImage::GetMinimumSize(){
	if( GetHasExplicitMinimumSize() ){
		return GetExplicitMinimumSize();
	}
	
	if( ! pImage ){
		return decPoint();
	}
	
	return decPoint( ( int )( ( float )pImage->GetWidth() * pScaling ),
		( int )( ( float )pImage->GetHeight() * pScaling ) );
}



void dealWidgetImage::RenderContent( const sRenderContext &context ){
	if( ! pImage ){
		return;
	}
	
	dealShader &shader = *GetDisplay().GetTexturedShapeShader();
	const decPoint &size = GetSize();
	
	shader.Activate();
	
	ShaderSetTransform( context, shader, 0, 0, size.x - 1, size.y - 1 );
	ShaderSetTCTransform( shader );
	
	if( GetEnabled() ){
		ShaderSetColorMatrix( shader, pColorize );
		
	}else{
		ShaderSetColorMatrixDisabled( shader, pColorize );
	}
	
	ShaderSetGamma( shader, 1.0f );
	ShaderSetClipRect( context, shader );
	ShaderSetTCClamp( shader );
	
	BindTexture( 0, *pImage, true, false );
	
	DrawRectangle();
	
	OGL_CHECK( GetDisplay().GetLauncher(), glBindTexture( GL_TEXTURE_2D, 0 ) );
}
