package ch.dragengine.dstestproject;

import android.app.Activity;
import android.content.Intent;
import android.content.res.AssetManager;
import android.net.Uri;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

import java.io.IOException;

public class RequestFile extends AppCompatActivity {
    /**
     * Access game data intent action.
     */
    static public String ACCESS_GAME_DATA = "ch.dragengine.intent.action.AccessGameData";

    /**
     * Launch game data intent action.
     */
    static public String LAUNCH_GAME_ARCHIVE = "ch.dragengine.intent.action.LaunchGameArchive";

    /**
     * Access game data response.
     */
    static public String FILE_GAME_DATA = "ch.dragengine.intent.action.FileGameData";

    /**
     * Game data MIME type.
     */
    static public String MIME_GAME_DATA = "delauncher/gamearchive";



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        final Intent intent = createIntentGameData(this, ACCESS_GAME_DATA);
        if(intent != null) {
            setResult(Activity.RESULT_OK, intent);

        }else{
            setResult(Activity.RESULT_CANCELED, null);
        }
        finish();
    }

    /**
     * Create intent to grant access to game data or null if failed.
     */
    static public Intent createIntentGameData(Activity activity, String action) {
        // get asset manager to access the files
        final AssetManager assetManager = activity.getAssets();

        // get game archive
        String filenameGameData = null;
        try {
            for(String filename : assetManager.list("")) {
                Log.i("filterFiles", filename);
                if (filename.endsWith(".delga")) {
                    filenameGameData = filename;
                    break;
                }
            }

        } catch (IOException e) {
            Log.e("RequestFile", "Failed listing asset files", e);
            return null;
        }

        if (filenameGameData == null) {
            Log.e("RequestFile", "Failed locating game data archive in assets");
            return null;
        }

        // open
        // create URI for caller to access the game archive
        Uri uriGameArchive = null;

        try {
            /*uriGameArchive = FileProvider.getUriForFile(RequestFile.this,
                    "ch.dragengine.dstestproject.fileprovider", filenameGameData);*/
            uriGameArchive = Uri.parse("content://" + activity.getPackageName()
                    + ".fileprovider/" + filenameGameData);

        } catch (IllegalArgumentException e) {
            Log.e("RequestFile", "Game archive file can not be shared", e);
            return null;
        }

        // create intent
        final Intent intent = new Intent(action);

        // make game definition archive file accessible to the caller
        intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);

        // set data to access
        intent.setDataAndType(uriGameArchive, MIME_GAME_DATA);
        //intent.setDataAndType(uriGameArchive, activity.getContentResolver().getType(uriGameArchive));

        return intent;
    }
}
