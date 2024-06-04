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

#ifndef _DEOGLRENDERTASKCONFIGTEXTURE_H_
#define _DEOGLRENDERTASKCONFIGTEXTURE_H_

class deoglPipeline;
class deoglRenderTaskSharedInstance;
class deoglRenderTaskSharedTexture;
class deoglRenderTaskSharedVAO;


/**
 * Render task texture configuration.
 */
class deoglRenderTaskConfigTexture{
private:
	int pRenderTaskFilter;
	
	const deoglPipeline *pPipeline;
	const deoglRenderTaskSharedTexture *pTexture;
	const deoglRenderTaskSharedVAO *pVAO;
	const deoglRenderTaskSharedInstance *pInstance;
	
	int pPipelineIndex;
	int pTextureIndex;
	int pVAOIndex;
	int pInstanceIndex;
	
	int pGroupIndex;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render task texture configuration. */
	deoglRenderTaskConfigTexture();
	
	/** Clean up render task texture configuration. */
	~deoglRenderTaskConfigTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render task filter. */
	inline int GetRenderTaskFilter() const{ return pRenderTaskFilter; }
	
	/** Set render task filter. */
	void SetRenderTaskFilter( int filter );
	
	/** Pipeline. */
	inline const deoglPipeline *GetPipeline() const{ return pPipeline; }
	
	/** Set pipeline. */
	void SetPipeline( const deoglPipeline *pipeline );
	
	/** Texture. */
	inline const deoglRenderTaskSharedTexture *GetTexture() const{ return pTexture; }
	
	/** Set texture. */
	void SetTexture( const deoglRenderTaskSharedTexture *texture );
	
	/** VAO. */
	inline const deoglRenderTaskSharedVAO *GetVAO() const{ return pVAO; }
	
	/** Set VAO. */
	void SetVAO( const deoglRenderTaskSharedVAO *vao );
	
	/** Instance. */
	inline const deoglRenderTaskSharedInstance *GetInstance() const{ return pInstance; }
	
	/** Set instance. */
	void SetInstance( const deoglRenderTaskSharedInstance *instance );
	
	
	
	/** Pipeline index. */
	inline int GetPipelineIndex() const{ return pPipelineIndex; }
	
	/** Texture index. */
	inline int GetTextureIndex() const{ return pTextureIndex; }
	
	/** VAO index. */
	inline int GetVAOIndex() const{ return pVAOIndex; }
	
	/** Instance index. */
	inline int GetInstanceIndex() const{ return pInstanceIndex; }
	
	
	
	/** Sub instance group index. */
	inline int GetGroupIndex() const{ return pGroupIndex; }
	
	/** Set sub instance group index. */
	void SetGroupIndex( int groupIndex );
	
	
	
	/** Clear. */
	void Clear();
	/*@}*/
};

#endif
