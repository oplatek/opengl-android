package ondrej.platek.bind;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import ondrej.platek.objLoader.OBJParser;
import ondrej.platek.objLoader.TDModel;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;
import android.util.Log;


public class NativeRenderer implements Renderer {
	private static String TAG = "NativeRendererJavaPart";
	
	private String objfile ; 
	
	private int vertexes_size = 0;
	private float[] vertexes;
	
	private int width;
	private int height;
	
	private boolean paused = false;
	
	/** Triangle instance */
	private OBJParser parser;
	private TDModel model;

	
    private native void init();
    private native void releaseCppResources();
    private native void step();
     
    NativeRenderer() {
         System.loadLibrary("gl_code");
    }
	public NativeRenderer(String objFile) {
		this.objfile = objFile;
	}
     @Override
     protected void finalize(){
    	releaseCppResources(); 
     }
	
    public void onDrawFrame(GL10 glUnused) { 
    	if(!getPaused()) {
    		step();
    	}
    }
    
    public void onSurfaceChanged(GL10 glUnused, int Width, int Height) {
		parser = new OBJParser();
		// TODO put the file name into variable
		model = parser.parseOBJ(objfile);
		
		// TODO is this good method to set it up? 
		// do I need to parse the obj every time -> NO
		// does the surface change every time I need to load new object -> no
		// TODO result: create different methods
		vertexes = model.GetVertexArr();
		vertexes_size = model.VertexNumber(); 
		width = Width;
		height = Height;
		// Native part initialize width, height, vertexes and vertexes_size
		init();
		Log.i(TAG, "Changed screen: widht screen " + Integer.toString(width) + ", height " + Integer.toString(width));
    }
    
    public void onSurfaceCreated(GL10 glUnused, EGLConfig config) {
    	// TODO separate onSurfaceChanged and onSurfaceCreated if necessary
    }
    
	public void setPaused(boolean v) { this.paused = v; }
	public boolean getPaused() {return this.paused; }
	public boolean togglePause() { 
		setPaused(!getPaused());
		return getPaused(); 
	}
	
	public String GetObjFile(){
		return this.objfile;
	}
} // end of class Renderer