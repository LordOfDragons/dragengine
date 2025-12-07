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

#ifndef _DEOGLTEXSAMPLERCONFIG_H_
#define _DEOGLTEXSAMPLERCONFIG_H_

#include "../deoglTextureStageManager.h"

class deoglRenderThread;



/**
 * OpenGL texture sampler configuration.
 */
class deoglTexSamplerConfig{
private:
	deoglRenderThread &pRenderThread;
	
	GLuint pSamplerObject;
	
	int pMinLodLevel;
	int pMaxLodLevel;
	deoglTextureStageManager::eTextureFiltering pFilterMode;
	GLenum pWrapMode;
	bool pDepthCompareMode;
	GLenum pDepthCompareFunc;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create opengl texture sampler configuration. */
	deoglTexSamplerConfig(deoglRenderThread &renderThread);
	
	/** Clean up opengl texture sampler configuration. */
	~deoglTexSamplerConfig();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Sampler object handle. */
	inline GLuint GetSamplerObject() const{return pSamplerObject;}
	
	
	
	/** Minimum lod level. */
	inline int GetMinLodLevel() const{return pMinLodLevel;}
	
	/** Set minimum lod level. */
	void SetMinLodLevel(int level);
	
	/** Maximum lod level. */
	inline int GetMaxLodLevel() const{return pMaxLodLevel;}
	
	/** Set maximum lod level. */
	void SetMaxLodLevel(int level);
	
	/** Filter mode. */
	inline deoglTextureStageManager::eTextureFiltering GetFilterMode() const{return pFilterMode;}
	
	/** Set filter mode. */
	void SetFilterMode(deoglTextureStageManager::eTextureFiltering filterMode);
	
	/** Wrapping mode. */
	inline GLenum GetWrapMode() const{return pWrapMode;}
	
	/** Set wrapping mode. */
	void SetWrapMode(GLenum wrapMode);
	
	/** Texture uses depth compare mode. */
	inline bool GetDepthCompareMode() const{return pDepthCompareMode;}
	
	/** Set if texture uses depth compare mode. */
	void SetDepthCompareMode(bool depthCompareMode);
	
	/** Depth compare function. */
	inline GLenum GetDepthCompareFunc() const{return pDepthCompareFunc;}
	
	/** Set depth compare function. */
	void SetDepthCompareFunc(GLenum func);
	/*@}*/
};

#endif
