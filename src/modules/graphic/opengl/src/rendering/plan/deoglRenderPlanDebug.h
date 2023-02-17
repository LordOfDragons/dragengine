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

#ifndef _DEOGLRENDERERPLANDEBUG_H_
#define _DEOGLRENDERERPLANDEBUG_H_



/**
 * Render Plan Debug.
 * Stores debug information for a render plan.
 */
class deoglRenderPlanDebug{
private:
	int pViewObjects;
	int pViewTriangles;
	int pViewLights;
	int pViewEnvMaps;
	
	int pLODTriangles;
	
	int pCullPSObjects;
	int pCullPSTriangles;
	int pCullPSStrips;
	int pCullPSLights;
	
	int pOccMapObjects;
	int pOccMapTriangles;
	float pOccMapRenderTime;
	float pOccTestTime;
	int pOccTestCount;
	
	int pRenderedPasses;
	int pRenderedObjects;
	int pRenderedTriangles;
	int pRenderedLights;
	int pRenderedEnvMaps;
	
	int pSolidObjects;
	int pSolidTriangles;
	
	int pTransparentPasses;
	int pTransparentObjects;
	int pTransparentTriangles;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new render plan debug. */
	deoglRenderPlanDebug();
	/** Cleans up the render plan debug. */
	~deoglRenderPlanDebug();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of objects in the view frustum. */
	inline int GetViewObjects() const{ return pViewObjects; }
	/** Increments the number of view objects. */
	inline void IncrementViewObjects( int count ){ pViewObjects += count; }
	/** Retrieves the number of view triangles. */
	inline int GetViewTriangles() const{ return pViewTriangles; }
	/** Increments the number of view triangles. */
	inline void IncrementViewTriangles( int count ){ pViewTriangles += count; }
	/** Retrieves the number of view lights. */
	inline int GetViewLights() const{ return pViewLights; }
	/** Increments the number of view lights. */
	inline void IncrementViewLights( int count ){ pViewLights += count; }
	/** Retrieves the number of view environment maps. */
	inline int GetViewEnvMaps() const{ return pViewEnvMaps; }
	/** Increments the number of view environment maps. */
	inline void IncrementViewEnvMaps( int count ){ pViewEnvMaps += count; }
	
	/** Retrieves the number of triangles before lod. */
	inline int GetLODTriangles() const{ return pLODTriangles; }
	/** Increments the number of triangles before lod. */
	inline void IncrementLODTriangles( int count ){ pLODTriangles += count; }
	
	/** Retrieves the number of objects in the view frustum after portal system culling. */
	inline int GetCullPSObjects() const{ return pCullPSObjects; }
	/** Increments the number of cull portal system objects. */
	inline void IncrementCullPSObjects( int count ){ pCullPSObjects += count; }
	/** Retrieves the number of cull portal system triangles. */
	inline int GetCullPSTriangles() const{ return pCullPSTriangles; }
	/** Increments the number of cull portal system triangles. */
	inline void IncrementCullPSTriangles( int count ){ pCullPSTriangles += count; }
	/** Retrieves the number of cull portal system strips. */
	inline int GetCullPSStrips() const{ return pCullPSStrips; }
	/** Increments the number of cull portal system strips. */
	inline void IncrementCullPSStrips( int count ){ pCullPSStrips += count; }
	/** Retrieves the number of lights in the view frustum after portal system culling. */
	inline int GetCullPSLights() const{ return pCullPSLights; }
	/** Increments the number of cull portal system lights. */
	inline void IncrementCullPSLights( int count ){ pCullPSLights += count; }
	
	/** Retrieves the number of occlusion map objects. */
	inline int GetOccMapObjects() const{ return pOccMapObjects; }
	/** Increments the number of occlusion map objects. */
	inline void IncrementOccMapObjects( int count ){ pOccMapObjects += count; }
	/** Retrieves the number of occlusion map triangles. */
	inline int GetOccMapTriangles() const{ return pOccMapTriangles; }
	/** Increments the number of occlusion map triangles. */
	inline void IncrementOccMapTriangles( int count ){ pOccMapTriangles += count; }
	/** Retrieves the occlusion map render time. */
	inline float GetOccMapRenderTime() const{ return pOccMapRenderTime; }
	/** Sets the occlusion map render time. */
	inline void SetOccMapRenderTime( float time ){ pOccMapRenderTime = time; }
	/** Retrieves the occlusion test time. */
	inline float GetOccTestTime() const{ return pOccTestTime; }
	/** Sets the occlusion test time. */
	inline void SetOccTestTime( float time ){ pOccTestTime = time; }
	/** Retrieves the number of occlusion tests. */
	inline int GetOccTestCount() const{ return pOccTestCount; }
	/** Increments the number of occlusion tests. */
	inline void IncrementOccTestCount( int count ){ pOccTestCount += count; }
	
	/** Retrieves the number of rendered passes. */
	inline int GetRenderedPasses() const{ return pRenderedPasses; }
	/** Increments the number of rendered passes by one. */
	inline void IncrementRenderPasses(){ pRenderedPasses++; }
	/** Retrieves the number of rendered objects. */
	inline int GetRenderedObjects() const{ return pRenderedObjects; }
	/** Increments the number of rendered objects by one. */
	inline void IncrementRenderedObjects(){ pRenderedObjects++; }
	/** Retrieves the number of rendered triangles. */
	inline int GetRenderedTriangles() const{ return pRenderedTriangles; }
	/** Increments the number of rendered triangles. */
	inline void IncrementRenderedTriangles( int count ){ pRenderedTriangles += count; }
	/** Retrieves the number of rendered lights. */
	inline int GetRenderedLights() const{ return pRenderedLights; }
	/** Increments the number of rendered lights by one. */
	inline void IncrementRenderedLights(){ pRenderedLights++; }
	/** Retrieves the number of rendered environment maps. */
	inline int GetRenderedEnvMaps() const{ return pRenderedEnvMaps; }
	/** Increments the number of rendered environment maps. */
	inline void IncrementRenderedEnvMaps( int count ){ pRenderedEnvMaps += count; }
	
	/** Retrieves the number of solid pass objects. */
	inline int GetSolidObjects() const{ return pSolidObjects; }
	/** Increments the number of solid pass objects by one. */
	inline void IncrementSolidObjects(){ pSolidObjects++; }
	/** Retrieves the number of solid pass triangles. */
	inline int GetSolidTriangles() const{ return pSolidTriangles; }
	/** Increments the number of solid pass triangles. */
	inline void IncrementSolidTriangles( int count ){ pSolidTriangles += count; }
	
	/** Retrieves the number of transparent passes. */
	inline int GetTransparentPasses() const{ return pTransparentPasses; }
	/** Increments the number of transparent passes by one. */
	inline void IncrementTransparentPasses(){ pTransparentPasses++; }
	/** Retrieves the number of transparent objects. */
	inline int GetTransparentObjects() const{ return pTransparentObjects; }
	/** Increments the number of transparent objects by one. */
	inline void IncrementTransparentObjects(){ pTransparentObjects++; }
	/** Retrieves the number of transparent triangles. */
	inline int GetTransparentTriangles() const{ return pTransparentTriangles; }
	/** Increments the number of transparent triangles. */
	inline void IncrementTransparentTriangles( int count ){ pTransparentTriangles += count; }
	
	/** Resets the debug object. */
	void Reset();
	/*@}*/
};

#endif
