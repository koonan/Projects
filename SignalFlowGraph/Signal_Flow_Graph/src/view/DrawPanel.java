package view;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.RenderingHints;
import java.util.ArrayList;

import javax.swing.JPanel;


public class DrawPanel extends JPanel {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	public static int distance;
	public static ArrayList<Branch> branches;
	public static ArrayList<Point> nodes;
	public static ArrayList<String> nodesNames = new ArrayList<String>();
	private static Font font = new Font("Arial", Font.BOLD, 12);


	public DrawPanel() {
		setBackground(Color.WHITE);
		setPreferredSize(new Dimension(2000, 2000));
		addMouseListener(new MouseMotion());
		addMouseMotionListener(new MouseMotion());
		Branch.width=2000;
		Branch.height=2000;
		distance = 40;
		nodes = new ArrayList<Point>();
		branches = new ArrayList<Branch>();
		setFocusable(true);
	}

	public static void resetDrawPanel() {
		nodesNames = new ArrayList<String>();
		nodes = new ArrayList<Point>();
		branches = new ArrayList<Branch>();

	}

	public void drawing() {
		repaint();
	}

	public static boolean isUniqueVariableName(String name) {
		for (int i = 0; i < nodesNames.size(); i++) {
			if (nodesNames.get(i).equals(name))
				return false;
		}
		return true;
	}

	public static boolean isNodeExist(Point pt) {
		for (int i = 0; i < nodes.size(); i++) {
			if (nodes.get(i).x == pt.x && nodes.get(i).y == pt.y)
				return true;
		}
		return false;
	}

	public static int getNodeIndex(Point pt) {
		for (int i = 0; i < nodes.size(); i++) {
			if (nodes.get(i).x == pt.x && nodes.get(i).y == pt.y)
				return i;
		}
		return 0;
	}

	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		Graphics2D newG = (Graphics2D) g;
		newG.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

		if (CommandPanel.addBranch || CommandPanel.addNode)
			guideCircles(newG);

		 if (CommandPanel.addBranch && MouseMotion.branch != null) {
		 MouseMotion.branch.paintComponenet(newG);
		 if (MouseMotion.movedPoint != null)
		 newG.drawLine(MouseMotion.branch.getLastPointX(),
		 MouseMotion.branch.getLastPointY(),
		 MouseMotion.movedPoint.x, MouseMotion.movedPoint.y);
		 }

		for (int i = 0; i < branches.size(); i++)
			branches.get(i).paintComponenet(newG);

		for (int i = 0; i < nodes.size(); i++) {
			newG.setColor(Color.BLACK);
			newG.fillOval(nodes.get(i).x, nodes.get(i).y, distance / 2, distance / 2);
			newG.setColor(Color.WHITE);
			newG.setFont(font);
			newG.drawString(nodesNames.get(i), nodes.get(i).x + distance / 6, nodes.get(i).y + distance / 3);
		}

	}

	public void guideCircles(Graphics2D newG) {
		newG.setColor(Color.gray);
		for (int i = 20; i < 1980; i += distance) {
			for (int j = 20; j < 1980; j += distance) {
				newG.drawOval(i, j, distance / 2, distance / 2);
			}
		}
	}

}