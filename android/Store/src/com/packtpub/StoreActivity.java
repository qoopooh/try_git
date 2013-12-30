package com.packtpub;

import android.app.Activity;
import android.os.Bundle;
import android.content.pm.PackageManager.NameNotFoundException;

public class StoreActivity extends Activity
{
    private EditText mUIKeyEdit, mUIValueEdit;
    private Spinner mUITypeSpinner;
    private Button mUIGetButton, mUISetButton;
    private Store mStore;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        try {
            setTitle("Store V" + getPackageManager()
                    .getPackageInfo(getPackageName(), 0)
                    .versionName);
        } catch (NameNotFoundException e) {
            e.printStackTrace();
        }

        mStore = new Store();
    }

    private void onGetValue() {
        String lKey = mUIKeyEdit.getText().toString();
        StoreType lType = (StoreType) mUITypeSpinner
                .getSelectedItem();
        switch (lType) {
        case Integer:
            mUIValueEdit.setText(Integer.toString(mStore
                    .getInteger(lKey)));
            break;
        case String:
            mUIValueEdit.setText(mStore.getString(lKey));
            break;
        }
    }
}

