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
import android.os.Looper;
import com.jeffboody.a3d.A3DSurfaceView;
import com.jeffboody.a3d.A3DNativeRenderer;
import com.jeffboody.a3d.A3DResource;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import com.jeffboody.BlueSmirf.BlueSmirfSPP;

public class SimonSays extends Activity implements Runnable
{
	private static final String TAG = "SimonSays";

	private A3DNativeRenderer Renderer;
	private A3DSurfaceView    Surface;

	// Bluetooth
	private boolean      mIsRunning        = false;
	private BlueSmirfSPP mSPP              = new BlueSmirfSPP();
	private String       mBluetoothAddress = "none";

	// Parser state
	private static final int PARSER_BEGIN     = 0;
	private static final int PARSER_LED       = 1;
	private static final int PARSER_BUTTON    = 2;
	private static final int PARSER_MESSAGE   = 3;
	private static final int PARSER_MAX_COUNT = 32;
	private int mParserState = PARSER_BEGIN;

	// Native interface
	private native void NativeLed(int a, int b, int c, int d);
	private native void NativeButton(int a, int b, int c, int d);
	private native void NativeMessage(String message);

	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		Intent i = getIntent();
		mBluetoothAddress = i.getStringExtra("ADDRESS");

		A3DResource r = new A3DResource(this, R.raw.timestamp);
		r.Add(R.raw.whitrabt, "whitrabt.texgz");

		// Make window fullscreen
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
		                     WindowManager.LayoutParams.FLAG_FULLSCREEN);

		Renderer = new A3DNativeRenderer(this);
		Surface  = new A3DSurfaceView(Renderer, r, this);
		setContentView(Surface);
	}

	@Override
	protected void onResume()
	{
		super.onResume();
		Surface.ResumeRenderer();

		// start Bluetooth thread
		Thread t = new Thread(this);
		t.start();

		mIsRunning = true;
	}

	@Override
	protected void onPause()
	{
		mIsRunning = false;
		mSPP.writeByte('d');
		mSPP.disconnect();
		Surface.PauseRenderer();
		super.onPause();
	}

	@Override
	protected void onDestroy()
	{
		Surface.StopRenderer();
		Surface = null;
		Renderer = null;
		super.onDestroy();
	}

	/*
	 * main loop
	 */

	public void run()
	{
		int a          = 0;
		int b          = 0;
		int c          = 0;
		int d          = 0;
		int count      = 0;
		byte[] message = new byte[PARSER_MAX_COUNT];

		Looper.prepare();

		while(mIsRunning)
		{
			NativeMessage("Connecting");
			mSPP.connect(mBluetoothAddress);
			mSPP.writeByte('c');
			while(mSPP.isConnected())
			{
				if(mSPP.isError())
				{
					mSPP.disconnect();
					break;
				}

				byte ch = (byte) mSPP.readByte();

				if(ch == '\r')
				{
					// ignore '\r'
					continue;
				}
				else if(ch == '\n')
				{
					// end of command / message

					if(mParserState == PARSER_LED)
					{
						NativeLed(a, b, c, d);
					}
					else if(mParserState == PARSER_BUTTON)
					{
						NativeButton(a, b, c, d);
					}
					else if(mParserState == PARSER_MESSAGE)
					{
						NativeMessage(new String(message, 0, count));
					}
					mParserState = PARSER_BEGIN;
				}
				else if(mParserState == PARSER_BEGIN)
				{
					// initialize state
					a     = 0;
					b     = 0;
					c     = 0;
					d     = 0;
					count = 0;

					// check command type
					if(ch == '*')
					{
						mParserState = PARSER_LED;
					}
					else if(ch == '#')
					{
						mParserState = PARSER_BUTTON;
					}
					else
					{
						mParserState = PARSER_MESSAGE;
					}
				}
				else if((mParserState == PARSER_LED) ||
				        (mParserState == PARSER_BUTTON))
				{
					// led lit or button pressed

					if(ch == 'A')
					{
						a = 1;
					}
					else if(ch == 'B')
					{
						b = 1;
					}
					else if(ch == 'C')
					{
						c = 1;
					}
					else if(ch == 'D')
					{
						d = 1;
					}
				}

				// always process messages in case we drop
				// through from PARSER_BEGIN
				if((mParserState == PARSER_MESSAGE) &&
				   (count < PARSER_MAX_COUNT))
				{
					message[count++] = ch;
				}
			}
			if(mIsRunning)
			{
				NativeMessage("Disconnected");
				try { Thread.sleep(1000); }
				catch(InterruptedException e) { }
			}
		}
	}

	static
	{
		System.loadLibrary("a3d");
		System.loadLibrary("SimonSays");
	}
}
