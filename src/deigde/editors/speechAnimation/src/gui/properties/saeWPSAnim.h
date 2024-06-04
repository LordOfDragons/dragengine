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
