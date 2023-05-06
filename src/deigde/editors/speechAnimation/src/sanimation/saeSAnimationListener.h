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

#ifndef _SAESANIMATIONLISTENER_H_
#define _SAESANIMATIONLISTENER_H_

#include <dragengine/deObject.h>

class saeSAnimation;
class saePhoneme;
class saeWord;



/**
 * Speech Animation Listener.
 */
class saeSAnimationListener : public deObject{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new listener. */
	saeSAnimationListener();
	/** Cleans up the listener. */
	virtual ~saeSAnimationListener();
	/*@}*/
	
	/** \name Notifications */
	/*@{*/
	/** Changed or saved state changed. */
	virtual void StateChanged( saeSAnimation *sanimation );
	/** Undo system changed. */
	virtual void UndoChanged( saeSAnimation *sanimation );
	/** View properties changed. */
	virtual void ViewChanged( saeSAnimation *sanimation );
	/** Speech animation changed. */
	virtual void SAnimationChanged( saeSAnimation *sanimation );
	/** Model changed. */
	virtual void ModelChanged( saeSAnimation *sanimation );
	/** Sky changed. */
	virtual void SkyChanged( saeSAnimation *sanimation );
	/** Camera changed. */
	virtual void CameraChanged( saeSAnimation *sanimation );
	
	/** Phoneme count or order changed. */
	virtual void PhonemeStructureChanged( saeSAnimation *sanimation );
	/** Phoneme changed. */
	virtual void PhonemeChanged( saeSAnimation *sanimation, saePhoneme *phoneme );
	/** Active phoneme changed. */
	virtual void ActivePhonemeChanged( saeSAnimation *sanimation );
	
	/** Word count or order changed. */
	virtual void WordStructureChanged( saeSAnimation *sanimation );
	/** Word name changed. */
	virtual void WordNameChanged( saeSAnimation *sanimation, saeWord *word );
	/** Word changed. */
	virtual void WordChanged( saeSAnimation *sanimation, saeWord *word );
	/** Active word changed. */
	virtual void ActiveWordChanged( saeSAnimation *sanimation );
	/*@}*/
};

#endif
