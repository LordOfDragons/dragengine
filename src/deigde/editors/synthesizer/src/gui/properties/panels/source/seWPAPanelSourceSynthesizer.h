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

#ifndef _SEWPAPANELSOURCESYNTHESIZER_H_
#define _SEWPAPANELSOURCESYNTHESIZER_H_

#include "seWPAPanelSource.h"

#include <deigde/gui/composed/igdeEditPath.h>



/**
 * \brief Synthesizer source panel.
 */
class seWPAPanelSourceSynthesizer : public seWPAPanelSource{
private:
	igdeEditPath::Ref pEditPathSynthesizer;
	igdeComboBox::Ref pCBConTarget;
	igdeButton::Ref pBtnConTargetMore;
	igdeButton::Ref pBtnConTargetLess;
	igdeComboBox::Ref pCBConController;
	bool pPreventUpdate;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	seWPAPanelSourceSynthesizer(seWPSource &wpSources);
	
	/** \brief Clean up panel. */
	virtual ~seWPAPanelSourceSynthesizer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Update connection target list. */
	void UpdateConTargetList();
	
	/** \brief Selected controller target. */
	int GetCBControllerTarget() const;
	
	/** \brief Synthesizer path changed. */
	virtual void OnSynthesizerPathChanged();
	
	/** \brief Update controller list. */
	virtual void UpdateControllerList();
	
	/** \brief Update source. */
	virtual void UpdateSource();
	
	/** \brief Update connection. */
	virtual void UpdateConnection();
	
	/** \brief Prevent update for listener use only. */
	inline bool GetPreventUpdate() const{ return pPreventUpdate; }
	/*@}*/
};

#endif
