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

#ifndef _SESOURCESYNTHESIZER_H_
#define _SESOURCESYNTHESIZER_H_

#include "seSource.h"

#include <dragengine/common/string/decString.h>

class seController;

class deSynthesizer;
class deSynthesizerSourceSynthesizer;



/**
 * \brief Synthesizer source synthesizer.
 */
class seSourceSynthesizer : public seSource{
private:
	deEngine *pEngine;
	
	decString pPathSynthesizer;
	deSynthesizer::Ref pChildSynthesizer;
	decString *pControllerNames;
	int pControllerNameCount;
	
	seController **pConnections;
	int pConnectionCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create source. */
	seSourceSynthesizer(deEngine *engine);
	
	/** \brief Create copy of source. */
	seSourceSynthesizer(const seSourceSynthesizer &copy);
	
	/** \brief Clean up source. */
	~seSourceSynthesizer() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Path to child synthesizer. */
	inline const decString &GetPathSynthesizer() const{ return pPathSynthesizer; }
	
	/** \brief Set path to child synthesizer. */
	void SetPathSynthesizer(const char *path);
	
	/** \brief Child synthesizer or \em NULL if absent. */
	inline const deSynthesizer::Ref &GetChildSynthesizer() const{ return pChildSynthesizer; }
	
	/** \brief Update child synthesizer using stored path. */
	void UpdateChildSynthesizer();
	
	
	
	/** \brief Number of controller names. */
	inline int GetControllerNameCount() const{ return pControllerNameCount; }
	
	/** \brief Controller name at position. */
	const decString &GetControllerNameAt(int position) const;
	
	
	
	/** \brief Number of connections. */
	inline int GetConnectionCount() const{ return pConnectionCount; }
	
	/** \brief Set number of connections. */
	void SetConnectionCount(int count);
	
	/** \brief Controller for child controller or \em NULL if not set. */
	seController *GetControllerAt(int position) const;
	
	/** \brief Set controller for child controller or \em NULL if not set. */
	void SetControllerAt(int position, seController *controller);
	
	
	
	/** \brief Create an engine synthesizer source. */
	virtual deSynthesizerSource *CreateEngineSource();
	
	/** \brief Create a copy of this source. */
	seSource *CreateCopy() const override;
	
	/** \brief List all links of all source targets. */
	void ListLinks(seLinkList& list) override;
	
	/** \brief Parent synthesizer changed. */
	void SynthesizerChanged() override;
	
	/** \brief Synthesizer directory changed. */
	void SynthesizerDirectoryChanged() override;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another sub synthesizer source to this sub synthesizer source. */
	virtual seSourceSynthesizer &operator=(const seSourceSynthesizer &copy);
	/*@}*/
	
	
	
private:
	void pUpdateConnections(deSynthesizerSourceSynthesizer &source) const;
};

#endif
