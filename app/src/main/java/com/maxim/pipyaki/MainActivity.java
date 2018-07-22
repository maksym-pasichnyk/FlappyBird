package com.maxim.pipyaki;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.view.InputQueue;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.View;

public class MainActivity extends Activity implements SurfaceHolder.Callback2, InputQueue.Callback {
    private long handle;

    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(new View(this));
        getWindow().takeSurface(this);
        getWindow().takeInputQueue(this);

        handle = NativeOnCreate(getAssets());
    }

    @Override
    protected void onResume() {
        //NativeOnResume(handle);
        super.onResume();
    }

    @Override
    protected void onPause() {
        //NativeOnPause(handle);
        super.onPause();
    }

    @Override
    protected void onDestroy() {
        NativeOnDestroy(handle);
        super.onDestroy();
		handle = 0;
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
		NativeWindowsFocusChanged(handle, hasFocus);
    }

    @Override
    public void surfaceRedrawNeeded(SurfaceHolder holder) {
        NativeSurfaceRedraw(handle);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        NativeSurfaceCreated(handle, holder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        NativeSurfaceDestroyed(handle);
    }

    @Override
    public void onInputQueueCreated(InputQueue queue) {
        NativeInputQueueCreated(handle, queue);
    }

    @Override
    public void onInputQueueDestroyed(InputQueue queue) {
        NativeInputQueueDestroyed(handle);
    }

    public void quit() {
        finishAffinity();
        System.exit(0);
    }

    public native long NativeOnCreate(AssetManager assetManager);
    public native void NativeOnResume(long handle);
    public native void NativeOnPause(long handle);
    public native void NativeOnDestroy(long handle);
    public native void NativeWindowsFocusChanged(long handle, boolean focused);
    public native void NativeSurfaceRedraw(long handle);
    public native void NativeSurfaceCreated(long handle, Surface surface);
    public native void NativeSurfaceDestroyed(long handle);
    public native void NativeInputQueueCreated(long handle, InputQueue queue);
    public native void NativeInputQueueDestroyed(long handle);
}
