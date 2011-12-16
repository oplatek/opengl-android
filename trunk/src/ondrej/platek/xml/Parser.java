package ondrej.platek.xml;

import java.io.FileInputStream;
import java.net.URL;
import java.util.List;

import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import ondrej.platek.bind.ObjSource;

import org.xml.sax.InputSource;
import org.xml.sax.XMLReader;

public class Parser {
	public static List<ObjSource> ParseNotes(String path) {
		try {
			/** Handling XML */
			SAXParserFactory spf = SAXParserFactory.newInstance();
			SAXParser sp = spf.newSAXParser();
			XMLReader xr = sp.getXMLReader();			
	
			/** Create handler to handle XML Tags ( extends DefaultHandler ) */
			MyNotesHandler myXMLHandler = new MyNotesHandler();
			xr.setContentHandler(myXMLHandler);
			xr.parse(new InputSource(new FileInputStream(path)));
			return myXMLHandler.getSources();
		} catch (Exception e) {
			System.out.println("XML Pasing Excpetion = " + e);
			return null;
		}
	}
}
