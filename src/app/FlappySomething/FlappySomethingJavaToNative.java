package app.FlappySomething;

public class FlappySomethingJavaToNative
{
    static {
        System.loadLibrary( "FlappySomething" );
    }

    public static native void OnCreate(FlappySomethingBitmapLoader bitmapLoader);
    public static native void OnStop();
    public static native void SetSurfaceSize(int width, int height);
    public static native void OnDraw();   
    public static native void OnTouchDown(float x, float y);
     
}