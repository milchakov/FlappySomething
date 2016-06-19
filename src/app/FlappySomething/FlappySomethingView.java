package app.FlappySomething;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;

public class FlappySomethingView extends GLSurfaceView {

    public FlappySomethingView( Context context ) {
        
        super( context );
        setEGLContextClientVersion(2);                  // use OpenGl ES 2.x context
        setRenderer( new FlappySomethingRenderer() );
    }

    @Override
    public boolean onTouchEvent(final MotionEvent event) {

        if( MotionEvent.ACTION_DOWN == event.getAction() ) {
            FlappySomethingJavaToNative.OnTouchDown(event.getX(), event.getY());
        }

        return true;
    }

    private static class FlappySomethingRenderer implements GLSurfaceView.Renderer {

        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig eglconfig) {
        }

        @Override
        public void onSurfaceChanged( GL10 gl, int width, int height ) {
            FlappySomethingJavaToNative.SetSurfaceSize( width, height );
        }

        @Override
        public void onDrawFrame( GL10 gl ) {
            FlappySomethingJavaToNative.OnDraw();
        }
    }
}