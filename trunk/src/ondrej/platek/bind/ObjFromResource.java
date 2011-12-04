package ondrej.platek.bind;

import java.io.FileNotFoundException;
import java.io.InputStreamReader;

import android.app.Activity;

public class ObjFromResource extends ObjSource {
	private int resID;
	public ObjFromResource(int resID) {
		this.resID = resID;
	}
	
	@Override
	public InputStreamReader GetObjReader(Activity ctx) throws FileNotFoundException {
		InputStreamReader res = new InputStreamReader(ctx.getResources().openRawResource(resID));
		return res;
	}

}
