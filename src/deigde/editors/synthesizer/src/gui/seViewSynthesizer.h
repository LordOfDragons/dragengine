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

#ifndef _SEVIEWSYNTHESIZER_H_
#define _SEVIEWSYNTHESIZER_H_

#include <deigde/gui/layout/igdeContainerBorder.h>

class seWPSource;
class seWPLink;
class seWPController;
class seWPSynthesizer;
class seWindowMain;
class seSynthesizer;



/**
 * \brief Synthesizer view.
 */
class seViewSynthesizer : public igdeContainerBorder{
private:
	seWindowMain &pWindowMain;
	
	seSynthesizer *pSynthesizer;
	
	seWPController *pWPController;
	seWPLink *pWPLink;
	seWPSource *pWPSource;
	seWPSynthesizer *pWPSynthesizer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view. */
	seViewSynthesizer( seWindowMain &windowMain );
	
protected:
	/** \brief Clean up view. */
	virtual ~seViewSynthesizer();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline seWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Reset view. */
	void ResetView();
	
	/** \brief Synthesizer or \em NULL if not set. */
	inline seSynthesizer *GetSynthesizer() const{ return pSynthesizer; }
	
	/** \brief Set synthesizer or \em NULL if not set. */
	void SetSynthesizer( seSynthesizer *synthesizer );
	
	/** \brief Synthesizer path changed. */
	void OnSynthesizerPathChanged();
	
	/** \brief Game like frame update. */
	void OnFrameUpdate( float elapsed );
	/*@}*/
};

#endif
