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

#ifndef _DESYNBASICS_H_
#define _DESYNBASICS_H_



class deDESynthesizer;



/** \brief 8-bit sample type. */
typedef signed char Sample8;

/** \brief 16-bit sample type. */
typedef signed short Sample16;



/** \brief 8-bit mono sample. */
struct sSampleMono8{
	Sample8 value;
};

/** \brief 8-bit stereo sample. */
struct sSampleStereo8{
	Sample8 left;
	Sample8 right;
};

/** \brief 16-bit mono sample. */
struct sSampleMono16{
	Sample16 value;
};

/** \brief 16-bit stereo sample. */
struct sSampleStereo16{
	Sample16 left;
	Sample16 right;
};



/** \brief 8-bit sample zero value. */
#define SAMPLE8_ZERO		(Sample8)0

/** \brief 8-bit sample minimum value. */
#define SAMPLE8_MIN			(Sample8)-128

/** \brief 8-bit sample minimum value. */
#define SAMPLE8_MIN_F		-128.0f

/** \brief 8-bit sample maximum value. */
#define SAMPLE8_MAX			(Sample8)127

/** \brief 8-bit sample maximum value. */
#define SAMPLE8_MAX_F		127.0f

/** \brief 8-bit sample range. */
#define SAMPLE8_RANGE		(Sample8)127

/** \brief 8-bit sample range as floating point value. */
#define SAMPLE8_RANGE_F		127.0f

/** \brief 8-bit sample to floating point value. */
inline float sample8ToFloat(const Sample8 sample){ return (1.0f / SAMPLE8_RANGE_F) * (float)sample; }



/** \brief 16-bit sample zero value. */
#define SAMPLE16_ZERO		(Sample16)0

/** \brief 16-bit sample minimum value. */
#define SAMPLE16_MIN		(Sample16)-32768

/** \brief 16-bit sample minimum value. */
#define SAMPLE16_MIN_F		-32768.0f

/** \brief 16-bit sample maximum value. */
#define SAMPLE16_MAX		(Sample16)32767

/** \brief 16-bit sample maximum value. */
#define SAMPLE16_MAX_F		32767.0f

/** \brief 16-bit sample range. */
#define SAMPLE16_RANGE		(Sample16)32767

/** \brief 16-bit sample range as floating point value. */
#define SAMPLE16_RANGE_F	32767.0f

/** \brief 16-bit sample to floating point value. */
inline float sample16ToFloat(const Sample16 sample){ return (1.0f / SAMPLE16_RANGE_F) * (float)sample; }



#endif
