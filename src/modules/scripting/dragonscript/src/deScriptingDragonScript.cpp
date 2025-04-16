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

#include "deScriptingDragonScript.h"
#include "deDSEngineManager.h"
#include "peers/dedsCollider.h"
#include "peers/dedsServer.h"
#include "peers/dedsConnection.h"
#include "peers/dedsNetworkState.h"
#include "peers/dedsTouchSensor.h"
#include "peers/dedsPropField.h"
#include "peers/dedsParticleEmitter.h"
#include "peers/dedsSpeaker.h"
#include "peers/dedsSoundLevelMeter.h"
#include "peers/dedsService.h"
#include "deScriptSource.h"
#include "deClassPathes.h"

//#include "locks/dedsLockManager.h"

#include "classes/deClassCache.h"
#include "classes/deClassEngine.h"
#include "classes/deClassGame.h"
#include "classes/deClassModuleParameter.h"

#include "classes/ai/deClassNavigationSpace.h"
#include "classes/ai/deClassNavigator.h"
#include "classes/ai/deClassNavigatorPath.h"
#include "classes/ai/deClassNavigationInfo.h"
#include "classes/ai/deClassNavigationBlocker.h"
#include "classes/ai/deClassLocomotion.h"

#include "classes/animation/deClassAnimation.h"
#include "classes/animation/deClassAnimationBuilder.h"
#include "classes/animation/deClassAnimator.h"
#include "classes/animation/deClassAnimatorController.h"
#include "classes/animation/deClassAnimatorRule.h"
#include "classes/animation/deClassAnimatorInstance.h"
#include "classes/animation/rules/deClassARAnimation.h"
#include "classes/animation/rules/deClassARAnimationDifference.h"
#include "classes/animation/rules/deClassARAnimationSelect.h"
#include "classes/animation/rules/deClassARBoneTransformator.h"
#include "classes/animation/rules/deClassARForeignState.h"
#include "classes/animation/rules/deClassARGroup.h"
#include "classes/animation/rules/deClassARInverseKinematic.h"
#include "classes/animation/rules/deClassARLimit.h"
#include "classes/animation/rules/deClassARStateManipulator.h"
#include "classes/animation/rules/deClassARStateSnapshot.h"
#include "classes/animation/rules/deClassARSubAnimator.h"
#include "classes/animation/rules/deClassARTrackTo.h"
#include "classes/animation/rules/deClassARMirror.h"

#include "classes/cachedveg/deClassCachedVegetation.h"

#include "classes/canvas/deClassCanvas.h"
#include "classes/canvas/deClassCaptureCanvas.h"
#include "classes/canvas/deClassCanvasCanvasView.h"
#include "classes/canvas/deClassCanvasImage.h"
#include "classes/canvas/deClassCanvasPaint.h"
#include "classes/canvas/deClassCanvasRenderWorld.h"
#include "classes/canvas/deClassCanvasText.h"
#include "classes/canvas/deClassCanvasVideoPlayer.h"
#include "classes/canvas/deClassCanvasView.h"

#include "classes/collider/deClassCollider.h"
#include "classes/collider/deClassColliderBreakingListener.h"
#include "classes/collider/deClassColliderListener.h"
#include "classes/collider/deClassColliderConstraint.h"
#include "classes/collider/deClassColliderCollisionTest.h"
#include "classes/collider/deClassColliderVolume.h"
#include "classes/collider/deClassColliderComponent.h"
#include "classes/collider/deClassColliderRig.h"

#include "classes/curve/deClassCurve2D.h"
#include "classes/curve/deClassCurveBezier.h"
#include "classes/curve/deClassCurveBezier3D.h"
#include "classes/curve/deClassCurveDistanceMapping.h"

#include "classes/debug/deClassDebugDrawer.h"

#include "classes/effects/deClassEffect.h"
#include "classes/effects/deClassEffectColorMatrix.h"
#include "classes/effects/deClassEffectDistortImage.h"
#include "classes/effects/deClassEffectFilterKernel.h"
#include "classes/effects/deClassEffectOverlayImage.h"

#include "classes/file/deClassFileReader.h"
#include "classes/file/deClassFileWriter.h"
#include "classes/file/deClassMemoryFile.h"

#include "classes/graphics/deClassBillboard.h"
#include "classes/graphics/deClassCamera.h"
#include "classes/graphics/deClassColor.h"
#include "classes/graphics/deClassComponent.h"
#include "classes/graphics/deClassEnvMapProbe.h"
#include "classes/graphics/deClassFont.h"
#include "classes/graphics/deClassImage.h"
#include "classes/graphics/deClassImagePixels.h"
#include "classes/graphics/deClassEditableImage.h"
#include "classes/graphics/deClassLight.h"
#include "classes/graphics/deClassLumimeter.h"
#include "classes/graphics/deClassOcclusionMesh.h"
#include "classes/graphics/deClassOcclusionMeshBuilder.h"
#include "classes/graphics/deClassFontBuilder.h"

#include "classes/heightterrain/deClassHeightTerrain.h"

#include "classes/input/deClassInputDevice.h"
#include "classes/input/deClassInputDeviceAxis.h"
#include "classes/input/deClassInputDeviceButton.h"
#include "classes/input/deClassInputDeviceFeedback.h"
#include "classes/input/deClassInputDeviceComponent.h"
#include "classes/input/deClassInputEvent.h"

#include "classes/particle/deClassParticleEmitter.h"
#include "classes/particle/deClassParticleEmitterController.h"
#include "classes/particle/deClassParticleEmitterInstance.h"
#include "classes/particle/deClassParticleEmitterInstanceListener.h"

#include "classes/physics/deClassLayerMask.h"
#include "classes/physics/deClassCollisionFilter.h"
#include "classes/physics/deClassCollisionInfo.h"
#include "classes/physics/deClassForceField.h"
#include "classes/physics/deClassTouchSensor.h"
#include "classes/physics/deClassTouchSensorListener.h"
#include "classes/physics/deClassCollisionTester.h"

#include "classes/propfield/deClassPropField.h"
#include "classes/propfield/deClassPropFieldListener.h"

#include "classes/math/deClassMath.h"
#include "classes/math/deClassColorMatrix.h"
#include "classes/math/deClassDVector.h"
#include "classes/math/deClassDMatrix.h"
#include "classes/math/deClassDMatrix4.h"
#include "classes/math/deClassMatrix.h"
#include "classes/math/deClassMatrix4.h"
#include "classes/math/deClassPoint3.h"
#include "classes/math/deClassPoint.h"
#include "classes/math/deClassQuaternion.h"
#include "classes/math/deClassTexMatrix.h"
#include "classes/math/deClassTexMatrix2.h"
#include "classes/math/deClassVector.h"
#include "classes/math/deClassVector2.h"
#include "classes/math/smooth/deClassSmoothFloat.h"
#include "classes/math/smooth/deClassSmoothDouble.h"
#include "classes/math/smooth/deClassSmoothVector.h"
#include "classes/math/smooth/deClassSmoothVector2.h"
#include "classes/math/smooth/deClassSmoothDVector.h"

#include "classes/network/deClassConnection.h"
#include "classes/network/deClassConnectionListener.h"
#include "classes/network/deClassNetworkMessage.h"
#include "classes/network/deClassNetworkState.h"
#include "classes/network/deClassNetworkStateListener.h"
#include "classes/network/deClassServer.h"
#include "classes/network/deClassServerListener.h"

#include "classes/resources/deClassResourceListener.h"

#include "classes/sound/deClassMicrophone.h"
#include "classes/sound/deClassSound.h"
#include "classes/sound/deClassSpeaker.h"
#include "classes/sound/deClassSoundLevelMeter.h"
#include "classes/sound/deClassSoundLevelMeterSpeaker.h"
#include "classes/sound/deClassSoundLevelMeterListener.h"

#include "classes/service/deClassService.h"
#include "classes/service/deClassServiceListener.h"
#include "classes/service/deClassServiceObject.h"

#include "classes/sky/deClassSky.h"
#include "classes/sky/deClassSkyBody.h"
#include "classes/sky/deClassSkyController.h"
#include "classes/sky/deClassSkyInstance.h"
#include "classes/sky/deClassSkyLayer.h"
#include "classes/sky/deClassSkyLink.h"
#include "classes/sky/deClassSkyTarget.h"

#include "classes/string/deClassStringID.h"
#include "classes/string/deClassUnicodeString.h"
#include "classes/string/deClassUTF8Decoder.h"

#include "classes/synthesizer/deClassSynthesizer.h"
#include "classes/synthesizer/deClassSynthesizerController.h"
#include "classes/synthesizer/deClassSynthesizerEffect.h"
#include "classes/synthesizer/deClassSynthesizerInstance.h"
#include "classes/synthesizer/deClassSynthesizerSource.h"
#include "classes/synthesizer/effects/deClassSEStretch.h"
#include "classes/synthesizer/sources/deClassSSSound.h"
#include "classes/synthesizer/sources/deClassSSWave.h"
#include "classes/synthesizer/sources/deClassSSChain.h"
#include "classes/synthesizer/sources/deClassSSSynthesizer.h"
#include "classes/synthesizer/sources/deClassSSGroup.h"

#include "classes/systems/deClassAISystem.h"
#include "classes/systems/deClassAnimatorSystem.h"
#include "classes/systems/deClassAudioSystem.h"
#include "classes/systems/deClassCrashRecoverySystem.h"
#include "classes/systems/deClassFileSystem.h"
#include "classes/systems/deClassGraphicSystem.h"
#include "classes/systems/deClassInputSystem.h"
#include "classes/systems/deClassPhysicsSystem.h"
#include "classes/systems/deClassNetworkSystem.h"
#include "classes/systems/deClassScriptSystem.h"
#include "classes/systems/deClassSynthesizerSystem.h"
#include "classes/systems/deClassSystem.h"
#include "classes/systems/deClassVRSystem.h"

#include "classes/translation/deClassLanguagePack.h"
#include "classes/translation/deClassLanguagePackBuilder.h"

#include "classes/utils/deClassEasyXMLElement.h"
#include "classes/utils/deClassEasyXML.h"
#include "classes/utils/deClassMutableID.h"
#include "classes/utils/deClassShapeList.h"
#include "classes/utils/deClassUniqueID.h"
#include "classes/utils/deClassRuntimeMeter.h"
#include "classes/utils/deClassSafeArray.h"

#include "classes/video/deClassVideo.h"
#include "classes/video/deClassVideoPlayer.h"

#include "classes/world/deClassDecal.h"
#include "classes/world/deClassDynamicSkin.h"
#include "classes/world/deClassModel.h"
#include "classes/world/deClassModelBuilder.h"
#include "classes/world/deClassRig.h"
#include "classes/world/deClassRigBuilder.h"
#include "classes/world/deClassSkin.h"
#include "classes/world/deClassSkinBuilder.h"
#include "classes/world/deClassWorld.h"

#include "resourceloader/dedsResourceLoader.h"

#include "utils/dedsColliderListenerAdaptor.h"
#include "utils/dedsColliderListenerClosest.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deCmdLineArgs.h>
#include <dragengine/app/deOS.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/exceptions/deException.h>
#include <dragengine/errortracing/deErrorTrace.h>
#include <dragengine/errortracing/deErrorTracePoint.h>
#include <dragengine/errortracing/deErrorTraceValue.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/filesystem/deCollectFileSearchVisitor.h>
#include <dragengine/filesystem/deVFSNull.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/loader/deResourceLoader.h>
#include <dragengine/resources/loader/deResourceLoaderInfo.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/systems/deScriptingSystem.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>

#include <libdscript/exceptions.h>

#ifdef OS_W32_VS
#include <dragengine/app/deOSWindows.h>
#endif

// definitions
#define DESM_GAME_PACKAGE		"GamePackage"

#ifndef DS_MODULE_VERSION
#include "module_version.h"
#endif


// export definition
#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *DSCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif
#endif


// entry function
// used to create a dragonscript scripting module.
// has to be named CreateModule returning deBaseModule.
// returns NULL on error.
/////////////////////////////////////////////////////////

deBaseModule *DSCreateModule( deLoadableModule *loadableModule ){
	try{
		return new deScriptingDragonScript( *loadableModule );
	}catch( ... ){
		return nullptr;
	}
}



// class deScriptingDragonScript
//////////////////////////////////

deScriptingDragonScript::sModuleVersion::sModuleVersion() : major( 0 ), minor( 0 ), patch( 0 ){
}

void deScriptingDragonScript::sModuleVersion::SetVersion( const char *pversion ){
	version = pversion;
	const decStringList parts( version.Split( '.' ) );
	major = parts.GetAt( 0 ).ToInt();
	minor = parts.GetAt( 1 ).ToInt();
	patch = parts.GetCount() > 2 ? parts.GetAt( 2 ).ToInt() : 0;
}

// constructor, destructor
deScriptingDragonScript::deScriptingDragonScript( deLoadableModule &loadableModule ) :
deBaseScriptingModule( loadableModule ),
pState( esStopped ),
pClsAISys( nullptr ),
pClsAnim( nullptr ),
pClsAnimBuilder( nullptr ),
pClsAr( nullptr ),
pClsArI( nullptr ),
pClsAnimatorCtrl( nullptr ),
pClsArR( nullptr ),
pClsAnimatorSystem( nullptr ),
pClsARAnim( nullptr ),
pClsARAnimDiff( nullptr ),
pClsARAnimSelect( nullptr ),
pClsARBoneTrans( nullptr ),
pClsARFSta( nullptr ),
pClsARGroup( nullptr ), 
pClsARIK( nullptr ),
pClsARLimit( nullptr ),
pClsARMirror( nullptr ),
pClsARStaM( nullptr ),
pClsARSnap( nullptr ),
pClsARSubA( nullptr ),
pClsARTrack( nullptr ),
pClsAudSys( nullptr ),
pClsBillboard( nullptr ),
pClsCVeg( nullptr ),
pClsCam( nullptr ),
pClsCanvas( nullptr ),
pClsCanvasCView( nullptr ),
pClsCanvasImage( nullptr ),
pClsCanvasPaint( nullptr ),
pClsCanvasRenW( nullptr ),
pClsCanvasText( nullptr ),
pClsCanvasVidP( nullptr ),
pClsCanvasView( nullptr ),
pClsCapCan( nullptr ),
pClsCache( nullptr ),
pClsCLL( nullptr ),
pClsCBL( nullptr ),
pClsCCon( nullptr ),
pClsCCT( nullptr ),
pClsCol( nullptr ),
pClsColVol( nullptr ),
pClsColComp( nullptr ),
pClsColRig( nullptr ),
pClsCF( nullptr ),
pClsCI( nullptr ),
pClsCT( nullptr ),
pClsClr( nullptr ),
pClsClrMat( nullptr ),
pClsComp( nullptr ),
pClsConL( nullptr ),
pClsCon( nullptr ),
pClsCurve2D( nullptr ),
pClsCBe( nullptr ),
pClsCBezier3D( nullptr ),
pClsCDistMap( nullptr ),
pClsCRSys( nullptr ),
pClsDD( nullptr ),
pClsDec( nullptr ),
pClsDVec( nullptr ),
pClsDMat( nullptr ),
pClsDMat4( nullptr ),
pClsDSkin( nullptr ),
pClsXMLEl( nullptr ),
pClsXML( nullptr ),
pClsEff( nullptr ),
pClsEffClrMat( nullptr ),
pClsEffDistImg( nullptr ),
pClsEffFilKer( nullptr ),
pClsEffOverImg( nullptr ),
pClsEngine( nullptr ),
pClsEnvMapProbe( nullptr ),
pClsFileReader( nullptr ),
pClsFileWriter( nullptr ),
pClsFileSys( nullptr ),
pClsFnt( nullptr ),
pClsFontBuilder( nullptr ),
pClsFF( nullptr ),
pClsGame( nullptr ),
pClsGraSys( nullptr ),
pClsHT( nullptr ),
pClsImg( nullptr ),
pClsImagePixels( nullptr ),
pClsEditableImage( nullptr ),
pClsInpEvent( nullptr ),
pClsInpDev( nullptr ),
pClsInpDevAxis( nullptr ),
pClsInpDevBtn( nullptr ),
pClsInpDevFb( nullptr ),
pClsInpDevComp( nullptr ),
pClsInpSys( nullptr ),
pClsLP( nullptr ),
pClsLangPackBuilder( nullptr ),
pClsLyM( nullptr ),
pClsLig( nullptr ),
pClsLoco( nullptr ),
pClsLM( nullptr ),
pClsMath( nullptr ),
pClsMat( nullptr ),
pClsMat4( nullptr ),
pClsMemoryFile( nullptr ),
pClsMic( nullptr ),
pClsMdl( nullptr ),
pClsModelBuilder( nullptr ),
pClsModPar( nullptr ),
pClsMUID( nullptr ),
pClsNavInfo( nullptr ),
pClsNavSpace( nullptr ),
pClsNavBlocker( nullptr ),
pClsNavigator( nullptr ),
pClsNM( nullptr ),
pClsNSL( nullptr ),
pClsNetSys( nullptr ),
pClsOccM( nullptr ),
pClsOccMBuilder( nullptr ),
pClsNS( nullptr ),
pClsPE( nullptr ),
pClsPEC( nullptr ),
pClsPEI( nullptr ),
pClsPEIL( nullptr ),
pClsPhySys( nullptr ),
pClsPt3( nullptr ),
pClsPt( nullptr ),
pClsPF( nullptr ),
pClsPFL( nullptr ),
pClsQuat( nullptr ),
pClsRN( nullptr ),
pClsRig( nullptr ),
pClsRigBuilder( nullptr ),
pClsRTM( nullptr ),
pClsSvrL( nullptr ),
pClsSA( nullptr ),
pClsShaList( nullptr ),
pClsService( nullptr ),
pClsServiceListener( nullptr ),
pClsServiceObject( nullptr ),
pClsSvr( nullptr ),
pClsSkin( nullptr ),
pClsSkinBuilder( nullptr ),
pClsSky( nullptr ),
pClsSkyBody( nullptr ),
pClsSkyCtrl( nullptr ),
pClsSkyInst( nullptr ),
pClsSkyLayer( nullptr ),
pClsSkyLink( nullptr ),
pClsSkyTarget( nullptr ),
pClsSmFlt( nullptr ),
pClsSmDbl( nullptr ),
pClsSmVec( nullptr ),
pClsSmVec2( nullptr ),
pClsSmDVec( nullptr ),
pClsSnd( nullptr ),
pClsSoundLevelMeter( nullptr ),
pClsSoundLevelMeterSpeaker( nullptr ),
pClsSoundLevelMeterListener( nullptr ),
pClsSpk( nullptr ),
pClsSID( nullptr ),
pClsSyn( nullptr ),
pClsSynEff( nullptr ),
pClsSynI( nullptr ),
pClsSynS( nullptr ),
pClsSynthesizerSystem( nullptr ),
pClsSEStretch( nullptr ),
pClsSSSound( nullptr ),
pClsSSWave( nullptr ),
pClsSSChain( nullptr ),
pClsSSSyn( nullptr ),
pClsSSGroup( nullptr ),
pClsTexMatrix( nullptr ),
pClsTexMatrix2( nullptr ),
pClsTS( nullptr ),
pClsTSL( nullptr ),
pClsUS( nullptr ),
pClsUID( nullptr ),
pClsU8D( nullptr ),
pClsVec( nullptr ),
pClsVec2( nullptr ),
pClsVid( nullptr ),
pClsVP( nullptr ),
pClsVRSys( nullptr ),
pClsWorld( nullptr ),
pClsResourceLoaderType( nullptr ),
pScriptEngine( nullptr ),
pClsGameObj( nullptr ),
pResourceLoader( nullptr ),
//pLockManager( nullptr ),
pColInfo( nullptr ),
pColliderListenerClosest( nullptr ),
pColliderListenerAdaptor( nullptr ),
pGameObj( nullptr ),
pRestartRequested( false )
{
	pModuleVersion.SetVersion( DS_MODULE_VERSION );
}

deScriptingDragonScript::~deScriptingDragonScript(){
	ShutDown();
}



// Management
///////////////

const char *deScriptingDragonScript::GetSharedDataDir() const{
	return "data";
}

const char *deScriptingDragonScript::GetVFSSharedDataDir() const{
	return "/shareddata";
}

bool deScriptingDragonScript::Init( const char *scriptDirectory, const char *gameObject ){
	if( pState != esStopped && pState != esRestartInit ){
		return false;
	}
	
	deDSEngineManager *dsmanager = nullptr;
	
	try{
		pInitScriptDirectory = scriptDirectory;
		pInitGameObject = gameObject;
		
		decString version( GetGameEngine()->GetScriptingSystem()->GetScriptVersion() );
		if( version.IsEmpty() ){
			version = "1.8"; // added in version 1.9
		}
		
		pCompatibleVersion.SetVersion( version );
		LogInfoFormat( "Requested compatible script version: %s", pCompatibleVersion.version.GetString() );
		LogInfoFormat( "Module version: %s", pModuleVersion.version.GetString() );
		
		// create lock manager
	//	pLockManager = new dedsLockManager;
	//	if( ! pLockManager ) return false;
		
		// if library has been installed as contrib adjust load path
		#ifdef USE_INTERNAL_DSCRIPT
		decPath pathContrib;
		pathContrib.SetFromNative( GetGameEngine()->GetOS()->GetPathShare() );
		pathContrib.AddComponent( "modules" );
		pathContrib.AddComponent( GetGameEngine()->GetModuleSystem()->GetTypeDirectory( GetLoadableModule().GetType() ) );
		pathContrib.AddComponent( GetLoadableModule().GetDirectoryName() );
		pathContrib.AddComponent( GetLoadableModule().GetVersion() );
		pathContrib.AddComponent( "dsinstall" );
		LogInfoFormat( "Set contrib installation path '%s'", pathContrib.GetPathNative().GetString() );
		
		#ifdef OS_W32_VS
		TCHAR envValBuf[ MAX_PATH ];
		deOSWindows::Utf8ToWide( pathContrib.GetPathNative(), envValBuf, sizeof( envValBuf ) );
		if( ! SetEnvironmentVariable( L"DS_PAKAGE_PATH", envValBuf ) ){
			DSTHROW( dueInvalidAction );
		}

		#elif defined OS_W32
		decString pestr;
		pestr.Format( "DS_PAKAGE_PATH=%s", pathContrib.GetPathNative().GetString() );
		if( putenv( pestr ) ){
			DSTHROW( dueInvalidAction );
		}

		#else
		if( setenv( "DS_PAKAGE_PATH", pathContrib.GetPathNative(), 1 ) ){
			DSTHROW( dueInvalidAction );
		}
		#endif // OS_W32
		#endif // USE_INTERNAL_DSCRIPT
		
		// create script engine
		pScriptEngine = new dsEngine;
		DEASSERT_NOTNULL( pScriptEngine )
		#ifdef USE_INTERNAL_DSCRIPT
		/*
		const int sharePathCount = pScriptEngine->GetSharedPathCount();
		int sharePathIndex;
		for( sharePathIndex=0; sharePathIndex<sharePathCount; sharePathIndex++ ){
			LogInfoFormat( "Share path %i = '%s'", sharePathIndex, pScriptEngine->GetSharedPath( sharePathIndex ) );
		}
		*/
		#endif
		
		// set script engine manager
		dsmanager = new deDSEngineManager( this );
		
		pScriptEngine->SetEngineManager( dsmanager );
		dsmanager = nullptr;
		
		pLoadingScreen.TakeOver( new dedsLoadingScreen( *this ) );
		
		pState = esSkipOneFrame;
		
	}catch( const duException &e ){
		if( dsmanager ){
			delete dsmanager;
		}
		SetErrorTraceDS( e );
		LogExceptionDS( e );
		deErrorTracePoint * const tracePoint = AddErrorTracePoint( "deScriptingDragonScript::Init", __LINE__ );
		tracePoint->AddValue( "scriptDirectory", scriptDirectory );
		tracePoint->AddValue( "gameObject", gameObject );
		pAddExceptionTrace( tracePoint );
		pScriptEngine->GetMainRunTime()->ClearExceptionTrace();
		return false;
		
	}catch( const deException &e ){
		if( dsmanager ){
			delete dsmanager;
		}
		SetErrorTrace( e );
		LogException( e );
		deErrorTracePoint * const tracePoint = AddErrorTracePoint( "deScriptingDragonScript::Init", __LINE__ );
		tracePoint->AddValue( "scriptDirectory", scriptDirectory );
		tracePoint->AddValue( "gameObject", gameObject );
		pAddExceptionTrace( tracePoint );
		pScriptEngine->GetMainRunTime()->ClearExceptionTrace();
		return false;
	}
	
	// debug
	/*
	LogWarn( "" );
	LogWarn( "*************************** DragonScript TODO ****************************" );
	LogWarn( "" );
	LogWarn( "Improve performance of Container.doLayout()." );
	LogWarn( "Do expensive layout only if pBlockLayout=false or layout is dirty." );
	LogWarn( "Layout is marked dirty if pBlockLayout=true while doLayout() has been called.");
	LogWarn( "" );
	LogWarn( "Add parameter 'colorTransform' to DefaultWidgetDesignerFactory." );
	LogWarn( "Uses setColorTransformation on widget (see WindowLists.ds in text project)." );
	LogWarn( "" );
	LogWarn( "**************************************************************************" );
	LogWarn( "" );
	*/
	
	// finished
	return true;
}

void deScriptingDragonScript::ShutDown(){
	pLoadingScreen = nullptr;
	if( ! pScriptEngine ){
		return;
	}
	
	if( pColliderListenerClosest ){
		pColliderListenerClosest->SetListener( nullptr );
	}
	if( pColliderListenerAdaptor ){
		pColliderListenerAdaptor->SetListener( nullptr );
	}
	
	if( pGameObj ){
		ExitGame(); // the game should be exited by now but if not do so
	}
	
	DeleteValuesDeleteLater();
	
	if( pResourceLoader ){
		delete pResourceLoader;
		pResourceLoader = nullptr;
	}
	
//	pLockManager->RemoveAllLocks();
	
	if( pColliderListenerClosest ){
		delete pColliderListenerClosest;
		pColliderListenerClosest = nullptr;
	}
	if( pColliderListenerAdaptor ){
		delete pColliderListenerAdaptor;
		pColliderListenerAdaptor = nullptr;
	}
	
	if( pColInfo ){
		pColInfo->Clear();
	}
	
	delete pScriptEngine;
	pScriptEngine = nullptr;
	
	pClsGameObj = nullptr;
	
	DeleteValuesDeleteLater();
	
	pRemoveVFSContainerHideScriptDirectory();
	
	if( pColInfo ){
		pColInfo->FreeReference();
		pColInfo = nullptr;
	}
	
	// free the lock manager
//	delete pLockManager;
//	pLockManager = nullptr;
	
	pState = esStopped;
}

void deScriptingDragonScript::LoadPackage( const char *name, const char *directory ){
	if( ! name || ! directory ){
		DSTHROW( dueInvalidParam );
	}
	
	dsPackage *package = NULL;
	
	try{
		LogInfoFormat( "Load Package from '%s'\n", directory );
		
		// create package
		package = new dsPackage( name );
		
		// add scripts from directory ( recursive )
		pAddScripts( package, *GetGameEngine()->GetVirtualFileSystem(), directory );
		
		// add package to engine
		if( ! pScriptEngine->AddPackage( package ) ){
			DSTHROW( dueInvalidParam ); // DETHROW(deeScriptError)
		}
		
		package = NULL;
		
	}catch( const duException &e ){
		SetErrorTraceDS( e );
		deErrorTracePoint *tracePoint = AddErrorTracePoint( "deScriptingDragonScript::LoadPackage", __LINE__ );
		tracePoint->AddValue( "name", name );
		tracePoint->AddValue( "directory", directory );
		if( package ){
			tracePoint->AddValueInt( "packageClassCount", package->GetClassCount() );
			tracePoint->AddValueInt( "packageHostClassCount", package->GetHostClassCount() );
		}
		LogExceptionDS( e );
		pScriptEngine->GetMainRunTime()->ClearExceptionTrace();
		if( package ){
			delete package;
		}
		throw;
		
	}catch( const deException &e ){
		SetErrorTrace( e );
		deErrorTracePoint *tracePoint = AddErrorTracePoint( "deScriptingDragonScript::LoadPackage", __LINE__ );
		tracePoint->AddValue( "name", name );
		tracePoint->AddValue( "directory", directory );
		if( package ){
			tracePoint->AddValueInt( "packageClassCount", package->GetClassCount() );
			tracePoint->AddValueInt( "packageHostClassCount", package->GetHostClassCount() );
		}
		pScriptEngine->GetMainRunTime()->ClearExceptionTrace();
		LogException( e );
		if( package ){
			delete package;
		}
		throw;
	}
}

deBaseScriptingCollider *deScriptingDragonScript::CreateCollider( deCollider *collider ){
	return new dedsCollider( *this, collider );
}

deBaseScriptingServer *deScriptingDragonScript::CreateServer( deServer *server ){
	return new dedsServer( this, server );
}

deBaseScriptingConnection *deScriptingDragonScript::CreateConnection( deConnection *connection ){
	return new dedsConnection( *this, connection );
}

deBaseScriptingNetworkState *deScriptingDragonScript::CreateNetworkState( deNetworkState *state ){
	return new dedsNetworkState( *this, state );
}

deBaseScriptingTouchSensor *deScriptingDragonScript::CreateTouchSensor( deTouchSensor *touchSensor ){
	return new dedsTouchSensor( *this, touchSensor );
}

deBaseScriptingPropField *deScriptingDragonScript::CreatePropField( dePropField *propField ){
	return new dedsPropField( this, propField );
}

deBaseScriptingParticleEmitterInstance *deScriptingDragonScript::CreateParticleEmitterInstance( deParticleEmitterInstance *instance ){
	return new dedsParticleEmitter( this, instance );
}

deBaseScriptingSoundLevelMeter *deScriptingDragonScript::CreateSoundLevelMeter( deSoundLevelMeter *meter ){
	return new dedsSoundLevelMeter( *this, meter );
}

deBaseScriptingSpeaker *deScriptingDragonScript::CreateSpeaker( deSpeaker *speaker ){
	return new dedsSpeaker( *this, speaker );
}

deBaseScriptingService *deScriptingDragonScript::CreateService( deService *service ){
	return new dedsService( *this, service );
}

bool deScriptingDragonScript::InitGame(){
	return true;
}

bool deScriptingDragonScript::ExitGame(){
	if( ! pGameObj ){
		return true;
	}
	if( ( pState == esReady || pState == esRestartShutdown ) && ! pCallFunction( "cleanUp" ) ){
		return false;
	}
	
	try{
		pScriptEngine->GetMainRunTime()->FreeValue( pGameObj );
		pGameObj = nullptr;
		
		// free static and run garbage collection. this should clean up all
		// but can also result in ugly segfaults if this can segfault in so many bad ways it's not funny.
		//pScriptEngine->Clear();
		
		pState = esStopped;
		
	}catch( const duException &e ){
		pGameObj = nullptr;
		LogExceptionDS( e );
		deErrorTracePoint *tracePoint = AddErrorTracePoint( "deScriptingDragonScript::InitGame", __LINE__ );
		pAddExceptionTrace( tracePoint );
		pScriptEngine->GetMainRunTime()->ClearExceptionTrace();
		return false;
		
	}catch( const deException &e ){
		pGameObj = nullptr;
		SetErrorTrace( e );
		LogException( e );
		LogExceptionDSTrace();
		deErrorTracePoint * const tracePoint = AddErrorTracePoint( "deScriptingDragonScript::InitGame", __LINE__ );
		pAddExceptionTrace( tracePoint );
		pScriptEngine->GetMainRunTime()->ClearExceptionTrace();
		return false;
	}
	
	return true;
}

//#define SPECIAL_DEBUG
#ifdef SPECIAL_DEBUG
extern int timerCollisionResponse;
extern int timerCollisionResponseCount;
extern int timerCanHitCollider;
extern int timerCanHitColliderCount;
extern int timerColliderChanged;
extern int timerColliderChangedCount;
#endif
bool deScriptingDragonScript::OnFrameUpdate(){
	if( pLoadingScreen ){
		pLoadingScreen->Update();
	}
	
	switch( pState ){
	case esReady:{
		if( pRestartRequested ){
			LogInfoFormat( "Restart request using info '%s'", pRestartInfo.GetString() );
			pRestartRequested = false;
			pState = esRestartShutdown;
			return true;
		}
		
		pLoadingScreen = nullptr;
		
		#ifdef SPECIAL_DEBUG
		timerCollisionResponse = 0; timerCollisionResponseCount = 0;
		timerCanHitCollider = 0; timerCanHitColliderCount = 0;
		timerColliderChanged = 0; timerColliderChangedCount = 0;
		#endif
		pResourceLoader->OnFrameUpdate();
		pClsInpSys->OnFrameUpdate();
		pClsVRSys->OnFrameUpdate();
		
		DeleteValuesDeleteLater();
		
		const bool result = pCallFunction( "onFrameUpdate" );
		#ifdef SPECIAL_DEBUG
		LogInfoFormat( "OnFrameUpdate: collisionResponse(%i) = %iys", timerCollisionResponseCount, timerCollisionResponse );
		LogInfoFormat( "OnFrameUpdate: canHitCollider(%i) = %iys", timerCanHitColliderCount, timerCanHitCollider );
		LogInfoFormat( "OnFrameUpdate: colliderChanged(%i) = %iys", timerColliderChangedCount, timerColliderChanged );
		#endif
		return result;
		
		// pResourceLoader->Update();
		// DeleteValuesDeleteLater();
		// return pCallFunction( "onFrameUpdate" );
		}
		
	case esSkipOneFrame:
		pState = esLoadBasicPackages;  // required to allow graphic module to update once
		return true;
		
	case esLoadBasicPackages:
		try{
			pLoadBasicPackage();
			pState = esLoadGame;
			return true;
			
		}catch( const duException &e ){
			SetErrorTraceDS( e );
			LogExceptionDS( e );
			deErrorTracePoint * const tracePoint = AddErrorTracePoint( "deScriptingDragonScript::OnFrameUpdate", __LINE__ );
			tracePoint->AddValue( "scriptDirectory", pInitScriptDirectory.GetString() );
			tracePoint->AddValue( "gameObject", pInitGameObject.GetString() );
			pAddExceptionTrace( tracePoint );
			pScriptEngine->GetMainRunTime()->ClearExceptionTrace();
			return false;
			
		}catch( const deException &e ){
			SetErrorTrace( e );
			LogException( e );
			deErrorTracePoint * const tracePoint = AddErrorTracePoint( "deScriptingDragonScript::OnFrameUpdate", __LINE__ );
			tracePoint->AddValue( "scriptDirectory", pInitScriptDirectory.GetString() );
			tracePoint->AddValue( "gameObject", pInitGameObject.GetString() );
			pAddExceptionTrace( tracePoint );
			pScriptEngine->GetMainRunTime()->ClearExceptionTrace();
			return false;
		}
		
	case esLoadGame:
		try{
			pLoadGamePackage( pInitScriptDirectory, pInitGameObject );
			pAddVFSContainerHideScriptDirectory();
			
			pResourceLoader = new dedsResourceLoader( this );
			pColInfo = new deCollisionInfo;
			pColliderListenerClosest = new dedsColliderListenerClosest( *this );
			pColliderListenerAdaptor = new dedsColliderListenerAdaptor( *this );
			
			pState = esCreateGameObject;
			return true;
			
		}catch( const duException &e ){
			SetErrorTraceDS( e );
			LogExceptionDS( e );
			deErrorTracePoint * const tracePoint = AddErrorTracePoint( "deScriptingDragonScript::OnFrameUpdate", __LINE__ );
			tracePoint->AddValue( "scriptDirectory", pInitScriptDirectory.GetString() );
			tracePoint->AddValue( "gameObject", pInitGameObject.GetString() );
			pAddExceptionTrace( tracePoint );
			pScriptEngine->GetMainRunTime()->ClearExceptionTrace();
			return false;
			
		}catch( const deException &e ){
			SetErrorTrace( e );
			LogException( e );
			deErrorTracePoint * const tracePoint = AddErrorTracePoint( "deScriptingDragonScript::OnFrameUpdate", __LINE__ );
			tracePoint->AddValue( "scriptDirectory", pInitScriptDirectory.GetString() );
			tracePoint->AddValue( "gameObject", pInitGameObject.GetString() );
			pAddExceptionTrace( tracePoint );
			pScriptEngine->GetMainRunTime()->ClearExceptionTrace();
			return false;
		}
		
	case esCreateGameObject:
		try{
			if( ! pScriptEngine ){
				DSTHROW( dueInvalidParam );
			}
			if( ! pClsGameObj ){
				DSTHROW( dueInvalidParam );
			}
			
			LogInfo( "Create game object" );
			dsRunTime &rt = *pScriptEngine->GetMainRunTime();
			pGameObj = rt.CreateValue( pClsGameObj );
			rt.CreateObject( pGameObj, pClsGameObj, 0 );
			
			pState = esInitGameObject;
			return true;
			
		}catch( const duException &e ){
			SetErrorTraceDS( e );
			LogExceptionDS( e );
			deErrorTracePoint *tracePoint = AddErrorTracePoint( "deScriptingDragonScript::OnFrameUpdate", __LINE__ );
			pAddExceptionTrace( tracePoint );
			pScriptEngine->GetMainRunTime()->ClearExceptionTrace();
			return false;
			
		}catch( const deException &e ){
			SetErrorTrace( e );
			LogException( e );
			deErrorTracePoint * const tracePoint = AddErrorTracePoint( "deScriptingDragonScript::OnFrameUpdate", __LINE__ );
			pAddExceptionTrace( tracePoint );
			pScriptEngine->GetMainRunTime()->ClearExceptionTrace();
			return false;
		}
		
	case esInitGameObject:
		LogInfo( "Init game object" );
		if( ! pCallFunction( "initGame" ) ){
			return false;
		}
		
		pState = esReady;
		return true;
		
	case esRestartShutdown:
		LogInfo( "Restart: Shutdown..." );
		ShutDown();
		pRemoveVFSContainerHideScriptDirectory();
		pState = esRestartInit;
		return true;
		
	case esRestartInit:{
		LogInfo( "Restart: Init..." );
		const decString scriptDir( pInitScriptDirectory ), gameObj( pInitGameObject );
		if( Init( scriptDir, gameObj ) ){
			LogInfo( "Restart: Finished" );
			return true;
			
		}else{
			LogInfo( "Restart: Init failed!" );
			ShutDown();
			return false;
		}
		}
		
	default:
		return false;
	}
}

bool deScriptingDragonScript::OnResizeRenderWindow(){
	if(pClsGraSys){
		pClsGraSys->ResizeActiveRenderWindowCanvas();
	}
	return pState != esReady || pCallFunction("onResizeRenderWindow");
}

bool deScriptingDragonScript::SendEvent(deInputEvent *event){
	if(pState != esReady){
		return true; // ignore
	}
	
	switch(event->GetType()){
	//case deInputEvent::eeDeviceAttached: // deprecated
	//case deInputEvent::eeDeviceDetached: // deprecated
	case deInputEvent::eeDeviceParamsChanged:
	case deInputEvent::eeDevicesAttachedDetached:
		switch(event->GetSource()){
		case deInputEvent::esInput:
			pClsInpSys->InvalidCachedDevices();
			break;
			
		case deInputEvent::esVR:
			pClsVRSys->InvalidCachedDevices();
			break;
		}
		break;
		
	default:
		break;
	}
	
	dsRunTime &rt = *pScriptEngine->GetMainRunTime();
	
	try{
		pPreprocessEventDpiAware(*event);
		pClsInpEvent->PushInputEvent(&rt, *event);
		rt.RunFunction(pGameObj, "inputEvent", 1); // inputEvent(event)
		
	}catch(const duException &e){
		SetErrorTraceDS(e);
		LogExceptionDS(e);
		deErrorTracePoint * const tracePoint = AddErrorTracePoint(
			"deScriptingDragonScript::SendEvent", __LINE__);
		pAddSendEventTrace(*tracePoint, *event);
		pAddExceptionTrace(tracePoint);
		pScriptEngine->GetMainRunTime()->ClearExceptionTrace();
		return false;
		
	}catch(const deException &e){
		SetErrorTrace(e);
		LogException(e);
		deErrorTracePoint * const tracePoint = AddErrorTracePoint(
			"deScriptingDragonScript::SendEvent", __LINE__);
		pAddSendEventTrace(*tracePoint, *event);
		pAddExceptionTrace(tracePoint);
		pScriptEngine->GetMainRunTime()->ClearExceptionTrace();
		return false;
	}
	
	return true;
}

void deScriptingDragonScript::UserRequestQuit(){
	if( pState != esReady ){
		return;
	}
	
	dsRunTime &rt = *pScriptEngine->GetMainRunTime();
	
	try{
		rt.RunFunction( pGameObj, "userRequestedQuit", 0 ); // userRequestedQuit()
		
	}catch( const duException &e ){
		SetErrorTraceDS( e );
		LogExceptionDS( e );
		deErrorTracePoint *tracePoint = AddErrorTracePoint( "deScriptingDragonScript::UserRequestQuit", __LINE__ );
		pAddExceptionTrace( tracePoint );
		pScriptEngine->GetMainRunTime()->ClearExceptionTrace();
		GetGameEngine()->Quit();
		
	}catch( const deException &e ){
		SetErrorTrace( e );
		LogException( e );
		deErrorTracePoint *tracePoint = AddErrorTracePoint( "deScriptingDragonScript::UserRequestQuit", __LINE__ );
		pAddExceptionTrace( tracePoint );
		pScriptEngine->GetMainRunTime()->ClearExceptionTrace();
		GetGameEngine()->Quit();
	}
}

bool deScriptingDragonScript::OnAppActivate(){
	return pState != esReady || pCallFunction( "onAppActivate" );
}



// helper functions
const decVector &deScriptingDragonScript::GetVector( dsRealObject *myself ) const{
	return pClsVec->GetVector( myself );
}
void deScriptingDragonScript::PushVector( dsRunTime *rt, const decVector &v ){
	pClsVec->PushVector( rt, v );
}
const decQuaternion &deScriptingDragonScript::GetQuaternion( dsRealObject *myself ) const{
	return pClsQuat->GetQuaternion( myself );
}
void deScriptingDragonScript::PushQuaternion( dsRunTime *rt, const decQuaternion &v ){
	pClsQuat->PushQuaternion( rt, v );
}
const decMatrix &deScriptingDragonScript::GetMatrix( dsRealObject *myself ) const{
	return pClsMat->GetMatrix( myself );
}
void deScriptingDragonScript::PushMatrix( dsRunTime *rt, const decMatrix &m ){
	pClsMat->PushMatrix( rt, m );
}
const decColor &deScriptingDragonScript::GetColor( dsRealObject *myself ) const{
	return pClsClr->GetColor( myself );
}
void deScriptingDragonScript::PushColor( dsRunTime *rt, const decColor &c ){
	pClsClr->PushColor( rt, c );
}
const decPoint &deScriptingDragonScript::GetPoint( dsRealObject *myself ) const{
	return pClsPt->GetPoint( myself );
}
void deScriptingDragonScript::PushPoint( dsRunTime *rt, const decPoint &p ){
	pClsPt->PushPoint( rt, p );
}
const decPoint3 &deScriptingDragonScript::GetPoint3( dsRealObject *myself ) const{
	return pClsPt3->GetPoint( myself );
}
void deScriptingDragonScript::PushPoint3( dsRunTime *rt, const decPoint3 &pt ){
	pClsPt3->PushPoint( rt, pt );
}

void deScriptingDragonScript::RequestRestart( const char *info ){
	pRestartInfo = info;
	pRestartRequested = true;
}

void deScriptingDragonScript::AddValueDeleteLater( dsValue *value ){
	pDeleteValuesLaterList.Add( value );
}

void deScriptingDragonScript::DeleteValuesDeleteLater(){
	while( pDeleteValuesLaterList.GetCount() > 0 ){
		const int index = pDeleteValuesLaterList.GetCount() - 1;
		pScriptEngine->GetMainRunTime()->FreeValue( ( dsValue* )pDeleteValuesLaterList.GetAt( index ) );
		pDeleteValuesLaterList.RemoveFrom( index );
	}
}

void deScriptingDragonScript::SetErrorTraceDS( const duException &exception ){
	decString text;
	
	if( strlen( exception.GetInfo() ) == 0 ){
		text.Format( "Exception %s: %s", exception.GetName(), exception.GetDescription() );
		
	}else{
		text.Format( "Exception %s: %s (%s)", exception.GetName(), exception.GetDescription(), exception.GetInfo().Pointer() );
	}
	
	GetGameEngine()->GetErrorTrace()->AddAndSetIfEmpty( text.GetString(),
		&GetLoadableModule(), exception.GetFile(), exception.GetLine() );
}

void deScriptingDragonScript::LogExceptionDS( const duException &exception ){
	if( strlen( exception.GetInfo() ) > 0 ){
		GetGameEngine()->GetLogger()->LogErrorFormat( GetLoadableModule().GetLoggingName().GetString(),
			"%s(%s:%i): %s (%s)", exception.GetName(), exception.GetFile(),
			exception.GetLine(), exception.GetDescription(), exception.GetInfo().Pointer() );
		
	}else{
		GetGameEngine()->GetLogger()->LogErrorFormat( GetLoadableModule().GetLoggingName().GetString(),
			"%s(%s:%i): %s", exception.GetName(), exception.GetFile(),
			exception.GetLine(), exception.GetDescription() );
	}
	
	LogExceptionDSTrace();
}



// private functions
void deScriptingDragonScript::pLoadBasicPackage(){
	deVirtualFileSystem &vfs = GetVFS();
	deEngine *engine = GetGameEngine();
	dsPackage *package = NULL;
	decPath path;
	
	// load basic package
	try{
		LogInfo( "Load basic package" );
		
		// load engine class Math
		pScriptEngine->LoadPackage( "Math" );
		
		// add the missing System class, which is a temporary hack until the
		// streaming framework is in place
		package = new dsPackage( "Dragonscript Classes" );
		package->AddHostClass( new deClassSystem( this ) );
		if( ! pScriptEngine->AddPackage( package ) ){
			DSTHROW( dueInvalidParam ); // DETHROW(deeScriptError)
		}
		package = NULL;
		
		// create and add native classes
		package = new dsPackage( "Drag[en]gine Classes" );
		
		package->AddHostClass( pClsUS = new deClassUnicodeString );
		package->AddHostClass( pClsU8D = new deClassUTF8Decoder );
		package->AddHostClass( pClsUID = new deClassUniqueID( *this ) );
		package->AddHostClass( pClsMUID = new deClassMutableID( *this ) );
		package->AddHostClass( pClsSID = new deClassStringID( *this ) );
		
		package->AddHostClass( pClsVec = new deClassVector( engine, this ) );
		package->AddHostClass( pClsVec2 = new deClassVector2( engine, this ) );
		package->AddHostClass( pClsQuat = new deClassQuaternion( engine, this ) );
		package->AddHostClass( pClsMat = new deClassMatrix( this ) );
		package->AddHostClass( pClsMat4 = new deClassMatrix4( this ) );
		package->AddHostClass( pClsDVec = new deClassDVector( engine, this ) );
		package->AddHostClass( pClsDMat = new deClassDMatrix( this ) );
		package->AddHostClass( pClsDMat4 = new deClassDMatrix4( this ) );
		package->AddHostClass( pClsClr = new deClassColor( engine, this ) );
		package->AddHostClass( pClsClrMat = new deClassColorMatrix( *this ) );
		package->AddHostClass( pClsPt = new deClassPoint( engine, this ) );
		package->AddHostClass( pClsPt3 = new deClassPoint3( engine, this ) );
		package->AddHostClass( pClsTexMatrix = new deClassTexMatrix( this ) );
		package->AddHostClass( pClsTexMatrix2 = new deClassTexMatrix2( *this ) );
		
		package->AddHostClass( pClsLyM = new deClassLayerMask( this ) );
		package->AddHostClass( pClsCF = new deClassCollisionFilter( this ) );
		package->AddHostClass( pClsCurve2D = new deClassCurve2D( this ) );
		package->AddHostClass( pClsCBe = new deClassCurveBezier( this ) );
		package->AddHostClass( pClsCBezier3D = new deClassCurveBezier3D( *this ) );
		package->AddHostClass( pClsCDistMap = new deClassCurveDistanceMapping( *this ) );
		
		package->AddHostClass( pClsCLL = new deClassColliderListener( engine, this ) );
		package->AddHostClass( pClsCBL = new deClassColliderBreakingListener( this ) );
		package->AddHostClass( pClsCCon = new deClassColliderConstraint( engine, this ) );
		package->AddHostClass( pClsSvrL = new deClassServerListener( engine, this ) );
		package->AddHostClass( pClsConL = new deClassConnectionListener( engine, this ) );
		package->AddHostClass( pClsNSL = new deClassNetworkStateListener( engine, this ) );
		package->AddHostClass( pClsTSL = new deClassTouchSensorListener( engine, this ) );
		package->AddHostClass( pClsPFL = new deClassPropFieldListener( this ) );
		package->AddHostClass( pClsPEIL = new deClassParticleEmitterInstanceListener( this ) );
		package->AddHostClass( pClsRN = new deClassResourceListener( engine, this ) );
		
		package->AddHostClass( pClsGame = new deClassGame( *this ) );
		package->AddHostClass( pClsEngine = new deClassEngine( *this ) );
		package->AddHostClass( pClsMath = new deClassMath( this ) );
		package->AddHostClass( pClsModPar = new deClassModuleParameter( engine, this ) );
		package->AddHostClass( pClsScrSys = new deClassScriptSystem( *this ) );
		package->AddHostClass( pClsCRSys = new deClassCrashRecoverySystem( *this ) );
		package->AddHostClass( pClsAISys = new deClassAISystem( *this ) );
		package->AddHostClass( pClsGraSys = new deClassGraphicSystem( *this ) );
		package->AddHostClass( pClsAnimatorSystem = new deClassAnimatorSystem( *this ) );
		package->AddHostClass( pClsAudSys = new deClassAudioSystem( *this ) );
		package->AddHostClass( pClsSynthesizerSystem = new deClassSynthesizerSystem( *this ) );
		package->AddHostClass( pClsInpSys = new deClassInputSystem( *this ) );
		package->AddHostClass( pClsVRSys = new deClassVRSystem( *this ) );
		package->AddHostClass( pClsInpEvent = new deClassInputEvent( *this ) );
		package->AddHostClass( pClsInpDev = new deClassInputDevice( *this ) );
		package->AddHostClass( pClsInpDevAxis = new deClassInputDeviceAxis( *this ) );
		package->AddHostClass( pClsInpDevBtn = new deClassInputDeviceButton( *this ) );
		package->AddHostClass( pClsInpDevFb = new deClassInputDeviceFeedback( *this ) );
		package->AddHostClass( pClsInpDevComp = new deClassInputDeviceComponent( *this ) );
		package->AddHostClass( pClsPhySys = new deClassPhysicsSystem( *this ) );
		package->AddHostClass( pClsNetSys = new deClassNetworkSystem( *this ) );
		package->AddHostClass( pClsEnvMapProbe = new deClassEnvMapProbe( this ) );
		package->AddHostClass( pClsFileReader = new deClassFileReader( this ) );
		package->AddHostClass( pClsFileWriter = new deClassFileWriter( this ) );
		package->AddHostClass( pClsMemoryFile = new deClassMemoryFile( *this ) );
		package->AddHostClass( pClsFileSys = new deClassFileSystem( this ) );
		package->AddHostClass( pClsFnt = new deClassFont( *this ) );
		package->AddHostClass( pClsFontBuilder = new deClassFontBuilder( *this ) );
		package->AddHostClass( pClsMdl = new deClassModel( this ) );
		package->AddHostClass( pClsModelBuilder = new deClassModelBuilder( *this ) );
		package->AddHostClass( pClsRig = new deClassRig( this ) );
		package->AddHostClass( pClsRigBuilder = new deClassRigBuilder( *this ) );
		package->AddHostClass( pClsRTM = new deClassRuntimeMeter( this ) );
		package->AddHostClass( pClsAnim = new deClassAnimation( engine, this ) );
		package->AddHostClass( pClsAnimBuilder = new deClassAnimationBuilder( *this ) );
		package->AddHostClass( pClsAr = new deClassAnimator( this ) );
		package->AddHostClass( pClsArI = new deClassAnimatorInstance( *this ) );
		package->AddHostClass( pClsAnimatorCtrl = new deClassAnimatorController( *this ) );
		package->AddHostClass( pClsArR = new deClassAnimatorRule( *this ) );
		package->AddHostClass( pClsARAnim = new deClassARAnimation( *this ) );
		package->AddHostClass( pClsARAnimDiff = new deClassARAnimationDifference( *this ) );
		package->AddHostClass( pClsARAnimSelect = new deClassARAnimationSelect( *this ) );
		package->AddHostClass( pClsARBoneTrans = new deClassARBoneTransformator( *this ) );
		package->AddHostClass( pClsARFSta = new deClassARForeignState( *this ) );
		package->AddHostClass( pClsARGroup = new deClassARGroup( *this ) );
		package->AddHostClass( pClsARIK = new deClassARInverseKinematic( *this ) );
		package->AddHostClass( pClsARLimit = new deClassARLimit( *this ) );
		package->AddHostClass( pClsARStaM = new deClassARStateManipulator( *this ) );
		package->AddHostClass( pClsARSnap = new deClassARStateSnapshot( *this ) );
		package->AddHostClass( pClsARSubA = new deClassARSubAnimator( *this ) );
		package->AddHostClass( pClsARTrack = new deClassARTrackTo( *this ) );
		package->AddHostClass( pClsARMirror = new deClassARMirror( *this ) );
		package->AddHostClass( pClsCI = new deClassCollisionInfo( engine, this ) );
		package->AddHostClass( pClsCT = new deClassCollisionTester( *this ) );
		package->AddHostClass( pClsSkin = new deClassSkin( *this ) );
		package->AddHostClass( pClsSkinBuilder = new deClassSkinBuilder( *this ) );
		package->AddHostClass( pClsDSkin = new deClassDynamicSkin( this ) );
		package->AddHostClass( pClsImg = new deClassImage( engine, this ) );
		package->AddHostClass( pClsImagePixels = new deClassImagePixels( *this ) );
		package->AddHostClass( pClsEditableImage = new deClassEditableImage( *this ) );
		package->AddHostClass( pClsComp = new deClassComponent( engine, this ) );
		package->AddHostClass( pClsCol = new deClassCollider( *this ) );
		package->AddHostClass( pClsColVol = new deClassColliderVolume( *this ) );
		package->AddHostClass( pClsColComp = new deClassColliderComponent( *this ) );
		package->AddHostClass( pClsColRig = new deClassColliderRig( *this ) );
		package->AddHostClass( pClsCCT = new deClassColliderCollisionTest( *this ) );
		package->AddHostClass( pClsLig = new deClassLight( *this ) );
		package->AddHostClass( pClsLoco = new deClassLocomotion( *this ) );
		package->AddHostClass( pClsCam = new deClassCamera( *this ) );
		package->AddHostClass( pClsCanvas = new deClassCanvas( *this ) );
		package->AddHostClass( pClsCanvasCView = new deClassCanvasCanvasView( *this ) );
		package->AddHostClass( pClsCanvasImage = new deClassCanvasImage( *this ) );
		package->AddHostClass( pClsCanvasPaint = new deClassCanvasPaint( *this ) );
		package->AddHostClass( pClsCanvasRenW = new deClassCanvasRenderWorld( *this ) );
		package->AddHostClass( pClsCanvasText = new deClassCanvasText( *this ) );
		package->AddHostClass( pClsCanvasVidP = new deClassCanvasVideoPlayer( *this ) );
		package->AddHostClass( pClsCanvasView = new deClassCanvasView( *this ) );
		package->AddHostClass( pClsCapCan = new deClassCaptureCanvas( *this ) );
		package->AddHostClass( pClsCache = new deClassCache( *this ) );
		package->AddHostClass( pClsService = new deClassService( *this ) );
		package->AddHostClass( pClsServiceListener = new deClassServiceListener( *this ) );
		package->AddHostClass( pClsServiceObject = new deClassServiceObject( *this ) );
		package->AddHostClass( pClsSky = new deClassSky( *this ) );
		package->AddHostClass( pClsSkyBody = new deClassSkyBody( *this ) );
		package->AddHostClass( pClsSkyCtrl = new deClassSkyController( *this ) );
		package->AddHostClass( pClsSkyInst = new deClassSkyInstance( *this ) );
		package->AddHostClass( pClsSkyLayer = new deClassSkyLayer( *this ) );
		package->AddHostClass( pClsSkyLink = new deClassSkyLink( *this ) );
		package->AddHostClass( pClsSkyTarget = new deClassSkyTarget( *this ) );
		package->AddHostClass( pClsSmFlt = new deClassSmoothFloat( *this ) );
		package->AddHostClass( pClsSmDbl = new deClassSmoothDouble( *this ) );
		package->AddHostClass( pClsSmVec = new deClassSmoothVector( *this ) );
		package->AddHostClass( pClsSmVec2 = new deClassSmoothVector2( *this ) );
		package->AddHostClass( pClsSmDVec = new deClassSmoothDVector( *this ) );
		package->AddHostClass( pClsWorld = new deClassWorld( engine, this ) );
		package->AddHostClass( pClsEff = new deClassEffect( *this ) );
		package->AddHostClass( pClsEffClrMat = new deClassEffectColorMatrix( *this ) );
		package->AddHostClass( pClsEffDistImg = new deClassEffectDistortImage( *this ) );
		package->AddHostClass( pClsEffFilKer = new deClassEffectFilterKernel( *this ) );
		package->AddHostClass( pClsEffOverImg = new deClassEffectOverlayImage( *this ) );
		package->AddHostClass( pClsXML = new deClassEasyXML( *this ) );
		package->AddHostClass( pClsXMLEl = new deClassEasyXMLElement( *this ) );
		package->AddHostClass( pClsLP = new deClassLanguagePack( *this ) );
		package->AddHostClass( pClsLangPackBuilder = new deClassLanguagePackBuilder( *this ) );
		package->AddHostClass( pClsSnd = new deClassSound( *this ) );
		package->AddHostClass( pClsSoundLevelMeter = new deClassSoundLevelMeter( *this ) );
		package->AddHostClass( pClsSoundLevelMeterSpeaker = new deClassSoundLevelMeterSpeaker( *this ) );
		package->AddHostClass( pClsSoundLevelMeterListener = new deClassSoundLevelMeterListener( *this ) );
		package->AddHostClass( pClsSpk = new deClassSpeaker( engine, this ) );
		package->AddHostClass( pClsMic = new deClassMicrophone( engine, this ) );
		package->AddHostClass( pClsSvr = new deClassServer( engine, this ) );
		package->AddHostClass( pClsCon = new deClassConnection( engine, this ) );
		package->AddHostClass( pClsNS = new deClassNetworkState( *this ) );
		package->AddHostClass( pClsNM = new deClassNetworkMessage( *this ) );
		package->AddHostClass( pClsBillboard = new deClassBillboard( *this ) );
		package->AddHostClass( pClsDec = new deClassDecal( *this ) );
		package->AddHostClass( pClsLM = new deClassLumimeter( engine, this ) );
		package->AddHostClass( pClsTS = new deClassTouchSensor( engine, this ) );
		package->AddHostClass( pClsHT = new deClassHeightTerrain( *this ) );
		package->AddHostClass( pClsPF = new deClassPropField( this ) );
		package->AddHostClass( pClsCVeg = new deClassCachedVegetation( this ) );
		package->AddHostClass( pClsFF = new deClassForceField( *this ) );
		package->AddHostClass( pClsPE = new deClassParticleEmitter( this ) );
		package->AddHostClass( pClsPEC = new deClassParticleEmitterController( *this ) );
		package->AddHostClass( pClsPEI = new deClassParticleEmitterInstance( this ) );
		package->AddHostClass( pClsDD = new deClassDebugDrawer( this ) );
		package->AddHostClass( pClsVid = new deClassVideo( this ) );
		package->AddHostClass( pClsVP = new deClassVideoPlayer( this ) );
		package->AddHostClass( pClsNavInfo = new deClassNavigationInfo( *this ) );
		package->AddHostClass( pClsNavBlocker = new deClassNavigationBlocker( this ) );
		package->AddHostClass( pClsNavSpace = new deClassNavigationSpace( this ) );
		package->AddHostClass( pClsNavigator = new deClassNavigator( this ) );
		package->AddHostClass( pClsNavigatorPath = new deClassNavigatorPath( *this ) );
		package->AddHostClass( pClsOccM = new deClassOcclusionMesh( this ) );
		package->AddHostClass( pClsOccMBuilder = new deClassOcclusionMeshBuilder( *this ) );
		package->AddHostClass( pClsShaList = new deClassShapeList( this ) );
		package->AddHostClass( pClsSA = new deClassSafeArray( this ) );
		package->AddHostClass( pClsSyn = new deClassSynthesizer( *this ) );
		package->AddHostClass( pClsSynCtrl = new deClassSynthesizerController( *this ) );
		package->AddHostClass( pClsSynEff = new deClassSynthesizerEffect( *this ) );
		package->AddHostClass( pClsSynI = new deClassSynthesizerInstance( *this ) );
		package->AddHostClass( pClsSynS = new deClassSynthesizerSource( *this ) );
		package->AddHostClass( pClsSEStretch = new deClassSEStretch( *this ) );
		package->AddHostClass( pClsSSSound = new deClassSSSound( *this ) );
		package->AddHostClass( pClsSSWave = new deClassSSWave( *this ) );
		package->AddHostClass( pClsSSChain = new deClassSSChain( *this ) );
		package->AddHostClass( pClsSSSyn = new deClassSSSynthesizer( *this ) );
		package->AddHostClass( pClsSSGroup = new deClassSSGroup( *this ) );
		
		// add script classes from shared directory
		pAddScripts( package, vfs, "/share/scripts" );
		
		// compile and add package		
		if( ! pScriptEngine->AddPackage( package ) ){
			DSTHROW( dueInvalidParam ); // DETHROW(deeScriptError)
		}
		
//		package->PrintClasses();
		package = NULL;
		
		// find constant classes
		pClsResourceLoaderType = pScriptEngine->GetClass( "Dragengine.ResourceLoaderType" );
		
	}catch( const duException &e ){
		if( package ){
			delete package;
		}
		SetErrorTraceDS( e );
		LogExceptionDS( e );
		deErrorTracePoint *tracePoint = AddErrorTracePoint( "deScriptingDragonScript::pLoadBasicPackage", __LINE__ );
		pAddExceptionTrace( tracePoint );
		pScriptEngine->GetMainRunTime()->ClearExceptionTrace();
		throw;
		
	}catch( const deException &e ){
		if( package ){
			delete package;
		}
		SetErrorTrace( e );
		LogExceptionDSTrace();
		LogException( e );
		deErrorTracePoint *tracePoint = AddErrorTracePoint( "deScriptingDragonScript::pLoadBasicPackage", __LINE__ );
		pAddExceptionTrace( tracePoint );
		pScriptEngine->GetMainRunTime()->ClearExceptionTrace();
		throw;
	}
}

// #include <libdscript/objects/dsClass.h>
// #include <libdscript/objects/dsVariable.h>
// #include <libdscript/objects/dsByteCode.h>

void deScriptingDragonScript::pLoadGamePackage( const char *directory, const char *gameClass ){
//	deVirtualFileSystem *vfs = GetGameEngine()->GetVirtualFileSystem();
//	const char *basePath = vfs->GetBasePath();
	dsPackage *package = NULL;
	int i;
	
	try{
		if( ! pClsGame ){
			DSTHROW_INFO( dueInvalidParam, "Game class could not be found" ); // deeScriptError
		}
		
		LogInfo( "Load game package" );
		
		// load package
//		if( ! ( path = new char[strlen( basePath )+strlen( directory )+2] ) ) DSTHROW( dueOutOfMemory );
//		sprintf( path, "%s/%s", basePath, directory );
		LoadPackage( DESM_GAME_PACKAGE, directory /*path*/ );
//		delete [] path;
		
		// examine package to find class to start
		package = pScriptEngine->GetPackage( DESM_GAME_PACKAGE );
		if( ! package ){
			DSTHROW_INFO( dueInvalidParam, "Game script package could not be found" ); // deeScriptError
		}
		
		for( i=0; i<package->GetClassCount(); i++ ){
			dsClass * const cls = package->GetClass( i );
			if( strcmp( cls->GetName(), gameClass ) != 0 ){
				continue;
			}
			
			dsClass *findBaseCls = cls->GetBaseClass();
			while( findBaseCls ){
				if( pClsGame->IsEqualTo( findBaseCls ) ){
					break;
				}
				findBaseCls = findBaseCls->GetBaseClass();
			};
			if( ! findBaseCls ){
				continue;
			}
			
			pClsGameObj = cls;
			break;
		}
		
		if( ! pClsGameObj ){
			DSTHROW_INFO( dueInvalidParam, "Game class not found" ); // deeScriptError
		}
		
	}catch( const duException &e ){
		SetErrorTraceDS( e );
		LogExceptionDS( e );
		deErrorTracePoint *tracePoint = AddErrorTracePoint( "deScriptingDragonScript::pLoadGamePackage", __LINE__ );
		tracePoint->AddValue( "directory", directory );
		tracePoint->AddValue( "gameClass", gameClass );
		tracePoint->AddValueBool( "gameClassPresent", pClsGame != NULL );
		if( package ){
			tracePoint->AddValueInt( "packageClassCount", package->GetClassCount() );
			tracePoint->AddValueInt( "packageHostClassCount", package->GetHostClassCount() );
		}
		pAddExceptionTrace( tracePoint );
		pScriptEngine->GetMainRunTime()->ClearExceptionTrace();
		throw;
		
	}catch( const deException &e ){
		SetErrorTrace( e );
		LogExceptionDSTrace();
		LogException( e );
		deErrorTracePoint *tracePoint = AddErrorTracePoint( "deScriptingDragonScript::pLoadGamePackage", __LINE__ );
		tracePoint->AddValue( "directory", directory );
		if( pClsGame ){
			tracePoint->AddValue( "clsGame", pClsGame->GetName() );
		}
		if( package ){
			tracePoint->AddValueInt( "packageClassCount", package->GetClassCount() );
			tracePoint->AddValueInt( "packageHostClassCount", package->GetHostClassCount() );
		}
		pAddExceptionTrace( tracePoint );
		pScriptEngine->GetMainRunTime()->ClearExceptionTrace();
		throw;
	}
	
// 	dsSignature s;
// 	s.AddParameter( pScriptEngine->GetClassFloat() );
// 	pScriptEngine->GetClass( "Epsylon" )->GetInnerClass( "Elements" )
// 	->GetInnerClass( "Actors" )->GetInnerClass( "Dragons" )
// 	->GetInnerClass( "Actions" )->GetInnerClass( "DAStand" )
// 	->FindFunction( "updateAICollider", &s, false )->GetByteCode()->DebugPrint();
}

class deScriptingDragonScriptAddSource : public deFileSearchVisitor{
private:
	dsPackage &pPackage;
	
public:
	deScriptingDragonScriptAddSource( dsPackage &package ) : pPackage( package ){
	}
	
	virtual bool VisitFile( const deVirtualFileSystem &vfs, const decPath &path ){
		if( ! path.GetLastComponent().MatchesPattern( "*.ds" ) ){
			return true;
		}
		
		deScriptSource *source = NULL;
		try{
			source = new deScriptSource( vfs, path );
			pPackage.AddScript( source );
			
		}catch( ... ){
			if( source ){
				delete source;
			}
			throw;
		}
		
		return true;
	}
	
	virtual bool VisitDirectory( const deVirtualFileSystem &vfs, const decPath &path ){
		vfs.SearchFiles( path, *this );
		return true;
	}
};

void deScriptingDragonScript::pAddScripts( dsPackage *package, deVirtualFileSystem &vfs, const char *pathDir ){
//LogInfoFormat( "Scan Directory '%s'\n", pathDir );
	try{
		deScriptingDragonScriptAddSource visitor( *package );
		vfs.SearchFiles( decPath::CreatePathUnix( pathDir ), visitor );
		
	}catch( const duException &e ){
		SetErrorTraceDS( e );
		LogExceptionDS( e );
		deErrorTracePoint *tracePoint = AddErrorTracePoint( "deScriptingDragonScript::pAddScripts", __LINE__ );
		tracePoint->AddValue( "package", package->GetName() );
		tracePoint->AddValue( "pathDir", pathDir );
		pScriptEngine->GetMainRunTime()->ClearExceptionTrace();
		throw;
		
	}catch( const deException &e ){
		SetErrorTrace( e );
		LogExceptionDSTrace();
		deErrorTracePoint *tracePoint = AddErrorTracePoint( "deScriptingDragonScript::pAddScripts", __LINE__ );
		tracePoint->AddValue( "package", package->GetName() );
		tracePoint->AddValue( "pathDir", pathDir );
		pScriptEngine->GetMainRunTime()->ClearExceptionTrace();
		LogException( e );
		throw;
	}
}
bool deScriptingDragonScript::pIsFileType( const char *Filename, const char *Extension ){
	const char *vExt = strrchr( Filename, '.' );
	if( ! vExt ) return false;
	return strcmp( vExt+1, Extension ) == 0;
}
int deScriptingDragonScript::pGetConstantValue( dsClass *theClass, const char *name ) const{
	dsConstant *constant = theClass->FindConstant( name, false );
	if( ! constant ) DSTHROW( dueInvalidParam ); // deeScriptError
	return constant->GetInt();
}

bool deScriptingDragonScript::pCallFunction( const char *name ){
	if( ! pGameObj ){
		return false;
	}
	
	dsRunTime &rt = *pScriptEngine->GetMainRunTime();
	
	try{
		rt.RunFunction( pGameObj, name, 0 );
		
	}catch( const duException &e ){
		SetErrorTraceDS( e );
		LogExceptionDS( e );
		deErrorTracePoint *tracePoint = AddErrorTracePoint( "deScriptingDragonScript::pCallFunction", __LINE__ );
		tracePoint->AddValue( "function", name );
		pAddExceptionTrace( tracePoint );
		pScriptEngine->GetMainRunTime()->ClearExceptionTrace();
		return false;
		
	}catch( const deException &e ){
		SetErrorTrace( e );
		LogExceptionDSTrace();
		LogException( e );
		deErrorTracePoint *tracePoint = AddErrorTracePoint( "deScriptingDragonScript::pCallFunction", __LINE__ );
		tracePoint->AddValue( "function", name );
		pAddExceptionTrace( tracePoint );
		pScriptEngine->GetMainRunTime()->ClearExceptionTrace();
		return false;
	}
	
	return true;
}

void deScriptingDragonScript::pAddExceptionTrace( deErrorTracePoint *tracePoint ){
	dsRunTime *rt = pScriptEngine->GetMainRunTime();
	dsRealObject *exception = rt->GetRaisedException()->GetRealObject();
	dsClassException *clsExcep = ( dsClassException* )pScriptEngine->GetClassException();
	dsExceptionTrace *trace = NULL;
	deErrorTraceValue *traceValue;
	decString key, value;
	int i;
	
	if( exception ){
		trace = clsExcep->GetTrace( exception );
	}
	
	try{
		// add reason and type of the exception
		if( exception && trace ){
			traceValue = tracePoint->AddValue( "exception", trace->GetReason() );
			traceValue->AddSubValue( "classname", BuildFullName( exception->GetType() ) );
			
			// add each trace point
			for( i=0; i<trace->GetPointCount(); i++ ){
				const dsExceptionTracePoint &point = *trace->GetPointAt( i );
				const decString fullName( BuildFullName( point.GetClass() ) );
				const char * const funcName = point.GetFunction()->GetName();
				const int line = point.GetLine();
				
				if( point.GetFunction()->GetTypeModifiers() & DSTM_NATIVE ){
					value.Format( "Native %s.%s at line %d", fullName.GetString(), funcName, line );
					
				}else{
					value.Format( "Script %s.%s at line %d", fullName.GetString(), funcName, line );
				}
				
				key.Format( "trace %d", i + 1 );
				
				traceValue->AddSubValue( key, value );
			}
			
		}else{
			traceValue = tracePoint->AddValue( "exception", "Script problem but no exception object present. This is very bad!" );
		}
		
	}catch( const duException &e ){
		LogExceptionDS( e );
		throw;
		
	}catch( const deException &e ){
		LogException( e );
		throw;
	}
}

void deScriptingDragonScript::LogExceptionDSTrace(){
	dsRealObject * const exception = pScriptEngine->GetMainRunTime()->GetRaisedException()->GetRealObject();
	if( ! exception ){
		return;
	}
	
	dsClassException * const clsExcep = ( dsClassException* )pScriptEngine->GetClassException();
	dsExceptionTrace * const trace = clsExcep->GetTrace( exception );
	if( ! trace ){
		return;
	}
	
	decString key, value;
	int i;
	
	const decString exceptionClassName( BuildFullName( exception->GetType() ) );
	LogErrorFormat( "%s: %s", exceptionClassName.GetString(), trace->GetReason() );
	
	for( i=0; i<trace->GetPointCount(); i++ ){
		const dsExceptionTracePoint &point = *trace->GetPointAt( i );
		const decString className( BuildFullName( point.GetClass() ) );
		const char * const funcName = point.GetFunction()->GetName();
		const int line = point.GetLine();
		
		LogErrorFormat( "%d) %s %s.%s at line %d", i + 1,
			point.GetFunction()->GetTypeModifiers() & DSTM_NATIVE ? "Native" : "Script",
			className.GetString(), funcName, line );
	}
}

void deScriptingDragonScript::pAddSendEventTrace( deErrorTracePoint &tracePoint, const deInputEvent &event ){
	deErrorTraceValue *traceValue;
	
	switch( event.GetType() ){
	case deInputEvent::eeKeyPress:
		tracePoint.AddValue( "function", "eventKeyPress" );
		traceValue = tracePoint.AddValue( "event", "<deInputEvent>" );
		traceValue->AddSubValueInt( "keyChar", event.GetKeyChar() );
		traceValue->AddSubValueInt( "modifiers", event.GetState() );
		traceValue->AddSubValueInt( "keyCode", event.GetCode() );
		break;
		
	case deInputEvent::eeKeyRelease:
		tracePoint.AddValue( "Function", "eventKeyRelease" );
		traceValue = tracePoint.AddValue( "event", "<deInputEvent>" );
		traceValue->AddSubValueInt( "keyChar", event.GetKeyChar() );
		traceValue->AddSubValueInt( "modifiers", event.GetState() );
		traceValue->AddSubValueInt( "keyCode", event.GetCode() );
		break;
		
	case deInputEvent::eeMousePress:
		tracePoint.AddValue( "Function", "eventMousePress" );
		traceValue = tracePoint.AddValue( "event", "<deInputEvent>" );
		traceValue->AddSubValueInt( "modifiers", event.GetState() );
		traceValue->AddSubValueInt( "button", event.GetCode() );
		break;
		
	case deInputEvent::eeMouseRelease:
		tracePoint.AddValue( "Function", "eventMouseRelease" );
		traceValue = tracePoint.AddValue( "event", "<deInputEvent>" );
		traceValue->AddSubValueInt( "modifiers", event.GetState() );
		traceValue->AddSubValueInt( "button", event.GetCode() );
		break;
		
	case deInputEvent::eeMouseMove:
		tracePoint.AddValue( "Function", "eventMouseMove" );
		traceValue = tracePoint.AddValue( "event", "<deInputEvent>" );
		traceValue->AddSubValueInt( "modifiers", event.GetState() );
		traceValue->AddSubValueInt( "button", event.GetCode() );
		traceValue->AddSubValueInt( "x", event.GetX() );
		traceValue->AddSubValueInt( "y", event.GetY() );
		break;
		
	default:
		tracePoint.AddValue( "function", "<Unknown Event Type>" );
	}
}

void deScriptingDragonScript::pAddVectorTrace( deErrorTraceValue *traceValue, const decVector &vector, const char *name ){
	deErrorTraceValue *tvVector = traceValue->AddSubValue( name, "<decVector>" );
	tvVector->AddSubValueFloat( "x", vector.x );
	tvVector->AddSubValueFloat( "y", vector.y );
	tvVector->AddSubValueFloat( "z", vector.z );
}

void deScriptingDragonScript::pAddColliderTrace( deErrorTracePoint *tracePoint, deCollider *collider, const char *name ){
	deErrorTraceValue *tvCollider;

	if( collider ){
		tvCollider = tracePoint->AddValue( name, "<deCollider>" );
		
		pAddVectorTrace( tvCollider, collider->GetPosition().ToVector(), "position" );
		pAddVectorTrace( tvCollider, decMatrix::CreateFromQuaternion( collider->GetOrientation() ).GetEulerAngles() / DEG2RAD, "rotation" );
	}else{
		tracePoint->AddValue( name, "<null>" );
	}
}

decString deScriptingDragonScript::BuildFullName( const dsClass *theClass ) const{
	if( ! theClass ){
		DSTHROW( dueInvalidParam );
	}
	
	const dsClass *curClass = theClass;
	decString fullname;
	
	while( curClass ){
		if( fullname.IsEmpty() ){
			fullname = curClass->GetName();
			
		}else{
			fullname = decString( curClass->GetName() ) + "." + fullname;
		}
		curClass = curClass->GetParent();
	}
	
	return fullname;
}

void deScriptingDragonScript::pAddVFSContainerHideScriptDirectory(){
	pRemoveVFSContainerHideScriptDirectory();
	
	const deVFSNull::Ref container( deVFSNull::Ref::New(
		new deVFSNull( decPath::CreatePathUnix( pInitScriptDirectory ) ) ) );
	container->SetHidden( true );
	container->AddHiddenPath( decPath::CreatePathUnix( "/" ) );
	pVFSContainerHideScriptDirectory = container;
	
	GetGameEngine()->GetVirtualFileSystem()->AddContainer( container );
}


void deScriptingDragonScript::pRemoveVFSContainerHideScriptDirectory(){
	if( ! pVFSContainerHideScriptDirectory ){
		return;
	}
	
	deVirtualFileSystem &vfs = *GetGameEngine()->GetVirtualFileSystem();
	if( vfs.HasContainer( pVFSContainerHideScriptDirectory ) ){
		vfs.RemoveContainer( pVFSContainerHideScriptDirectory );
	}
	pVFSContainerHideScriptDirectory = nullptr;
}

void deScriptingDragonScript::pPreprocessEventDpiAware(deInputEvent& event) const{
	if(pClsEngine->GetDpiAware()){
		return;
	}
	
	/*
	if(GetGameEngine()->GetInputSystem()->GetCaptureInputDevices()){
		return;
	}
	*/

	if(event.GetType() != deInputEvent::eeMouseMove){
		return;
	}

	event.SetX(pClsGraSys->CoordWindows2CanvasAlways(event.GetX()));
	event.SetY(pClsGraSys->CoordWindows2CanvasAlways(event.GetY()));
}

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

class dedsModuleInternal : public deInternalModule{
public:
	dedsModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("DragonScript");
		SetDescription("Provides access to the Drag[en]gine using the DragonScript\
scripting language. Loaded by default is a basic script package with\
mathematical classes. Additional frameworks can be loaded depending\
on the needs of your game. This is an interprated language hence\
if you need time critical calculations using another language might be better.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(DS_MODULE_VERSION);
		SetType(deModuleSystem::emtScript);
		SetDirectoryName("dragonscript");
		SetPriority(1);
	}
	
	void CreateModule() override{
		SetModule(DSCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deInternalModule *dedsRegisterInternalModule(deModuleSystem *system){
	return new dedsModuleInternal(system);
}
#endif
