/*
 * Copyright (C) 2008 Google Inc.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

package ondrej.platek.bind;

import java.security.KeyStore.LoadStoreParameter;

import ondrej.platek.R;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;

public class ExternObjDB {

    private static final String DATABASE_NAME = "externalData";
    private static final int DATABASE_VERSION = 4;
    
    private static final String TABLE_OBJ = "objSources";
    public static final String KEY_ROWID = "_id";
    public static final String KEY_TITLE = "title";
    public static final String KEY_PATH = "path";
    public static final String KEY_RESRC_ID = "resource_id";
    public static final String KEY_INFO = "info";

    private static final String TABLE_PREFERENCES = "preferences";
    public static final String  KEY_PREFERENCE = "preference"; 
    public static final String KEY_STATE = "preference_state"; 
    
    public static final String PREF_DB_LD = "db_loaded";
    public static final String VALUE_NO = "no";
    
    private static final String TAG = "ExternOBJ_DB";
    private DatabaseHelper mDbHelper;
    private SQLiteDatabase mDb;
    
    
    /**
     * Database creation sql statement
     */
    private static final String DATABASE_CREATE00 = "drop table if exists preferences;" ;
    private static final String DATABASE_CREATE0 = "drop table if exists objSources;" ;
    private static final String DATABASE_CREATE2 =
    		"create table preferences( _id integer primary key autoincrement, preference text not null, preference_state text not null);" ;
    private static final String DATABASE_CREATE3 =
    		"insert into preferences (preference,preference_state) values ('db_loaded','no');" ;
    private static final String DATABASE_CREATE1 =
//    		"create table objSources( _id integer primary key autoincrement, title text not null, path text, resource_id integer default -1, info text not null);" ;
        "create table " + TABLE_OBJ +    "( "  
        		+ KEY_ROWID + " integer primary key autoincrement, "
        		+ KEY_TITLE + " text not null, "
        		+ KEY_PATH + " text, " // it has to nullable
        		+ KEY_RESRC_ID + " integer default -1, " // read this only if KEY_PATH is null
        		+ KEY_INFO + " text not null); " 
//        + 
//        " create table " + TABLE_PREFERENCES +    "( "  
//        		+ KEY_ROWID + " integer primary key autoincrement, "
//        		+ KEY_PREFERENCE + " text not null, "
//        		+ KEY_STATE + " text not null); " 
//        +
//        " insert into " + TABLE_PREFERENCES + " (" + KEY_PREFERENCE +","+ KEY_STATE + ") values ('" + PREF_DB_LD + "','" + VALUE_NO +"');"
        		;
    private static final String SQL_RETURN_LOADED = 
    		"select preference_state from preferences where preference='db_loaded';";
//    		"select " + KEY_STATE + " from " + TABLE_PREFERENCES 
//    		+ " where " + KEY_PREFERENCE + "='" + PREF_DB_LD + "';";

    protected final Context mCtx;

    private static class DatabaseHelper extends SQLiteOpenHelper {

        DatabaseHelper(Context context) {
            super(context, DATABASE_NAME, null, DATABASE_VERSION);
        }

        @Override
        public void onCreate(SQLiteDatabase db) {
        	Log.i(TAG,DATABASE_CREATE00);        	
            db.execSQL(DATABASE_CREATE00);
        	Log.i(TAG,DATABASE_CREATE0);        	
            db.execSQL(DATABASE_CREATE0);
        	Log.i(TAG,DATABASE_CREATE1);        	
            db.execSQL(DATABASE_CREATE1);
        	Log.i(TAG,DATABASE_CREATE2);        	
            db.execSQL(DATABASE_CREATE2);
        	Log.i(TAG,DATABASE_CREATE3);        	
            db.execSQL(DATABASE_CREATE3);
        }

        @Override
        public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
            Log.w(TAG, "Upgrading database from version " + oldVersion + " to "
                    + newVersion + ", which will destroy all old data");
            db.execSQL("DROP TABLE IF EXISTS " + TABLE_OBJ + ";");
            db.execSQL("DROP TABLE IF EXISTS " + TABLE_PREFERENCES+ ";");
            onCreate(db);
        }
    }

    
    /**
     * Constructor - takes the context to allow the database to be
     * opened/created
     * 
     * @param ctx the Context within which to work
     */
    public ExternObjDB(Context ctx) {    	
        this.mCtx = ctx;
    }

    /**
     * Open the notes database. If it cannot be opened, try to create a new
     * instance of the database. If it cannot be created, throw an exception to
     * signal the failure
     * 
     * @return this (self reference, allowing this to be chained in an
     *         initialization call)
     * @throws SQLException if the database could be neither opened or created
     */
    public ExternObjDB open() throws SQLException {
        mDbHelper = new DatabaseHelper(mCtx);
        mDb = mDbHelper.getWritableDatabase();
        defaultObjFromResource();
        return this;
    }

    public void close() {
        mDbHelper.close();
    }


    /**
     * Create a new note using the title and body provided. If the note is
     * successfully created return the new rowId for that note, otherwise return
     * a -1 to indicate failure.
     * 
     * @param title the title of the note
     * @param body the body of the note
     * @return rowId or -1 if failed
     */
    public long createNote(String title, String path,  String info) {
        ContentValues initialValues = new ContentValues();
        initialValues.put(KEY_TITLE, title);
        initialValues.put(KEY_PATH, path);
        initialValues.put(KEY_INFO, info);

        return mDb.insert(TABLE_OBJ, null, initialValues);
    }

    public long createNote(String title,  int resource_id, String info) {
        ContentValues initialValues = new ContentValues();
        initialValues.put(KEY_TITLE, title);
    	initialValues.put(KEY_RESRC_ID, resource_id);
        initialValues.put(KEY_INFO, info);

        return mDb.insert(TABLE_OBJ, null, initialValues);
    }
    
    /**
     * Delete the note with the given rowId
     * 
     * @param rowId id of note to delete
     * @return true if deleted, false otherwise
     */
    public boolean deleteNote(long rowId) {

        return mDb.delete(TABLE_OBJ, KEY_ROWID + "=" + rowId, null) > 0;
    }

    /**
     * Return a Cursor over the list of all notes in the database
     * 
     * @return Cursor over all notes
     */
    public Cursor fetchAllNotes() {

        return mDb.query(TABLE_OBJ, new String[] {
        		KEY_ROWID, KEY_TITLE, KEY_PATH, KEY_RESRC_ID, KEY_INFO}, 
        		null, null, null, null, null);
    }

    /**
     * Return a Cursor positioned at the note that matches the given rowId
     * 
     * @param rowId id of note to retrieve
     * @return Cursor positioned to matching note, if found
     * @throws SQLException if note could not be found/retrieved
     */
    public Cursor fetchNote(long rowId) throws SQLException {

        Cursor mCursor =

            mDb.query(true, TABLE_OBJ, new String[] {
            		KEY_ROWID, KEY_TITLE, KEY_PATH, KEY_RESRC_ID, KEY_INFO}, KEY_ROWID + "=" + rowId, null,
                    null, null, null, null);
        if (mCursor != null) {
            mCursor.moveToFirst();
        }
        return mCursor;

    }

    /**
     * Update the note using the details provided. The note to be updated is
     * specified using the rowId, and it is altered to use the title and body
     * values passed in
     * 
     * @param rowId id of note to update
     * @param title value to set note title to
     * @return true if the note was successfully updated, false otherwise
     */
    public boolean updateNote(long rowId, String title, String path, String info) {
        ContentValues args = new ContentValues();
        args.put(KEY_TITLE, title);
        args.put(KEY_PATH, path);
        args.put(KEY_INFO, info);

        return mDb.update(TABLE_OBJ, args, KEY_ROWID + "=" + rowId, null) > 0;
    }
    
    public boolean updateNote(long rowId, String title, String info) {
        ContentValues args = new ContentValues();
        args.put(KEY_TITLE, title);
        args.put(KEY_INFO, info);

        return mDb.update(TABLE_OBJ, args, KEY_ROWID + "=" + rowId, null) > 0;
    }
    
    public boolean isFromResource(long rowId){
        Cursor mCursor =
            mDb.query(true, TABLE_OBJ, 
            		new String[] { KEY_RESRC_ID}, KEY_ROWID + "=" + rowId, null,
                    null, null, null, null);        
        mCursor.moveToFirst();
        // TODO hardcoded value for column KEY_RESRC_ID = "resource_id"
        int key_resrc_id =mCursor.getInt(mCursor.getColumnIndex(KEY_RESRC_ID));
        // resources have resource id > 0
        return (key_resrc_id != -1);    	
    }
    
	void defaultObjFromResource() {	
//		SELECT name FROM sqlite_master WHERE type='table' AND name='table_name';
//		Cursor c = mDb.query(TABLE_PREFERENCES, new String[]{ KEY_STATE }, KEY_PREFERENCE + "='" + PREF_DB_LD + "'", null, null, null, null, null);
		Cursor t =mDb.rawQuery("select name from sqlite_master;",null);
		int ind = t.getColumnIndex("name");
		t.moveToFirst();
		Log.i(TAG, t.getString(ind));
		while(t.moveToNext()) {
			Log.i(TAG, t.getString(ind));
		}
		
		Log.i(TAG,SQL_RETURN_LOADED);
		Cursor c = mDb.rawQuery(SQL_RETURN_LOADED, null);
		if(c.moveToFirst()){
			String populated = c.getString(c.getColumnIndex(KEY_STATE));
			if( populated == VALUE_NO ) {
				Log.i(TAG, "the database was NOT populated. Let's populated");
				this.createNote(mCtx.getString(R.string.cube), R.raw.cube, mCtx.getString(R.string.cube_info));
				this.createNote(mCtx.getString(R.string.triangle), R.raw.triangle, mCtx.getString(R.string.triangle_info));
				
				// TODO not to load default sources from sdcard
				this.createNote("Sdcard_cube","/sdcard/opengl-android.obj","test info");
			}
		}
		else {
			Log.e(TAG,"Could not determine if database was populated with default items");
		}
	}
}