/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#ifndef _SESOURCEGROUP_H_
#define _SESOURCEGROUP_H_

#include "seSource.h"
#include "seSourceList.h"

#include <dragengine/resources/synthesizer/source/deSynthesizerSourceGroup.h>



/**
 * \brief Synthesizer source group.
 */
class seSourceGroup : public seSource{
private:
	seSourceList pSources;
	
	deSynthesizerSourceGroup::eApplicationTypes pApplicationType;
	
	seControllerTarget pTargetSelect;
	
	bool pTreeListExpanded;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create source. */
	seSourceGroup();
	
	/** \brief Create copy of source. */
	seSourceGroup( const seSourceGroup &copy );
	
	/** \brief Clean up source. */
	virtual ~seSourceGroup();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Sources. */
	inline const seSourceList &GetSources() const{ return pSources; }
	
	/** \brief Add source. */
	void AddSource( seSource *source );
	
	/** \brief Insert source. */
	void InsertSourceAt( seSource *source, int index );
	
	/** \brief Move source. */
	void MoveSourceTo( seSource *source, int index );
	
	/** \brief Remove source. */
	void RemoveSource( seSource *source );
	
	/** \brief Remove all sources. */
	void RemoveAllSources();
	
	
	
	/** \brief Application type. */
	inline deSynthesizerSourceGroup::eApplicationTypes GetApplicationType() const{ return pApplicationType; }
	
	/** \brief Set application type. */
	void SetApplicationType( deSynthesizerSourceGroup::eApplicationTypes type );
	
	/** \brief Select target. */
	inline seControllerTarget &GetTargetSelect(){ return pTargetSelect; }
	inline const seControllerTarget &GetTargetSelect() const{ return pTargetSelect; }
	
	
	
	/** \brief Create an engine synthesizer source. */
	virtual deSynthesizerSource *CreateEngineSource();
	
	/** \brief Update targets. */
	virtual void UpdateTargets();
	
	/** \brief Retrieve the number of targets using a given link. */
	virtual int CountLinkUsage( seLink *link ) const;
	
	/** \brief Removes a link from all targets using it. */
	virtual void RemoveLinkFromTargets( seLink *link );
	
	/** \brief Removes all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	/** \brief Create a copy of this source. */
	virtual seSource *CreateCopy() const;
	
	/** \brief List all links of all source targets. */
	virtual void ListLinks( seLinkList& list );
	
	
	
	/** \brief Tree list expanded state. */
	inline bool GetTreeListExpanded() const{ return pTreeListExpanded; }
	
	/** \brief Set tree list expanded state. */
	void SetTreeListExpanded( bool expanded );
	
	/** \brief Parent synthesizer changed. */
	virtual void SynthesizerChanged();
	
	/** \brief Synthesizer directory changed. */
	virtual void SynthesizerDirectoryChanged();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copies another group source to this group source. */
	virtual seSourceGroup &operator=( const seSourceGroup &copy );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
