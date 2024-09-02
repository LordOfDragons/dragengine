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

#ifndef _DEOALSOUNDRAYINTERACTIONLIST_H_
#define _DEOALSOUNDRAYINTERACTIONLIST_H_

class deoalSoundRayInteraction;



/**
 * \brief Sound ray interaction list.
 * 
 * List memory is not shrinking to avoid memory allocations.
 */
class deoalSoundRayInteractionList{
private:
	deoalSoundRayInteraction *pRays;
	int pCount;
	int pSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sound ray interaction list. */
	deoalSoundRayInteractionList();
	
	/** \brief Clean up sound ray interaction list. */
	~deoalSoundRayInteractionList();
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/** \brief Number of elements. */
	inline int GetCount() const{ return pCount; }
	
	/** \brief Element at index. */
	const deoalSoundRayInteraction &GetAt( int index ) const;
	
	/** \brief Add element. */
	deoalSoundRayInteraction &Add();
	
	/** \brief Remove all elements. */
	void RemoveAll();
	
	/** \brief Reserve space in the array to make future Add() more efficient. */
	void ReserveSize( int size );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Assign. */
	deoalSoundRayInteractionList &operator=( const deoalSoundRayInteractionList &list );
	
	/** \brief Append. */
	deoalSoundRayInteractionList &operator+=( const deoalSoundRayInteractionList &list );
	/*@}*/
};

#endif
