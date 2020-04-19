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

#ifndef _SELINK_H_
#define _SELINK_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/curve/decCurveBezier.h>

class seController;
class seLinkMapping;
class seSynthesizer;
class deSynthesizerLink;



/**
 * \brief Synthesizer link.
 */
class seLink : public deObject{
private:
	seSynthesizer *pSynthesizer;
	
	deSynthesizerLink *pEngLink;
	
	decString pName;
	
	seController *pController;
	int pRepeat;
	decCurveBezier pCurve;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create link. */
	seLink();
	
	/** \brief Create copy of link. */
	seLink( const seLink &copy );
	
	/** \brief Clean up link. */
	virtual ~seLink();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**  \brief Synthesizer. */
	inline seSynthesizer *GetSynthesizer() const{ return pSynthesizer; }
	
	/** \brief Set synthesizer. */
	void SetSynthesizer( seSynthesizer *synthesizer );
	
	
	
	/** \brief Engine controller link or \em NULL if not managed. */
	inline deSynthesizerLink *GetEngineLink() const{ return pEngLink; }
	
	
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	
	
	/** \brief Controller or \em NULL. */
	inline seController *GetController() const{ return pController; }
	
	/** \brief Set controller or \em NULL. */
	void SetController( seController *controller );
	
	/** \brief Repeat count of input value. */
	inline int GetRepeat() const{ return pRepeat; }
	
	/**
	 * \brief Set repeat count of input value.
	 * \throws deeInvalidParam \em repeat is less than 1.
	 */
	void SetRepeat( int repeat );
	
	/** \brief Curve. */
	inline decCurveBezier &GetCurve(){ return pCurve; }
	inline const decCurveBezier &GetCurve() const{ return pCurve; }
	
	
	
	/** \brief Notify engine object about changes in this link. */
	void NotifyLinkChanged();
	
	
	
	/** \brief Update controller. */
	void UpdateController();
	
	/** \brief Updates engine curve. */
	void UpdateCurve();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy link to this link. */
	seLink &operator=( const seLink &copy );
	/*@}*/
};

#endif
