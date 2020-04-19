package ch.dragengine.dstestproject;

import android.content.ContentProvider;
import android.content.ContentValues;
import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.database.Cursor;
import android.database.MatrixCursor;
import android.net.Uri;
import android.os.CancellationSignal;
import android.provider.OpenableColumns;
import android.util.Log;

import java.io.FileNotFoundException;
import java.io.IOException;

/**
 * Copy from internet:
 * https://stackoverflow.com/questions/31555722/share-assets-using-fileprovider
 */

public class AssetsProvider extends ContentProvider {
    static final String TAG = "AssetsProvider";

    private boolean fileExists(String path) {
        try {
            assets.openFd(path).close();
            return true;

        } catch (IOException e) {
            return false;
        }
    }



    private AssetManager assets;

    @Override
    public boolean onCreate() {
        final Context ctx = getContext();
        if (ctx == null) {
            return false;
        }
        assets = ctx.getAssets();
        return true;
    }

    @Override
    public String getType(Uri uri) {
        if(uri.getLastPathSegment().endsWith(".delga")) {
            return RequestFile.MIME_GAME_DATA;
        }
        return null;
    }

    @Override
    public AssetFileDescriptor openAssetFile(Uri uri, String mode) throws FileNotFoundException {
        Log.v(TAG, "AssetsGetter: Open asset file " + uri.toString());

        if(!mode.equals("r")) {
            throw new SecurityException("Only read-only access is supported, mode must be [r]");
        }

        final String filename = uri.getLastPathSegment();
        if(filename == null) {
            throw new FileNotFoundException();
        }
        if(!filename.endsWith(".delga")) {
            throw new FileNotFoundException();
        }

        try {
            return assets.openFd(filename);

        } catch (IOException e) {
            Log.e(TAG, Log.getStackTraceString(e));
            throw new FileNotFoundException();
        }
    }

    @Override
    public Cursor query(Uri uri, String[] projection, String selection,
                        String[] selectionArgs, String sortOrder) {
        final String path = uri.getLastPathSegment();
        if(!fileExists(path)) {
            Log.e(TAG, "Requested file doesn't exist at " + path);
            return null;
        }

        if(projection == null) {
            projection = new String[]{OpenableColumns.DISPLAY_NAME, OpenableColumns.SIZE};
        }

        final MatrixCursor matrixCursor = new MatrixCursor(projection, 1);
        final Object[] row = new Object[projection.length];
        for(int col = 0; col < projection.length; col++) {
            if(OpenableColumns.DISPLAY_NAME.equals(projection[col])) {
                row[col] = path;
            }else if(OpenableColumns.SIZE.equals(projection[col])) {
                try {
                    AssetFileDescriptor afd = openAssetFile(uri, "r");
                    if (afd != null) {
                        row[col] = afd.getLength();
                        afd.close();
                    }

                } catch (IOException e) {
                    Log.e(TAG, e.toString());
                }
            }
        }
        matrixCursor.addRow(row);
        return matrixCursor;
    }

    @Override
    public int delete(Uri p1, String p2, String[] p3) {
        throw new RuntimeException("Operation not supported");
    }

    @Override
    public Uri insert(Uri p1, ContentValues p2) {
        throw new RuntimeException("Operation not supported");
    }

    @Override
    public int update(Uri p1, ContentValues p2, String p3, String[] p4) {
        throw new RuntimeException("Operation not supported");
    }
}
