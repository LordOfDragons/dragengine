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

#ifndef _DECOLLISIONFILTER_H_
#define _DECOLLISIONFILTER_H_

#include "decLayerMask.h"


/**
 * \brief Defines a collision filter using layer masks.
 * 
 * Composes of a category and filter layer mask. Category is usually
 * a single bit and the filter one or more bits. In special cases this
 * can be different. Two collision filters collide with each other if
 * A.category.Matches(B.filter) and B.category.Matches(A.filter). For
 * simple layer mask matching without filtering set both category and
 * filter to the same value. Otherwise category is usually set to the
 * object category and the filter to all categories the object is
 * allowed to collide with.
 */
class decCollisionFilter{
private:
	decLayerMask pCategory;
	decLayerMask pFilter;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create new collision filter.
	 * 
	 * Category is set empty and filter set filled.
	 */
	decCollisionFilter();
	
	/** \brief Create new collision filter with category and filter set to the same mask. */
	decCollisionFilter( const decLayerMask &mask );
	
	/** \brief Create new collision filter from a set of layer masks. */
	decCollisionFilter( const decLayerMask &category, const decLayerMask &filter );
	
	/** \brief Create copy of a collision filter. */
	decCollisionFilter( const decCollisionFilter &other );
	
	/** \brief Clean up collision filter. */
	~decCollisionFilter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Category layer mask. */
	inline decLayerMask &GetCategory(){ return pCategory; }
	inline const decLayerMask &GetCategory() const{ return pCategory; }
	
	/** \brief Filter layer mask. */
	inline decLayerMask &GetFilter(){ return pFilter; }
	inline const decLayerMask &GetFilter() const{ return pFilter; }
	
	/**
	 * \brief Determines if two collision filters can collide.
	 * 
	 * Tests if for both collision filters the category matches the other filter.
	 * Hence the result is this.filter.Matches(other.category) and
	 * other.filter.Matches(this.category).
	 */
	bool Collides( const decCollisionFilter &other ) const;
	
	/**
	 * \brief Determines if two collision filters can not collide.
	 * 
	 * Tests if for any of both collision filters the category matches not the other filter.
	 * Hence the result is this.filter.MatchesNot(other.category) or
	 * other.filter.MatchesNot(this.category).
	 */
	bool CollidesNot( const decCollisionFilter &other ) const;
	
	/**
	 * \brief Collision filters can collide with another collision filter.
	 * 
	 * Returns true if the both the category and filter layer mask are not empty. Can be used
	 * to test if collision calculations are required for the collision filter.
	 */
	bool CanCollide() const;
	
	/**
	 * \brief Collision filters can never collide with another collision filter.
	 * 
	 * Returns true if the category and/or the filter is empty. Can be used to avoid
	 * collision calculations if a collision filter has no chance to ever collide.
	 */
	bool CanNotCollide() const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Determines if collision filters are equal. */
	bool operator==( const decCollisionFilter &other ) const;
	
	/** \brief Determines if collision filters are not equal. */
	bool operator!=( const decCollisionFilter &other ) const;
	
	/** \brief Sets this collision filter from another one. */
	decCollisionFilter &operator=( const decCollisionFilter &other );
	/*@}*/
};

#endif
