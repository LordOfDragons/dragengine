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

#ifndef _SAEWPSANIM_H_
#define _SAEWPSANIM_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class saeWindowProperties;
class saeWPSAnimListener;
class saeSAnimation;
class saePhoneme;
class saeWord;



/**
 * \brief Speech Animation property window.
 */
class saeWPSAnim : public igdeContainerScroll{
private:
	saeWindowProperties &pWindowProperties;
	
	saeWPSAnimListener *pListener;
	saeSAnimation *pSAnimation;
	
	igdeActionReference pActionWordAddIpa;
	
	igdeEditPathReference pEditRigPath;
	igdeEditPathReference pEditAnimPath;
	igdeComboBoxFilterReference pCBNeutralMove;
	
	igdeComboBoxReference pCBPhoneme;
	igdeTextFieldReference pEditPhonemeIPA;
	igdeTextFieldReference pEditPhonemeSampleText;
	igdeTextFieldReference pEditPhonemeLength;
	igdeComboBoxFilterReference pCBPhonemeMove;
	
	igdeComboBoxFilterReference pCBWord;
	igdeTextFieldReference pEditWordName;
	igdeTextFieldReference pEditWordPhonetics;
	igdeButtonReference pBtnWordAddIPA;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	saeWPSAnim( saeWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~saeWPSAnim();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Properties window. */
	inline saeWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief Speech animation. */
	inline saeSAnimation *GetSAnimation() const{ return pSAnimation; }
	
	/** \brief Set speech animation. */
	void SetSAnimation( saeSAnimation *sanimation );
	
	/** \brief Speech animation path changed. */
	void OnSAnimationPathChanged();
	
	
	
	/** \brief Update speech animation. */
	void UpdateSAnimation();
	
	/** \brief Update neutral move list. */
	void UpdateNeutralMoveList();
	
	
	
	/** \brief Active phoneme or NULL. */
	saePhoneme *GetActivePhoneme() const;
	
	/** \brief Update phoneme list. */
	void UpdatePhonemeList();
	
	/** \brief Change selection to active phoneme. */
	void SelectActivePhoneme();
	
	/** \brief Update phoneme. */
	void UpdatePhoneme();
	
	/** \brief Update phoneme move list. */
	void UpdatePhonemeMoveList();
	
	
	
	/** \brief Active word or NULL. */
	saeWord *GetActiveWord() const;
	
	/** \brief Update word list. */
	void UpdateWordList();
	
	/** \brief Change selection to active word. */
	void SelectActiveWord();
	
	/** \brief Update word. */
	void UpdateWord();
	/*@}*/
};

#endif
