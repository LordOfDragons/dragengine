#include "Game.h"
#include <cstring>
#include <delauncher/delLauncher.h>
#include <delauncher/game/delGame.h>
#include <delauncher/game/delGameList.h>
#include <delauncher/game/delGameXML.h>
#include <delauncher/game/icon/delGameIcon.h>
#include <delauncher/game/fileformat/delFileFormat.h>
#include <dragengine/common/file/decMemoryFileReader.h>

GameInfo::GameInfo(JNIEnv *env) :
pEnv(env),

pClsFormat(env, JPATH_BASE "GameFileFormat"),
pFldFormatType(pClsFormat.GetFieldInt("type")),
pFldFormatPattern(pClsFormat.GetFieldString("pattern")),
pFldFormatSupported(pClsFormat.GetFieldBool("supported")),

pClsIcon(env, JPATH_BASE "GameIcon"),
pFldIconSize(pClsIcon.GetFieldInt("size")),
pFldIconPath(pClsIcon.GetFieldString("path")),
pFldIconContent(pClsIcon.GetFieldByteArray("content")),

pClsGame(env, JPATH_BASE "GameInfo"),
pFldGameIdentifier(pClsGame.GetFieldString("identifier")),
pFldGameAliasIdentifier(pClsGame.GetFieldString("aliasIdentifier")),
pFldGameTitle(pClsGame.GetFieldString("title")),
pFldGameDescription(pClsGame.GetFieldString("description")),
pFldGameCreator(pClsGame.GetFieldString("creator")),
pFldGameHomepage(pClsGame.GetFieldString("homepage")),
pFldGameIcons(pClsGame.GetFieldObjectArray("icons", pClsIcon)),
pFldGameDataDirectory(pClsGame.GetFieldString("dataDirectory")),
pFldGameScriptDirectory(pClsGame.GetFieldString("scriptDirectory")),
pFldGameGameObject(pClsGame.GetFieldString("gameObject")),
pFldGameScriptModule(pClsGame.GetFieldString("scriptModule")),
pFldGameScriptModuleVersion(pClsGame.GetFieldString("scriptModuleVersion")),
pFldGameFileFormats(pClsGame.GetFieldObjectArray("fileFormats", pClsFormat)),
pFldGamePathConfig(pClsGame.GetFieldString("pathConfig")),
pFldGamePathCapture(pClsGame.GetFieldString("pathCapture")),
pFldGameRunArguments(pClsGame.GetFieldString("runArguments")){
}

jobject GameInfo::Convert(const delGame &game) {
    const delGameIconList &icons = game.GetIcons();
    const int iconCount = icons.GetCount();
    const JniObjectArray objIcons(pEnv, pClsIcon, iconCount);
    int j;
    for(j=0; j<iconCount; j++){
        const delGameIcon &icon = *icons.GetAt(j);
        const JniObject objIcon(pClsIcon.New());
        pFldIconSize.Set(objIcon, icon.GetSize());
        pFldIconPath.Set(objIcon, icon.GetPath());

        if(icon.GetContent()) {
            const JniByteArray objContent(pEnv, icon.GetContent()->GetLength());
            objContent.Set(icon.GetContent()->GetPointer());
            pFldIconContent.Set(objIcon, objContent);
        }

        objIcons.SetAt(j, objIcon);
    }

    const delFileFormatList &formats = game.GetFileFormats();
    const int formatCount = formats.GetCount();
    const JniObjectArray objFormats(pEnv, pClsFormat, formatCount);
    for(j=0; j<formatCount; j++){
        const delFileFormat &format = *formats.GetAt(j);
        const JniObject objFormat(pClsFormat.New());
        pFldFormatType.Set(objFormat, (int)format.GetType());
        pFldFormatPattern.Set(objFormat, format.GetPattern());
        pFldFormatSupported.Set(objFormat, format.GetSupported());
        objFormats.SetAt(j, objFormat);
    }

    JniObject objGame(pClsGame.New());
    pFldGameIdentifier.Set(objGame, game.GetIdentifier().ToHexString(false));
    pFldGameAliasIdentifier.Set(objGame, game.GetAliasIdentifier());
    pFldGameTitle.Set(objGame, game.GetTitle());
    pFldGameDescription.Set(objGame, game.GetDescription());
    pFldGameCreator.Set(objGame, game.GetCreator());
    pFldGameHomepage.Set(objGame, game.GetHomepage());
    pFldGameIcons.Set(objGame, objIcons);
    pFldGameDataDirectory.Set(objGame, game.GetDataDirectory());
    pFldGameScriptDirectory.Set(objGame, game.GetScriptDirectory());
    pFldGameGameObject.Set(objGame, game.GetGameObject());
    pFldGameScriptModule.Set(objGame, game.GetScriptModule());
    pFldGameScriptModuleVersion.Set(objGame, game.GetScriptModuleVersion());
    pFldGameFileFormats.Set(objGame, objFormats);
    pFldGamePathConfig.Set(objGame, game.GetPathConfig());
    pFldGamePathCapture.Set(objGame, game.GetPathCapture());
    pFldGameRunArguments.Set(objGame, game.GetRunArguments());

    return objGame.ReturnValue();
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Game_gameRelease(
JNIEnv *env, jobject thiz, jlong pgame){
    ((delGame*)pgame)->FreeReference();
}

extern "C"
JNIEXPORT jobject JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Game_gameGetInfo(
JNIEnv *env, jobject thiz, jlong pgame){
    JniHelpers h(env);
    try {
        const delGame &game = *((delGame*)pgame);
        return GameInfo(env).Convert(game);
    }catch(const deException &e){
        h.throwException(e);
        return nullptr; // keep compiler happy. code never gets here
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Game_gameLoadConfig(
JNIEnv *env, jobject thiz, jlong pgame) {
    JniHelpers h(env);
    try {
        ((delGame*)pgame)->LoadConfig();
    }catch(const deException &e){
        h.throwException(e);
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Game_gameVerifyRequirements(
JNIEnv *env, jobject thiz, jlong pgame) {
    JniHelpers h(env);
    try {
        ((delGame*)pgame)->VerifyRequirements();
    }catch(const deException &e){
        h.throwException(e);
    }
}


// GameStatus
///////////////

GameStatus::GameStatus(JNIEnv *env) :
        pEnv(env),

        pClsStatus(env, JPATH_BASE "GameStatus"),
        pFldAllFormatsSupported(pClsStatus.GetFieldBool("allFormatsSupported")),
        pFldScriptModuleFound(pClsStatus.GetFieldBool("scriptModuleFound")),
        pFldScriptModuleFoundVersion(pClsStatus.GetFieldString("scriptModuleFoundVersion")),
        pFldGameUpToDate(pClsStatus.GetFieldBool("gameUpToDate")),
        pFldCanRun(pClsStatus.GetFieldBool("canRun")){
}

jobject GameStatus::Convert(const delGame &game){
    JniObject objStatus(pClsStatus.New());
    pFldAllFormatsSupported.Set(objStatus, game.GetAllFormatsSupported());
    pFldScriptModuleFound.Set(objStatus, game.GetScriptModuleFound());
    pFldScriptModuleFoundVersion.Set(objStatus, game.GetScriptModuleFoundVersion());
    pFldGameUpToDate.Set(objStatus, game.GetGameUpToDate());
    pFldCanRun.Set(objStatus, game.GetCanRun());
    return objStatus.ReturnValue();
}

extern "C"
JNIEXPORT jobject JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Game_gameGetStatus(
JNIEnv *env, jobject thiz, jlong pgame) {
    JniHelpers h(env);
    try {
        const delGame &game = *((delGame*)pgame);
        return GameStatus(env).Convert(game);
    }catch(const deException &e){
        h.throwException(e);
        return nullptr; // keep compiler happy. code never gets here
    }
}


// GameConfig
///////////////

GameConfig::GameConfig(JNIEnv *env) :
pEnv(env),

pClsConfig(env, JPATH_BASE "GameConfig"),
pFldCustomProfile(pClsConfig.GetFieldPointer("customProfile")),
pFldActiveProfile(pClsConfig.GetFieldPointer("activeProfile")){
}

jobject GameConfig::Convert(const delGame &game){
    JniObject objConfig(pClsConfig.New());
    pFldCustomProfile.Set(objConfig, game.GetCustomProfile());
    pFldActiveProfile.Set(objConfig, game.GetActiveProfile());
    return objConfig.ReturnValue();
}

void GameConfig::Store(jobject objConfig, delGame &game){
    game.SetCustomProfile((delGameProfile*)pFldCustomProfile.Get(objConfig));
    game.SetActiveProfile((delGameProfile*)pFldActiveProfile.Get(objConfig));
}

extern "C"
JNIEXPORT jobject JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Game_gameGetConfig(
JNIEnv *env, jobject thiz, jlong pgame){
    JniHelpers h(env);
    try {
        return GameConfig(env).Convert(*((delGame*)pgame));
    }catch(const deException &e){
        h.throwException(e);
        return nullptr; // keep compiler happy. code never gets here
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Game_gameSetConfig(
JNIEnv *env, jobject thiz, jlong pgame, jobject pconfig){
    JniHelpers h(env);
    try {
        GameConfig(env).Store(pconfig, *((delGame*)pgame));
    }catch(const deException &e){
        h.throwException(e);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Game_gameLoadStringConfig(
JNIEnv *env, jobject thiz, jlong pgame, jstring pconfig, jlong plauncher){
    JniHelpers h(env);
    try {
        const decString config(h.convertString(pconfig));
        delLauncher &launcher = *((delLauncher*)plauncher);
        delGame &game = *((delGame*)pgame);

        {
            const decMemoryFile::Ref fileGameConfig(decMemoryFile::Ref::New(
                    new decMemoryFile("game.degame")));
            fileGameConfig->Resize(config.GetLength());
            memcpy(fileGameConfig->GetPointer(), config.GetString(), config.GetLength());

            delGameXML gameXml(launcher.GetLogger(), "DELauncher");
            gameXml.ReadFromFile(decMemoryFileReader::Ref::New(
                new decMemoryFileReader(fileGameConfig)), game);

            game.SetDefaultLogFile();
        }
    }catch(const deException &e){
        h.throwException(e);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Game_gameSetGameDirectory(
JNIEnv *env, jobject thiz, jlong pgame, jstring pdirectory){
    JniHelpers h(env);
    try {
        ((delGame*)pgame)->SetGameDirectory(h.convertString(pdirectory));
    }catch(const deException &e){
        h.throwException(e);
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Game_gameIsRunning(
JNIEnv *env, jobject thiz, jlong pgame){
    return ((delGame*)pgame)->IsRunning();
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Game_gameStopGame(
JNIEnv *env, jobject thiz, jlong pgame){
    JniHelpers h(env);
    try {
        ((delGame*)pgame)->StopGame();
    }catch(const deException &e){
        h.throwException(e);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Game_gameKillGame(
JNIEnv *env, jobject thiz, jlong pgame){
    JniHelpers h(env);
    try {
        ((delGame*)pgame)->KillGame();
    }catch(const deException &e){
        h.throwException(e);
    }
}

