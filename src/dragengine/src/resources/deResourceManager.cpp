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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deResource.h"
#include "deResourceManager.h"
#include "../deEngine.h"
#include "../logger/deLogger.h"
#include "../common/exceptions.h"



// Definitions
////////////////

#define LOGSOURCE "Dragengine"



// Class deResource
/////////////////////

// Constructor, destructor
////////////////////////////

deResourceManager::deResourceManager( deEngine *engine, eResourceType type ) :
pEngine( engine ),
pResourceType( type )
{
	DEASSERT_NOTNULL( engine )
}

deResourceManager::~deResourceManager(){
}



// Management
///////////////

void deResourceManager::SetLoggingName( const char *name ){
	pLoggingName = name;
}

void deResourceManager::ReleaseLeakingResources(){
}



// Convenience Functions
//////////////////////////

deModuleSystem *deResourceManager::GetModuleSystem() const{
	return pEngine->GetModuleSystem();
}

deGraphicSystem *deResourceManager::GetGraphicSystem() const{
	return pEngine->GetGraphicSystem();
}

dePhysicsSystem *deResourceManager::GetPhysicsSystem() const{
	return pEngine->GetPhysicsSystem();
}

deScriptingSystem *deResourceManager::GetScriptingSystem() const{
	return pEngine->GetScriptingSystem();
}

deAnimatorSystem *deResourceManager::GetAnimatorSystem() const{
	return pEngine->GetAnimatorSystem();
}

deAISystem *deResourceManager::GetAISystem() const{
	return pEngine->GetAISystem();
}

deAudioSystem *deResourceManager::GetAudioSystem() const{
	return pEngine->GetAudioSystem();
}

deNetworkSystem *deResourceManager::GetNetworkSystem() const{
	return pEngine->GetNetworkSystem();
}

deImageManager *deResourceManager::GetImageManager() const{
	return pEngine->GetImageManager();
}

deVideoManager *deResourceManager::GetVideoManager() const{
	return pEngine->GetVideoManager();
}

deSkinManager *deResourceManager::GetSkinManager() const{
	return pEngine->GetSkinManager();
}

deSynthesizerSystem *deResourceManager::GetSynthesizerSystem() const{
	return pEngine->GetSynthesizerSystem();
}



// Systems Support
////////////////////

void deResourceManager::SystemGraphicLoad(){
}

void deResourceManager::SystemGraphicUnload(){
}

void deResourceManager::SystemPhysicsLoad(){
}

void deResourceManager::SystemPhysicsUnload(){
}

void deResourceManager::SystemScriptingLoad(){
}

void deResourceManager::SystemScriptingUnload(){
}

void deResourceManager::SystemAnimatorLoad(){
}

void deResourceManager::SystemAnimatorUnload(){
}

void deResourceManager::SystemAudioLoad(){
}

void deResourceManager::SystemAudioUnload(){
}

void deResourceManager::SystemNetworkLoad(){
}

void deResourceManager::SystemNetworkUnload(){
}

void deResourceManager::SystemAILoad(){
}

void deResourceManager::SystemAIUnload(){
}

void deResourceManager::SystemSynthesizerLoad(){
}

void deResourceManager::SystemSynthesizerUnload(){
}



// Debugging
//////////////

void deResourceManager::LogInfo( const char *message ){
	pEngine->GetLogger()->LogInfo( LOGSOURCE, message );
}

void deResourceManager::LogInfoFormat( const char *message, ... ){
	va_list list;
	va_start( list, message );
	pEngine->GetLogger()->LogInfoFormatUsing( LOGSOURCE, message, list );
	va_end( list );
}

void deResourceManager::LogInfoFormatUsing( const char *message, va_list args ){
	pEngine->GetLogger()->LogInfoFormatUsing( LOGSOURCE, message, args );
}

void deResourceManager::LogWarn( const char *message ){
	pEngine->GetLogger()->LogWarn( LOGSOURCE, message );
}

void deResourceManager::LogWarnFormat( const char *message, ... ){
	va_list list;
	va_start( list, message );
	pEngine->GetLogger()->LogWarnFormatUsing( LOGSOURCE, message, list );
	va_end( list );
}

void deResourceManager::LogWarnFormatUsing( const char *message, va_list args ){
	pEngine->GetLogger()->LogWarnFormatUsing( LOGSOURCE, message, args );
}

void deResourceManager::LogError( const char *message ){
	pEngine->GetLogger()->LogError( LOGSOURCE, message );
}

void deResourceManager::LogErrorFormat( const char *message, ... ){
	va_list list;
	va_start( list, message );
	pEngine->GetLogger()->LogErrorFormatUsing( LOGSOURCE, message, list );
	va_end( list );
}

void deResourceManager::LogErrorFormatUsing( const char *message, va_list args ){
	pEngine->GetLogger()->LogErrorFormatUsing( LOGSOURCE, message, args );
}

void deResourceManager::LogException( const deException &exception ){
	pEngine->GetLogger()->LogException( LOGSOURCE, exception );
}
