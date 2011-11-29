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


import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.os.Bundle;
//import android.os.Debug;
import android.os.Environment;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.widget.Toast;



public class BINDActivity extends Activity {
    static private String tag = "BINDActivity.java";
    BINDView mView;
    private boolean explanation;
    private boolean cameraStatic;
    private String logfile = "/sdcard/opengl-method.log";
    private String objfile = "/sdcard/opengl-android.obj";
    
	private void launchScreensaverSettings() {
    	debugDoGLAction();
		// TODO Auto-generated method stub
	}
	
	private void launchHelp(String string) {
    	debugDoGLAction();
		// TODO Auto-generated method stub
		
	}

    public void setExplanation(boolean v) { 
    	this.explanation = v;
    	debugDoGLAction();
    }
    public boolean getExplanation() { return this.explanation; }

    public void setCameraStatic(boolean b) {
    	this.explanation = b;
    	debugDoGLAction();
	}
    public boolean getCameraStatic() { return this.cameraStatic; }
    

    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        
//		Debug.startMethodTracing(logfile);
		// the file in second parameter is located in sd-card
        mView = new BINDView(this, objfile); 
        
		// test if the SD card is working
		if(!Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())){
		    Toast.makeText(this, "External SD card not mounted", Toast.LENGTH_LONG).show();
		}
		
        // Check if the system supports OpenGL ES 2.0.
        final ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        final ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();
        final boolean supportsEs2 = configurationInfo.reqGlEsVersion >= 0x20000;
     
        if (supportsEs2) {
            // Request an OpenGL ES 2.0 compatible context.
            // mView.setEGLContextClientVersion(2); // available from Api Level 8
        }
        else {
            // This is where you could create an OpenGL ES 1.x compatible
            // renderer if you wanted to support both ES 1 and ES 2.
        	String msg = "TODO OpenGL ES 1.X is not yet supported";
			Log.e(tag, msg);
        	
        	Toast.makeText(getApplication(), msg, Toast.LENGTH_SHORT);
            return;
        }
        setContentView(mView);
    }

    @Override protected void onPause() {
//    	Debug.stopMethodTracing(); 
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() {
//		Debug.startMethodTracing(logfile);
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
    
	@Override
	protected void onDestroy(){
//		 Debug.stopMethodTracing();
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
