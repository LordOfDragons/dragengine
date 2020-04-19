/* 
 * Drag[en]gine OpenAL Audio Module
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

#ifndef _DEOALWOVSLMFINDSPEAKERS_H_
#define _DEOALWOVSLMFINDSPEAKERS_H_

#include "../world/octree/deoalWorldOctreeVisitor.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>

class deoalAWorld;
class deoalSpeakerList;
class deoalASoundLevelMeter;


/**
 * \brief World octree visitor finding speakers for sound level meters.
 */
class deoalWOVSLMFindSpeakers : public deoalWorldOctreeVisitor{
private:
	const deoalASoundLevelMeter &pSoundLevelMeter;
	double pRangeSquared;
	deoalSpeakerList &pList;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deoalWOVSLMFindSpeakers( const deoalASoundLevelMeter &soundLevelMeter, deoalSpeakerList &list );
	
	/** \brief Clean up visitor. */
	virtual ~deoalWOVSLMFindSpeakers();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit world with visitor. */
	void Visit( deoalAWorld &world );
	
	
	
	/** \brief Visits components affecting sound hit by ray. */
	virtual void VisitNode( deoalDOctree *node, int intersection );
	/*@}*/
};

#endif
