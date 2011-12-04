package ondrej.platek.bind;

import java.io.FileNotFoundException;
import java.io.InputStreamReader;

import android.app.Activity;

public class ObjFromResource extends ObjSource {
	private int resID;
	private Activity ctx;
	public ObjFromResource(int resID, Activity ctx) {
		this.resID = resID;
	}
	
	@Override
	public InputStreamReader GetObjReader() throws FileNotFoundException {
		return new InputStreamReader(ctx.getResources().openRawResource(resID));		
	}

}
