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
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class saeWindowProperties;
class saeWPSAnimListener;
class saeSAnimation;
class saePhoneme;
class saeWord;



/**
 * Speech Animation property window.
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
	
	igdeListBoxReference pListNeutralVertexPositionSets;
	igdeComboBoxFilterReference pCBNeutralVertexPositionSets;
	igdeButtonReference pBtnNeutralVertexPositionSetAdd;
	igdeButtonReference pBtnNeutralVertexPositionSetDel;
	
	igdeComboBoxReference pCBPhoneme;
	igdeTextFieldReference pEditPhonemeIPA;
	igdeTextFieldReference pEditPhonemeSampleText;
	igdeTextFieldReference pEditPhonemeLength;
	igdeComboBoxFilterReference pCBPhonemeMove;
	igdeComboBoxFilterReference pCBPhonemeVertexPositionSet;
	
	igdeComboBoxFilterReference pCBWord;
	igdeTextFieldReference pEditWordName;
	igdeTextFieldReference pEditWordPhonetics;
	igdeButtonReference pBtnWordAddIPA;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	saeWPSAnim( saeWindowProperties &windowProperties );
	
protected:
	/** Clean up panel. */
	virtual ~saeWPSAnim();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Properties window. */
	inline saeWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** Speech animation. */
	inline saeSAnimation *GetSAnimation() const{ return pSAnimation; }
	
	/** Set speech animation. */
	void SetSAnimation( saeSAnimation *sanimation );
	
	/** Speech animation path changed. */
	void OnSAnimationPathChanged();
	
	
	
	/** Update speech animation. */
	void UpdateSAnimation();
	
	/** Update neutral move list. */
	void UpdateNeutralMoveList();
	
	/** Update neutral vertex position set list. */
	void UpdateNeutralVertexPositionSetList();
	
	/** Neutral vertex position set combo box text. */
	const decString &GetCBNeutralVertexPositionSetText() const;
	
	/** Set neutral vertex position set combo box text. */
	void SetCBNeutralVertexPositionSetText( const char *text );
	
	
	
	/** Active phoneme or NULL. */
	saePhoneme *GetActivePhoneme() const;
	
	/** Update phoneme list. */
	void UpdatePhonemeList();
	
	/** Change selection to active phoneme. */
	void SelectActivePhoneme();
	
	/** Update phoneme. */
	void UpdatePhoneme();
	
	/** Update phoneme move list. */
	void UpdatePhonemeMoveList();
	
	/** Update phoneme vertex position set list. */
	void UpdatePhonemeVertexPositionSetList();
	
	
	
	/** Active word or NULL. */
	saeWord *GetActiveWord() const;
	
	/** Update word list. */
	void UpdateWordList();
	
	/** Change selection to active word. */
	void SelectActiveWord();
	
	/** Update word. */
	void UpdateWord();
	/*@}*/
};

#endif
