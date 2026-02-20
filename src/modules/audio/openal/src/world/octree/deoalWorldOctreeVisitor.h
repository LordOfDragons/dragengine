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
	~deoalWorldOctreeVisitor() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Microphones are visited. */
	inline bool GetVisitMicrophones() const{ return pVisitMicrophones; }
	
	/** \brief Set if microphones are visited. */
	void SetVisitMicrophones(bool visitMicrophones);
	
	/** \brief Components are visited. */
	inline bool GetVisitComponents() const{ return pVisitComponents; }
	
	/** \brief Set if components are visited. */
	void SetVisitComponents(bool visitComponents);
	
	/** \brief Speakers are visited. */
	inline bool GetVisitSpeakers() const{ return pVisitSpeakers; }
	
	/** \brief Set if speakers are visited. */
	void SetVisitSpeakers(bool visitSpeakers);
	
	/** \brief Sound level meters are visited. */
	inline bool GetVisitSoundLevelMeters() const{ return pVisitSoundLevelMeters; }
	
	/** \brief Set if sound level meters are visited. */
	void SetVisitSoundLevelMeters(bool visitSoundLevelMeters);
	
	/** \brief Set visiting of all elements. */
	void SetVisitAll(bool visitAll);
	
	/** \brief Environment probes are visited. */
	inline bool GetVisitEnvProbes() const{ return pVisitEnvProbes; }
	
	/** \brief Set if environment probes are visited. */
	void SetVisitEnvProbes(bool visitEnvProbes);
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
	virtual void VisitNode(deoalWorldOctree &node);
	
	/**
	 * \brief Visit octree node.
	 * 
	 * Visits all elements marked as to be visited using the respective functions.
	 */
	void VisitNode(deoalDOctree *node, int intersection) override;
	
	/** \brief Visit microphone. */
	virtual void VisitMicrophone(deoalAMicrophone *microphone);
	
	/** \brief Visit component. */
	virtual void VisitComponent(deoalAComponent *component);
	
	/** \brief Visit speaker. */
	virtual void VisitSpeaker(deoalASpeaker *speaker);
	
	/** \brief Visit environment probes. */
	virtual void VisitEnvProbe(deoalEnvProbe *envProbe);
	
	/** \brief Visit sound level meter. */
	virtual void VisitSoundLevelMeter(deoalASoundLevelMeter *soundLevelMeter);
	/*@}*/
};

#endif
