package ondrej.platek.bind;


import ondrej.platek.R;
import ondrej.platek.fileDialog.FileDialog;
import ondrej.platek.xml.Parser;
import android.app.Activity;
import android.app.ListActivity;
import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.util.Log;
import android.view.ContextMenu;
import android.view.ContextMenu.ContextMenuInfo;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView.AdapterContextMenuInfo;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;
import android.widget.Toast;

public class MenuActivity extends ListActivity {
    private static final int ACT_LOAD_XML = 1;
    private static final int ACT_EDIT_NOTE = 3;
    private static final int MENU_DELETE_NOTE = 4;
    private static final int MENU_EDIT_NOTE = 5;

    private ExternObjDB extObjDB;
    
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.knots_list);
        extObjDB = new ExternObjDB(this);
        extObjDB.open();
        
        updateList();
        
        registerForContextMenu(getListView());
    }
    
    public synchronized void onActivityResult(final int requestCode, int resultCode, final Intent data) {
        if (resultCode == Activity.RESULT_OK) {

            switch(requestCode) {
                case ACT_LOAD_XML:
                    String xmlpath = data.getStringExtra(FileDialog.RESULT_PATH);                    
                    Parser.ParseNotes(xmlpath);
                    break;
                case ACT_EDIT_NOTE:
                    updateList();                    
                    break;
                default:
                    // for future "intends"
                    break;
            }                
        }
    }
    
    @Override
    protected void onListItemClick(ListView l, View v, int position, long id) {
        super.onListItemClick(l, v, position, id);
        
        Cursor note = extObjDB.fetchNote(id);
        startManagingCursor(note);
        ObjSource s;
        int resource_id = note.getInt(
                note.getColumnIndexOrThrow(ExternObjDB.KEY_RESRC_ID));
        if(resource_id == -1) {
            // obj from sdcard
            String path = note.getString(
                    note.getColumnIndexOrThrow(ExternObjDB.KEY_PATH));
            s = new ObjFromSDcard(path);
        } 
        else {
            // obj from resource
            s = new ObjFromResource(resource_id);
        }
        s.Title = note.getString(
                note.getColumnIndexOrThrow(ExternObjDB.KEY_TITLE));
        s.Info = note.getString(
                note.getColumnIndexOrThrow(ExternObjDB.KEY_INFO));
        s.ID =  note.getInt(
                note.getColumnIndexOrThrow(ExternObjDB.KEY_ROWID));
        Bundle bundle = new Bundle();

        bundle.putSerializable(ObjSource.TITLE, s);

        Intent mIntent = new Intent();
        mIntent.putExtras(bundle);
        setResult(RESULT_OK, mIntent);
        finish();
    }
    
    @Override
    protected void onDestroy() {
        extObjDB.close();
        super.onDestroy();
    }
    
    private void updateList(){
        Cursor notesCursor = extObjDB.fetchAllNotes();
        startManagingCursor(notesCursor);
        
        // Create an array to specify the fields we want to display in the list (only TITLE)
        String[] from = new String[]{ExternObjDB.KEY_TITLE};

        // and an array of the fields we want to bind those fields to (in this case just text1)
        int[] to = new int[]{R.id.text1};
        
        // Now create a simple cursor adapter and set it to display
        SimpleCursorAdapter notes = 
            new SimpleCursorAdapter(this, R.layout.knot_row, notesCursor, from, to);
        setListAdapter(notes);       
    }
    
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        super.onCreateOptionsMenu(menu);
        getMenuInflater().inflate(R.menu.load, menu);
        return true;
    }
    
    
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle item selection
        switch (item.getItemId()) {
        case R.id.load_xml:
            FileDialog.StartFileDialog(this,ACT_LOAD_XML);
            // TODO 
            return true;
        case R.id.add_obj:
            Intent i = new Intent(this, NoteEdit.class);
            // create and edit is the same, depends on parameters
            
            startActivityForResult(i, MENU_EDIT_NOTE);            
            return true;
        case R.id.default_knots:
            extObjDB.AddDefaultKnots();
            updateList();
        case R.id.help_load:
            // TODO fill help
            return true;
        default:
            return super.onOptionsItemSelected(item);
        }
    }
    @Override
    public void onCreateContextMenu(ContextMenu menu, View v,
            ContextMenuInfo menuInfo) {
        super.onCreateContextMenu(menu, v, menuInfo);
        menu.add(0, MENU_DELETE_NOTE, 0, R.string.note_delete);
        menu.add(0, MENU_EDIT_NOTE, 1, R.string.note_edit);
    }

    @Override
    public boolean onContextItemSelected(MenuItem item) {
        AdapterContextMenuInfo info = (AdapterContextMenuInfo) item.getMenuInfo();
        long id = info.id;
        switch(item.getItemId()) {
            case MENU_DELETE_NOTE:
                extObjDB.deleteNote(id);
                updateList();
                return true;
            case MENU_EDIT_NOTE:
                startEditNote(id);
                return true;
               default:
                return super.onContextItemSelected(item);
        }
    }
    
    void startEditNote(long id) {
        Intent i = new Intent(this, NoteEdit.class);
        i.putExtra(ExternObjDB.KEY_ROWID, id);
        startActivityForResult(i, ACT_EDIT_NOTE);
    }
    
    public void addSDcardObj(String title, String path, String info){
        extObjDB.createNote(title, path, info);
    }
    
}
