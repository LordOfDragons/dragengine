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
#include <stdlib.h>
#include <string.h>

#include "deoglDelayedSaveImage.h"
#include "../deGraphicOpenGl.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/systems/modules/image/deBaseImageModule.h>
#include <dragengine/systems/deModuleSystem.h>



// Class deoglDelayedSaveImage
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDelayedSaveImage::deoglDelayedSaveImage( const decPath &path, int width,
int height, int depth, int componentCount, int bitCount, char *data ) :
pPath( path ),
pWidth( width ),
pHeight( height ),
pDepth( depth ),
pComponentCount( componentCount ),
pBitCount( bitCount ),
pData( data ){
	if( ! data ){
		DETHROW( deeInvalidParam );
	}
}

deoglDelayedSaveImage::~deoglDelayedSaveImage(){
	if( pData ){
		delete [] pData;
	}
}



// Management
///////////////

void deoglDelayedSaveImage::SaveImage( deGraphicOpenGl &ogl, deVirtualFileSystem &vfs ){
	// NOTE this call is used only for debug images created by deoglDebugSaveTexture.
	//      for this reason no optimization with parallel tasks is used
	deImageManager &imgmgr = *ogl.GetGameEngine()->GetImageManager();
	const decString filename( pPath.GetPathUnix() );
	decBaseFileWriter *fileWriter = NULL;
	decPath noConstPath( pPath );
	deImage *image = NULL;
	
	try{
		deBaseImageModule * const module = ( deBaseImageModule* )ogl.GetGameEngine()->
			GetModuleSystem()->GetModuleAbleToLoad( deModuleSystem::emtImage, filename );
		if( ! module ){
			DETHROW( deeInvalidParam );
		}
		
		image = imgmgr.CreateImage( pWidth, pHeight, pDepth, pComponentCount, pBitCount );
		memcpy( image->GetData(), pData, pWidth * pHeight * pDepth * pComponentCount * ( pBitCount >> 3 ) );
		fileWriter = vfs.OpenFileForWriting( noConstPath );
		module->SaveImage( *fileWriter, *image );
		
		fileWriter->FreeReference();
		fileWriter = NULL;
		
		image->FreeReference();
		
	}catch( const deException & ){
		if( fileWriter ){
			fileWriter->FreeReference();
		}
		if( image ){
			image->FreeReference();
		}
		throw;
	}
}
