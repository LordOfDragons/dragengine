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

// include only once
#ifndef _DEOGLOCCLUSIONQUERYMANAGER_H_
#define _DEOGLOCCLUSIONQUERYMANAGER_H_

// predefinitions
class deoglRenderThread;
class deoglOcclusionQuery;



/**
 * Occlusion Query Manager.
 *
 * Manages occlusion queries. Queries are stored in a greedy fashion
 * therefore not freed once no more used. Upon requesting a new query
 * one is picked from the list of unused queries or a new one created
 * if the list is empty. Claimed queries are more to a new list and
 * are not usable until released by the owner in which case they are
 * moved back to the list of free queries. In addition the active
 * query is stored.
 */
class deoglOcclusionQueryManager{
private:
	deoglRenderThread &pRenderThread;
	
	deoglOcclusionQuery **pQueries;
	int pQueryCount;
	int pQuerySize;
	
	int pFreeQueryCount;
	
	deoglOcclusionQuery *pActiveQuery;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new occlusion queries. */
	deoglOcclusionQueryManager( deoglRenderThread &renderThread );
	/** Cleans up the occlusion queries. */
	~deoglOcclusionQueryManager();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Claim a new query. */
	deoglOcclusionQuery *ClaimQuery();
	/** Release a claimed query. */
	void ReleaseQuery( deoglOcclusionQuery *query );
	
	/** Retrieves the active query. */
	inline deoglOcclusionQuery *GetActiveQuery() const{ return pActiveQuery; }
	/** Sets the active query. */
	void SetActiveQuery( deoglOcclusionQuery *query );
	/** Ends the active query if there is one. */
	void EndActiveQuery();
	/*@}*/
	
private:
	int IndexOfQuery( deoglOcclusionQuery *query, int from, int to ) const;
};

// end of include only once
#endif
