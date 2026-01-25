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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deErrorTrace.h"
#include "deErrorTracePoint.h"
#include "deErrorTraceValue.h"
#include "../dragengine_configuration.h"
#include "../common/exceptions.h"
#include "../systems/deModuleSystem.h"
#include "../systems/modules/deLoadableModule.h"
#include "../systems/modules/deBaseModule.h"
#include "../systems/modules/deModuleParameter.h"



// class deErrorTrace
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////
#include "../logger/deLogger.h"

deErrorTrace::deErrorTrace() :
pError("Unknown"){
}

deErrorTrace::~deErrorTrace(){
}



// Management
///////////////

void deErrorTrace::SetError(const char *error){
	pError = error;
}

void deErrorTrace::Clear(){
	pError = "Unknown";
	pPoints.RemoveAll();
}



// Trace Point Management
///////////////////////////

const deErrorTracePoint::Ref &deErrorTrace::GetPoint(int index) const{
	return pPoints.GetAt(index);
}

void deErrorTrace::AddPoint(deErrorTracePoint::Ref &&point){
	DEASSERT_NOTNULL(point)
	
	pPoints.Add(std::move(point));
}

void deErrorTrace::RemoveAllPoints(){
	pPoints.RemoveAll();
}



// Convenience Functions
//////////////////////////

deErrorTracePoint *deErrorTrace::AddAndSetIfEmpty(const char *error, deLoadableModule *sourceModule, const char *sourceFunc, int sourceLine){
	if(!error || !sourceFunc || sourceLine < 0) DETHROW(deeInvalidParam);
	if(pPoints.GetCount() > 0) return nullptr;
	pError = error;
	return AddPoint(sourceModule, sourceFunc, sourceLine);
}

const deErrorTracePoint::Ref &deErrorTrace::AddPoint(deLoadableModule *sourceModule, const char *sourceFunc, int sourceLine){
	if(!sourceFunc || sourceLine < 0) DETHROW(deeInvalidParam);
	AddPoint(deErrorTracePoint::Ref::New(sourceModule, sourceFunc, sourceLine));
	return pPoints.Last();
}

void deErrorTrace::PrintTrace(deLogger &logger){
	const char *loggingName = "ErrorTrace";
	pPoints.VisitIndexed([&](int i, const deErrorTracePoint &point){
		if(point.GetSourceModule()){
			logger.LogErrorFormat(loggingName, "%i) %s %s at %i.", i + 1,
				point.GetSourceModule()->GetName().GetString(),
				point.GetSourceFunction().GetString(), point.GetSourceLine());
		}else{
			//moduleType = "Drag[en]gine";
			logger.LogErrorFormat(loggingName, "%i) Game Engine %s at %i.", i + 1,
				point.GetSourceFunction().GetString(), point.GetSourceLine());
		}
		
		point.GetValues().Visit([&](const deErrorTraceValue &value){
			logger.LogErrorFormat(loggingName, "  - %s = '%s'.",
				value.GetName().GetString(), value.GetValue().GetString());
		});
	});
}
