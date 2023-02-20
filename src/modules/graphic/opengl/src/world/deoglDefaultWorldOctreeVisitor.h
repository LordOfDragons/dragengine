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

#ifndef _DEOGLDEFAULTWORLDOCTREEVISITOR_H_
#define _DEOGLDEFAULTWORLDOCTREEVISITOR_H_

#include "deoglWorldOctreeVisitor.h"

class deoglEnvironmentMap;
class deoglRBillboard;
class deoglRComponent;
class deoglRLight;
class deoglRParticleEmitterInstance;



/**
 * Default world octree visitor.
 */
class deoglDefaultWorldOctreeVisitor : public deoglWorldOctreeVisitor{
private:
	bool pVisitBillboards;
	bool pVisitComponents;
	bool pVisitEnvMaps;
	bool pVisitLights;
	bool pVisitParticleEmitters;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create world octree visitor. */
	deoglDefaultWorldOctreeVisitor();
	
	/** Clean up world octree visitor. */
	virtual ~deoglDefaultWorldOctreeVisitor();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Visit billboards. */
	inline bool GetVisitBillboards() const{ return pVisitBillboards; }
	
	/** Set if billboards are visited. */
	void SetVisitBillboards( bool visitBillboards );
	
	/** Visit particle emitters. */
	inline bool GetVisitParticleEmitters() const{ return pVisitParticleEmitters; }
	
	/** Set if particle emitters are visited. */
	void SetVisitParticleEmitters( bool visitEmitters );
	
	/** Visit components. */
	inline bool GetVisitComponents() const{ return pVisitComponents; }
	
	/** Set if components are visited. */
	void SetVisitComponents( bool visitComponents );
	
	/** Visit lights. */
	inline bool GetVisitLights() const{ return pVisitLights; }
	
	/** Set if lights are visited. */
	void SetVisitLights( bool visitLights );
	
	/** Visit environment maps. */
	inline bool GetVisitEnvMaps() const{ return pVisitEnvMaps; }
	
	/** Set if environment maps are visited. */
	void SetVisitEnvMaps( bool visitEnvMaps );
	
	/** Set if all elements are visited. */
	void SetVisitAll( bool visitAll );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** Visit octree node. */
	virtual void VisitNode( deoglDOctree *node, int intersection );
	
	/** Visit billboard. */
	virtual void VisitBillboard( deoglRBillboard *billboard );
	
	/** Visit particle emitter. */
	virtual void VisitParticleEmitter( deoglRParticleEmitterInstance *emitter );
	
	/** Visit component. */
	virtual void VisitComponent( deoglRComponent *component );
	
	/** Visit light. */
	virtual void VisitLight( deoglRLight *light );
	
	/** Visit environment maps. */
	virtual void VisitEnvMaps( deoglEnvironmentMap *envmap );
	/*@}*/
};

#endif
