/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLSKINSTATERENDERABLE_H_
#define _DEOGLSKINSTATERENDERABLE_H_

class deoglRenderPlan;
class deoglRenderThread;
class deoglTexture;
class deoglRSkin;
class deoalSkinState;
class deoglRDynamicSkin;


/**
 * \brief Skin State Renderable.
 * 
 * Stores the host renderable linked to this skin renderable and additional
 * objects required for masked rendering if required.
 */
class deoglSkinStateRenderable{
private:
	deoglSkinState &pSkinState;
	int pIndex;
	int pHostRenderable;
	deoglRenderPlan *pPlan;
	deoglTexture *pTexture;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create skin state renderable. */
	deoglSkinStateRenderable( deoglSkinState &skinState, int index );
	
	/** \brief Clean up skin state renderable. */
	~deoglSkinStateRenderable();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Host renderable index or -1 if not used. */
	inline int GetHostRenderable() const{ return pHostRenderable; }
	
	/** \brief Set host renderable index or -1 if not used. */
	void SetHostRenderable( int hostRenderable );
	
	/** \brief Render plan or NULL if not using masked rendering. */
	inline deoglRenderPlan *GetPlan() const{ return pPlan; }
	
	/** \brief Set render plan or NULL if not using masked rendering. */
	void SetPlan( deoglRenderPlan *plan );
	
	/** \brief Temporary texture or NULL if not existing. */
	inline deoglTexture *GetTexture() const{ return pTexture; }
	
	/** \brief Set temporary texture or NULL if not existing. */
	void SetTexture( deoglTexture *texture );
	
	/** \brief Clear renderable to be used for another one. */
	void Clear();
	
	
	
	/** \brief Add render plans. */
	void AddRenderPlans( deoglRenderPlan &plan );
	/*@}*/
};

#endif
