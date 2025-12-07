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

#ifndef _DEBASEGRAPHICCOMPONENT_H_
#define _DEBASEGRAPHICCOMPONENT_H_

#include "../../../dragengine_export.h"

class decVector;
class deDecal;
class deComponentTexture;
class deRenderable;


/**
 * \brief Graphic Module Component Peer.
 */
class DE_DLL_EXPORT deBaseGraphicComponent{
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
	virtual void TextureChanged(int index, deComponentTexture &texture);
	
	/**
	 * \brief Retrieve index of the texture of the face closest to a position or -1 if not found.
	 * 
	 * Limits the search to the provided radius.
	 */
	virtual int IndexOfTextureClosestTo(const decVector &vector, float radius);
	
	/** \brief Dynamic skin changed. */
	virtual void DynamicSkinChanged();
	
	/** \brief Render only animator changed. */
	virtual void AnimatorChanged();
	
	/** \brief Layer mask changed. */
	virtual void LayerMaskChanged();
	
	
	
	/** \brief Decal has been added. */
	virtual void DecalAdded(deDecal *decal);
	
	/** \brief Decal has been removed. */
	virtual void DecalRemoved(deDecal *decal);
	
	/** \brief All decals have been removed. */
	virtual void AllDecalsRemoved();
	/*@}*/
};

#endif
