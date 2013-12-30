package com.packtpub;

import android.app.Activity;
import android.os.Bundle;
import android.content.pm.PackageManager.NameNotFoundException;

public class StoreActivity extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        try {
            setTitle("Store V" + getPackageManager()
                .getPackageInfo(getPackageName(), 0).versionName);
        } catch (NameNotFoundException e) {
            e.printStackTrace();
        }
    }
}
