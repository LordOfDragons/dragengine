/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "deoglRenderBase.h"
#include "plan/deoglRenderPlan.h"
#include "defren/deoglDeferredRendering.h"

#include "../configuration/deoglConfiguration.h"
#include "../capabilities/deoglCapabilities.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTDebug.h"
#include "../debug/deoglDebugInformation.h"
#include "../devmode/deoglDeveloperMode.h"
#include "../shaders/deoglShaderDefines.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderBase
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderBase::deoglRenderBase( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ){
}

deoglRenderBase::~deoglRenderBase(){
}



// Management
///////////////

void deoglRenderBase::AddBasicDefines( deoglShaderDefines &defines ){
	if( pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Broken() ){
		defines.AddDefine( "UBO_IDMATACCBUG", "1" );
	}
	
	if( pRenderThread.GetCapabilities().GetUBODirectLinkDeadloop().Broken() ){
		defines.AddDefine( "BUG_UBO_DIRECT_LINK_DEAD_LOOP", "1" );
	}
}

deoglTexSamplerConfig &deoglRenderBase::GetSamplerClampNearest() const{
	return *pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampNearest );
}

deoglTexSamplerConfig &deoglRenderBase::GetSamplerClampLinear() const{
	return *pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampLinear );
}

deoglTexSamplerConfig &deoglRenderBase::GetSamplerClampNearestMipMap() const{
	return *pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampNearestMipMap );
}

deoglTexSamplerConfig &deoglRenderBase::GetSamplerClampLinearMipMap() const{
	return *pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampLinearMipMap );
}

deoglTexSamplerConfig &deoglRenderBase::GetSamplerClampLinearMipMapNearest() const{
	return *pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampLinearMipMapNearest );
}

deoglTexSamplerConfig &deoglRenderBase::GetSamplerRepeatNearest() const{
	return *pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscRepeatNearest );
}

deoglTexSamplerConfig &deoglRenderBase::GetSamplerRepeatLinear() const{
	return *pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscRepeatLinear );
}

deoglTexSamplerConfig &deoglRenderBase::GetSamplerRepeatLinearMipMap() const{
	return *pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscRepeatLinearMipMap );
}

deoglTexSamplerConfig &deoglRenderBase::GetSamplerShadowClampLinear() const{
	return *pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscShadowClampLinear );
}

deoglTexSamplerConfig &deoglRenderBase::GetSamplerShadowClampLinearInverse() const{
	if( pRenderThread.GetDeferredRendering().GetUseInverseDepth() ){
		return *pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscShadowClampLinearInverse );
		
	}else{
		return *pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscShadowClampLinear );
	}
}



void deoglRenderBase::SetCullMode( bool renderBackFaces ){
	if( renderBackFaces ){
		OGL_CHECK( GetRenderThread(), glCullFace( GL_FRONT ) );
		
	}else{
		OGL_CHECK( GetRenderThread(), glCullFace( GL_BACK ) );
	}
}



// Debug
//////////

void deoglRenderBase::DebugTimersReset( const deoglRenderPlan &plan, bool waitGPU ) {
	if( ! plan.GetDebugTiming() ){
		return;
	}
	
	if( waitGPU && pRenderThread.GetDebug().GetDeveloperMode().GetDebugInfoSync() ){
		glFinish();
	}
	
	pDebugTimer[ 0 ].Reset();
	pDebugTimer[ 1 ].Reset();
	pDebugTimer[ 2 ].Reset();
	pDebugTimer[ 3 ].Reset();
}

void deoglRenderBase::DebugTimer1Reset( const deoglRenderPlan &plan, bool waitGPU ){
	pDebugTimerReset( pDebugTimer[ 0 ], plan, waitGPU );
}

void deoglRenderBase::DebugTimer1Sample( const deoglRenderPlan &plan,
deoglDebugInformation &debugInfo, bool waitGPU ){
	pDebugTimerSample( pDebugTimer[ 0 ], plan, debugInfo, waitGPU );
}

void deoglRenderBase::DebugTimer1SampleCount( const deoglRenderPlan &plan,
deoglDebugInformation &debugInfo, int count, bool waitGPU ){
	pDebugTimerSampleCount( pDebugTimer[ 0 ], plan, debugInfo, count, waitGPU );
}

void deoglRenderBase::DebugTimer2Reset( const deoglRenderPlan &plan, bool waitGPU ){
	pDebugTimerReset( pDebugTimer[ 1 ], plan, waitGPU );
}

void deoglRenderBase::DebugTimer2Sample( const deoglRenderPlan &plan,
deoglDebugInformation &debugInfo, bool waitGPU ){
	pDebugTimerSample( pDebugTimer[ 1 ], plan, debugInfo, waitGPU );
}

void deoglRenderBase::DebugTimer2SampleCount( const deoglRenderPlan &plan,
deoglDebugInformation &debugInfo, int count, bool waitGPU ){
	pDebugTimerSampleCount( pDebugTimer[ 1 ], plan, debugInfo, count, waitGPU );
}

void deoglRenderBase::DebugTimer3Reset( const deoglRenderPlan &plan, bool waitGPU ){
	pDebugTimerReset( pDebugTimer[ 2 ], plan, waitGPU );
}

void deoglRenderBase::DebugTimer3Sample( const deoglRenderPlan &plan,
deoglDebugInformation &debugInfo, bool waitGPU ){
	pDebugTimerSample( pDebugTimer[ 2 ], plan, debugInfo, waitGPU );
}

void deoglRenderBase::DebugTimer3SampleCount( const deoglRenderPlan &plan,
deoglDebugInformation &debugInfo, int count, bool waitGPU ){
	pDebugTimerSampleCount( pDebugTimer[ 2 ], plan, debugInfo, count, waitGPU );
}

void deoglRenderBase::DebugTimer4Reset( const deoglRenderPlan &plan, bool waitGPU ){
	pDebugTimerReset( pDebugTimer[ 3 ], plan, waitGPU );
}

void deoglRenderBase::DebugTimer4Sample( const deoglRenderPlan &plan,
deoglDebugInformation &debugInfo, bool waitGPU ){
	pDebugTimerSample( pDebugTimer[ 3 ], plan, debugInfo, waitGPU );
}

void deoglRenderBase::DebugTimer4SampleCount( const deoglRenderPlan &plan,
deoglDebugInformation &debugInfo, int count, bool waitGPU ){
	pDebugTimerSampleCount( pDebugTimer[ 3 ], plan, debugInfo, count, waitGPU );
}



void deoglRenderBase::AddTopLevelDebugInfo(){
}

void deoglRenderBase::DevModeDebugInfoChanged(){
}



// Private Functions
//////////////////////

void deoglRenderBase::pDebugTimerReset( decTimer &timer, const deoglRenderPlan &plan, bool waitGPU ){
	if( ! plan.GetDebugTiming() ){
		return;
	}
	
	if( waitGPU && pRenderThread.GetDebug().GetDeveloperMode().GetDebugInfoSync() ){
		glFinish();
	}
	timer.Reset();
}

void deoglRenderBase::pDebugTimerSample( decTimer &timer, const deoglRenderPlan &plan,
deoglDebugInformation &debugInfo, bool waitGPU ){
	if( ! plan.GetDebugTiming() || ! debugInfo.GetVisible() ){
		return;
	}
	
	if( waitGPU && pRenderThread.GetDebug().GetDeveloperMode().GetDebugInfoSync() ){
		glFinish();
	}
	debugInfo.IncrementElapsedTime( timer.GetElapsedTime() );
}

void deoglRenderBase::pDebugTimerSampleCount( decTimer &timer, const deoglRenderPlan &plan,
deoglDebugInformation &debugInfo, int count, bool waitGPU ){
	if( ! plan.GetDebugTiming() || ! debugInfo.GetVisible() ){
		return;
	}
	
	if( waitGPU && pRenderThread.GetDebug().GetDeveloperMode().GetDebugInfoSync() ){
		glFinish();
	}
	debugInfo.IncrementElapsedTime( timer.GetElapsedTime() );
	debugInfo.IncrementCounter( count );
}
