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

#ifndef _DEOALRAYTRACEHITELEMENTLIST_H_
#define _DEOALRAYTRACEHITELEMENTLIST_H_

class deoalRayTraceHitElement;



/**
 * \brief Ray tracing hit element list.
 * 
 * List memory is not shrinking to avoid memory allocations.
 */
class deoalRayTraceHitElementList{
private:
	deoalRayTraceHitElement *pElements;
	int pCount;
	int pSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create ray trace hit element list. */
	deoalRayTraceHitElementList();
	
	/** \brief Clean up ray trace hit element list. */
	~deoalRayTraceHitElementList();
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/** \brief Number of elements. */
	inline int GetCount() const{ return pCount; }
	
	/** \brief Element at index. */
	const deoalRayTraceHitElement &GetAt( int index ) const;
	
	/** \brief Add element. */
	void Add( const deoalRayTraceHitElement &element );
	
	/** \brief Add element to hit component face. */
	void AddComponentFace( float distance, const decDVector &point, const decDVector &normal,
		deoalAComponent *component, int face, bool frontFacing );
	
	/** \brief Remove all elements. */
	void RemoveAll();
	/*@}*/
	
	
	
private:
	deoalRayTraceHitElement &pBareAdd();
};

#endif
