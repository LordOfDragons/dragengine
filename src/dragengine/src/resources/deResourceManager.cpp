/* 
 * Drag[en]gine Game Engine
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
	if( ! engine ){
		DETHROW( deeInvalidParam );
	}
	if( type < ertAnimation || type > ertWorld ){
		DETHROW( deeInvalidParam );
	}
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
