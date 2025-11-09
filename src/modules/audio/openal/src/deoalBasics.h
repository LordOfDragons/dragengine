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

#ifndef _DEOALBASICS_H_
#define _DEOALBASICS_H_


#include <dragengine/dragengine_configuration.h>

#include "extensions/al.h"
#include "extensions/alc.h"
#include "extensions/efx.h"
#include "extensions/deoalExtResult.h"

class deoalAudioThread;


#ifdef WITH_DEBUG
#define OAL_THREAD_CHECK 1
#endif

#ifdef OAL_THREAD_CHECK
	void oaldbgInitThreadCheck();
	#define OAL_INIT_THREAD_CHECK		oaldbgInitThreadCheck()
	void oaldbgInitMainThreadCheck();
	#define OAL_INIT_MAIN_THREAD_CHECK	oaldbgInitMainThreadCheck()
	
	void oaldbgOnMainThreadCheck();
	#define OAL_ON_MAIN_THREAD			oaldbgOnMainThreadCheck();
	
#else
	#define OAL_INIT_THREAD_CHECK
	#define OAL_ON_MAIN_THREAD
#endif



#ifdef WITH_DEBUG
#define OAL_CHECKCOMMANDS 1
#endif

#ifdef OAL_CHECKCOMMANDS
void dbgCheckOalError( deoalAudioThread &audioThread, const char *file, int line );
#	define OAL_CHECK(audioThread,cmd)	alGetError(); cmd; dbgCheckOalError( audioThread, __FILE__, __LINE__ )
#	define OAL_IF_CHECK(audioThread)	audioThread
#else
#	define OAL_CHECK(audioThread,cmd)	cmd
#	define OAL_IF_CHECK(audioThread)
#endif



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



/** \brief Sound speed (m/s). */
#define SOUND_SPEED 343.0f

/** \brief Inverse sound speed (s/m). */
#define INV_SOUND_SPEED ( 1.0f / SOUND_SPEED )


#endif
