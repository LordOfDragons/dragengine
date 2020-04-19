/* 
 * Drag[en]gine IGDE Speech Animation Editor
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

#ifndef _SAEWPSANIMLISTENER_H_
#define _SAEWPSANIMLISTENER_H_

#include "../../sanimation/saeSAnimationListener.h"

class saeWPSAnim;



/**
 * \brief Speech Animation panel listener.
 */
class saeWPSAnimListener : public saeSAnimationListener{
private:
	saeWPSAnim &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	saeWPSAnimListener( saeWPSAnim &panel );
	
	/** \brief Clean up listener. */
	virtual ~saeWPSAnimListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Speech animation changed. */
	virtual void SAnimationChanged( saeSAnimation *sanimation );
	
	/** \brief Phoneme count or order changed. */
	virtual void PhonemeStructureChanged( saeSAnimation *sanimation );
	
	/** \brief Phoneme changed. */
	virtual void PhonemeChanged( saeSAnimation *sanimation, saePhoneme *phoneme );
	
	/** \brief Active phoneme changed. */
	virtual void ActivePhonemeChanged( saeSAnimation *sanimation );
	
	/** \brief Word count or order changed. */
	virtual void WordStructureChanged( saeSAnimation *sanimation );
	
	/** \brief Word changed. */
	virtual void WordChanged( saeSAnimation *sanimation, saeWord *word );
	
	/** \brief Active word changed. */
	virtual void ActiveWordChanged( saeSAnimation *sanimation );
	/*@}*/
};

#endif
