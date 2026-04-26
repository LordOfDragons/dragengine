/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deoglRTCBackend.h"
#include "../deoglRTContext.h"


// Class deoglRTCBackend
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTCBackend::deoglRTCBackend(deoglRTContext &context) :
pRTContext(context),
pCompileContextCount(0){
}

deoglRTCBackend::~deoglRTCBackend() = default;


// Management
///////////////

void deoglRTCBackend::InitPhase2(){
}

void deoglRTCBackend::InitPhase3(){
}

void deoglRTCBackend::InitPhase4(){
}

void deoglRTCBackend::AssignOSWindow(deRenderWindow*){
}

void deoglRTCBackend::CleanUp(){
}

void deoglRTCBackend::DropCompileContexts(int){
}

void deoglRTCBackend::BeforeDeactivateContext(deoglRRenderWindow&){
}

void deoglRTCBackend::ActivateContext(deoglRRenderWindow&){
}

void deoglRTCBackend::DeactivateContext(){
}

bool deoglRTCBackend::HasLoaderContext() const{
	return false;
}

void deoglRTCBackend::ActivateLoaderContext(){
}

void deoglRTCBackend::DeactivateLoaderContext(){
}

void deoglRTCBackend::ActivateCompileContext(int){
}

void deoglRTCBackend::DeactivateCompileContext(int){
}

void deoglRTCBackend::SwapBuffers(deoglRRenderWindow&){
}

void deoglRTCBackend::ApplyVSync(deoglRRenderWindow&, deoglConfiguration::eVSyncMode){
}

void *deoglRTCBackend::GetFunctionPointer(const char*){
	return nullptr;
}
