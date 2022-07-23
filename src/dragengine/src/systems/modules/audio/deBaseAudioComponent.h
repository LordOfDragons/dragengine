/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEBASEAUDIOCOMPONENT_H_
#define _DEBASEAUDIOCOMPONENT_H_

#include "../../../dragengine_export.h"

class deDecal;
class deComponentTexture;



/**
 * \brief Audio module component peer.
 */
class DE_DLL_EXPORT deBaseAudioComponent{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deBaseAudioComponent();
	
	/** \brief Clean up peer. */
	virtual ~deBaseAudioComponent();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Scaling changed. */
	virtual void ScalingChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Model changed. */
	virtual void ModelChanged();
	
	/** \brief Rig changed. */
	virtual void RigChanged();
	
	/** \brief Skin changed. */
	virtual void SkinChanged();
	
	/** \brief Model and skin changed. */
	virtual void ModelAndSkinChanged();
	
	/** \brief Audio model changed. */
	virtual void AudioModelChanged();
	
	/** \brief Visitility changed. */
	virtual void VisibilityChanged();
	
	/** \brief Extends changed. */
	virtual void ExtendsChanged();
	
	/** \brief Layer mask changed. */
	virtual void LayerMaskChanged();
	
	/**
	 * \brief Mesh vertices have been invalidated.
	 * 
	 * Called if Model changed or bones have been invalidated.
	 */
	virtual void MeshDirty();
	
	/** \brief Parameter or hint changed. */
	virtual void ParametersChanged();
	
	/** \brief Texture changed. */
	virtual void TextureChanged( int index, deComponentTexture &texture );
	
	/** \brief Dynamic skin changed. */
	virtual void DynamicSkinChanged();
	
	
	
	/** \brief Decal has been added. */
	virtual void DecalAdded( deDecal *decal );
	
	/** \brief Decal has been removed. */
	virtual void DecalRemoved( deDecal *decal );
	
	/** \brief All decals have been removed. */
	virtual void AllDecalsRemoved();
	/*@}*/
};

#endif
