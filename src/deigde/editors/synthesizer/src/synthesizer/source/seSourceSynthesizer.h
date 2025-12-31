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
#include "../controller/seController.h"

#include <dragengine/common/collection/decTList.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>

class deSynthesizerSourceSynthesizer;


/**
 * Synthesizer source synthesizer.
 */
class seSourceSynthesizer : public seSource{
public:
	typedef deTObjectReference<seSourceSynthesizer> Ref;
	typedef decTObjectList<seController> ConnectionList;
	
private:
	deEngine *pEngine;
	
	decString pPathSynthesizer;
	deSynthesizer::Ref pChildSynthesizer;
	decStringList pControllerNames;
	ConnectionList pConnections;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create source. */
	seSourceSynthesizer(deEngine *engine);
	
	/** Create copy of source. */
	seSourceSynthesizer(const seSourceSynthesizer &copy);
	
	/** Clean up source. */
protected:
	~seSourceSynthesizer() override;
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Path to child synthesizer. */
	inline const decString &GetPathSynthesizer() const{ return pPathSynthesizer; }
	
	/** Set path to child synthesizer. */
	void SetPathSynthesizer(const char *path);
	
	/** Child synthesizer or \em nullptr if absent. */
	inline const deSynthesizer::Ref &GetChildSynthesizer() const{ return pChildSynthesizer; }
	
	/** Update child synthesizer using stored path. */
	void UpdateChildSynthesizer();
	
	
	
	/** Controller names. */
	inline const decStringList &GetControllerNames() const{ return pControllerNames; }
	
	
	
	/** Connections. */
	inline const ConnectionList &GetConnections() const{ return pConnections; }
	
	/** Set number of connections. */
	void SetConnectionCount(int count);
	
	/** Set controller for child controller or \em nullptr if not set. */
	void SetControllerAt(int position, seController *controller);
	
	
	
	/** Create an engine synthesizer source. */
	virtual deSynthesizerSource::Ref CreateEngineSource();
	
	/** Create a copy of this source. */
	seSource::Ref CreateCopy() const override;
	
	/** List all links of all source targets. */
	void ListLinks(seLink::List& list) override;
	
	/** Parent synthesizer changed. */
	void SynthesizerChanged() override;
	
	/** Synthesizer directory changed. */
	void SynthesizerDirectoryChanged() override;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Copy another sub synthesizer source to this sub synthesizer source. */
	virtual seSourceSynthesizer &operator=(const seSourceSynthesizer &copy);
	/*@}*/
	
	
	
private:
	void pUpdateConnections(deSynthesizerSourceSynthesizer &source) const;
};

#endif
