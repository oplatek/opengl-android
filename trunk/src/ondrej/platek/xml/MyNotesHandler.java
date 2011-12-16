package ondrej.platek.xml;

import java.util.ArrayList;
import java.util.List;

import ondrej.platek.bind.ObjFromSDcard;
import ondrej.platek.bind.ObjSource;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import android.util.Log;

public class MyNotesHandler extends DefaultHandler{
	static String Tag = "PARSER";
	static String NODE_MAIN_OBJ = "models";
	static String NODE_NOTE = "note";
	static String NODE_INFO = "info";
	static String NODE_TITLE = "title";
	static String NODE_PATH = "path";
	Boolean currentElement = false;
	String currentValue = null;
	public List<ObjSource> sources = null;

	public List<ObjSource> getSources() {
	return sources;
	}

	// We will add new items
	public MyNotesHandler(List<ObjSource> Sources) {
		sources = Sources;
	}
	
	// We will replaced the items by loading new one
	public MyNotesHandler() {
		sources = new ArrayList<ObjSource>();
	}	

	/** Called when tag starts ( ex:- <name>AndroidPeople</name>
	* -- <name> )*/
	@Override
	public void startElement(String uri, String localName, String qName, Attributes attributes) 
			throws SAXException {
	    Log.i(Tag, "startElement: " + localName);
		currentElement = true;
		if (localName.equals(NODE_MAIN_OBJ))
		{
			Log.i(Tag, "Starting in main tag");
		} else if (localName.equals(NODE_NOTE)) {
		/** Get attributes value */
			String path = attributes.getValue(NODE_PATH);
			String title = attributes.getValue(NODE_TITLE);
			String info = attributes.getValue(NODE_PATH);
			Log.i(Tag, "Getting attributes: " + path + ", " + title + ", " + info);
			ObjSource s = new ObjFromSDcard(path);
		    s.Title = title;
		    s.Info = info;
		    sources.add(s);
		}
	}

	/** Called when tag closing ( ex:- <name>AndroidPeople</name>
	* -- </name> )*/
	@Override
	public void endElement(String uri, String localName, String qName)
			throws SAXException {
		Log.i(Tag, "endElement: " + localName);
		currentElement = false;		
		/** set value */
		/*
		if (localName.equalsIgnoreCase("name"))
			sitesList.setName(currentValue);
		else if (localName.equalsIgnoreCase("website"))
			sitesList.setWebsite(currentValue);
		*/
	}

	/** TODO Called to get tag characters ( ex:- <name>AndroidPeople</name>
	* -- to get AndroidPeople Character ) */
	@Override
	public void characters(char[] ch, int start, int length)
			throws SAXException {
		if (currentElement) {
			currentValue = new String(ch, start, length);
			currentElement = false;
		}
	}
}
