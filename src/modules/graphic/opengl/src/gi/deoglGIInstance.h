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

#ifndef _DEOGLGIINSTANCE_H_
#define _DEOGLGIINSTANCE_H_

#include "../component/deoglComponentListener.h"

#include <dragengine/deObject.h>
#include <dragengine/deObjectReference.h>

class deoglRComponent;


/**
 * Global illumination instance.
 */
class deoglGIInstance : public deObject{
private:
	class cComponentListener : public deoglComponentListener {
	private:
		deoglGIInstance &pInstance;
		
	public:
		cComponentListener( deoglGIInstance &instance );
		virtual void ComponentDestroyed( deoglRComponent &component );
		virtual void ParentWorldChanged( deoglRComponent &component );
		virtual void LayerMaskChanged( deoglRComponent &component );
		virtual void BoundariesChanged( deoglRComponent &component );
		virtual void OcclusionMeshChanged( deoglRComponent &component );
		virtual void TexturesChanged( deoglRComponent &component );
	};
	
	deoglRComponent *pComponent;
	deoglRComponent *pOcclusionMesh;
	deObjectReference pComponentListener;
	
	int pIndexMaterial;
	bool pChanged;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create global illumination instance. */
	deoglGIInstance();
	
	/** Clean up global illumination instance. */
	virtual ~deoglGIInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Component or NULL. */
	inline deoglRComponent *GetComponent() const{ return pComponent; }
	
	/** Set component or NULL. */
	void SetComponent( deoglRComponent *component );
	
	/** Occlusion mesh or NULL. */
	inline deoglRComponent *GetOcclusionMesh() const{ return pOcclusionMesh; }
	
	/** Set occlusion mesh or NULL. */
	void SetOcclusionMesh( deoglRComponent *occlusionMesh );
	
	/** First material index. */
	inline int GetIndexMaterial() const{ return pIndexMaterial; }
	
	/** Set first material index. */
	void SetIndexMaterial( int index );
	
	/** Instance changed in a way ray cast results are invalidated. */
	inline bool GetChanged() const{ return pChanged; }
	
	/** Set instance changed in a way ray cast results are invalidated. */
	void SetChanged( bool changed );
	
	/** Slot is empty. */
	inline bool Empty() const{ return pComponent == NULL; }
	
	/** Slot is not empty. */
	inline bool NotEmpty() const{ return ! Empty(); }
	
	/** Clear instance. */
	void Clear();
	/*@}*/
};

#endif
