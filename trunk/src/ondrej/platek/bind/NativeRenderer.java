package ondrej.platek.bind;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import ondrej.platek.objLoader.TDModel;
import android.opengl.GLSurfaceView.Renderer;
import android.util.Log;


public class NativeRenderer implements Renderer {
    // right place to load native OpenGL ES library,
	private static String TAG = "NativeRendererJavaPart";
	
    static {
    	System.loadLibrary("test");
    	// TODO move EGL set up from java to native code
    	// change init BINDView to native!
    }
    
	private String objfile ; 
	
	private int pAppCtx = 0; // set the pointer to NULL !!!
	private int vertices_size = 0;
	private float[] vertices;
	private int parts_number;
	private int[] parts_sizes;
	private float[][] normals;
	private short[][] faces;
	
	private int width;
	private int height;
	
	private boolean paused = false;
	
	private TDModel model;
	
    private native void initV(float[][] normals, short[][] faces);
    private native void initG();
    private native void step();
    private native void changeWH();
    private native void releaseCppResources();
    public native void Zoom(float dz);    
    public native void RotateAnchor(float dy,float dx);

    
	public NativeRenderer(TDModel Model) {
		model = Model;
    	SetModel(model);
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
		width = Width;
		height = Height;
		changeWH();
		
		// Native part initialize AppCtx, width, height, vertexes..
		Log.i(TAG, "gonna change screen " + Integer.toString(width) + ", height " + Integer.toString(Height));
    }
    
    public void onSurfaceCreated(GL10 glUnused, EGLConfig config) {
		initG();
    }
    
    public void SetModel(TDModel NewModel) {    	
    	model = NewModel;
		vertices = model.GetVertexArr();
		vertices_size = model.getVertexNumber(); 
		parts_number = model.getPartsNumber();
		normals = model.getNormals();
		faces = model.getFaces();
		parts_sizes = model.getPartsSizes();
		
		initV(normals, faces);
		
//		for(int i=0; i<parts_number; ++i) {
//			for(int j=0; j<parts_sizes[i]; ++j) {
//				short[]tmp = faces[i];
//				Log.i(TAG, Integer.toString(i) +". face[ " + Integer.toString(j) +"]= " + Short.toString(tmp[j]));				
//			}
//		}
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