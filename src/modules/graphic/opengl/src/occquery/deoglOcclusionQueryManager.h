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
	deoglOcclusionQueryManager(deoglRenderThread &renderThread);
	/** Cleans up the occlusion queries. */
	~deoglOcclusionQueryManager();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{return pRenderThread;}
	
	/** Claim a new query. */
	deoglOcclusionQuery *ClaimQuery();
	/** Release a claimed query. */
	void ReleaseQuery(deoglOcclusionQuery *query);
	
	/** Retrieves the active query. */
	inline deoglOcclusionQuery *GetActiveQuery() const{return pActiveQuery;}
	/** Sets the active query. */
	void SetActiveQuery(deoglOcclusionQuery *query);
	/** Ends the active query if there is one. */
	void EndActiveQuery();
	/*@}*/
	
private:
	int IndexOfQuery(deoglOcclusionQuery *query, int from, int to) const;
};

// end of include only once
#endif
