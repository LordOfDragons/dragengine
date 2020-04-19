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



/**
 * \brief Skin listener.
 */
class seSkinListener : public deObject{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create skin listener. */
	seSkinListener();
	
	/** \brief Clean up listener. */
	virtual ~seSkinListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Changed or saved state changed. */
	virtual void StateChanged( seSkin *skin );
	
	/** \brief Undo changed. */
	virtual void UndoChanged( seSkin *skin );
	
	/** \brief Skin parameters changed. */
	virtual void SkinChanged( seSkin *skin );
	
	/** \brief The sky changed. */
	virtual void SkyChanged( seSkin *skin );
	
	/** \brief The environment object changed. */
	virtual void EnvObjectChanged( seSkin *skin );
	
	/** \brief The view changed. */
	virtual void ViewChanged( seSkin *skin );
	
	/** \brief The camera changed. */
	virtual void CameraChanged( seSkin *skin );
	
	
	
	/** \brief Textures have been added or removed. */
	virtual void TextureStructureChanged ( seSkin *skin );
	
	/** \brief Texture changed. */
	virtual void TextureChanged( seSkin *skin, seTexture *texture );
	
	/** \brief Texture name changed. */
	virtual void TextureNameChanged( seSkin *skin, seTexture *texture );
	
	/** \brief Active texture changed. */
	virtual void ActiveTextureChanged( seSkin *skin );
	
	
	
	/** \brief Properties have been added or removed. */
	virtual void PropertyStructureChanged ( seSkin *skin, seTexture *texture );
	
	/** \brief Property changed. */
	virtual void PropertyChanged( seSkin *skin, seTexture *texture, seProperty *property );
	
	/** \brief Active property changed. */
	virtual void ActivePropertyChanged( seSkin *skin, seTexture *texture );
	
	/** \brief Property node structre changed. */
	virtual void PropertyNodeStructureChanged( seSkin *skin, seTexture *texture, seProperty *property );
	
	/** \brief Property node changed. */
	virtual void PropertyNodeChanged( seSkin *skin, seTexture *texture, seProperty *property, sePropertyNode *node );
	
	/** \brief Active property node changed. */
	virtual void PropertyActiveNodeChanged( seSkin *skin, seTexture *texture, seProperty *property );
	
	/** \brief Property selected nodes changed. */
	virtual void PropertyNodeSelectionChanged( seSkin *skin, seTexture *texture, seProperty *property );
	
	/** \brief Active property node group changed. */
	virtual void PropertyActiveNodeGroupChanged( seSkin *skin, seTexture *texture, seProperty *property );
	
	/** \brief Active property node layer changed. */
	virtual void PropertyActiveNodeLayerChanged( seSkin *skin, seTexture *texture, seProperty *property );
	
	
	
	/** \brief Dynamic skin renderables have been added or removed. */
	virtual void DynamicSkinRenderableStructureChanged ( seSkin *skin );
	
	/** \brief Dynamic skin renderable changed. */
	virtual void DynamicSkinRenderableChanged( seSkin *skin, seDynamicSkinRenderable *renderable );
	
	/** \brief Dynamic skin renderable name changed. */
	virtual void DynamicSkinRenderableNameChanged( seSkin *skin, seDynamicSkinRenderable *renderable );
	
	/** \brief Active dynamic skin renderable changed. */
	virtual void DynamicSkinActiveRenderableChanged( seSkin *skin );
	/*@}*/
};

#endif
