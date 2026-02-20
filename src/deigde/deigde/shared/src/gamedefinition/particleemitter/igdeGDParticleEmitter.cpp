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

#include "igdeGDParticleEmitter.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>



// Class igdeGDParticleEmitter
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDParticleEmitter::igdeGDParticleEmitter(const char *path, const char *name) :
pPath(path),
pName(name){
}

igdeGDParticleEmitter::igdeGDParticleEmitter(const igdeGDParticleEmitter &particleEmitter) :
pPath(particleEmitter.pPath),
pName(particleEmitter.pName),
pDescription(particleEmitter.pDescription),
pCategory(particleEmitter.pCategory){
}

igdeGDParticleEmitter::~igdeGDParticleEmitter(){
}



// Management
///////////////

void igdeGDParticleEmitter::SetPath(const char *path){
	pPath = path;
}

void igdeGDParticleEmitter::SetName(const char *name){
	pName = name;
}

void igdeGDParticleEmitter::SetDescription(const char *description){
	pDescription = description;
}

void igdeGDParticleEmitter::SetCategory(const char *category){
	pCategory = category;
}

deImage *igdeGDParticleEmitter::GetPreviewImage(int size) const{
	if(size < 0 || size > 2){
		DETHROW(deeInvalidParam);
	}
	return pPreviewImages[size];
}

void igdeGDParticleEmitter::SetPreviewImage(int size, deImage *image){
	if(size < 0 || size > 2){
		DETHROW(deeInvalidParam);
	}
	pPreviewImages[size] = image;
}
