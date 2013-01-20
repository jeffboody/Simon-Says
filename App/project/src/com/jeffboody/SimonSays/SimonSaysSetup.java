/*
 * Copyright (c) 2013 Jeff Boody
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

package com.jeffboody.SimonSays;

import android.util.Log;
import android.app.Activity;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;
import android.content.Intent;
import java.lang.String;
import android.widget.Spinner;
import android.widget.ArrayAdapter;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.content.Intent;
import java.util.Set;
import java.util.ArrayList;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;

public class SimonSaysSetup extends Activity implements OnItemSelectedListener
{
	private static final String TAG = "SimonSays";

	// app state
	private String            mBluetoothAddress;
	private ArrayList<String> mArrayListBluetoothAddress;

	// UI
	private Spinner      mSpinnerDevices;
	private ArrayAdapter mArrayAdapterDevices;

	public SimonSaysSetup()
	{
		mBluetoothAddress          = null;
		mArrayListBluetoothAddress = new ArrayList<String>();
	}

	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		// initialize UI
		setContentView(R.layout.main);
		ArrayList<String> items = new ArrayList<String>();
		mSpinnerDevices         = (Spinner) findViewById(R.id.ID_PAIRED_DEVICES);
		mArrayAdapterDevices    = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, items);
		mSpinnerDevices.setOnItemSelectedListener(this);
		mArrayAdapterDevices.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		mSpinnerDevices.setAdapter(mArrayAdapterDevices);
	}

	@Override
	protected void onResume()
	{
		super.onResume();

		// update the paired device(s)
		BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
		Set<BluetoothDevice> devices = adapter.getBondedDevices();
		mArrayAdapterDevices.clear();
		mArrayListBluetoothAddress.clear();
		if(devices.size() > 0)
		{
			for(BluetoothDevice device : devices)
			{
				mArrayAdapterDevices.add(device.getName());
				mArrayListBluetoothAddress.add(device.getAddress());
			}
		}
		else
		{
			mBluetoothAddress = null;
		}
	}

	@Override
	protected void onPause()
	{
		super.onPause();
	}

	@Override
	protected void onDestroy()
	{
		super.onDestroy();
	}

	/*
	 * Spinner callback
	 */

	public void onItemSelected(AdapterView<?> parent, View view, int pos, long id)
	{
		mBluetoothAddress = mArrayListBluetoothAddress.get(pos);
	}

	public void onNothingSelected(AdapterView<?> parent)
	{
		mBluetoothAddress = null;
	}

	/*
	 * buttons
	 */

	public void onBluetoothSettings(View view)
	{
		Intent i = new Intent(android.provider.Settings.ACTION_BLUETOOTH_SETTINGS);
		startActivity(i);
	}

	public void onPlayGame(View view)
	{
		if(mBluetoothAddress == null)
		{
			Log.e(TAG, "No Bluetooth device selected");
		}
		else
		{
			Intent i = new Intent("com.jeffboody.SimonSays.action.PLAY");
			i.putExtra("ADDRESS", mBluetoothAddress);
			startActivity(i);
		}
	}
}
