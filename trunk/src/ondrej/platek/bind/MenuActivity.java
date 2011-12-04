package ondrej.platek.bind;


import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStreamReader;
import java.util.SortedMap;
import java.util.TreeMap;

import android.app.ListActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

public abstract class MenuActivity extends ListActivity {
	private SortedMap<String, Object> actions = new TreeMap<String, Object>();
	private InputStreamReader defaultReader;
	private InputStreamReader returnReader;

	MenuActivity(InputStreamReader r) {
		returnReader = r;
		try{
			// TODO set up more wisely
			defaultReader = new InputStreamReader( getResources().openRawResource(R.raw.cube));
		} catch (Exception e) {
			Toast.makeText(this,"No default reader loaded. Report the bug please",Toast.LENGTH_LONG);
		}
	}
	
	todo how to pass returnReader between two classes
	todo 
	private void prepareReader(String key) {
		try{
			ObjSource s = (ObjSource) actions.get(key);
			returnReader = s.GetObjReader();
        } catch (FileNotFoundException e) {
			Toast.makeText(this, "File not found! The default definition of model is used instead", Toast.LENGTH_SHORT);
			returnReader = defaultReader;			
		}
	}
	
	@Override
	protected void onListItemClick(ListView l, View v, int position, long id) {
		String key = (String) l.getItemAtPosition(position);
		String path = (String) actions.get(key);
		
	}
	
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		prepareMenu();

		String[] keys = actions.keySet().toArray(
				new String[actions.keySet().size()]);

		// TODO update this
		setListAdapter(new ArrayAdapter<String>(this,
				android.R.layout.simple_list_item_1, keys));
	}
	
	public void addMenuItem(String label, String path){
		actions.put(label, new ObjFromSDcard(path));
	}
	public void addMenuItem(String label, int resID){
		actions.put(label, new ObjFromResource(resID,this));
	}
	
	abstract void prepareMenu();
}
