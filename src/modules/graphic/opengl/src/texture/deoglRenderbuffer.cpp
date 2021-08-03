/* 
 * Drag[en]gine OpenGL Graphic Module
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
#include <string.h>
#include <math.h>

#include "deoglRenderbuffer.h"
#include "../capabilities/deoglCapabilities.h"
#include "../capabilities/deoglCapsTextureFormat.h"
#include "../memory/deoglMemoryManager.h"
#include "../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderbuffer
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderbuffer::deoglRenderbuffer( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pRenderbuffer( 0 ),
pFormat( GL_RGB8 ),
pWidth( 1 ),
pHeight( 1 ),
pBitsPerPixel( 8 ),
pIsDepth( false ),
pMemUse( renderThread.GetMemoryManager().GetConsumption().renderbuffer ){
}

deoglRenderbuffer::~deoglRenderbuffer(){
	pCleanUp();
	
	pRenderbuffer = 0;
}



// Management
///////////////

void deoglRenderbuffer::SetSize( int width, int height ){
	if( width < 1 || height < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( width == pWidth && height == pHeight ){
		return;
	}
	
	DestroyBuffer();
	
	pWidth = width;
	pHeight = height;
}

void deoglRenderbuffer::SetFormat( GLint format ){
	if( format != pFormat ){
		pFormat = format;
		DestroyBuffer();
	}
}

void deoglRenderbuffer::CreateBuffer(){
	if( ! pRenderbuffer ){
		OGL_CHECK( pRenderThread, pglGenRenderbuffers( 1, &pRenderbuffer ) );
		if( pRenderbuffer == 0 ) DETHROW( deeOutOfMemory );
	}
	
	Activate();
	OGL_CHECK( pRenderThread, pglRenderbufferStorage( GL_RENDERBUFFER, pFormat, pWidth, pHeight ) );
	Deactivate();
	
	UpdateMemoryUsage();
}

void deoglRenderbuffer::DestroyBuffer(){
}

void deoglRenderbuffer::Activate(){
	OGL_CHECK( pRenderThread, pglBindRenderbuffer( GL_RENDERBUFFER, pRenderbuffer ) );
}

void deoglRenderbuffer::Deactivate(){
	OGL_CHECK( pRenderThread, pglBindRenderbuffer( GL_RENDERBUFFER, 0 ) );
}



void deoglRenderbuffer::UpdateMemoryUsage(){
	pMemUse.Clear();
	
	if( ! pRenderbuffer ){
		return;
	}
	
	int baseSize = pWidth * pHeight * ( pBitsPerPixel >> 3 );
	if( ( pBitsPerPixel & 0x7 ) > 0 ){
		baseSize >>= 1;
	}
	
	if( pIsDepth ){
		pMemUse.depth = baseSize;
		
	}else{
		pMemUse.color = baseSize;
	}
}



// Helper Functions
/////////////////////

void deoglRenderbuffer::SetFormatFromCaps( const deoglCapsTextureFormat &format ){
	pFormat = format.GetFormat();
	pBitsPerPixel = format.GetBitsPerPixel();
}

void deoglRenderbuffer::SetColorFormat( int bpp, int channels, bool useFloat ){
	deoglCapabilities &caps = pRenderThread.GetCapabilities();
	
	if( bpp == 8 ){
		if( channels == 1 ){
			if( useFloat ){
				SetFormatFromCaps( *caps.GetFormats().GetUseRenBufFormatFor( deoglCapsFmtSupport::eutfR16F ) );
				
			}else{
				SetFormatFromCaps( *caps.GetFormats().GetUseRenBufFormatFor( deoglCapsFmtSupport::eutfR8 ) );
			}
			
		}else if( channels == 2 ){
			if( useFloat ){
				SetFormatFromCaps( *caps.GetFormats().GetUseRenBufFormatFor( deoglCapsFmtSupport::eutfRG16F ) );
				
			}else{
				SetFormatFromCaps( *caps.GetFormats().GetUseRenBufFormatFor( deoglCapsFmtSupport::eutfRG8 ) );
			}
			
		}else if( channels == 3 ){
			if( useFloat ){
				SetFormatFromCaps( *caps.GetFormats().GetUseRenBufFormatFor( deoglCapsFmtSupport::eutfRGB16F ) );
				
			}else{
				SetFormatFromCaps( *caps.GetFormats().GetUseRenBufFormatFor( deoglCapsFmtSupport::eutfRGB8 ) );
			}
			
		}else if( channels == 4 ){
			if( useFloat ){
				SetFormatFromCaps( *caps.GetFormats().GetUseRenBufFormatFor( deoglCapsFmtSupport::eutfRGBA16F ) );
				
			}else{
				SetFormatFromCaps( *caps.GetFormats().GetUseRenBufFormatFor( deoglCapsFmtSupport::eutfRGBA8 ) );
			}
			
		}else{
			DETHROW( deeInvalidParam );
		}
		
	}else{
		DETHROW( deeInvalidParam );
	}
	
	pIsDepth = false;
	DestroyBuffer();
}

void deoglRenderbuffer::SetDepthFormat( bool packedStencil ){
	deoglCapabilities &caps = pRenderThread.GetCapabilities();
	
	if( packedStencil ){
		SetFormatFromCaps( *caps.GetFormats().GetUseRenBufFormatFor( deoglCapsFmtSupport::eutfDepth_Stencil ) );
		
	}else{
		SetFormatFromCaps( *caps.GetFormats().GetUseRenBufFormatFor( deoglCapsFmtSupport::eutfDepth ) );
	}
	
	pIsDepth = true;
	DestroyBuffer();
}

void deoglRenderbuffer::SetStencilFormat(){
	deoglCapabilities &caps = pRenderThread.GetCapabilities();
	
	SetFormatFromCaps( *caps.GetFormats().GetUseRenBufFormatFor( deoglCapsFmtSupport::eutfStencil ) );
	
	pIsDepth = true;
	DestroyBuffer();
}



// Private Functions
//////////////////////

void deoglRenderbuffer::pCleanUp(){
	if( pRenderbuffer ){
		pglDeleteRenderbuffers( 1, &pRenderbuffer );
	}
}
