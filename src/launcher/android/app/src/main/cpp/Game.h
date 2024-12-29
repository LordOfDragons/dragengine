#ifndef DELAUNCHER_GAME_H
#define DELAUNCHER_GAME_H

#include "JniHelper.h"

class delGame;
class delGameList;

class Game {
private:
    JNIEnv *pEnv;

    JniClass pClsFormat;
    const JniFieldInt pFldFormatType;
    const JniFieldString pFldFormatPattern;
    const JniFieldBool pFldFormatSupported;

    JniClass pClsIcon;
    const JniFieldInt pFldIconSize;
    const JniFieldString pFldIconPath;
    const JniFieldByteArray pFldIconContent;

    JniClass pClsGame;
    const JniFieldString pFldGameIdentifier;
    const JniFieldString pFldGameAliasIdentifier;
    const JniFieldString pFldGameTitle;
    const JniFieldString pFldGameDescription;
    const JniFieldString pFldGameCreator;
    const JniFieldString pFldGameHomepage;
    const JniFieldObjectArray pFldGameIcons;
    const JniFieldString pFldGameDataDirectory;
    const JniFieldString pFldGameScriptDirectory;
    const JniFieldString pFldGameGameObject;
    const JniFieldString pFldGameScriptModule;
    const JniFieldString pFldGameScriptModuleVersion;
    const JniFieldObjectArray pFldGameFileFormats;
    const JniFieldString pFldGamePathConfig;
    const JniFieldString pFldGamePathCapture;
    const JniFieldString pFldGameRunArguments;

public:
    explicit Game(JNIEnv *env);

    jobjectArray Convert(const delGameList &games);
    jobject Convert(const delGame &game);
};

#endif //DELAUNCHER_GAME_H
