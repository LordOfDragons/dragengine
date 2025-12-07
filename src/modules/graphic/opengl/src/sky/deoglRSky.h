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

#ifndef _DEOGLRSKY_H_
#define _DEOGLRSKY_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>

class deoglRSkyLayer;
class deoglRSkyLink;
class deoglRenderThread;

class deSky;



/**
 * Render sky.
 */
class deoglRSky : public deObject{
private:
	deoglRenderThread &pRenderThread;
	
	deoglRSkyLink **pLinks;
	int pLinkCount;
	
	deoglRSkyLayer **pLayers;
	int pLayerCount;
	
	decColor pBgColor;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglRSky> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Create new render sky. */
	deoglRSky(deoglRenderThread &renderThread);
	
	/** Clean up render sky. */
	~deoglRSky();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/** Background color of the sky. */
	inline const decColor &GetBgColor() const{ return pBgColor; }
	
	/** Set background color of the sky. */
	void SetBgColor(const decColor &color);
	
	
	
	/** Number of layers. */
	inline int GetLayerCount() const{ return pLayerCount; }
	
	/** Get link at index. */
	const deoglRSkyLink &GetLinkAt(int index) const;
	
	/**
	 * Rebuild links.
	 * 
	 * \note Called from main thread during synchronoization.
	 */
	void RebuildLinks(const deSky &sky);
	
	
	
	/** Layer at index. */
	deoglRSkyLayer &GetLayerAt(int index) const;
	
	/**
	 * Rebuild layers.
	 * 
	 * \note Called from main thread during synchronoization.
	 */
	void RebuildLayers(const deSky &sky);
	/*@}*/
};

#endif
