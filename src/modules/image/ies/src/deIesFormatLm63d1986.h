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

#ifndef _DEIESFORMATLM63D1986_H_
#define _DEIESFORMATLM63D1986_H_

#include "deIesImageInfo.h"

#include <dragengine/common/math/decMath.h>

class decBaseFileReader;


/**
 * \brief IES Format LM-63-1986.
 */
class deIesFormatLm63d1986 : public deIesImageInfo{
protected:
	struct sAngleFactor{
		float angle;
		float factor;
	};
	
	struct sSamplePoint{
		int angle1;
		int angle2;
		int index1;
		int index2;
		float blend1;
		float blend2;
		void Set(int index);
		void Set(int index, float blend);
	};
	
	enum ePhotometricType{
		eptTypeC,
		eptTypeB,
		eptTypeA
	};
	
	enum eUnitsType{
		eutFeet,
		eutMeters
	};
	
	enum eImageType{
		eitEquirectangular,
		eitCubeMap
	};
	
	eImageType pImageType;
	
	decString pTilt;
	
	int pAngleFactorCount;
	sAngleFactor *pAngleFactors;
	
	int pLampCount;
	float pLumensPerLamp;
	float pCandelaMultiplier;
	
	int pVerticalAngleCount;
	int pHorizontalAngleCount;
	
	ePhotometricType pPhotometricType;
	eUnitsType pUnitsType;
	
	float pLumWidth;
	float pLumHeight;
	float pLumLength;
	
	float pBallastFactor;
	float pFutureBallastFactor;
	
	float pFinalLumMultiplier;
	
	float *pVerticalAngles;
	float *pHorizontalAngles;
	float *pCandelaValues;
	
	float pSmallestVerticalStep;
	float pSmallestHorizontalStep;
	int pVerticalResolution;
	int pHorizontalResolution;
	sSamplePoint *pVerticalSamplePoints;
	sSamplePoint *pHorizontalSamplePoints;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create image information. */
	deIesFormatLm63d1986();
	
	/** \brief Clean up image information. */
	virtual ~deIesFormatLm63d1986();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Load header. */
	virtual bool LoadHeader(decBaseFileReader &reader);
	
	/** \brief Load file. */
	virtual void LoadFile(unsigned short *pixels);
	/*@}*/
	
	
	
protected:
	void pReadValues(decStringList &values, int count);
	bool pFindTilt();
	void pReadTilt();
	void pReadLampConfig();
	void pReadBallast();
	void pCalcFinalLumMuliplier();
	void pReadVerticalAngles();
	void pReadHorizontalAngles();
	void pFindSmallestSteps();
	void pReadCandelaValues();
	void pNormalizeCandelaValues();
	void pSanitizeCandelaValues();
	void pGammaCorrectCandelaValues();
	void pCreateSamplePoints();
	bool pIsAngle(float angle, float requiredAngle);
	void pFillSamples(int index, sSamplePoint *samples, int sampleCount);
	void pSample(const float *angles, int angleCount, sSamplePoint *samples, int sampleCount);
	void pMirrorSamples(const sSamplePoint *samplesFrom, sSamplePoint *samplesTo, int sampleCount);
	void pCopySamples(const sSamplePoint *samplesFrom, sSamplePoint *samplesTo, int sampleCount);
	void pSetPixelsEquirect(unsigned short *pixels);
	void pSetPixelsCubemap(unsigned short *pixels);
	void pSetPixelsCubemapFace(unsigned short *pixels, const decMatrix &matrix);
	void pGetMatrixForFace(decMatrix &matrix, int face);
};

#endif
