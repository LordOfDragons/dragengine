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

#ifndef _DEOALATLEAKTRACKER_H_
#define _DEOALATLEAKTRACKER_H_

#include <dragengine/common/collection/decPointerSet.h>
#include <dragengine/threading/deMutex.h>

class deoalAudioThread;


// enable leak tracking.
// #define ENABLE_LEAK_TRACKING


#ifdef ENABLE_LEAK_TRACKING
	#define IF_LEAK_CHECK(cmd) cmd
	
	#define LEAK_CHECK_CREATE(audioThread,type) \
		(audioThread).GetLeakTracker().Add ## type(this)
	
	#define LEAK_CHECK_FREE(audioThread,type) \
		(audioThread).GetLeakTracker().Remove ## type(this)
	
	#define LEAK_CHECK_REPORT_LEAKS(audioThread,type) \
		(audioThread).GetLeakTracker().ReportLeaks(#type, \
			(audioThread).GetLeakTracker().Get ## type())
	
#else
	#define IF_LEAK_CHECK(cmd)
	
	#define LEAK_CHECK_CREATE(audioThread,type)
	#define LEAK_CHECK_FREE(audioThread,type)
	
	#define LEAK_CHECK_REPORT_LEAKS(audioThread,type)
#endif


/**
 * \brief Audio thread leak tracker.
 */
class deoalATLeakTracker{
private:
	deoalAudioThread &pAudioThread;
	deMutex pMutex;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render thread texture related object. */
	deoalATLeakTracker(deoalAudioThread &audioThread);
	
	/** \brief Clean up render thread texture related object. */
	virtual ~deoalATLeakTracker();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Report leaks. */
	void ReportLeaks(const char *name, const decPointerSet &tracker);
	/*@}*/
	
	
	
#ifdef ENABLE_LEAK_TRACKING
#define DECLARE_TRACKER(type) \
private: decPointerSet p ## type; \
public: inline decPointerSet &Get ## type(){return p ## type;} \
public: inline void Add ## type(void *object){AddTracked(p ## type, object);} \
public: inline void Remove ## type(void *object){RemoveTracked(p ## type, object);}
	DECLARE_TRACKER(Component)
	DECLARE_TRACKER(Microphone)
	DECLARE_TRACKER(Model)
	DECLARE_TRACKER(Skin)
	DECLARE_TRACKER(Sound)
	DECLARE_TRACKER(SoundLevelMeter)
	DECLARE_TRACKER(Speaker)
	DECLARE_TRACKER(SynthesizerInstance)
	DECLARE_TRACKER(VideoPlayer)
	DECLARE_TRACKER(World)
	
	void AddTracked(decPointerSet &list, void *object);
	void RemoveTracked(decPointerSet &list, void *object);
#endif
};

#endif
