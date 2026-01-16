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

#include <string.h>

#include "deSkin.h"
#include "deSkinTexture.h"
#include "deSkinManager.h"
#include "deSkinMapped.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/graphic/deBaseGraphicSkin.h"
#include "../../systems/modules/audio/deBaseAudioSkin.h"
#include "../../systems/modules/physics/deBasePhysicsSkin.h"



// Class deSkin
/////////////////

// Constructor, destructor
////////////////////////////

deSkin::deSkin(deSkinManager *manager, deVirtualFileSystem *vfs, const char *filename,
	TIME_SYSTEM modificationTime) :
deFileResource(manager, vfs, filename, modificationTime),

pPeerGraphic(nullptr),
pPeerAudio(nullptr),
pPeerPhysics(nullptr){
}

deSkin::~deSkin(){
	pCleanUp();
}



// Textures
/////////////

void deSkin::AddTexture(deSkinTexture::Ref &&tex){
	DEASSERT_NOTNULL(tex)
	pTextures.Add(std::move(tex));
}

const deSkinTexture::Ref &deSkin::GetTextureAt(int index) const{
	return pTextures.GetAt(index);
}

int deSkin::IndexOfTextureNamed(const char *name) const{
	DEASSERT_NOTNULL(name)
	return pTextures.IndexOfMatching([&](const deSkinTexture &tex){
		return tex.GetName() == name;
	});
}



// Mapped Values
//////////////////

void deSkin::AddMapped(deSkinMapped *mapped){
	DEASSERT_NOTNULL(mapped)
	pMapped.AddOrThrow(mapped);
}



// System Peers
/////////////////

void deSkin::SetPeerGraphic(deBaseGraphicSkin *peer){
	if(peer != pPeerGraphic){
		if(pPeerGraphic) delete pPeerGraphic;
		pPeerGraphic = peer;
	}
}

void deSkin::SetPeerAudio(deBaseAudioSkin *peer){
	if(peer != pPeerAudio){
		if(pPeerAudio) delete pPeerAudio;
		pPeerAudio = peer;
	}
}

void deSkin::SetPeerPhysics(deBasePhysicsSkin *peer){
	if(peer != pPeerPhysics){
		if(pPeerPhysics) delete pPeerPhysics;
		pPeerPhysics = peer;
	}
}



// Private functions
//////////////////////

void deSkin::pCleanUp(){
	if(pPeerPhysics){
		delete pPeerPhysics;
	}
	if(pPeerAudio){
		delete pPeerAudio;
	}
	if(pPeerGraphic){
		delete pPeerGraphic;
	}
}
