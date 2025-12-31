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

#ifndef _SYNEWPAPANELSOURCECHAIN_H_
#define _SYNEWPAPANELSOURCECHAIN_H_

#include "syneWPAPanelSource.h"

#include <deigde/gui/composed/igdeEditPath.h>


/**
 * \brief Chain source panel.
 */
class syneWPAPanelSourceChain : public syneWPAPanelSource{
public:
	typedef deTObjectReference<syneWPAPanelSourceChain> Ref;
	
private:
	igdeListBox::Ref pListSounds;
	
	igdeEditPath::Ref pEditPathSound;
	igdeTextField::Ref pEditMinSpeed;
	igdeTextField::Ref pEditMaxSpeed;
	igdeTextField::Ref pLabSoundInfo;
	
	igdeAction::Ref pActionSoundUp;
	igdeAction::Ref pActionSoundDown;
	igdeAction::Ref pActionSoundAdd;
	igdeAction::Ref pActionSoundRemove;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	syneWPAPanelSourceChain(syneWPSource &wpSource);
	
protected:
	/** \brief Clean up panel. */
	~syneWPAPanelSourceChain() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Update synthesizer. */
	void UpdateSynthesizer() override;
	
	/** \brief Synthesizer path changed. */
	void OnSynthesizerPathChanged() override;
	
	/** \brief Update source. */
	void UpdateSource() override;
	
	/** \brief Update target list. */
	void UpdateTargetList() override;
	
	/** \brief Update sound list. */
	void UpdateSoundList();
	
	/** \brief Update sound. */
	void UpdateSound();
	
	/** \brief Update sound information. */
	void UpdateSoundInfo();
	
	/** \brief Selected sound in list. */
	int GetSelectedSoundInList() const;
	
	/** \brief Select sound in list. */
	void SelectSoundInList(int index);
	
	
	
	/** \brief Actions. */
	inline const igdeAction::Ref &GetActionSoundUp() const{ return pActionSoundUp; }
	inline const igdeAction::Ref &GetActionSoundDown() const{ return pActionSoundDown; }
	inline const igdeAction::Ref &GetActionSoundAdd() const{ return pActionSoundAdd; }
	inline const igdeAction::Ref &GetActionSoundRemove() const{ return pActionSoundRemove; }
	/*@}*/
};

#endif
