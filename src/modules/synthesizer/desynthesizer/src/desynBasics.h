/* 
 * Drag[en]gine Synthesizer Module
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
#define SAMPLE8_ZERO		( Sample8 )0

/** \brief 8-bit sample minimum value. */
#define SAMPLE8_MIN			( Sample8 )-128

/** \brief 8-bit sample minimum value. */
#define SAMPLE8_MIN_F		-128.0f

/** \brief 8-bit sample maximum value. */
#define SAMPLE8_MAX			( Sample8 )127

/** \brief 8-bit sample maximum value. */
#define SAMPLE8_MAX_F		127.0f

/** \brief 8-bit sample range. */
#define SAMPLE8_RANGE		( Sample8 )127

/** \brief 8-bit sample range as floating point value. */
#define SAMPLE8_RANGE_F		127.0f

/** \brief 8-bit sample to floating point value. */
inline float sample8ToFloat( const Sample8 sample ){ return ( 1.0f / SAMPLE8_RANGE_F ) * ( float )sample; }



/** \brief 16-bit sample zero value. */
#define SAMPLE16_ZERO		( Sample16 )0

/** \brief 16-bit sample minimum value. */
#define SAMPLE16_MIN		( Sample16 )-32768

/** \brief 16-bit sample minimum value. */
#define SAMPLE16_MIN_F		-32768.0f

/** \brief 16-bit sample maximum value. */
#define SAMPLE16_MAX		( Sample16 )32767

/** \brief 16-bit sample maximum value. */
#define SAMPLE16_MAX_F		32767.0f

/** \brief 16-bit sample range. */
#define SAMPLE16_RANGE		( Sample16 )32767

/** \brief 16-bit sample range as floating point value. */
#define SAMPLE16_RANGE_F	32767.0f

/** \brief 16-bit sample to floating point value. */
inline float sample16ToFloat( const Sample16 sample ){ return ( 1.0f / SAMPLE16_RANGE_F ) * ( float )sample; }



#endif
