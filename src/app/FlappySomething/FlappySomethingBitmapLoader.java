package app.FlappySomething;


import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

public class FlappySomethingBitmapLoader
{
    private AssetManager assetManager;  // can not to extend asset manager, so use as a member

     public FlappySomethingBitmapLoader(AssetManager assetManager){
        this.assetManager = assetManager;
    }

    public Bitmap LoadBitmap(final String path)
    {
        try
        {
            if(null == path)
            {
                System.out.println("FlappySomethingBitmapLoader.LoadBitmap the path is null");
            }
            else if(null == assetManager)
            {
                System.out.println("FlappySomethingBitmapLoader.LoadBitmap the assetManager is null");
            }
            else
            {
                return BitmapFactory.decodeStream(assetManager.open(path));
            }
            
        }
        catch (Exception e) 
        {
            e.printStackTrace(); 
        }
        return null;
    }

    public int GetBitmapWidth(final Bitmap bitmap)
    {
        return bitmap.getWidth();
    }

    public int GetBitmapHeight(final Bitmap bitmap)
    {
        return bitmap.getHeight();
    }

    public void GetBitmapData(final Bitmap bitmap, int []data)
    {
        if(null != data)
        {
            bitmap.getPixels(data, 0, bitmap.getWidth(), 0, 0, bitmap.getWidth(), bitmap.getHeight());    
        }
    }

    public void ReleaseBitmap(final Bitmap bitmap)
    {
        bitmap.recycle();
    }
}