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

#ifndef _DEOALWORLDOCTREEVISITOR_H_
#define _DEOALWORLDOCTREEVISITOR_H_

#include "../../utils/octree/deoalDOctreeVisitor.h"

class deoalASpeaker;
class deoalAComponent;
class deoalAMicrophone;
class deoalEnvProbe;
class deoalASoundLevelMeter;
class deoalWorldOctree;



/**
 * \brief OpenAL World Octree Visitor.
 * 
 * Visitor for the world octree. Provides support to visit the individual world elements in
 * addition to the octree nodes. Use this class only on deoalWorldOctree or subclass thereof.
 */
class deoalWorldOctreeVisitor : public deoalDOctreeVisitor{
private:
	bool pVisitMicrophones;
	bool pVisitComponents;
	bool pVisitSpeakers;
	bool pVisitEnvProbes;
	bool pVisitSoundLevelMeters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create world octree visitor. */
	deoalWorldOctreeVisitor();
	
	/** \brief Clean up world octree visitor. */
	virtual ~deoalWorldOctreeVisitor();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Microphones are visited. */
	inline bool GetVisitMicrophones() const{ return pVisitMicrophones; }
	
	/** \brief Set if microphones are visited. */
	void SetVisitMicrophones( bool visitMicrophones );
	
	/** \brief Components are visited. */
	inline bool GetVisitComponents() const{ return pVisitComponents; }
	
	/** \brief Set if components are visited. */
	void SetVisitComponents( bool visitComponents );
	
	/** \brief Speakers are visited. */
	inline bool GetVisitSpeakers() const{ return pVisitSpeakers; }
	
	/** \brief Set if speakers are visited. */
	void SetVisitSpeakers( bool visitSpeakers );
	
	/** \brief Sound level meters are visited. */
	inline bool GetVisitSoundLevelMeters() const{ return pVisitSoundLevelMeters; }
	
	/** \brief Set if sound level meters are visited. */
	void SetVisitSoundLevelMeters( bool visitSoundLevelMeters );
	
	/** \brief Set visiting of all elements. */
	void SetVisitAll( bool visitAll );
	
	/** \brief Environment probes are visited. */
	inline bool GetVisitEnvProbes() const{ return pVisitEnvProbes; }
	
	/** \brief Set if environment probes are visited. */
	void SetVisitEnvProbes( bool visitEnvProbes );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/**
	 * \brief Visit octree node.
	 * 
	 * Visitor is responsible to check if the node and children thereof have to be
	 * visited. This allows to apply special checks not handled by deoalDOctree.
	 * 
	 * To visit world call VisitNode(*world.GetOctree()).
	 * 
	 * The default implementation calls VisitNode(deoalDOctree*,int) then visits child nodes.
	 */
	virtual void VisitNode( deoalWorldOctree &node );
	
	/**
	 * \brief Visit octree node.
	 * 
	 * Visits all elements marked as to be visited using the respective functions.
	 */
	virtual void VisitNode( deoalDOctree *node, int intersection );
	
	/** \brief Visit microphone. */
	virtual void VisitMicrophone( deoalAMicrophone *microphone );
	
	/** \brief Visit component. */
	virtual void VisitComponent( deoalAComponent *component );
	
	/** \brief Visit speaker. */
	virtual void VisitSpeaker( deoalASpeaker *speaker );
	
	/** \brief Visit environment probes. */
	virtual void VisitEnvProbe( deoalEnvProbe *envProbe );
	
	/** \brief Visit sound level meter. */
	virtual void VisitSoundLevelMeter( deoalASoundLevelMeter *soundLevelMeter );
	/*@}*/
};

#endif
