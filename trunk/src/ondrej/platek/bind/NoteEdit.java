package ondrej.platek.bind;

import ondrej.platek.R;
import ondrej.platek.fileDialog.FileDialog;
import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;

public class NoteEdit extends Activity {
	private static final int NOTE_EDIT = 13;
	private static String TAG = "NoteEdit";
    private EditText titleText;
    private EditText infoText;
    private EditText pathText;
    private Long mRowId;
    private boolean fromResource; 
    private ExternObjDB mDbHelper;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mDbHelper = new ExternObjDB(this);
        mDbHelper.open();

        setContentView(R.layout.note_edit);

        titleText = (EditText) findViewById(R.id.edit_title);
        infoText = (EditText) findViewById(R.id.edit_info);
        pathText = (EditText) findViewById(R.id.edit_path);

        Button confirmButton = (Button) findViewById(R.id.confirm);
        Button browseButton = (Button) findViewById(R.id.bt_browse);

        mRowId = (savedInstanceState == null) ? null :
            (Long) savedInstanceState.getSerializable(ExternObjDB.KEY_ROWID);
		if (mRowId == null) {
			Bundle extras = getIntent().getExtras();
			if(extras != null) {				
				mRowId =  extras.getLong(ExternObjDB.KEY_ROWID);
				fromResource = mDbHelper.isFromResource(mRowId);
			} else {
				mRowId = null;
				fromResource = false;
			}
		}

		populateFields();

        confirmButton.setOnClickListener(new OnClickListener() {
			@Override
            public void onClick(View view) {
            	saveState();
                setResult(RESULT_OK);
                finish();
            }
        });
        browseButton.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				browseForPath();
			}
		});
    }
    
    private void browseForPath(){
		FileDialog.StartFileDialog(this, NOTE_EDIT);
    }

    public synchronized void onActivityResult(final int requestCode, int resultCode, final Intent data) {
        if (resultCode == Activity.RESULT_OK) {

	        switch(requestCode) {
	            case NOTE_EDIT:
	            	String path = data.getStringExtra(FileDialog.RESULT_PATH);
		            pathText.setText(path); 
	                break;
	            default:
	            	// for future "intends"
	            	break;
	        }                
        }
    }
    
    private void populateFields() {
        if (mRowId != null) {
            Cursor note = mDbHelper.fetchNote(mRowId);
            startManagingCursor(note);
            titleText.setText(note.getString(note.getColumnIndexOrThrow(
            		ExternObjDB.KEY_TITLE)));
            infoText.setText(note.getString(note.getColumnIndexOrThrow(
            		ExternObjDB.KEY_INFO)));
            pathText.setText(note.getString(note.getColumnIndex(
            		ExternObjDB.KEY_PATH)));
        }
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        saveState();
        outState.putSerializable(ExternObjDB.KEY_ROWID, mRowId);
    }

    @Override
    protected void onPause() {
        super.onPause();
        saveState();
    }

    @Override
    protected void onResume() {
        super.onResume();
        populateFields();
    }

    private void saveState() {
        String title = titleText.getText().toString();
        String info = infoText.getText().toString();
        String path = pathText.getText().toString();
        

        if (mRowId == null) {
        	Log.i(TAG, "Creating Note: " + title + ", path: " + path + ", " + info );
            long id = mDbHelper.createNote(title,path,info);
            if (id > 0) {
                mRowId = id;
            }
        } else {
        	if(fromResource) {
        		Log.i(TAG, "Updating Note: " + title + ", path: " + path + ", " + info );
        		mDbHelper.updateNote(mRowId, title, path, info);
        	}
        }
    }

}
