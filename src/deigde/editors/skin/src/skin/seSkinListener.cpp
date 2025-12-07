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

#include "seSkinListener.h"



// Class seSkinListener
/////////////////////////

// Constructor, destructor
////////////////////////////

seSkinListener::seSkinListener(){
}

seSkinListener::~seSkinListener(){
}



// Notifications
//////////////////

void seSkinListener::StateChanged(seSkin*){
}

void seSkinListener::UndoChanged(seSkin*){
}

void seSkinListener::SkinChanged(seSkin*){
}

void seSkinListener::SkyChanged(seSkin*){
}

void seSkinListener::EnvObjectChanged(seSkin*){
}

void seSkinListener::ViewChanged(seSkin*){
}

void seSkinListener::CameraChanged(seSkin*){
}



void seSkinListener::MappedStructureChanged(seSkin*){
}

void seSkinListener::MappedChanged(seSkin*, seMapped*){
}

void seSkinListener::MappedNameChanged(seSkin *skin, seMapped *mapped){
	MappedChanged(skin, mapped);
}

void seSkinListener::ActiveMappedChanged(seSkin*){
}



void seSkinListener::TextureStructureChanged(seSkin*){
}

void seSkinListener::TextureChanged(seSkin*, seTexture*){
}

void seSkinListener::TextureNameChanged(seSkin *skin, seTexture *texture){
	TextureChanged(skin, texture);
}

void seSkinListener::ActiveTextureChanged(seSkin*){
}



void seSkinListener::PropertyStructureChanged(seSkin*, seTexture*){
}

void seSkinListener::PropertyChanged(seSkin*, seTexture*, seProperty*){
}

void seSkinListener::ActivePropertyChanged(seSkin*, seTexture*){
}

void seSkinListener::PropertyNodeStructureChanged(seSkin*, seTexture*, seProperty*){
}

void seSkinListener::PropertyNodeChanged(seSkin*, seTexture*, seProperty*, sePropertyNode*){
}

void seSkinListener::PropertyActiveNodeChanged(seSkin*, seTexture*, seProperty*){
}

void seSkinListener::PropertyNodeSelectionChanged(seSkin*, seTexture*, seProperty*){
}

void seSkinListener::PropertyActiveNodeGroupChanged(seSkin*, seTexture*, seProperty*){
}

void seSkinListener::PropertyActiveNodeLayerChanged(seSkin*, seTexture*, seProperty*){
}



void seSkinListener::DynamicSkinRenderableStructureChanged(seSkin*){
}

void seSkinListener::DynamicSkinRenderableChanged(seSkin*, seDynamicSkinRenderable*){
}

void seSkinListener::DynamicSkinRenderableNameChanged(seSkin*, seDynamicSkinRenderable*){
}

void seSkinListener::DynamicSkinActiveRenderableChanged(seSkin*){
}
