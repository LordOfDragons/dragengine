/* 
 * Drag[en]gine OpenAL Audio Module
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
