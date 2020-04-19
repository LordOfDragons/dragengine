package ch.dragengine.dstestproject;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // send intent granting access to files to a launcher
        final Intent intent = RequestFile.createIntentGameData(this, RequestFile.LAUNCH_GAME_ARCHIVE);
        if(intent != null){
            startActivity(intent);
        }
        finish();
    }
}
