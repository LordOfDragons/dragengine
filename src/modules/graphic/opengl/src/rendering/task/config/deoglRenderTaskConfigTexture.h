/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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
