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

#include "deoglRModel.h"
#include "deoglModelLOD.h"
#include "face/deoglModelFace.h"
#include "texture/deoglModelTexture.h"
#include "../deoglCaches.h"
#include "../deGraphicOpenGl.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../imposter/deoglImposterBillboard.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../shaders/paramblock/shared/deoglSharedSPBListUBO.h"
#include "../utils/vcoptimizer/deoglVCOptimizer.h"
#include "../utils/vcoptimizer/deoglVCSimulator.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/filesystem/deCacheHelper.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelBone.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/model/deModelVertexPositionSet.h>
#include <dragengine/threading/deMutexGuard.h>



// Definitions
////////////////

// Cache version in the range from 0 to 255. Increment each time the cache
// format changed. If reaching 256 wrap around to 0. Important is only the
// number changes to force discarding old caches
#define CACHE_VERSION 5



// Class deoglRModel
/////////////////////

// Constructor, destructor
////////////////////////////

deoglRModel::deoglRModel( deoglRenderThread &renderThread, const deModel &model ) :
pRenderThread( renderThread ),
pFilename( model.GetFilename() ),
pHasWeightlessExtends( false ),
pBoneExtends( NULL ),
pBoneCount( 0 ),
pLODs( NULL ),
pLODCount( 0 ),
pDoubleSided( false ),
pIsCached( false ),
pImposterBillboard( NULL ),
pSharedSPBListUBO( NULL )
{
// 	decTimer timer;
	// try loading from cache if present
	try{
		pLoadCached( model.GetLODCount(), model.GetBoneCount() );
		
	}catch( const deException & ){
		if( pLODs ){
			int i;
			for( i=0; i<pLODCount; i++ ){
				delete pLODs[ i ];
			}
			delete [] pLODs;
			pLODs = NULL;
			pLODCount = 0;
		}
		
		pDoubleSided = false;
		pIsCached = false;
	}
	
	// if cache is not present or loading the cache failed go on without
	try{
		if( ! pIsCached ){
			pInitLODs( model );
			pSaveCached();
		}
		
		// debug
		/*
		for( int i=0; i<pLODCount; i++ ){
			const deoglModelLOD &lod = *pLODs[ i ];
			renderThread.GetOgl().LogInfoFormat( "Model '%s' LOD %i: faces=%i double=%d decal=%d error(%g,%g)",
				pFilename.GetString(), i, lod.GetFaceCount(), lod.GetDoubleSided(), lod.GetDecal(),
				lod.GetAvgError(), lod.GetMaxError() );
		}
		*/
		// debug
		
		pInitBoneNames( model );
		pInitTextureNames( model );
		pInitVPSNames( model );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	//ogl->LogInfoFormat( "model %s: constructor %ims", pFilename.GetString(), ( int )( timer.GetElapsedTime() * 1000.0f ) );
	
// 	pOgl->LogInfoFormat( "Loaded '%s' in %fs", pFilename.GetString(), timer.GetElapsedTime() );
	LEAK_CHECK_CREATE( renderThread, Model );
}

deoglRModel::~deoglRModel(){
	LEAK_CHECK_FREE( pRenderThread, Model );
	pCleanUp();
}



// Management
///////////////

deoglModelLOD &deoglRModel::GetLODAt( int index ) const{
	if( index < 0 ){
		index += pLODCount;
	}
	if( index < 0 || index >= pLODCount ){
		DETHROW( deeInvalidParam );
	}
	return *pLODs[ index ];
}

void deoglRModel::PrepareImposterBillboard(){
	if( ! pImposterBillboard ){
		pCreateImposterBillboard();
	}
}

deoglSharedSPBListUBO &deoglRModel::GetSharedSPBListUBO(){
	if( ! pSharedSPBListUBO ){
		pSharedSPBListUBO = new deoglSharedSPBListUBO( pRenderThread,
			pRenderThread.GetBufferObject().GetLayoutSkinInstanceUBO() );
	}
	return *pSharedSPBListUBO;
}



void deoglRModel::PrepareOctrees(){
	int i;
	for( i=0; i<pLODCount; i++ ){
		pLODs[ i ]->PrepareOctree();
	}
}

void deoglRModel::PrintDebugInfo(){
	if( pLODCount < 2 ){
		return;
	}
	
	pRenderThread.GetLogger().LogInfoFormat( "model '%s' lod information:", pFilename.GetString() );
	
	int i;
	for( i=1; i<pLODCount; i++ ){
		pRenderThread.GetLogger().LogInfoFormat( "- lod %i: maxError=%g avgError=%f", i,
			pLODs[ i ]->GetMaxError(), pLODs[ i ]->GetAvgError() );
	}
}

void deoglRModel::DebugVCOptimize(){
	deoglRTLogger &logger = pRenderThread.GetLogger();
	decTimer timer;
	
	const deoglModelLOD &lod = *pLODs[ 0 ];
	const deoglModelFace * const lodFaces = lod.GetFaces();
	deoglVCOptimizer optimizer;
	int i, count;
	
	// optimize
	timer.Reset();
	
	optimizer.SetVertexCount( lod.GetVertexCount() );
	count = lod.GetFaceCount();
	optimizer.SetFaceCount( count );
	for( i=0; i<count; i++ ){
		optimizer.SetFaceAt( i , lodFaces[ i ].GetVertex1(), lodFaces[ i ].GetVertex2(), lodFaces[ i ].GetVertex3() );
	}
	count = lod.GetTextureCount();
	optimizer.SetGroupCount( count );
	for( i=0; i<count; i++ ){
		optimizer.SetGroupAt( i, lod.GetTextureAt( i ).GetFirstFace(), lod.GetTextureAt( i ).GetFaceCount() );
	}
	
	optimizer.Optimize();
	//optimizer.DebugPrint( pRenderThread );
	
	const float elapsedOptimize = timer.GetElapsedTime();
	
	// simulate
	const int * const reorderedFaces = optimizer.GetReorderedFaces();
	int orgCacheMissCount, newCacheMissCount;
	int orgCacheHitCount, newCacheHitCount;
	deoglVCSimulator simulator;
	float orgAcmr, newAcmr;
	int j;
	
	simulator.SetCacheSize( 32 );
	
	logger.LogInfoFormat( "simulate model %s", pFilename.GetString() );
	count = lod.GetFaceCount();
	
	for( i=0; i<lod.GetTextureCount(); i++ ){
		// simulate original face order
		simulator.Reset();
		
		for( j=0; j<count; j++ ){
			const deoglModelFace &lodFace = lodFaces[ j ];
			
			if( lodFace.GetTexture() == i ){
				simulator.ProcessFace( lodFace.GetVertex1(), lodFace.GetVertex2(), lodFace.GetVertex3() );
			}
		}
		
		if( simulator.GetFaceCount() > 0 ){
			orgCacheHitCount = simulator.GetCacheHitCount();
			orgCacheMissCount = simulator.GetCacheMissCount();
			orgAcmr = simulator.GetAvgCacheMissRatio();
			
			// simulate new face order
			simulator.Reset();
			
			for( j=0; j<count; j++ ){
				const deoglModelFace &lodFace = lodFaces[ reorderedFaces[ j ] ];
				
				if( lodFace.GetTexture() == i ){
					simulator.ProcessFace( lodFace.GetVertex1(), lodFace.GetVertex2(), lodFace.GetVertex3() );
				}
			}
			
			newCacheHitCount = simulator.GetCacheHitCount();
			newCacheMissCount = simulator.GetCacheMissCount();
			newAcmr = simulator.GetAvgCacheMissRatio();
			
			// results
			logger.LogInfoFormat( "- texture %i: %i faces", i, simulator.GetFaceCount() );
			logger.LogInfoFormat( "  - original order: cacheHitCount=%i cacheMissCount=%i acmr=%.2f",
				orgCacheHitCount, orgCacheMissCount, orgAcmr );
			logger.LogInfoFormat( "  - new order: cacheHitCount=%i cacheMissCount=%i acmr=%.2f",
				newCacheHitCount, newCacheMissCount, newAcmr );
			logger.LogInfoFormat( "  - result: cacheHitCount=%i%% cacheMissCount=%i%% acmr=%i%%",
				( int )( ( float )newCacheHitCount / ( float )orgCacheHitCount * 100.0f ),
				( int )( ( float )newCacheMissCount / ( float )orgCacheMissCount * 100.0f ),
				( int )( newAcmr / orgAcmr * 100.0f ) );
		}
	}
	
	const float elapsedSimulate = timer.GetElapsedTime();
	logger.LogInfoFormat( "  - elapsed: optimize=%ims simulate=%ims",
		( int )( elapsedOptimize * 1e3f ), ( int )( elapsedSimulate * 1e3f ) );
}



// Private functions
//////////////////////

void deoglRModel::pCleanUp(){
	if( pBoneExtends ){
		delete [] pBoneExtends;
		pBoneExtends = NULL;
	}
	if( pSharedSPBListUBO ){
		delete pSharedSPBListUBO;
	}
	if( pImposterBillboard ){
		delete pImposterBillboard;
	}
	if( pLODs ){
		int i;
		for( i=0; i<pLODCount; i++ ){
			delete pLODs[ i ];
		}
		delete [] pLODs;
	}
}



void deoglRModel::pInitBoneNames( const deModel &engModel ){
	const int count = engModel.GetBoneCount();
	int i;
	for( i=0; i<count; i++ ){
		pBoneNames.Add( engModel.GetBoneAt( i )->GetName() );
	}
}

void deoglRModel::pInitTextureNames( const deModel &engModel ){
	const int count = engModel.GetTextureCount();
	int i;
	for( i=0; i<count; i++ ){
		pTextureNames.Add( engModel.GetTextureAt( i )->GetName() );
	}
}

void deoglRModel::pInitVPSNames( const deModel &engModel ){
	const int count = engModel.GetVertexPositionSetCount();
	int i;
	for( i=0; i<count; i++ ){
		pVPSNames.Add( engModel.GetVertexPositionSetAt( i )->GetName() );
	}
}

void deoglRModel::pInitLODs( const deModel &engModel ){
	const int lodCount = engModel.GetLODCount();
	
	//pRenderThread.GetOgl().LogInfoFormat( "Model '%s': Init without cache", pFilename .GetString());
	
	pLODs = new deoglModelLOD*[ lodCount ];
	
	if( lodCount == 0 ){
		return;
	}
	
	// init base lod
	pLODs[ 0 ] = new deoglModelLOD( *this, pLODCount, engModel );
	pDoubleSided = pLODs[ 0 ]->GetDoubleSided();
	pLODCount = 1;
	
	// init extends. this has to come now and not after higher lods since error calculation
	// requires creating octrees which in turn require the base lod extends
	pInitExtends( engModel, *pLODs[ 0 ] );
	
	// init higher lod levels
	for( ; pLODCount<lodCount; pLODCount++ ){
		pLODs[ pLODCount ] = new deoglModelLOD( *this, pLODCount, engModel );
		if( pLODs[ pLODCount ]->GetDoubleSided() ){
			pDoubleSided = true;
		}
	}
}

void deoglRModel::pInitExtends( const deModel &engModel, const deoglModelLOD &baseLod ){
	// extends of all points
	const oglModelPosition * const positions = baseLod.GetPositions();
	const int positionCount = baseLod.GetPositionCount();
	int i;
	
	if( positionCount > 0 ){
		pExtends.minimum = pExtends.maximum = positions[ 0 ].position;
		for( i=1; i<positionCount; i++ ){
			const decVector &position = positions[ i ].position;
			pExtends.minimum.SetSmallest( position );
			pExtends.maximum.SetLargest( position );
		}
		
	}else{
		pExtends.minimum.SetZero();
		pExtends.maximum.SetZero();
	}
	
	// extends of all points without weights and bone extends
	const int boneCount = engModel.GetBoneCount();
	if( boneCount == 0 ){
		pWeightlessExtends.minimum = pExtends.minimum;
		pWeightlessExtends.maximum = pExtends.maximum;
		pHasWeightlessExtends = true;
		return;
	}
	
	pWeightlessExtends.minimum.SetZero();
	pWeightlessExtends.maximum.SetZero();
	
	pBoneExtends = new sExtends[ boneCount ];
	pBoneCount = boneCount;
	
	const int weightsCount = baseLod.GetWeightsCount();
	if( weightsCount > 0 ){
		const int * const weightsCounts = baseLod.GetWeightsCounts();
		const oglModelWeight *weightEntries = baseLod.GetWeightsEntries();
		int * const dominatingBones = new int[ weightsCount ];
		bool * const boneHasExtends = new bool[ boneCount ];
		
		for( i=0; i<boneCount; i++ ){
			boneHasExtends[ i ] = false;
		}
		
		for( i=0; i<weightsCount; i++ ){
			dominatingBones[ i ] = -1;
			
			const int weightEntryCount = weightsCounts[ i ];
			if( weightEntryCount == 0 ){
				continue;
			}
			
			if( weightEntries->weight > 0.001f ){ // 0.499f ){
				dominatingBones[ i ] = weightEntries->bone;
			}
			weightEntries += weightEntryCount;
		}
		
		for( i=0; i<positionCount; i++ ){
			const decVector &position = positions[ i ].position;
			
			int dominatingBone = -1;
			if( positions[ i ].weights != -1 ){
				dominatingBone = dominatingBones[ positions[ i ].weights ];
			}
			
			if( dominatingBone != -1 ){
				sExtends &boneExtends = pBoneExtends[ dominatingBone ];
				const decVector p( engModel.GetBoneAt( dominatingBone )->GetInverseMatrix() * position );
				
				if( boneHasExtends[ dominatingBone ] ){
					boneExtends.minimum.SetSmallest( p );
					boneExtends.maximum.SetLargest( p );
					
				}else{
					boneExtends.minimum = boneExtends.maximum = p;
					boneHasExtends[ dominatingBone ] = true;
				}
				
			}else{
				if( pHasWeightlessExtends ){
					pWeightlessExtends.minimum.SetSmallest( position );
					pWeightlessExtends.maximum.SetLargest( position );
					
				}else{
					pWeightlessExtends.minimum = pWeightlessExtends.maximum = position;
					pHasWeightlessExtends = true;
				}
			}
		}
		
		delete [] boneHasExtends;
		delete [] dominatingBones;
	}
}

void deoglRModel::pLoadCached( int lodCount, int boneCount ){
	deGraphicOpenGl &ogl = pRenderThread.GetOgl();
	deVirtualFileSystem &vfs = *ogl.GetGameEngine()->GetVirtualFileSystem();
	deoglCaches &caches = ogl.GetCaches();
	deCacheHelper &cacheModels = caches.GetModels();
	decPath path;
	
	path.SetFromUnix(pFilename);
	if(!vfs.CanReadFile(path)){
		return; // without a source file no cache since it is no more unique
	}
	
	try{
		decBaseFileReader::Ref reader;
		{
		const deMutexGuard guard(caches.GetMutex());
		reader.TakeOver(cacheModels.Read(pFilename));
		}
		
		if(reader){
			//ogl.LogInfoFormat("Model '%s': Load from cache", pFilename.GetString());
			
			// check cache version in case we upgraded
			if(reader->ReadByte() != CACHE_VERSION){
				// cache file outdated
				reader = nullptr;
				{
				const deMutexGuard guard(caches.GetMutex());
				cacheModels.Delete(pFilename);
				}
				ogl.LogInfoFormat("Model '%s': Cache version changed. Cache discarded",
					pFilename.GetString());
				return;
			}
			
			// check file modification times to reject the cached file if the source model changed
			const TIME_SYSTEM checkTime = (TIME_SYSTEM)reader->ReadUInt();
			if(vfs.GetFileModificationTime(path) != checkTime){
				// cache file outdated
				reader = nullptr;
				{
				const deMutexGuard guard(caches.GetMutex());
				cacheModels.Delete(pFilename);
				}
				ogl.LogInfoFormat("Model '%s': Modification time changed. Cache discarded",
					pFilename.GetString());
				return;
			}
			
			// create lods
			pLODs = new deoglModelLOD*[lodCount];
			
			for(pLODCount=0; pLODCount<lodCount; pLODCount++){
				pLODs[pLODCount] = new deoglModelLOD(*this, pLODCount, *reader);
				
				if(pLODs[pLODCount]->GetDoubleSided()){
					pDoubleSided = true;
				}
			}
			
			// read extends
			pExtends.minimum = reader->ReadVector();
			pExtends.maximum = reader->ReadVector();
			pWeightlessExtends.minimum = reader->ReadVector();
			pWeightlessExtends.maximum = reader->ReadVector();
			pHasWeightlessExtends = reader->ReadByte() != 0;
			if(boneCount > 0){
				pBoneExtends = new sExtends[boneCount];
				for(pBoneCount=0; pBoneCount<boneCount; pBoneCount++){
					pBoneExtends[pBoneCount].minimum = reader->ReadVector();
					pBoneExtends[pBoneCount].maximum = reader->ReadVector();
				}
			}
			
			// done
			pIsCached = true;
		}
		
	}catch(const deException &e){
		{
		const deMutexGuard guard(caches.GetMutex());
		cacheModels.Delete(pFilename);
		}
		ogl.LogErrorFormat("Model '%s': Loading Cache failed with exception",
			pFilename.GetString());
		ogl.LogException(e);
		pIsCached = false; // safety
		throw;
		
	}catch(...){
		{
		const deMutexGuard guard(caches.GetMutex());
		cacheModels.Delete(pFilename);
		}
		ogl.LogErrorFormat("Model '%s': Loading Cache failed with unknown exception",
			pFilename.GetString());
		pIsCached = false; // safety
		DETHROW(deeInvalidAction);
	}
	
	if(pLODCount == 0){ // sanity check
		DETHROW(deeInvalidParam);
	}
}

void deoglRModel::pSaveCached(){
	deGraphicOpenGl &ogl = pRenderThread.GetOgl();
	deVirtualFileSystem &vfs = *ogl.GetGameEngine()->GetVirtualFileSystem();
	deoglCaches &caches = ogl.GetCaches();
	deCacheHelper &cacheModels = caches.GetModels();
	decPath path;
	int i;
	
	path.SetFromUnix(pFilename);
	if(!vfs.CanReadFile(path)){
		return; // without a source file no cache since it is no more unique
	}
	
	decBaseFileWriter::Ref writer;
	{
	const deMutexGuard guard(caches.GetMutex());
	writer.TakeOver(cacheModels.Write(pFilename));
	}
	
	// write cache version
	writer->WriteByte(CACHE_VERSION);
	
	// write file modification times to reject the cached file if the source model changed
	writer->WriteUInt((unsigned int)vfs.GetFileModificationTime(path));
	
	// write lods
	for(i=0; i<pLODCount; i++){
		pLODs[i]->SaveToCache(*writer);
	}
	
	// write extends
	writer->WriteVector(pExtends.minimum);
	writer->WriteVector(pExtends.maximum);
	writer->WriteVector(pWeightlessExtends.minimum);
	writer->WriteVector(pWeightlessExtends.maximum);
	writer->WriteByte(pHasWeightlessExtends ? 1 : 0);
	for(i=0; i<pBoneCount; i++){
		writer->WriteVector(pBoneExtends[i].minimum);
		writer->WriteVector(pBoneExtends[i].maximum);
	}
	
	// pRenderThread.GetOgl().LogInfoFormat("Model: '%s' written to cache", pFilename.GetString());
}

void deoglRModel::pCreateImposterBillboard(){
	if( pLODCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	pImposterBillboard = new deoglImposterBillboard( pRenderThread );
	
	// calculate the extends from the first lod mesh
	const oglModelPosition * const positions = pLODs[ 0 ]->GetPositions();
	const int positionCount = pLODs[ 0 ]->GetPositionCount();
	decVector axisView, axisUp, position;
	decVector2 minExtend, maxExtend;
	decMatrix matrix;
	int i;
	
	axisUp.Set( 0.0f, 1.0f, 0.0f );
	axisView.Set( 0.0f, 0.0f, -1.0f );
	matrix.SetCamera( decVector(), axisView, axisUp );
	
	for( i=0; i<positionCount; i++ ){
		position = matrix * positions[ i ].position;
		
		if( i == 0 ){
			minExtend.Set( position.x, position.y );
			maxExtend = minExtend;
			
		}else{
			if( position.x < minExtend.x ){
				minExtend.x = position.x;
				
			}else if( position.x > maxExtend.x ){
				maxExtend.x = position.x;
			}
			
			if( position.y < minExtend.y ){
				minExtend.y = position.y;
				
			}else if( position.y > maxExtend.y ){
				maxExtend.y = position.y;
			}
		}
	}
	
	pImposterBillboard->SetExtends( minExtend, maxExtend );
	
	// render the billboard from the first lod mesh
}
