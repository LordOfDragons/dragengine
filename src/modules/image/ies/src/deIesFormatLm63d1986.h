/* 
 * Drag[en]gine IES Photometric Image Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEIESFORMATLM63D1986_H_
#define _DEIESFORMATLM63D1986_H_

#include "deIesImageInfo.h"

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
		int index1;
		int index2;
		float blend1;
		float blend2;
		void Set( int index );
		void Set( int index, float blend );
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
	
	int pAngularResolution;
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
	virtual bool LoadHeader( decBaseFileReader &reader );
	
	/** \brief Load file. */
	virtual void LoadFile( unsigned short *pixels );
	/*@}*/
	
	
	
protected:
	void pReadValues( decStringList &values, int count );
	bool pFindTilt();
	void pReadTilt();
	void pReadLampConfig();
	void pReadBallast();
	void pCalcFinalLumMuliplier();
	void pReadVerticalAngles();
	void pReadHorizontalAngles();
	void pReadCandelaValues();
	void pNormalizeCandelaValues();
	void pSanitizeCandelaValues();
	void pGammaCorrectCandelaValues();
	void pCreateSamplePoints();
	bool pIsAngle( float angle, float requiredAngle );
	void pFillSamples( int index, sSamplePoint *samples, int sampleCount );
	void pSample( const float *angles, int angleCount, sSamplePoint *samples, int sampleCount );
	void pMirrorSamples( const sSamplePoint *samplesFrom, sSamplePoint *samplesTo, int sampleCount );
	void pCopySamples( const sSamplePoint *samplesFrom, sSamplePoint *samplesTo, int sampleCount );
	void pSetPixelsEquirect( unsigned short *pixels );
	void pSetPixelsCubemap( unsigned short *pixels );
};

#endif
