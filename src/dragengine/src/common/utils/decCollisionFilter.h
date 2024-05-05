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
class DE_DLL_EXPORT decCollisionFilter{
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
