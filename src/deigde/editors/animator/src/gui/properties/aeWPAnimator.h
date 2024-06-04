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

#ifndef _AEWPANIMATOR_H_
#define _AEWPANIMATOR_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>


class aeAnimator;
class aeWindowProperties;
class aeWPAnimatorListener;



/**
 * Animator panel.
 */
class aeWPAnimator : public igdeContainerScroll{
private:
	aeWindowProperties &pWindowProperties;
	aeWPAnimatorListener *pListener;
	aeAnimator *pAnimator;
	
	igdeEditPathReference pEditRigPath;
	igdeEditPathReference pEditAnimPath;
	
	igdeListBoxReference pListBones;
	igdeComboBoxFilterReference pCBBones;
	igdeButtonReference pBtnBoneAdd;
	igdeButtonReference pBtnBoneDel;
	
	igdeListBoxReference pListVertexPositionSets;
	igdeComboBoxFilterReference pCBVertexPositionSets;
	igdeButtonReference pBtnVertexPositionSetAdd;
	igdeButtonReference pBtnVertexPositionSetDel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	aeWPAnimator( aeWindowProperties &windowProperties );
	
protected:
	/** Clean up panel. */
	virtual ~aeWPAnimator();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Properties window. */
	inline aeWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** Animator. */
	inline aeAnimator *GetAnimator() const{ return pAnimator; }
	
	/** Set animator. */
	void SetAnimator( aeAnimator *animator );
	
	/** Update animator. */
	void UpdateAnimator();
	
	/** Animator path changed. */
	void OnAnimatorPathChanged();
	
	/** Update rig bone list. */
	void UpdateRigBoneList();
	
	/** Rig bone combo box text. */
	const decString &GetCBRigBoneText() const;
	
	/** Set rig bone combo box text. */
	void SetCBRigBoneText( const char *text );
	
	/** Update rig vertex position set list. */
	void UpdateModelVertexPositionSetList();
	
	/** Vertex position set combo box text. */
	const decString &GetCBModelVertexPositionSetText() const;
	
	/** Set vertex position set combo box text. */
	void SetCBModelVertexPositionSetText( const char *text );
	/*@}*/
};

#endif
