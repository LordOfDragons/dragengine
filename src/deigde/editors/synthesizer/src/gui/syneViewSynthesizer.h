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

#ifndef _SYNEVIEWSYNTHESIZER_H_
#define _SYNEVIEWSYNTHESIZER_H_

#include "properties/syneWPSource.h"
#include "properties/syneWPLink.h"
#include "properties/syneWPController.h"
#include "properties/syneWPSynthesizer.h"
#include "../synthesizer/syneSynthesizer.h"

#include <deigde/gui/layout/igdeContainerBorder.h>

class syneWindowMain;


/**
 * \brief Synthesizer view.
 */
class syneViewSynthesizer : public igdeContainerBorder{
public:
	typedef deTObjectReference<syneViewSynthesizer> Ref;
	
private:
	syneWindowMain &pWindowMain;
	
	syneSynthesizer::Ref pSynthesizer;
	
	syneWPController::Ref pWPController;
	syneWPLink::Ref pWPLink;
	syneWPSource::Ref pWPSource;
	syneWPSynthesizer::Ref pWPSynthesizer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view. */
	syneViewSynthesizer(syneWindowMain &windowMain);
	
protected:
	/** \brief Clean up view. */
	virtual ~syneViewSynthesizer();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline syneWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Reset view. */
	void ResetView();
	
	/** \brief Synthesizer or \em nullptr if not set. */
	inline const syneSynthesizer::Ref &GetSynthesizer() const{ return pSynthesizer; }
	
	/** \brief Set synthesizer or \em nullptr if not set. */
	void SetSynthesizer(syneSynthesizer *synthesizer);
	
	/** \brief Synthesizer path changed. */
	void OnSynthesizerPathChanged();
	
	/** \brief Game like frame update. */
	void OnFrameUpdate(float elapsed);
	/*@}*/
};

#endif
