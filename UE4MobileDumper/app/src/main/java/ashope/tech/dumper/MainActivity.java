package ashope.tech.dumper;

import android.app.Activity;
import android.content.Context;

public class MainActivity extends Activity {

    static {
        System.loadLibrary("Dumper");
    }

    public static native void CheckOverlayPermission(Context context);
}