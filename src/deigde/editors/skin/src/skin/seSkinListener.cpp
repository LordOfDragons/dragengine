/* 
 * Drag[en]gine IGDE Skin Editor
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

void seSkinListener::StateChanged( seSkin* ){
}

void seSkinListener::UndoChanged( seSkin* ){
}

void seSkinListener::SkinChanged( seSkin* ){
}

void seSkinListener::SkyChanged( seSkin* ){
}

void seSkinListener::EnvObjectChanged( seSkin* ){
}

void seSkinListener::ViewChanged( seSkin* ){
}

void seSkinListener::CameraChanged( seSkin* ){
}



void seSkinListener::TextureStructureChanged( seSkin* ){
}

void seSkinListener::TextureChanged( seSkin*, seTexture* ){
}

void seSkinListener::TextureNameChanged( seSkin *skin, seTexture *texture ){
	TextureChanged( skin, texture );
}

void seSkinListener::ActiveTextureChanged( seSkin* ){
}



void seSkinListener::PropertyStructureChanged( seSkin*, seTexture* ){
}

void seSkinListener::PropertyChanged( seSkin*, seTexture*, seProperty* ){
}

void seSkinListener::ActivePropertyChanged( seSkin*, seTexture* ){
}

void seSkinListener::PropertyNodeStructureChanged( seSkin*, seTexture*, seProperty* ){
}

void seSkinListener::PropertyNodeChanged( seSkin*, seTexture*, seProperty*, sePropertyNode* ){
}

void seSkinListener::PropertyActiveNodeChanged( seSkin*, seTexture*, seProperty* ){
}

void seSkinListener::PropertyNodeSelectionChanged( seSkin*, seTexture*, seProperty* ){
}

void seSkinListener::PropertyActiveNodeGroupChanged( seSkin*, seTexture*, seProperty* ){
}

void seSkinListener::PropertyActiveNodeLayerChanged( seSkin*, seTexture*, seProperty* ){
}



void seSkinListener::DynamicSkinRenderableStructureChanged( seSkin* ){
}

void seSkinListener::DynamicSkinRenderableChanged( seSkin*, seDynamicSkinRenderable* ){
}

void seSkinListener::DynamicSkinRenderableNameChanged( seSkin*, seDynamicSkinRenderable* ){
}

void seSkinListener::DynamicSkinActiveRenderableChanged( seSkin* ){
}
