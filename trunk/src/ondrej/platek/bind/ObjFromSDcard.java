package ondrej.platek.bind;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStreamReader;

import android.app.Activity;

public class ObjFromSDcard extends ObjSource {
	private String path;
	public ObjFromSDcard(String path) {
		this.path = path;
	}

	@Override
	public InputStreamReader GetObjReader(Activity ctx) throws FileNotFoundException {
		return new InputStreamReader(new FileInputStream(this.path));
	}

}
