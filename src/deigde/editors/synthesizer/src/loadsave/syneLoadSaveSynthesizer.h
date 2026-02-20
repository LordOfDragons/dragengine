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

#ifndef _SYNELSSYNTHESIZER_H_
#define _SYNELSSYNTHESIZER_H_

#include "../synthesizer/source/syneSource.h"
#include "../synthesizer/effect/syneEffect.h"

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class syneSynthesizer;
class syneController;
class syneLink;
class syneLinkMapping;
class syneEffectStretch;
class syneSourceSound;
class syneSourceWave;
class syneSourceChain;
class syneSourceGroup;
class syneSourceSynthesizer;
class syneControllerTarget;
class syneLoadSaveSystem;

class decXmlWriter;
class decXmlElementTag;
class decBaseFileReader;
class decBaseFileWriter;



/**
 * \brief Load/Save Synthesizer.
 */
class syneLoadSaveSynthesizer : public igdeBaseXML{
private:
	syneLoadSaveSystem *pLSSys;
	decString pName;
	decString pPattern;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create loader. */
	syneLoadSaveSynthesizer(syneLoadSaveSystem *lssys, deLogger *logger, const char *loggerSource);
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** \brief Load synthesizer from file. */
	void LoadSynthesizer(syneSynthesizer &synthesizer, decBaseFileReader &reader);
	
	/** \brief Save synthesizer to file. */
	void SaveSynthesizer(const syneSynthesizer &synthesizer, decBaseFileWriter &writer);
	/*@}*/
	
	
	
private:
	void pWriteSynthesizer(decXmlWriter &writer, const syneSynthesizer &synthesizer);
	
	void pWriteController(decXmlWriter &writer, const syneController &controller);
	
	void pWriteLink(decXmlWriter &writer, const syneSynthesizer &synthesizer, const syneLink &link);
	
	void pWriteSourceCommon(decXmlWriter &writer, const syneSynthesizer &synthesizer,
		const syneSource &source);
	
	void pWriteControllerTarget(decXmlWriter &writer, const syneSynthesizer &synthesizer,
		const syneControllerTarget &target, const char *name);
	
	void pWriteSource(decXmlWriter &writer, const syneSynthesizer &synthesizer, const syneSource &source);
	
	void pWriteSourceSound(decXmlWriter &writer, const syneSynthesizer &synthesizer,
		const syneSourceSound &source);
	
	void pWriteSourceWave(decXmlWriter &writer, const syneSynthesizer &synthesizer,
		const syneSourceWave &source);
	
	void pWriteSourceChain(decXmlWriter &writer, const syneSynthesizer &synthesizer,
		const syneSourceChain &source);
	
	void pWriteSourceGroup(decXmlWriter &writer, const syneSynthesizer &synthesizer,
		const syneSourceGroup &source);
	
	void pWriteSourceSynthesizer(decXmlWriter &writer, const syneSynthesizer &synthesizer,
		const syneSourceSynthesizer &source);
	
	void pWriteEffectCommon(decXmlWriter &writer, const syneSynthesizer &synthesizer,
		const syneEffect &effect);
	
	void pWriteEffect(decXmlWriter &writer, const syneSynthesizer &synthesizer, const syneEffect &effect);
	
	void pWriteEffectStretch(decXmlWriter &writer, const syneSynthesizer &synthesizer,
		const syneEffectStretch &effect);
	
	
	
	void pReadSynthesizer(const decXmlElementTag &root, syneSynthesizer &synthesizer);
	
	void pReadController(const decXmlElementTag &root, syneSynthesizer &synthesizer);
	
	void pReadControllerLimit(const decXmlElementTag &root, syneController &controller);
	
	void pReadLink(const decXmlElementTag &root, syneSynthesizer &synthesizer);
	
	syneSource::Ref pReadSource(const decXmlElementTag &root, syneSynthesizer &synthesizer);
	
	syneSource::Ref pReadSourceSound(const decXmlElementTag &root, syneSynthesizer &synthesizer);
	
	syneSource::Ref pReadSourceWave(const decXmlElementTag &root, syneSynthesizer &synthesizer);
	
	syneSource::Ref pReadSourceChain(const decXmlElementTag &root, syneSynthesizer &synthesizer);
	
	syneSource::Ref pReadSourceGroup(const decXmlElementTag &root, syneSynthesizer &synthesizer);
	
	syneSource::Ref pReadSourceSynthesizer(const decXmlElementTag &root, syneSynthesizer &synthesizer);
	
	bool pReadSourceCommon(const decXmlElementTag &root, syneSynthesizer &synthesizer, syneSource &source);
	
	void pReadControllerTarget(const decXmlElementTag &root,
		syneSynthesizer &synthesizer, syneControllerTarget &target);
	
	syneEffect::Ref pReadEffect(const decXmlElementTag &root, syneSynthesizer &synthesizer);
	
	syneEffect::Ref pReadEffectStretch(const decXmlElementTag &root, syneSynthesizer &synthesizer);
	
	bool pReadEffectCommon(const decXmlElementTag &root, syneSynthesizer &synthesizer, syneEffect &effect);
};

#endif
