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

#ifndef _DEOALENVIRONMENTDEBUG_H_
#define _DEOALENVIRONMENTDEBUG_H_

#include "deoalEnvProbeListenerSmooth.h"
#include "raytrace/deoalEnergyHistogram.h"

#include <dragengine/common/math/decMath.h>

class deoalAudioThread;
class deoalEnvProbe;
class deoalEnvProbeListener;
class deoalEnvironment;
class deoalImpuseResponse;

class deDebugBlockInfo;
class deDebugDrawer;



/**
 * \brief Environment debugging.
 */
class deoalEnvironmentDebug{
private:
	const deoalEnvironment &pEnvironment;
	
	decPoint pHistogramSize;
	
	deoalImpulseResponse pImpulseResponse;
	float pImpulseResponseScale;
	deoalEnergyHistogram pEnergyHistogram;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create environment debugging. */
	deoalEnvironmentDebug( const deoalEnvironment &environment);
	
	/** \brief Clean up environment debugging. */
	~deoalEnvironmentDebug();
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/**
	 * \brief Update debug information.
	 * \warning Called during synchronization time from main thread.
	 */
	void Print();
	
	void Prepare( const deoalEnvProbe &envProbe, const deoalEnvProbeListener listener );
	
	void UpdateInfo( deDebugBlockInfo &debugInfo );
	
	void SoundRays( deDebugDrawer &debugDrawer, const deoalEnvProbe &envProbe );
	/*@}*/
	
	
	
private:
	void pShowImpulseResponse( deDebugBlockInfo &debugInfo );
	void pShowEnergyHistogram( deDebugBlockInfo &debugInfo );
	decColor pColorGradient( float value ) const;
};

#endif
