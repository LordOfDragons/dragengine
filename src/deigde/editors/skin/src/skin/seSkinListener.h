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

#ifndef _SESKINLISTENER_H_
#define _SESKINLISTENER_H_

#include <dragengine/deObject.h>

class seDynamicSkinRenderable;
class seProperty;
class sePropertyNode;
class seSkin;
class seTexture;
class seMapped;



/**
 * Skin listener.
 */
class seSkinListener : public deObject{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin listener. */
	seSkinListener();
	
	/** Clean up listener. */
	virtual ~seSkinListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Changed or saved state changed. */
	virtual void StateChanged( seSkin *skin );
	
	/** Undo changed. */
	virtual void UndoChanged( seSkin *skin );
	
	/** Skin parameters changed. */
	virtual void SkinChanged( seSkin *skin );
	
	/** The sky changed. */
	virtual void SkyChanged( seSkin *skin );
	
	/** The environment object changed. */
	virtual void EnvObjectChanged( seSkin *skin );
	
	/** The view changed. */
	virtual void ViewChanged( seSkin *skin );
	
	/** The camera changed. */
	virtual void CameraChanged( seSkin *skin );
	
	
	
	/** Mapped have been added or removed. */
	virtual void MappedStructureChanged( seSkin *skin );
	
	/** Mapped changed. */
	virtual void MappedChanged( seSkin *skin, seMapped *mapped );
	
	/** Mapped name changed. */
	virtual void MappedNameChanged( seSkin *skin, seMapped *mapped );
	
	/** Active mapped changed. */
	virtual void ActiveMappedChanged( seSkin *skin );
	
	
	
	/** Textures have been added or removed. */
	virtual void TextureStructureChanged( seSkin *skin );
	
	/** Texture changed. */
	virtual void TextureChanged( seSkin *skin, seTexture *texture );
	
	/** Texture name changed. */
	virtual void TextureNameChanged( seSkin *skin, seTexture *texture );
	
	/** Active texture changed. */
	virtual void ActiveTextureChanged( seSkin *skin );
	
	
	
	/** Properties have been added or removed. */
	virtual void PropertyStructureChanged ( seSkin *skin, seTexture *texture );
	
	/** Property changed. */
	virtual void PropertyChanged( seSkin *skin, seTexture *texture, seProperty *property );
	
	/** Active property changed. */
	virtual void ActivePropertyChanged( seSkin *skin, seTexture *texture );
	
	/** Property node structre changed. */
	virtual void PropertyNodeStructureChanged( seSkin *skin, seTexture *texture, seProperty *property );
	
	/** Property node changed. */
	virtual void PropertyNodeChanged( seSkin *skin, seTexture *texture, seProperty *property, sePropertyNode *node );
	
	/** Active property node changed. */
	virtual void PropertyActiveNodeChanged( seSkin *skin, seTexture *texture, seProperty *property );
	
	/** Property selected nodes changed. */
	virtual void PropertyNodeSelectionChanged( seSkin *skin, seTexture *texture, seProperty *property );
	
	/** Active property node group changed. */
	virtual void PropertyActiveNodeGroupChanged( seSkin *skin, seTexture *texture, seProperty *property );
	
	/** Active property node layer changed. */
	virtual void PropertyActiveNodeLayerChanged( seSkin *skin, seTexture *texture, seProperty *property );
	
	
	
	/** Dynamic skin renderables have been added or removed. */
	virtual void DynamicSkinRenderableStructureChanged ( seSkin *skin );
	
	/** Dynamic skin renderable changed. */
	virtual void DynamicSkinRenderableChanged( seSkin *skin, seDynamicSkinRenderable *renderable );
	
	/** Dynamic skin renderable name changed. */
	virtual void DynamicSkinRenderableNameChanged( seSkin *skin, seDynamicSkinRenderable *renderable );
	
	/** Active dynamic skin renderable changed. */
	virtual void DynamicSkinActiveRenderableChanged( seSkin *skin );
	/*@}*/
};

#endif
