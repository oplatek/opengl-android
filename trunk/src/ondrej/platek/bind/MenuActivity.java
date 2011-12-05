package ondrej.platek.bind;


import java.util.SortedMap;
import java.util.TreeMap;

import ondrej.platek.R;
import ondrej.platek.fileDialog.FileDialog;
import android.app.Activity;
import android.app.ListActivity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

public class MenuActivity extends ListActivity {
	private static final int ADD_OBJ = 0;
	private static final int LOAD_XML = 1;
	private SortedMap<String, Object> objSources = new TreeMap<String, Object>();

	
	@Override
	protected void onListItemClick(ListView l, View v, int position, long id) {
		String key = (String) l.getItemAtPosition(position);
		ObjSource source = (ObjSource) objSources.get(key);
		
        Bundle bundle = new Bundle();

        bundle.putSerializable(ObjSource.TITLE, source);

        Intent mIntent = new Intent();
        mIntent.putExtras(bundle);
        setResult(RESULT_OK, mIntent);
        finish();
	}
	
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		prepareMenu();
		updateList();
	}
	
	private void updateList(){
		// TODO update this
		String[] keys = objSources.keySet().toArray( new String[objSources.keySet().size()]);
		setListAdapter(new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, keys));
	}
	
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        super.onCreateOptionsMenu(menu);
    	getMenuInflater().inflate(R.menu.load, menu);
        return true;
    }
    
    void startFileDialog(int signal) {
    	Intent intent = new Intent(this, FileDialog.class);
		intent.putExtra(FileDialog.START_PATH, "/sdcard");
		startActivityForResult(intent, signal);
    }
    
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle item selection
        switch (item.getItemId()) {
        case R.id.load_xml:
        	startFileDialog(LOAD_XML);
        	return true;
        case R.id.add_obj:
        	startFileDialog(ADD_OBJ);
        	return true;
    	default:
    		return super.onOptionsItemSelected(item);
        }
    }
    
    public synchronized void onActivityResult(final int requestCode, int resultCode, final Intent data) {
        if (resultCode == Activity.RESULT_OK) {

	        switch(requestCode) {
	            case ADD_OBJ:	            	
		            String path2obj = data.getStringExtra(FileDialog.RESULT_PATH);
		            int d = path2obj.length() -1;
		            if( d > 3 && (path2obj.substring(d-3,d).toLowerCase() != "obj") ) {
			            addMenuItem("Loaded", path2obj);
			            updateList();
		            } else {
		            	Toast.makeText(this, R.string.not_obj, Toast.LENGTH_LONG);
		            } 
	                break;
	            case LOAD_XML:
		            String xmlpath = data.getStringExtra(FileDialog.RESULT_PATH);
		            
	                break;
	            default:
	            	// for future "intends"
	            	break;
	        }
                

        }
    }
    
    
	public void addMenuItem(String label, String path){
		objSources.put(label, new ObjFromSDcard(path));
	}
	public void addMenuItem(String label, int resID){
		objSources.put(label, new ObjFromResource(resID));
	}
	
	void prepareMenu() {		
		addMenuItem(getString(R.string.cube), R.raw.cube);
		addMenuItem(getString(R.string.triangle), R.raw.triangle);
		addMenuItem("Sdcard_cube","/sdcard/opengl-android.obj");
		
		//TODO fill menu
	}
}
