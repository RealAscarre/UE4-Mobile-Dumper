package ashope.tech.dumper;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.res.ColorStateList;
import android.content.res.Resources;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.graphics.Point;
import android.graphics.PorterDuff;
import android.graphics.Typeface;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.GradientDrawable;
import android.os.Build;
import android.os.IBinder;
import android.util.Base64;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewConfiguration;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.FrameLayout;
import android.widget.HorizontalScrollView;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.ScrollView;
import android.widget.Switch;
import android.widget.TextView;

import java.io.File;

public class Floater extends Service {

    final LinearLayout[] contentLayouts, layoutParents, tabLayouts;
    final ScrollView[] scrollLayouts;
    final String[] TABS;

    RelativeLayout relativeLayout;
    LinearLayout g_mainLayout;
    WindowManager windowManager;
    HorizontalScrollView scrollViewr;
    ESPView overlayView;
    FrameLayout frameLayout;

    float density;

    boolean resetAvailable;

    int scanThread, screenHeight, screenWidth, dpi, selectedTab, type;

    static Context ctx;

    private native String Title();
    private native String Icon();
    public static native void DrawOn(ESPView espView, Canvas canvas);
    private native void DumperToggle(int setting_code, boolean jboolean1);

    /********** Menu Build up UI **********/
    public Floater() {
        String[] arrstring = new String[]{"Dumping Options", "Info"};
        TABS = arrstring;
        tabLayouts = new LinearLayout[arrstring.length];
        scrollLayouts = new ScrollView[arrstring.length];
        layoutParents = new LinearLayout[arrstring.length];
        contentLayouts = new LinearLayout[arrstring.length];
        resetAvailable = false;
        selectedTab = 0;
        scanThread = 1;
    }

    /********** Service Required **********/
    public IBinder onBind(Intent intent) {
        return null;
    }

    public int onStartCommand(Intent intent, int n, int n2) {
        return START_NOT_STICKY;
    }

    public void onTaskRemoved(Intent intent) {
        super.onTaskRemoved(intent);
        stopSelf();
        try {
            Thread.sleep(100);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void onDestroy() {
        super.onDestroy();
        overlayView = new ESPView(this);
        if (overlayView != null) {
            windowManager.removeView(overlayView);
            overlayView = null;
        }
        if (frameLayout != null) {
            windowManager.removeView(frameLayout);
        }
        if (relativeLayout != null) {
            windowManager.removeView(relativeLayout);
        }
    }

    public void onCreate() {
        DrawCanvas();
        MenuUI();
        Menu();
        MakeFolder();
    }

    private void MenuUI() {
        ctx = this;
        final LinearLayout linearLayout;
        final Point point = new Point();

        windowManager = (WindowManager) getSystemService(WINDOW_SERVICE);
        windowManager.getDefaultDisplay().getRealSize(point);
        screenWidth = point.x;
        screenHeight = point.y;
        dpi = Resources.getSystem().getDisplayMetrics().densityDpi;
        density = Resources.getSystem().getDisplayMetrics().density;

        final int n = convertSizeToDp(530.0f);
        final int n2 = convertSizeToDp(300.0f);

        g_mainLayout = linearLayout = new LinearLayout(this);
        linearLayout.setLayoutParams(new LinearLayout.LayoutParams(-1, -1));
        linearLayout.setBackgroundColor(Color.parseColor("#FF444444"));
        linearLayout.setOrientation(LinearLayout.VERTICAL);
        int n4 = convertSizeToDp(30.0f);

        frameLayout = new FrameLayout(this);
        frameLayout.setClickable(true);
        frameLayout.setFocusable(true);
        frameLayout.setFocusableInTouchMode(true);
        frameLayout.setLayoutParams(new LinearLayout.LayoutParams(-1, n4));
        frameLayout.setBackgroundColor(Color.rgb(68, 68, 68));
        linearLayout.addView(frameLayout);

        TextView textView = new TextView(this);
        textView.setText(Title());
        textView.setGravity(19);
        textView.setTextColor(-1);
        textView.setTypeface(null, Typeface.BOLD);
        textView.setPadding(convertSizeToDp(5.0f), convertSizeToDp(5.0f), convertSizeToDp(5.0f), convertSizeToDp(5.0f));
        textView.setTextSize(1, 15.0f);
        frameLayout.addView(textView);

        final Button button = new Button(this);
        button.setText("X");
        button.setTextColor(-1);
        button.setTextSize(1, 8.0f);
        button.setBackgroundColor(Color.TRANSPARENT);
        button.setX(n - dpi / 5.5f);
        button.setLayoutParams(new RelativeLayout.LayoutParams((int) ((dpi / 5.5f)), n4));
        frameLayout.addView(button);

        LinearLayout linearLayout2 = new LinearLayout(this);
        linearLayout2.setLayoutParams(new LinearLayout.LayoutParams(-1, -1));
        Integer n5 = 0;
        linearLayout2.setOrientation(LinearLayout.VERTICAL);
        linearLayout.addView(linearLayout2);
        final LinearLayout linearLayout3 = new LinearLayout(this);
        linearLayout3.setLayoutParams(new LinearLayout.LayoutParams(0, 0));
        linearLayout3.setBackgroundColor(Color.parseColor("#FF444444"));
        linearLayout3.setOrientation(LinearLayout.VERTICAL);

        final LinearLayout linearLayout90 = new LinearLayout(this);
        linearLayout90.setLayoutParams(new LinearLayout.LayoutParams(-1, -2));
        linearLayout90.setBackgroundColor(Color.parseColor("#FF444444"));
        linearLayout90.setOrientation(LinearLayout.VERTICAL);

        linearLayout2.addView(linearLayout90);
        linearLayout2.addView(linearLayout3);

        ScrollView scrollView = new ScrollView(this);
        scrollView.setLayoutParams(new RelativeLayout.LayoutParams(-1, -2));

        scrollViewr = new HorizontalScrollView(this);
        scrollViewr.setLayoutParams(new RelativeLayout.LayoutParams(-1, -2));

        linearLayout90.addView(scrollViewr);
        linearLayout3.addView(scrollView);

        LinearLayout linearLayout4 = new LinearLayout(this);
        linearLayout4.setLayoutParams(new LinearLayout.LayoutParams(-1, -2));
        linearLayout4.setOrientation(LinearLayout.HORIZONTAL);

        scrollViewr.addView(linearLayout4);
        LinearLayout linearLayout5 = new LinearLayout(this);
        linearLayout5.setLayoutParams(new LinearLayout.LayoutParams(-1, -2));
        linearLayout5.setBackgroundColor(Color.parseColor("#FF444444"));
        linearLayout5.setOrientation(LinearLayout.VERTICAL);
        linearLayout2.addView(linearLayout5);
        int n6 = 0;
        do {
            int n7 = TABS.length;
            TextView textView2 = textView;
            if (n6 >= n7) break;

            scrollLayouts[n6] = new ScrollView(this);
            ScrollView scrollView2 = scrollLayouts[n6];
            LinearLayout linearLayout6 = linearLayout2;
            scrollView2.setLayoutParams(new RelativeLayout.LayoutParams(-1, -2));

            layoutParents[n6] = new LinearLayout(this);
            layoutParents[n6].setLayoutParams(new LinearLayout.LayoutParams(-1, -1));
            layoutParents[n6].setBackgroundColor(Color.parseColor("#FF444444"));
            layoutParents[n6].setOrientation(LinearLayout.VERTICAL);
            scrollLayouts[n6].addView(layoutParents[n6]);
            contentLayouts[n6] = new LinearLayout(this);
            contentLayouts[n6].setLayoutParams(new LinearLayout.LayoutParams(-1, -1));
            contentLayouts[n6].setBackgroundColor(Color.parseColor("#FF444444"));
            contentLayouts[n6].setOrientation(LinearLayout.VERTICAL);

            if (n6 != 0) {
                contentLayouts[n6].setVisibility(View.GONE);
            }

            contentLayouts[n6].addView(scrollLayouts[n6]);
            linearLayout5.addView(contentLayouts[n6]);
            tabLayouts[n6] = new LinearLayout(this);
            tabLayouts[n6].setOrientation(LinearLayout.HORIZONTAL);
            tabLayouts[n6].setGravity(Gravity.CENTER_VERTICAL | Gravity.CENTER);
            tabLayouts[n6].setLayoutParams(new LinearLayout.LayoutParams(convertSizeToDp(120.0f), -2));
            TextView textView3 = new TextView(this);
            textView3.setText(TABS[n6]);
            textView3.setGravity(Gravity.CENTER_VERTICAL | Gravity.CENTER);
            textView3.setTextColor(-1);
            textView3.setTypeface(null, Typeface.BOLD);
            int n8 = convertSizeToDp(5.0f);
            int n9 = convertSizeToDp(5.0f);
            LinearLayout linearLayout7 = linearLayout5;
            int n10 = convertSizeToDp(5.0f);
            int n11 = n4;
            textView3.setPadding(n8, n9, n10, convertSizeToDp(5.0f));
            textView3.setTextSize(1, 15.0f);
            tabLayouts[n6].addView((View) textView3);
            GradientDrawable gradientDrawable = new GradientDrawable();
            gradientDrawable.setColor(Color.rgb(68, 68, 68));
            if (n6 == 0) {
                gradientDrawable.setStroke(6, Color.rgb(68, 68, 68));
            } else {
                gradientDrawable.setStroke(2, Color.parseColor("#FF444444"));
            }
            tabLayouts[n6].setBackground(gradientDrawable);
            linearLayout4.addView(tabLayouts[n6]);
            final int n12 = n6;
            tabLayouts[n6].setOnClickListener(view -> {
                contentLayouts[selectedTab].setVisibility(View.GONE);
                GradientDrawable gradientDrawable1 = new GradientDrawable();
                gradientDrawable1.setColor(Color.rgb(68, 68, 68));
                gradientDrawable1.setStroke(2, Color.rgb(68, 68, 68));
                tabLayouts[selectedTab].setBackground(gradientDrawable1);
                GradientDrawable gradientDrawable2 = new GradientDrawable();
                gradientDrawable2.setColor(Color.rgb(68, 68, 68));
                gradientDrawable2.setStroke(6, Color.argb(255, 255, 255, 255));
                tabLayouts[n12].setBackground(gradientDrawable2);
                contentLayouts[n12].setVisibility(View.VISIBLE);
                selectedTab = n12;
            });
            ++n6;
            linearLayout5 = linearLayout7;
            linearLayout2 = linearLayout6;
            textView = textView2;
            n4 = n11;
        } while (true);
        type = Build.VERSION.SDK_INT >= 26 ? 2038 : 2002;

        final WindowManager.LayoutParams layoutParams = new WindowManager.LayoutParams(n, n2, type, 520, -3);
        layoutParams.x = 280;
        layoutParams.y = 100;
        layoutParams.gravity = 51;

        frameLayout.setOnTouchListener(new View.OnTouchListener() {
            float deltaX;
            float deltaY;
            float maxX;
            float maxY;
            float newX;
            float newY;
            float pressedX;
            float pressedY;

            public boolean onTouch(View view, MotionEvent motionEvent) {
                float f, f2, f3, f4;
                int n = motionEvent.getActionMasked();
                if (n == 0) {
                    deltaX = (float) layoutParams.x - motionEvent.getRawX();
                    deltaY = (float) layoutParams.y - motionEvent.getRawY();
                    pressedX = motionEvent.getRawX();
                    pressedY = motionEvent.getRawY();
                    return false;
                }
                if (n != 1) {
                    float f5;
                    float f6;
                    if (n != 2) {
                        return false;
                    }
                    newX = motionEvent.getRawX() + deltaX;
                    newY = motionEvent.getRawY() + deltaY;
                    maxX = point.x - linearLayout.getWidth();
                    maxY = f6 = (point.y - linearLayout.getHeight());
                    float f7 = newX;
                    if (!(f7 < 0.0f || f7 > maxX || (f5 = newY) < 0.0f || f5 > f6)) {
                        linearLayout.setAlpha(1.0f);
                    } else {
                        linearLayout.setAlpha(0.5f);
                    }
                    layoutParams.x = (int) newX;
                    layoutParams.y = (int) newY;
                    windowManager.updateViewLayout(linearLayout, layoutParams);
                    return false;
                }
                maxX = point.x - linearLayout.getWidth();
                maxY = point.y - linearLayout.getHeight();
                if (newX < 0.0f) {
                    newX = 0.0f;
                }
                if ((f = newX) > (f4 = maxX)) {
                    newX = f4;
                }
                if (newY < 0.0f) {
                    newY = 0.0f;
                }
                if ((f3 = newY) > (f2 = maxY)) {
                    newY = (int) f2;
                }
                layoutParams.x = (int) newX;
                layoutParams.y = (int) newY;
                windowManager.updateViewLayout(linearLayout, layoutParams);
                linearLayout.setAlpha(1.0f);
                return true;
            }
        });

        relativeLayout = new RelativeLayout(this);
        relativeLayout.setLayoutParams(new RelativeLayout.LayoutParams(-2, -2));

        ImageView imageView = new ImageView(this);
        imageView.setLayoutParams(new ViewGroup.LayoutParams(convertSizeToDp(40), convertSizeToDp(40)));
        relativeLayout.addView((View) imageView);
        byte[] arrby = Base64.decode(Icon(), 0);
        imageView.setImageBitmap(BitmapFactory.decodeByteArray(arrby, 0, arrby.length));

        final WindowManager.LayoutParams layoutParams2 = new WindowManager.LayoutParams(-2, -2, type, 8, -3);
        layoutParams2.gravity = 51;
        layoutParams2.x = 0;
        layoutParams2.y = 0;
        relativeLayout.setVisibility(View.GONE);
        windowManager.addView(relativeLayout, layoutParams2);
        windowManager.addView(linearLayout, layoutParams);

        relativeLayout.setOnTouchListener(new View.OnTouchListener() {
            float deltaX;
            float deltaY;
            float newX;
            float newY;
            float pressedX;
            float pressedY;

            public boolean onTouch(View view, MotionEvent motionEvent) {
                int n = motionEvent.getActionMasked();
                if (n != 0) {
                    if (n != 1) {
                        if (n != 2) {
                            return false;
                        }
                        newX = motionEvent.getRawX() + deltaX;
                        newY = motionEvent.getRawY() + deltaY;
                        float f = screenWidth - view.getWidth();
                        float f2 = screenHeight - view.getHeight();
                        if (newX < 0.0f) {
                            newX = 0.0f;
                        }
                        if (newX > f) {
                            newX = f;
                        }
                        if (newY < 0.0f) {
                            newY = 0.0f;
                        }
                        if (newY > f2) {
                            newY = f2;
                        }
                        layoutParams2.x = (int) newX;
                        layoutParams2.y = (int) newY;
                        windowManager.updateViewLayout(relativeLayout, layoutParams2);
                        return false;
                    }
                    int n2 = (int) (motionEvent.getRawX() - pressedX);
                    int n3 = (int) (motionEvent.getRawY() - pressedY);
                    if (n2 == 0 && n3 == 0) {
                        linearLayout.setVisibility(View.VISIBLE);
                        relativeLayout.setVisibility(View.GONE);
                    }
                    return true;
                }
                deltaX = layoutParams2.x - motionEvent.getRawX();
                deltaY = layoutParams2.y - motionEvent.getRawY();
                pressedX = motionEvent.getRawX();
                pressedY = motionEvent.getRawY();
                return false;
            }
        });

        button.setOnClickListener(view -> {
            linearLayout.setVisibility(View.GONE);
            relativeLayout.setVisibility(View.VISIBLE);
        });
    }

    public void MakeFolder() {
        String PackageName = getApplicationContext().getPackageName().toString();
        File Directory = new File("/sdcard/DumpedGames");
        File DirectoryChild = new File("/sdcard/DumpedGames/" + PackageName);
        try {
            if (!Directory.exists()){
                Directory.mkdir();
                if (!DirectoryChild.exists()){
                    DirectoryChild.mkdir();
                }
            } else if (Directory.exists()){
                DirectoryChild.mkdir();
            }
        } catch (Exception e){
            e.printStackTrace();
        }
    }

    /********** Build Menu **********/
    public void Menu() {
        AddCategory(0, "Select LibDump Type");
        EspToggle(0, "Lib Dump", 0);
        AddCheckbox(0, "Full Dump", 1);

        AddCategory(0, "Select Dumping Option");
        EspToggle(0, "Name Dump", 2);
        EspToggle(0, "Objects Dump", 3);
        EspToggle(0, "SDK Dump (GUObjects)", 4);
        EspToggle(0, "SDKW Dump (GWorld)", 5);
        EspToggle(0, "Actors Dump", 6);

        AddText(1, "Made By Ascarre | Join @Ascarre_Hacks", 15, 1, "#FFFF0000");
        AddText(1, "1- Fixed Lib Dumper for 32-64 Bit", 15, 1, "#FF13FF00");
        AddText(1, "2- Removed Game Specific Stuffs", 15, 1, "#FF13FF00");
    }

    /********** Drawing Functions **********/
    public void DrawCanvas() {
        overlayView = new ESPView(this);
        int LAYOUT_FLAG;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            LAYOUT_FLAG = WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY;
        } else {
            LAYOUT_FLAG = WindowManager.LayoutParams.TYPE_SYSTEM_OVERLAY;
        }
        final WindowManager.LayoutParams params = new WindowManager.LayoutParams(WindowManager.LayoutParams.MATCH_PARENT, WindowManager.LayoutParams.MATCH_PARENT, 0, getNavigationBarHeight(), LAYOUT_FLAG, WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE | WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE | WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN | WindowManager.LayoutParams.FLAG_FULLSCREEN, PixelFormat.RGBA_8888);

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P)
            params.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;

        params.gravity = Gravity.TOP | Gravity.START;
        params.x = 0;
        params.y = 0;
        windowManager = (WindowManager) getSystemService(WINDOW_SERVICE);
        windowManager.addView(overlayView, params);
    }

    /********** Included Dependencies **********/
    int convertSizeToDp(float f) {
        return Math.round(TypedValue.applyDimension(1, f, getResources().getDisplayMetrics()));
    }

    private int getNavigationBarHeight() {
        boolean hasMenuKey = ViewConfiguration.get(this).hasPermanentMenuKey();
        int resourceId = getResources().getIdentifier("navigation_bar_height", "dimen", "android");
        if (resourceId > 0 && !hasMenuKey) {
            return getResources().getDimensionPixelSize(resourceId);
        }
        return 0;
    }

    /********** Menu Build up UI Elements **********/
    void AddText(Object object, String string, int n, int n2, String string2) {
        TextView textView = new TextView(this);
        textView.setText(string);
        textView.setTextColor(Color.parseColor(string2));
        textView.setTypeface(null, n2);
        textView.setPadding(convertSizeToDp(5.0f), convertSizeToDp(5.0f), convertSizeToDp(5.0f), convertSizeToDp(5.0f));
        textView.setTextSize(1, (float) n);
        textView.setLayoutParams(new LinearLayout.LayoutParams(-2, -2));
        if (object instanceof ViewGroup) {
            ((ViewGroup) object).addView(textView);
            return;
        }
        if (object instanceof Integer) {
            layoutParents[(Integer) object].addView(textView);
        }
    }

    private void EspToggle(Object object, String name, final int num) {
        Switch switch_ = new Switch((Context) this);
        switch_.setText(name);
        switch_.setTextColor(-1);
        switch_.setPadding(convertSizeToDp(10.0f), convertSizeToDp(5.0f), convertSizeToDp(10.0f), convertSizeToDp(5.0f));
        switch_.setLayoutParams((ViewGroup.LayoutParams) new LinearLayout.LayoutParams(-1, -1));
        ColorStateList colorStateList = new ColorStateList((int[][]) new int[][]{{-16842912}, {16842912}, new int[0]}, new int[]{-1, -7829368, -1});
        switch_.setButtonTintList(new ColorStateList((int[][]) new int[][]{{-16842912}, {16842912}}, new int[]{-1, -1}));
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            switch_.setTrackTintList(colorStateList);
            switch_.setTrackTintMode(PorterDuff.Mode.OVERLAY);
        }
        switch_.getThumbDrawable().setTintList(colorStateList);
        if (object instanceof ViewGroup) {
            ((ViewGroup) object).addView((View) switch_);
            return;
        }
        if (object instanceof Integer) {
            layoutParents[(Integer) object].addView((View) switch_);
        }
        switch_.setOnCheckedChangeListener((buttonView, isChecked) -> DumperToggle(num, isChecked));
    }

    void AddCheckbox(Object object, String string, final int num) {
        CheckBox checkBox = new CheckBox((Context) this);
        checkBox.setText((CharSequence) string);
        checkBox.setTextColor(-1);
        checkBox.setOnCheckedChangeListener((buttonView, isChecked) -> DumperToggle(num, isChecked));
        checkBox.setLayoutParams((ViewGroup.LayoutParams) new LinearLayout.LayoutParams(-2, -2));
        if (Build.VERSION.SDK_INT >= 21) {
            checkBox.setButtonTintList(new ColorStateList((int[][]) new int[][]{{-16842912}, {16842912}}, new int[]{-1, -1}));
        }
        if (object instanceof ViewGroup) {
            ((ViewGroup) object).addView((View) checkBox);
            return;
        }
        if (object instanceof Integer) {
            layoutParents[(Integer) object].addView((View) checkBox);
        }
    }

    private void AddCategory(Object object, String text) {
        TextView textView = new TextView(this);
        textView.setBackgroundColor(Color.parseColor("#000000"));
        textView.setText(text);
        textView.setGravity(17);
        textView.setTextSize(14.0f);
        textView.setTextColor(Color.parseColor("#ffffff"));
        textView.setTypeface(null, Typeface.BOLD);
        textView.setPadding(10, 5, 0, 5);

        if (object instanceof ViewGroup) {
            ((ViewGroup) object).addView((View) textView);
            return;
        }
        if (object instanceof Integer) {
            layoutParents[(Integer) object].addView((View) textView);
        }
    }
}
