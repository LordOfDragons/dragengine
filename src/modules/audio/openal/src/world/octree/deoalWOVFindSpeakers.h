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
	deoalWOVFindSpeakers( const decDVector &position, const decLayerMask &layerMask,
		deoalSpeakerList &speakerList );
	
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
	static void FindSpeakers( deoalAWorld &world, const decDVector &position,
		const decLayerMask &layerMask, deoalSpeakerList &speakerList );
	
	
	
	/** \brief Visit speaker. */
	virtual void VisitSpeaker( deoalASpeaker *speaker );
	/*@}*/
};

#endif
