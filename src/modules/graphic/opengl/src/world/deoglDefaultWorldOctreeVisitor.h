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
