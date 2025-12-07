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

#ifndef _DEOGLIMPOSTERBILLBOARD_H_
#define _DEOGLIMPOSTERBILLBOARD_H_

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglArrayTexture;



/**
 * Imposter Billboard.
 * Stores the textures and other information required for an imposter billboard.
 * Imposter billboards are typically created from skin shader driven objects but
 * can be possibly created from others. An imposter billboard composes of one or
 * more array textures storing the material properties the way a skin shader
 * expects them. The layers in the array texture correspond to views of the imposter
 * from different angles. The layout of the angles is set up by the owner of the
 * imposter billboard. Using array textures the appropriate imposter can be
 * selected for instanced geometry using the layer index.
 */
class deoglImposterBillboard{
private:
	deoglRenderThread &pRenderThread;
	
	deoglArrayTexture **pChannelTextures;
	
	decVector2 pMinExtend;
	decVector2 pMaxExtend;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new imposter billboard. */
	deoglImposterBillboard(deoglRenderThread &renderThread);
	/** Cleans up the imposter billboard. */
	~deoglImposterBillboard();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{return pRenderThread;}
	
	/** Retrieves the array texture for a skin texture channel. */
	deoglArrayTexture *GetChannelTextureAt(int channel) const;
	/** Sets if an array texture for a skin texture channel exists. */
	void EnableChannelTexture(int channel, bool enable);
	/** Determines if an array texture for a skin texture channel exists. */
	bool IsChannelTextureEnabled(int channel) const;
	
	/** Retrieves the minimum extend. */
	inline const decVector2 &GetMinExtend() const{return pMinExtend;}
	/** Retrieves the maximum extend. */
	inline const decVector2 &GetMaxExtend() const{return pMaxExtend;}
	/** Sets the minimum and maximum extend. */
	void SetExtends(const decVector2 &minExtend, const decVector2 &maxExtend);
	/*@}*/
};

#endif
