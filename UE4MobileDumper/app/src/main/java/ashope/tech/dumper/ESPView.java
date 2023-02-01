package ashope.tech.dumper;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.util.Log;
import android.view.View;

public class ESPView extends View implements Runnable {

    Paint mTextPaint;

    Thread mThread;
    int FPS = 60;

    static long sleepTime;

    static Context ctx;

    public ESPView(Context context) {
        super(context, null, 0);
        InitializePaints();
        setFocusableInTouchMode(false);
        setBackgroundColor(Color.TRANSPARENT);
        sleepTime = 1000 / FPS;
        mThread = new Thread(this);
        mThread.start();
        ctx = context;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        if (canvas != null && getVisibility() == VISIBLE) {
            ClearCanvas(canvas);
            Floater.DrawOn(this, canvas);
        }
    }

    @Override
    public void run() {
        android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_BACKGROUND);
        while (mThread.isAlive() && !mThread.isInterrupted()) {
            try {
                long t1 = System.currentTimeMillis();
                postInvalidate();
                long td = System.currentTimeMillis() - t1;
                Thread.sleep(Math.max(Math.min(0, sleepTime - td), sleepTime));
            } catch (InterruptedException it) {
                Log.e("OverlayThread", it.getMessage());
            }
        }
    }

    public void InitializePaints() {
        mTextPaint = new Paint();
        mTextPaint.setStyle(Paint.Style.FILL_AND_STROKE);
        mTextPaint.setAntiAlias(true);
        mTextPaint.setColor(Color.rgb(0, 0, 0));
        mTextPaint.setTextAlign(Paint.Align.CENTER);
        mTextPaint.setStrokeWidth(1.1f);

    }

    public void ClearCanvas(Canvas cvs) {
        cvs.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR);
    }

    public void DrawText(Canvas cvs, int a, int r, int g, int b, String txt, float posX, float posY, float size) {
        mTextPaint.setColor(Color.rgb(r, g, b));
        mTextPaint.setAlpha(a);

        if (getRight() > 1920 || getBottom() > 1920)
            mTextPaint.setTextSize(4 + size);
        else if (getRight() == 1920 || getBottom() == 1920)
            mTextPaint.setTextSize(2 + size);
        else
            mTextPaint.setTextSize(size);

        cvs.drawText(txt, posX, posY, mTextPaint);
    }

}