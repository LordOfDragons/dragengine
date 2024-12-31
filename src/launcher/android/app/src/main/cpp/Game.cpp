#include "Game.h"
#include <delauncher/game/delGame.h>
#include <delauncher/game/delGameList.h>
#include <delauncher/game/icon/delGameIcon.h>
#include <delauncher/game/fileformat/delFileFormat.h>

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
    delete (delGame::Ref*)pgame;
}

extern "C"
JNIEXPORT jobject JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Game_gameGetInfo(
JNIEnv *env, jobject thiz, jlong pgame){
    JniHelpers h(env);
    try {
        const delGame &game = *((delGame::Ref*)pgame);
        return GameInfo(env).Convert(game);
    }catch(const deException &e){
        h.throwException(e);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Game_gameLoadConfig(
JNIEnv *env, jobject thiz, jlong pgame) {
    JniHelpers h(env);
    try {
        const delGame::Ref &game = *((delGame::Ref*)pgame);
        game->LoadConfig();
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
        const delGame::Ref &game = *((delGame::Ref*)pgame);
        game->VerifyRequirements();
    }catch(const deException &e){
        h.throwException(e);
    }
}

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
        const delGame::Ref &game = *((delGame::Ref*)pgame);
        return GameStatus(env).Convert(game);
    }catch(const deException &e){
        h.throwException(e);
    }
}
