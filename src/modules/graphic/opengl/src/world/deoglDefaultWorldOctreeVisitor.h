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
 * \brief Default world octree visitor.
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
	/** \brief Create world octree visitor. */
	deoglDefaultWorldOctreeVisitor();
	
	/** \brief Clean up world octree visitor. */
	virtual ~deoglDefaultWorldOctreeVisitor();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Visit billboards. */
	inline bool GetVisitBillboards() const{ return pVisitBillboards; }
	
	/** \brief Set if billboards are visited. */
	void SetVisitBillboards( bool visitBillboards );
	
	/** \brief Visit particle emitters. */
	inline bool GetVisitParticleEmitters() const{ return pVisitParticleEmitters; }
	
	/** \brief Set if particle emitters are visited. */
	void SetVisitParticleEmitters( bool visitEmitters );
	
	/** \brief Visit components. */
	inline bool GetVisitComponents() const{ return pVisitComponents; }
	
	/** \brief Set if components are visited. */
	void SetVisitComponents( bool visitComponents );
	
	/** \brief Visit lights. */
	inline bool GetVisitLights() const{ return pVisitLights; }
	
	/** \brief Set if lights are visited. */
	void SetVisitLights( bool visitLights );
	
	/** \brief Visit environment maps. */
	inline bool GetVisitEnvMaps() const{ return pVisitEnvMaps; }
	
	/** \brief Set if environment maps are visited. */
	void SetVisitEnvMaps( bool visitEnvMaps );
	
	/** \brief Set if all elements are visited. */
	void SetVisitAll( bool visitAll );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit octree node. */
	virtual void VisitNode( deoglDOctree *node, int intersection );
	
	/** \brief Visit billboard. */
	virtual void VisitBillboard( deoglRBillboard *billboard );
	
	/** \brief Visit particle emitter. */
	virtual void VisitParticleEmitter( deoglRParticleEmitterInstance *emitter );
	
	/** \brief Visit component. */
	virtual void VisitComponent( deoglRComponent *component );
	
	/** \brief Visit light. */
	virtual void VisitLight( deoglRLight *light );
	
	/** \brief Visit environment maps. */
	virtual void VisitEnvMaps( deoglEnvironmentMap *envmap );
	/*@}*/
};

#endif
