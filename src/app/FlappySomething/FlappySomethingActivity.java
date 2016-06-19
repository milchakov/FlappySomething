package app.FlappySomething;

import android.app.Activity;
import android.os.Bundle;

public class FlappySomethingActivity extends Activity {

    public FlappySomethingView view = null;

    @Override protected void onCreate( Bundle savedInstanceState ) {
        super.onCreate( savedInstanceState );

        FlappySomethingJavaToNative.OnCreate(
            new FlappySomethingBitmapLoader(getApplication().getAssets()));

        view = new FlappySomethingView( getApplication() );

        setContentView( view );
    }

    @Override
    protected void onPause() {
        super.onPause();
        FlappySomethingJavaToNative.OnStop();
    }

    @Override
    protected void onStop() {
        super.onStop();
        FlappySomethingJavaToNative.OnStop();
    }

};