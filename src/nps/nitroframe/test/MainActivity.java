package nps.nitroframe.test;

import nps.nitroframe.lib.npGameActivity;
import android.os.Bundle;
import android.app.Activity;
import android.app.NativeActivity;
import android.view.Menu;
import android.view.MenuItem;

public class MainActivity extends npGameActivity {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //setContentView(R.laycout.activity_main);
    }
    
    static{
    	System.loadLibrary("Game");
    }
}
