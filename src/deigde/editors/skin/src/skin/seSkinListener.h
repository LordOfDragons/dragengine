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
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<seSkinListener> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin listener. */
	seSkinListener();
	
	/** Clean up listener. */
protected:
	virtual ~seSkinListener();
public:
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Changed or saved state changed. */
	virtual void StateChanged(seSkin *skin);
	
	/** Undo changed. */
	virtual void UndoChanged(seSkin *skin);
	
	/** Skin parameters changed. */
	virtual void SkinChanged(seSkin *skin);
	
	/** The sky changed. */
	virtual void SkyChanged(seSkin *skin);
	
	/** The environment object changed. */
	virtual void EnvObjectChanged(seSkin *skin);
	
	/** The view changed. */
	virtual void ViewChanged(seSkin *skin);
	
	/** The camera changed. */
	virtual void CameraChanged(seSkin *skin);
	
	
	
	/** Mapped have been added or removed. */
	virtual void MappedStructureChanged(seSkin *skin);
	
	/** Mapped changed. */
	virtual void MappedChanged(seSkin *skin, seMapped *mapped);
	
	/** Mapped name changed. */
	virtual void MappedNameChanged(seSkin *skin, seMapped *mapped);
	
	/** Active mapped changed. */
	virtual void ActiveMappedChanged(seSkin *skin);
	
	
	
	/** Textures have been added or removed. */
	virtual void TextureStructureChanged(seSkin *skin);
	
	/** Texture changed. */
	virtual void TextureChanged(seSkin *skin, seTexture *texture);
	
	/** Texture name changed. */
	virtual void TextureNameChanged(seSkin *skin, seTexture *texture);
	
	/** Active texture changed. */
	virtual void ActiveTextureChanged(seSkin *skin);
	
	
	
	/** Properties have been added or removed. */
	virtual void PropertyStructureChanged (seSkin *skin, seTexture *texture);
	
	/** Property changed. */
	virtual void PropertyChanged(seSkin *skin, seTexture *texture, seProperty *property);
	
	/** Active property changed. */
	virtual void ActivePropertyChanged(seSkin *skin, seTexture *texture);
	
	/** Property node structre changed. */
	virtual void PropertyNodeStructureChanged(seSkin *skin, seTexture *texture, seProperty *property);
	
	/** Property node changed. */
	virtual void PropertyNodeChanged(seSkin *skin, seTexture *texture, seProperty *property, sePropertyNode *node);
	
	/** Active property node changed. */
	virtual void PropertyActiveNodeChanged(seSkin *skin, seTexture *texture, seProperty *property);
	
	/** Property selected nodes changed. */
	virtual void PropertyNodeSelectionChanged(seSkin *skin, seTexture *texture, seProperty *property);
	
	/** Active property node group changed. */
	virtual void PropertyActiveNodeGroupChanged(seSkin *skin, seTexture *texture, seProperty *property);
	
	/** Active property node layer changed. */
	virtual void PropertyActiveNodeLayerChanged(seSkin *skin, seTexture *texture, seProperty *property);
	
	
	
	/** Dynamic skin renderables have been added or removed. */
	virtual void DynamicSkinRenderableStructureChanged (seSkin *skin);
	
	/** Dynamic skin renderable changed. */
	virtual void DynamicSkinRenderableChanged(seSkin *skin, seDynamicSkinRenderable *renderable);
	
	/** Dynamic skin renderable name changed. */
	virtual void DynamicSkinRenderableNameChanged(seSkin *skin, seDynamicSkinRenderable *renderable);
	
	/** Active dynamic skin renderable changed. */
	virtual void DynamicSkinActiveRenderableChanged(seSkin *skin);
	/*@}*/
};

#endif
