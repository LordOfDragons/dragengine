/* 
 * Drag[en]gine Game Engine
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

#ifndef _DESYNTHESIZERGROUPGROUP_H_
#define _DESYNTHESIZERGROUPGROUP_H_

#include "deSynthesizerSource.h"
#include "../../../common/collection/decObjectOrderedSet.h"


/**
 * \brief Synthesizer group source.
 */
class deSynthesizerSourceGroup : public deSynthesizerSource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deSynthesizerSourceGroup> Ref;
	
	
	
public:
	/** \brief Application types. */
	enum eApplicationTypes{
		/** \brief Output the mix of all sources in the group. */
		eatAll,
		
		/**
		 * \brief Use selection target to output blend between two selected sources.
		 * A target vlaue of 0 plays solo the first source in the group.
		 * A target value of 1 plays solo the last source in the group.
		 * Blends across all other sources in between in a linear way.
		 * To simulate blending over to the first source duplicate the
		 * first source also as last source.
		 */
		eatSelect,
		
		/**
		 * \brief Use selection target to output selected sources solo.
		 * Similar to eatSelect but does not blend between the two nearest
		 * sources but outputs only the nearest source. Source is chosen
		 * in a linear way.
		 */
		eatSolo
	};
	
	
	
private:
	decObjectOrderedSet pSources;
	
	eApplicationTypes pApplicationType;
	
	deSynthesizerControllerTarget pTargetSelect;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates synthesizer source. */
	deSynthesizerSourceGroup();
	
protected:
	/**
	 * \brief Clean up synthesizer source.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deSynthesizerSourceGroup();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Number of sources. */
	int GetSourceCount() const;
	
	/** \brief Source at index. */
	deSynthesizerSource *GetSourceAt( int index ) const;
	
	/** \brief Index of source or -1 if absent. */
	int IndexOfSource( deSynthesizerSource *source ) const;
	
	/** \brief Source is present. */
	bool HasSource( deSynthesizerSource *source ) const;
	
	/** \brief Add source. */
	void AddSource( deSynthesizerSource *source );
	
	/** \brief Remove source. */
	void RemoveSource( deSynthesizerSource *source );
	
	/** \brief Remove all sources. */
	void RemoveAllSources();
	
	
	
	/** \brief Source application type. */
	inline eApplicationTypes GetApplicationType() const{ return pApplicationType; }
	
	/** \brief Set source application type. */
	void SetApplicationType( eApplicationTypes type );
	
	/** \brief Select target. */
	inline deSynthesizerControllerTarget &GetTargetSelect(){ return pTargetSelect; }
	inline const deSynthesizerControllerTarget &GetTargetSelect() const{ return pTargetSelect; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the source. */
	virtual void Visit( deSynthesizerSourceVisitor &visitor );
	/*@}*/
};

#endif
