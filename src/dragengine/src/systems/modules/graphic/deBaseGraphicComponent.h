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

#ifndef _DEBASEGRAPHICCOMPONENT_H_
#define _DEBASEGRAPHICCOMPONENT_H_

class decVector;
class deDecal;
class deComponentTexture;
class deRenderable;


/**
 * \brief Graphic Module Component Peer.
 */
class deBaseGraphicComponent{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deBaseGraphicComponent();
	
	/** \brief Clean up peer. */
	virtual ~deBaseGraphicComponent();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Scaling changed. */
	virtual void ScalingChanged();
	
	/** \brief Model object changed. */
	virtual void ModelChanged();
	
	/** \brief Skin object changed. */
	virtual void SkinChanged();
	
	/** \brief Model and skin object changed. */
	virtual void ModelAndSkinChanged();
	
	/** \brief Rig object changed. */
	virtual void RigChanged();
	
	/** \brief Occlusion mesh changed. */
	virtual void OcclusionMeshChanged();
	
	/** \brief Parameter or hint changed. */
	virtual void ParametersChanged();
	
	/** \brief Visitility changed. */
	virtual void VisibilityChanged();
	
	/** \brief Extends changed. */
	virtual void ExtendsChanged();
	
	/**
	 * \brief Mesh vertices have been invalidated.
	 * 
	 * Called if Model changed or bones have been invalidated.
	 */
	virtual void MeshDirty();
	
	/** \brief Texture changed. */
	virtual void TextureChanged( int index, deComponentTexture &texture );
	
	/**
	 * \brief Retrieve index of the texture of the face closest to a position or -1 if not found.
	 * 
	 * Limits the search to the provided radius.
	 */
	virtual int IndexOfTextureClosestTo( const decVector &vector, float radius );
	
	/** \brief Dynamic skin changed. */
	virtual void DynamicSkinChanged();
	
	/** \brief Render only animator changed. */
	virtual void AnimatorChanged();
	
	/** \brief Layer mask changed. */
	virtual void LayerMaskChanged();
	
	
	
	/** \brief Decal has been added. */
	virtual void DecalAdded( deDecal *decal );
	
	/** \brief Decal has been removed. */
	virtual void DecalRemoved( deDecal *decal );
	
	/** \brief All decals have been removed. */
	virtual void AllDecalsRemoved();
	/*@}*/
};

#endif
