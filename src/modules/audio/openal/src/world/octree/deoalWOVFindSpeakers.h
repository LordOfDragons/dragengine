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

#ifndef _DEOALWOVFINDSPEAKERS_H_
#define _DEOALWOVFINDSPEAKERS_H_

#include <dragengine/common/math/decMath.h>

#include "deoalWorldOctreeVisitor.h"
#include "../../speaker/deoalSpeakerList.h"


class deoalSpeakerList;
class decLayerMask;
class deoalAWorld;


/**
 * \brief World octree visitor searching for speakers.
 * 
 * Found speakers are added to speaker list.
 */
class deoalWOVFindSpeakers : public deoalWorldOctreeVisitor{
private:
	const decDVector &pPosition;
	const decLayerMask &pLayerMask;
	deoalSpeakerList &pSpeakerList;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deoalWOVFindSpeakers(const decDVector &position, const decLayerMask &layerMask,
		deoalSpeakerList &speakerList);
	
	/** \brief Clean up visitor. */
	virtual ~deoalWOVFindSpeakers();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Speaker list. */
	inline deoalSpeakerList &GetSpeakerList() const{ return pSpeakerList; }
	
	
	
	/** \brief Convenience function to visit speakers  in world. */
	static void FindSpeakers(deoalAWorld &world, const decDVector &position,
		const decLayerMask &layerMask, deoalSpeakerList &speakerList);
	
	
	
	/** \brief Visit speaker. */
	virtual void VisitSpeaker(deoalASpeaker *speaker);
	/*@}*/
};

#endif
