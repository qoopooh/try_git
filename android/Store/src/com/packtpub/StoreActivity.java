package com.packtpub;

import java.lang.NumberFormatException;
import java.lang.IllegalArgumentException;

import android.app.Activity;
import android.os.Bundle;
import android.content.pm.PackageManager.NameNotFoundException;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import android.widget.Spinner;

import com.packtpub.exception.*;

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

        mUIKeyEdit = (EditText) findViewById(R.id.uiKeyEdit);
        mUIValueEdit = (EditText) findViewById(R.id.uiValueEdit);
        mUITypeSpinner = (Spinner) findViewById(R.id.uiTypeSpinner);
        mUITypeSpinner.setAdapter(new ArrayAdapter<StoreType>(this,
            android.R.layout.simple_spinner_item, StoreType.values()));
        mUIGetButton = (Button) findViewById(R.id.uiGetValueButton);
        mUIGetButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                onGetValue();
            }
        });
        mUISetButton = (Button) findViewById(R.id.uiSetValueButton);
        mUISetButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                onSetValue();
            }
        });

        mStore = new Store();
    }

    private void onGetValue() {
        String lKey = mUIKeyEdit.getText().toString();
        StoreType lType = (StoreType) mUITypeSpinner.getSelectedItem();

        try {
            String lValue = "";
            switch (lType) {
            case Integer:
                lValue = Integer.toString(mStore
                        .getInteger(lKey));
                break;
            case String:
                lValue = mStore.getString(lKey);
                break;
            case Color:
                Color c = mStore.getColor(lKey);
                if (c != null)
                    lValue = c.toString();
                break;
            }
            mUIValueEdit.setText(lValue);
        } catch (NotExistingKeyException e) {
            displayError("Key does not exist in store");
        } catch (InvalidTypeException e) {
            displayError("Incorrect type.");
        }
    }

    private void onSetValue() {
        String lKey = mUIKeyEdit.getText().toString();
        String lValue = mUIValueEdit.getText().toString();
        StoreType lType = (StoreType) mUITypeSpinner.getSelectedItem();

        try {
            switch (lType) {
            case Integer:
                mStore.setInteger(lKey, Integer.parseInt(lValue));
                break;
            case String:
                mStore.setString(lKey, lValue);
                break;
            case Color:
                mStore.setColor(lKey, new Color(lValue));
                break;
            }
        } catch (NumberFormatException e) {
            displayError("Incorrect value (Number Format).");
        } catch (IllegalArgumentException e) {
            displayError("Incorrect value (Illegal Argument).");
        } catch (StoreFullException e) {
            displayError("Store is full.");
        }
    }

    private void displayError(String pError) {
        Toast.makeText(getApplicationContext(), pError, Toast.LENGTH_LONG).show();
    }
}

