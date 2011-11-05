/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package ondrej.platek.bind;

import ondrej.platek.bind.R;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;



public class BINDActivity extends Activity {
    BINDView mView;
    
	private void launchScreensaverSettings() {
    	debugDoGLAction();
		// TODO Auto-generated method stub
		
	}
	private void launchHelp(String string) {
    	debugDoGLAction();
		// TODO Auto-generated method stub
		
	}

    private boolean explanation;
    public void setExplanation(boolean v) { 
    	this.explanation = v;
    	debugDoGLAction();
    }
    public boolean getExplanation() { return this.explanation; }

    private boolean cameraStatic;
    public void setCameraStatic(boolean b) {
    	this.explanation = b;
    	debugDoGLAction();
	}
    public boolean getCameraStatic() { return this.cameraStatic; }
    

    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        mView = new BINDView(getApplication());
        /*
        FrameLayout main = (FrameLayout) findViewById(R.layout.main); 
        main.addView(mView);
        setContentView(R.layout.main);*/
        setContentView(mView);
    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
    }
    
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        Log.i("motion", event.toString());
        debugDoGLAction();
        return true;
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
    	getMenuInflater().inflate(R.menu.play, menu);
    	return true;
    }
    
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle item selection
        switch (item.getItemId()) {
        case R.id.help_about:
        	launchHelp("About");
        	return true;
        case R.id.help_nav:
        	launchHelp("Navigation");
        	return true;
        case R.id.explanations_off:
        	setExplanation(false); 
        	return true;
        case R.id.explanations_on:
        	setExplanation(true); 
        	return true;
        case R.id.screensaver:
        	launchScreensaverSettings();
        	return true;
        case R.id.knot_select:
        	// TODO ListView
        case R.id.cameraStatic:
        	setCameraStatic(true);
        	return true;
        case R.id.cameraDynamic:
        	setCameraStatic(false);
        	return true;
        default:
            return super.onOptionsItemSelected(item);
        }
    }
    
	//  debugging help functions START TODO delete it after usage
    boolean debug_switch = false;
    private void debugDoGLAction() {
      debug_switch = !debug_switch;
      if (debug_switch) {
        this.onPause();
      }
      else {
        this.onResume(); 
      } 
    }
    // debugging help functions END 

}
