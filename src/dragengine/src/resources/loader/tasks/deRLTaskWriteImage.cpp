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

#include "deRLTaskWriteImage.h"
#include "../deResourceLoader.h"
#include "../../image/deImage.h"
#include "../../image/deImageManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decBaseFileWriter.h"
#include "../../../common/file/decBaseFileWriter.h"
#include "../../../common/file/decPath.h"
#include "../../../filesystem/deVirtualFileSystem.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/image/deBaseImageModule.h"



// Class deRLTaskWriteImage
/////////////////////////////

// Constructor, destructor
////////////////////////////

deRLTaskWriteImage::deRLTaskWriteImage( deEngine &engine,
deResourceLoader &resourceLoader, deImage *image,
deVirtualFileSystem *vfs, const char *path ) :
deResourceLoaderTask( engine, resourceLoader, vfs, path, deResourceLoader::ertImage ),
pImage( image ),
pSucceeded( false )
{
	if( ! image ){
		DETHROW( deeInvalidParam );
	}
	SetType( etWrite );
}

deRLTaskWriteImage::~deRLTaskWriteImage(){
}



// Management
///////////////

void deRLTaskWriteImage::Run(){
	LogRunEnter();
	deBaseImageModule * const module = ( deBaseImageModule* )GetEngine().
		GetModuleSystem()->GetModuleAbleToLoad( deModuleSystem::emtImage, GetPath() );
	if( ! module ){
		DETHROW( deeInvalidParam );
	}
	
	decPath path;
	path.SetFromUnix( GetPath() );
	
	decBaseFileWriter::Ref writer;
	writer.TakeOver( GetVFS()->OpenFileForWriting( path ) );
	module->SaveImage( writer, pImage );
	
	pSucceeded = true;
	LogRunExit();
}

void deRLTaskWriteImage::Finished(){
	LogFinishedEnter();
	if( pSucceeded ){
		SetResource( pImage );
		SetState( esSucceeded );
		
	}else{
		pImage = NULL;
		SetState( esFailed );
	}
	LogFinishedExit();
	GetResourceLoader().FinishTask( this );
}



// Debugging
//////////////

decString deRLTaskWriteImage::GetDebugName() const{
	return deResourceLoaderTask::GetDebugName() + "-Anim-Write";
}
