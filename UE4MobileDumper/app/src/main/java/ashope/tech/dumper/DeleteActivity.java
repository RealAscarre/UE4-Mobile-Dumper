package ashope.tech.dumper;

import static ashope.tech.dumper.MainActivity.CheckOverlayPermission;

import android.app.Activity;
import android.os.Bundle;

public class DeleteActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        CheckOverlayPermission(this);
    }
}
