package sim_mob.vis.network;

import java.awt.*;
import java.awt.geom.AffineTransform;
import java.awt.geom.Rectangle2D;
import java.util.ArrayList;

import sim_mob.vis.MainFrame;
import sim_mob.vis.controls.DrawableItem;
import sim_mob.vis.network.basic.Vect;
import sim_mob.vis.util.Utility;

/**
 * Links join Intersections and consist of Segments.
 * 
 *  \author Seth N. Hetu
 *  \author Zhang Shuai
 *  \author Matthew Bremer Bruchon
 */
public class Link implements DrawableItem {
	//Constants/Resources
	private static Font roadNameFont = new Font("Arial", Font.PLAIN, 16);
	//private static Color roadNameColor = new Color(0x33, 0x33, 0x33);
	private static Color roadColor = new Color(0x33, 0x99, 0x22);
	private static Stroke roadStroke = new BasicStroke(1.0F);
	
	private String name;
	private Node start;
	private Node end;
	private ArrayList<Integer> fwdPathSegmentIDs;
	private ArrayList<Integer> revPathSegmentIDs;
	
	public Link(String name, Node start, Node end) {
		this.name = name;
		this.start = start;
		this.end = end;
	}
	
	//See Segment for discussion 
	public Rectangle2D getBounds() {
		final double BUFFER_CM = 10*100; //1m
		Rectangle2D res = new Rectangle2D.Double(getStart().getPos().getUnscaledX(), getStart().getPos().getUnscaledY(), 0, 0);
		res.add(getEnd().getPos().getUnscaledX(), getEnd().getPos().getUnscaledY());
		Utility.resizeRectangle(res, res.getWidth()+BUFFER_CM, res.getHeight()+BUFFER_CM);
		return res;
	}
	
	
	public String getName() { return name; }
	public Node getStart() { return start; }
	public Node getEnd() { return end; }
	
	//Retrieve an "authoritative" road name. If two Links have the same authoritative road
	// name, it means their start and end points are the same (possibly reversed) and the 
	// name of the road itself is the same.
	public String getAuthoritativeRoadName() {
		return getName() + ":" + NodeNameHelper(getStart(), getEnd());
	}
	private static final String NodeNameHelper(Node n1, Node n2) {
		//A little messy, but good enough for now.
		if (n1==null && n2==null) { return "<null>:<null>"; }
		Node smaller = null;
		Node larger = null;
		if (n1==null) {
			smaller = n1; 
			larger = n2;
		} else if (n2==null) {
			smaller = n2; 
			larger = n1;
		} else {
			smaller = n1.hashCode()<n2.hashCode() ? n1 : n2;
			larger = n1.hashCode()>n2.hashCode() ? n1 : n2;
		}		
		return (smaller!=null?smaller.hashCode():"<null>") + ":" + (larger!=null?larger.hashCode():"<null>");
	}
	
	public ArrayList<Integer> getFwdPathSegmentIDs() { return fwdPathSegmentIDs; }
	public ArrayList<Integer> getRevPathSegmentIDs() { return revPathSegmentIDs; }
	public void setFwdPathSegmentIDs(ArrayList<Integer> segIDs) { fwdPathSegmentIDs = segIDs; }
	public void setRevPathSegmentIDs(ArrayList<Integer> segIDs) { revPathSegmentIDs = segIDs; }
	
	public void draw(Graphics2D g) {
		g.setColor(roadColor);
		g.setStroke(roadStroke);
		g.drawLine((int)start.getPos().getX(), (int)start.getPos().getY(), (int)end.getPos().getX(), (int)end.getPos().getY()); 
	}
	
	public void drawName(Graphics2D g, double scale) {
		g.setColor(MainFrame.Config.getLineColor("roadname"));
		g.setFont(roadNameFont);
		float targetX = (float)(start.getPos().getX()+(end.getPos().getX()-start.getPos().getX())/2);
		float targetY = (float)(start.getPos().getY()+(end.getPos().getY()-start.getPos().getY())/2);
		
		//Move the center left
		float halfStrWidth = g.getFontMetrics().stringWidth(name) / 2.0F;
		//targetX -= strWidth / 2.0F;
		
		//Save the old translation matrix
		AffineTransform oldTrans = g.getTransform();
		
		//Create a new translation matrix which is located at the center of the string.
		AffineTransform trans = AffineTransform.getTranslateInstance(targetX, targetY);
		
		//Figure out the rotational matrix of this line, from start to end.
		Vect line = new Vect(start.getPos().getX(), start.getPos().getY(), end.getPos().getX(), end.getPos().getY());
		trans.rotate(line.getMagX(), line.getMagY());
		
		//Next, translate X backwards by half the string width, and move it up slightly.
		if(scale<1.6)
			trans.translate(-halfStrWidth, -3);
		else 
			trans.translate(-halfStrWidth, -3 *scale*4);

		//Apply the transformation, draw the string at the origin.
		g.setTransform(trans);
		g.drawString(name, 0, 0);

		//Restore AffineTransform matrix.
		g.setTransform(oldTrans);
	}
}


