/*
 * Copyright (C) 2008 Google Inc.
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

import ondrej.platek.R;
import android.app.Activity;
import android.database.Cursor;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class NoteEdit extends Activity {

    private EditText mTitleText;
    private EditText mInfoText;
    private EditText mPathText;
    private Long mRowId;
    private boolean fromResource; 
    private ExternObjDB mDbHelper;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mDbHelper = new ExternObjDB(this);
        mDbHelper.open();

        setContentView(R.layout.note_edit);

        mTitleText = (EditText) findViewById(R.id.edit_title);
        mInfoText = (EditText) findViewById(R.id.edit_info);

        Button confirmButton = (Button) findViewById(R.id.confirm);

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

        confirmButton.setOnClickListener(new View.OnClickListener() {

            public void onClick(View view) {
                setResult(RESULT_OK);
                finish();
            }

        });
    }

    private void populateFields() {
        if (mRowId != null) {
            Cursor note = mDbHelper.fetchNote(mRowId);
            startManagingCursor(note);
            mTitleText.setText(note.getString(
                    note.getColumnIndexOrThrow(ExternObjDB.KEY_TITLE)));
            mInfoText.setText(note.getString(
                    note.getColumnIndexOrThrow(ExternObjDB.KEY_INFO)));
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
        String title = mTitleText.getText().toString();
        String info = mInfoText.getText().toString();
        String path = mPathText.getText().toString();
        

        if (mRowId == null) {
            long id = mDbHelper.createNote(title,path,info);
            if (id > 0) {
                mRowId = id;
            }
        } else {
        	if(fromResource) {
        		mDbHelper.updateNote(mRowId, title, info);
        	}
        }
    }

}
