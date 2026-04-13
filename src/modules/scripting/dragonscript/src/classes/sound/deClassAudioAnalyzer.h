/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DECLASSAUDIOANALYZER_H_
#define _DECLASSAUDIOANALYZER_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;
class deAudioAnalyzer;


/**
 * \brief Audio analyzer script class.
 * \version 1.32
 */
class deClassAudioAnalyzer : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	explicit deClassAudioAnalyzer(deScriptingDragonScript &ds);
	
	/** \brief Clean up script class. */
	~deClassAudioAnalyzer() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create script class members. */
	void CreateClassMembers(dsEngine *engine) override;
	
	/** \brief Audio analyzer from real object or nullptr if real object is nullptr. */
	deAudioAnalyzer *GetAudioAnalyze(dsRealObject *myself) const;
	
	/** \brief Push audio analyzer or nullptr onto the stack. */
	void PushAudioAnalyze(dsRunTime *rt, deAudioAnalyzer *analyzer);
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsAudioAnalyzer;
		
		dsClass *clsVoid;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsBool;
		dsClass *clsObject;
	};
	
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetUseAudioCapture);
	DEF_NATFUNC(nfSetUseAudioCapture);
	DEF_NATFUNC(nfGetSpectralPeaksCount);
	DEF_NATFUNC(nfSetSpectralPeaksCount);
	DEF_NATFUNC(nfGetFrequencyBandsCount);
	DEF_NATFUNC(nfSetFrequencyBandsCount);
	DEF_NATFUNC(nfUpdateResults);
	
	DEF_NATFUNC(nfGetRMS);
	DEF_NATFUNC(nfGetPeak);
	DEF_NATFUNC(nfGetZeroCrossingRate);
	DEF_NATFUNC(nfGetSpectralCentroid);
	DEF_NATFUNC(nfGetSpectralFlatness);
	DEF_NATFUNC(nfGetSpectralFlux);
	DEF_NATFUNC(nfGetSpectralRolloff);
	DEF_NATFUNC(nfGetSpectralPeakAt);
	DEF_NATFUNC(nfGetFrequencyBandAt);
	DEF_NATFUNC(nfGetPitch);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

#endif
