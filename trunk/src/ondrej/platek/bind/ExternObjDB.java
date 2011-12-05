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
    private static final int DATABASE_VERSION = 2;
    
    private static final String TABLE_OBJ = "objSources";
    public static final String KEY_ROWID = "_id";
    public static final String KEY_TITLE = "title";
    public static final String KEY_PATH = "path";
    public static final String KEY_RESRC_ID = "resource_id";
    public static final String KEY_INFO = "info";

    private static final String TABLE_PREFERENCES = "preferences";
    public static final String  COLL_ID = "_id"; 
    private static final int SINGLE_ROW_ID = 1;
    private static final int  STATE_LOADED = 0;
    public static final String COLL_LOADED = "resources_loaded"; 
    
    
    private static final String TAG = "ExternOBJ_DB";
    private DatabaseHelper mDbHelper;
    private SQLiteDatabase mDb;
    
    
    /**
     * Database creation sql statement
     */
    private static final String DATABASE_CREATE =
        "create table " + TABLE_OBJ +    "( "  
        		+ KEY_ROWID + " integer primary key autoincrement, "
        		+ KEY_TITLE + " text not null, "
        		+ KEY_PATH + " text, " // it has to nullable
        		+ KEY_RESRC_ID + " integer default -1, " // read this only if KEY_PATH is null
        		+ KEY_INFO + " text not null);" 
        + 
        "create table " + TABLE_PREFERENCES + "( "
        		+ COLL_ID  + " integer primary key, "
        		+ COLL_LOADED + " integer);"        		
        		;


    protected final Context mCtx;

    private static class DatabaseHelper extends SQLiteOpenHelper {

        DatabaseHelper(Context context) {
            super(context, DATABASE_NAME, null, DATABASE_VERSION);
        }

        @Override
        public void onCreate(SQLiteDatabase db) {
            db.execSQL(DATABASE_CREATE);
            ContentValues args = new ContentValues();           
            args.put(COLL_LOADED, STATE_LOADED);
            args.put(COLL_ID, SINGLE_ROW_ID);
            try{
            	db.insertOrThrow(TABLE_PREFERENCES, null, args);
            } catch (Exception e) {
            	Log.i(TAG,e.toString());
            	e.printStackTrace();
            }
//            db.execSQL( "INSERT INTO "+TABLE_PREFERENCES+ " VALUES ("+SINGLE_ROW_ID+", 0);" );
//            db.update(TABLE_PREFERENCES, args, COLL_ID + "=" + SINGLE_ROW_ID, null) ;
        }

        @Override
        public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
            Log.w(TAG, "Upgrading database from version " + oldVersion + " to "
                    + newVersion + ", which will destroy all old data");
            db.execSQL("DROP TABLE IF EXISTS " + TABLE_OBJ);
            db.execSQL("DROP TABLE IF EXISTS " + TABLE_PREFERENCES);
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
//		try{
//	        Cursor mCursor = mDb.query(TABLE_PREFERENCES, new String[] { COLL_ID, COLL_LOADED }, 
//	        		null, null, null, null, null);
//	        mCursor.moveToFirst();
//	        int columnOfState = mCursor.getColumnIndex(COLL_LOADED);
//	        int res = mCursor.getInt(columnOfState);
//	        if( res == STATE_LOADED) {
//	        	res ++;
//	        	ContentValues args = new ContentValues();
//	        	args.put(COLL_LOADED,res);
//	            mDb.update(TABLE_PREFERENCES, args, COLL_ID + "=" + SINGLE_ROW_ID, null) ;
//				//TODO change
				this.createNote(mCtx.getString(R.string.cube), R.raw.cube, mCtx.getString(R.string.cube_info));
				this.createNote(mCtx.getString(R.string.triangle), R.raw.triangle, mCtx.getString(R.string.triangle_info));
				
				// TODO not to load default sources from sdcard
				this.createNote("Sdcard_cube","/sdcard/opengl-android.obj","test info");
//			}
//		} catch (Exception e){
//			Log.e(TAG, e.toString());
//			e.printStackTrace();
//		}
	}
}