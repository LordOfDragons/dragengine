/* 
 * Drag[en]gine IGDE Animator Editor
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

#ifndef _AELINK_H_
#define _AELINK_H_

#include <dragengine/deObject.h>
#include <dragengine/common/curve/decCurveBezier.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class aeController;
class aeAnimator;
class deAnimatorLink;



/**
 * \brief Animator link.
 */
class aeLink : public deObject{
private:
	aeAnimator *pAnimator;
	
	deAnimatorLink *pEngLink;
	
	decString pName;
	
	aeController *pController;
	int pRepeat;
	decCurveBezier pCurve;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create link. */
	aeLink( const char *name = "Link" );
	
	/** \brief Create copy of link. */
	aeLink( const aeLink &copy );
	
	/** \brief Clean up link. */
	virtual ~aeLink();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**  \brief Animator. */
	inline aeAnimator *GetAnimator() const{ return pAnimator; }
	
	/** \brief Set animator. */
	void SetAnimator( aeAnimator *animator );
	
	
	
	/** \brief Engine controller link or \em NULL if not managed. */
	inline deAnimatorLink *GetEngineLink() const{ return pEngLink; }
	
	
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	
	
	/** \brief Controller or \em NULL. */
	inline aeController *GetController() const{ return pController; }
	
	/** \brief Set controller or \em NULL. */
	void SetController( aeController *controller, bool notify = true );
	
	/** \brief Repeat count of input value. */
	inline int GetRepeat() const{ return pRepeat; }
	
	/**
	 * \brief Set repeat count of input value.
	 * \throws deeInvalidParam \em repeat is less than 1.
	 */
	void SetRepeat( int repeat );
	
	/** \brief Curve. */
	const decCurveBezier &GetCurve() const{ return pCurve; }
	
	/** \brief Set curve. */
	void SetCurve( const decCurveBezier &curve );
	
	
	
	/** \brief Notify engine object about changes in this link. */
	void NotifyLinkChanged();
	
	
	
	/** \brief Update controller. */
	void UpdateController();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy link to this link. */
	aeLink &operator=( const aeLink &copy );
	/*@}*/
	
	
	
private:
	void pUpdateCurve();
};

#endif
