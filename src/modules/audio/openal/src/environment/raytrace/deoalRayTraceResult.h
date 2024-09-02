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

#ifndef _DEOALRAYTRACERESULT_H_
#define _DEOALRAYTRACERESULT_H_

#include <dragengine/common/math/decMath.h>

class deoalAudioThread;
class deoalRayTraceHitElement;
class deoalAComponent;



/**
 * \brief Ray tracing result.
 * 
 * Stores list of hit faces sorted by distance.
 */
class deoalRayTraceResult{
private:
	deoalRayTraceHitElement *pElements;
	int pElementCount;
	int pElementSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create ray trace result. */
	deoalRayTraceResult();
	
	/** \brief Clean up ray trace result. */
	~deoalRayTraceResult();
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/** \brief Number of elements. */
	inline int GetElementCount() const{ return pElementCount; }
	
	/** \brief Element at index. */
	const deoalRayTraceHitElement &GetElementAt( int index ) const;
	
	/** \brief Add element sorted by distance. */
	void AddElement( float distance, const decDVector &point, const decDVector &normal,
		deoalAComponent *component, int face, bool frontFacing );
	
	/** \brief Add element sorted by distance. */
	void AddElement( const deoalRayTraceHitElement &element );
	
	/** \brief Remove all elements. */
	void RemoveAllElements();
	
	/** \brier Clear. */
	void Clear();
	
	/** \brief Debug print. */
	void DebugPrint( deoalAudioThread &audioThread, const char *prefix );
	/*@}*/
	
	
	
protected:
	deoalRayTraceHitElement *pInsert( float distance );
};

#endif
