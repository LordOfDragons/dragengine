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

#ifndef _SELSSYNTHESIZER_H_
#define _SELSSYNTHESIZER_H_

#include "../synthesizer/source/seSource.h"
#include "../synthesizer/effect/seEffect.h"

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class seSynthesizer;
class seController;
class seLink;
class seLinkMapping;
class seEffectStretch;
class seSourceSound;
class seSourceWave;
class seSourceChain;
class seSourceGroup;
class seSourceSynthesizer;
class seControllerTarget;
class seLoadSaveSystem;

class decXmlWriter;
class decXmlElementTag;
class decBaseFileReader;
class decBaseFileWriter;



/**
 * \brief Load/Save Synthesizer.
 */
class seLoadSaveSynthesizer : public igdeBaseXML{
private:
	seLoadSaveSystem *pLSSys;
	decString pName;
	decString pPattern;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create loader. */
	seLoadSaveSynthesizer(seLoadSaveSystem *lssys, deLogger *logger, const char *loggerSource);
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** \brief Load synthesizer from file. */
	void LoadSynthesizer(seSynthesizer &synthesizer, decBaseFileReader &reader);
	
	/** \brief Save synthesizer to file. */
	void SaveSynthesizer(const seSynthesizer &synthesizer, decBaseFileWriter &writer);
	/*@}*/
	
	
	
private:
	void pWriteSynthesizer(decXmlWriter &writer, const seSynthesizer &synthesizer);
	
	void pWriteController(decXmlWriter &writer, const seController &controller);
	
	void pWriteLink(decXmlWriter &writer, const seSynthesizer &synthesizer, const seLink &link);
	
	void pWriteSourceCommon(decXmlWriter &writer, const seSynthesizer &synthesizer,
		const seSource &source);
	
	void pWriteControllerTarget(decXmlWriter &writer, const seSynthesizer &synthesizer,
		const seControllerTarget &target, const char *name);
	
	void pWriteSource(decXmlWriter &writer, const seSynthesizer &synthesizer, const seSource &source);
	
	void pWriteSourceSound(decXmlWriter &writer, const seSynthesizer &synthesizer,
		const seSourceSound &source);
	
	void pWriteSourceWave(decXmlWriter &writer, const seSynthesizer &synthesizer,
		const seSourceWave &source);
	
	void pWriteSourceChain(decXmlWriter &writer, const seSynthesizer &synthesizer,
		const seSourceChain &source);
	
	void pWriteSourceGroup(decXmlWriter &writer, const seSynthesizer &synthesizer,
		const seSourceGroup &source);
	
	void pWriteSourceSynthesizer(decXmlWriter &writer, const seSynthesizer &synthesizer,
		const seSourceSynthesizer &source);
	
	void pWriteEffectCommon(decXmlWriter &writer, const seSynthesizer &synthesizer,
		const seEffect &effect);
	
	void pWriteEffect(decXmlWriter &writer, const seSynthesizer &synthesizer, const seEffect &effect);
	
	void pWriteEffectStretch(decXmlWriter &writer, const seSynthesizer &synthesizer,
		const seEffectStretch &effect);
	
	
	
	void pReadSynthesizer(const decXmlElementTag &root, seSynthesizer &synthesizer);
	
	void pReadController(const decXmlElementTag &root, seSynthesizer &synthesizer);
	
	void pReadControllerLimit(const decXmlElementTag &root, seController &controller);
	
	void pReadLink(const decXmlElementTag &root, seSynthesizer &synthesizer);
	
	seSource::Ref pReadSource(const decXmlElementTag &root, seSynthesizer &synthesizer);
	
	seSource::Ref pReadSourceSound(const decXmlElementTag &root, seSynthesizer &synthesizer);
	
	seSource::Ref pReadSourceWave(const decXmlElementTag &root, seSynthesizer &synthesizer);
	
	seSource::Ref pReadSourceChain(const decXmlElementTag &root, seSynthesizer &synthesizer);
	
	seSource::Ref pReadSourceGroup(const decXmlElementTag &root, seSynthesizer &synthesizer);
	
	seSource::Ref pReadSourceSynthesizer(const decXmlElementTag &root, seSynthesizer &synthesizer);
	
	bool pReadSourceCommon(const decXmlElementTag &root, seSynthesizer &synthesizer, seSource &source);
	
	void pReadControllerTarget(const decXmlElementTag &root,
		seSynthesizer &synthesizer, seControllerTarget &target);
	
	seEffect::Ref pReadEffect(const decXmlElementTag &root, seSynthesizer &synthesizer);
	
	seEffect::Ref pReadEffectStretch(const decXmlElementTag &root, seSynthesizer &synthesizer);
	
	bool pReadEffectCommon(const decXmlElementTag &root, seSynthesizer &synthesizer, seEffect &effect);
};

#endif
