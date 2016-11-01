package view;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.geom.Path2D;
import java.awt.geom.QuadCurve2D;
import java.util.ArrayList;

public class Branch {

	private ArrayList<Point> points;
	@SuppressWarnings("unused")
	private boolean drawn;
	private int startNodeIndex, endNodeIndex;
	private double phi;
	private int arrowLength;
	private double weight;
	private Point weightPoint;
	public static int width;
	public static int height;
	private static Font font = new Font("Arial", Font.BOLD, 12);

	public Branch() {
		this.points = new ArrayList<Point>();
		this.drawn = false;
		this.arrowLength = 20;
		phi = Math.toRadians(30);
		this.weightPoint = new Point();

	}

	public void setWeight(double value) {
		this.drawn = true;
		this.weight = value;
		this.setWeightPosition();
	}

	public double getWeight() {
		return this.weight;
	}

	public void setWeightPosition() {

		if (points.size() % 2 != 0) // odd points
		{
			weightPoint.x = points.get(((points.size()) / 2)).x;
			weightPoint.y = points.get(((points.size()) / 2)).y;
		} else {
			weightPoint.x = ((points.get(((points.size()) / 2) - 1).x) + (points.get(((points.size()) / 2)).x)) / 2;
			weightPoint.y = ((points.get(((points.size()) / 2) - 1).y) + (points.get(((points.size()) / 2)).y)) / 2;
		}

	}

	public void paintComponenet(Graphics2D g) {
		double startX, startY, endX, endY;

		for (int i = 0; i < points.size(); i++) {
			startX = points.get(i).x;
			startY = points.get(i).y;
			if (i + 1 < points.size()) {
				endX = points.get(i + 1).x;
				endY = points.get(i + 1).y;
				g.setColor(Color.magenta);
				g.setStroke(new BasicStroke(2));
				Path2D.Double path = new Path2D.Double();
				double dx, dy, y;
				if (startX == endX && startY == endY) {
					g.drawOval((int) startX - 40, (int) startY, 40, 40);
					g.setColor(Color.black);
					path = new Path2D.Double();
					path.moveTo(startX - 30, startY - 10);
					path.lineTo(startX - 40, startY + 10);
					path.lineTo(startX - 20, startY + 10);
					g.fill(path);

					g.setFont(font);
					g.setColor(Color.black);
					g.drawString(weight + "", (int) (startX - 30), (int) startY + 20);

				} else if (startX == endX) {
					// QuadCurve2D q = new QuadCurve2D.Float();
					// dx = startY - endY;
					// dy = (startY + endY) / 2;
					// q.setCurve(startX, startY, dx, dy, endX, endY);
					// g.draw(q);
					QuadCurve2D q = new QuadCurve2D.Float();
					if (startY > endY) {
						y = (startY - endY) / 2;
						dx = startX - y;
						dy = (endY + startY) / 2;
						q.setCurve(startX, startY, dx, dy, endX, endY);
						g.draw(q);
						// draw arrow
						g.setColor(Color.black);
						double dX = dx + y / 2 - 10;

						path = new Path2D.Double();
						path.moveTo(dX + 10, dy - 10);
						path.lineTo(dX, dy + 10);
						path.lineTo(dX + 20, dy + 10);
						g.fill(path);

						// write weight
						g.setColor(Color.black);
						g.setFont(font);
						g.drawString(weight + "", (int) dX, (int) dy - 15);
					} else {
						// draw arc
						y = (endY - startY) / 2;
						dx = startX + y;
						dy = (endY + startY) / 2;
						q.setCurve(startX, startY, dx, dy, endX, endY);
						g.draw(q);

						// draw arrow
						g.setColor(Color.green);
						double dX = dx - y / 2 + 10;

						path = new Path2D.Double();
						path.moveTo(dX - 10, dy + 10);
						path.lineTo(dX, dy - 10);
						path.lineTo(dX - 20, dy - 10);
						g.fill(path);

						// write weight
						g.setColor(Color.black);
						g.setFont(font);
						g.drawString(weight + "", (int) dX, (int) dy + 15);
					}
				} else if (startY == endY) {
					QuadCurve2D q = new QuadCurve2D.Float();
					if (startX > endX) {
						y = (startX - endX) / 2;
						dx = (endX + startX) / 2;
						dy = startY + y;
						q.setCurve(startX, startY, dx, dy, endX, endY);
						g.draw(q);

						// draw arrow
						g.setColor(Color.black);
						double dY = dy - 10 - y / 2;

						path = new Path2D.Double();
						path.moveTo(dx - 10, dY + 10);
						path.lineTo(dx + 10, dY);
						path.lineTo(dx + 10, dY + 20);
						g.fill(path);

						// write weight
						g.setColor(Color.black);
						g.setFont(font);
						g.drawString(weight + "", (int) dx - 15, (int) dY);
					} else {
						y = (endX - startX) / 2;
						dx = (endX + startX) / 2;
						dy = startY - y;
						q.setCurve(startX, startY, dx, dy, endX, endY);
						g.draw(q);

						// draw arrow
						g.setColor(Color.black);
						double dY = dy + 10 + y / 2;

						path = new Path2D.Double();
						path.moveTo(dx + 10, dY - 10);
						path.lineTo(dx - 10, dY);
						path.lineTo(dx - 10, dY - 20);
						g.fill(path);

						// write weight
						g.setColor(Color.black);
						g.setFont(font);
						g.drawString(weight + "", (int) dx + 15, (int) dY);

					}
				} else if (startX > endX) {
					g.drawLine((int) startX, (int) startY, (int) endX, (int) endY);
					// draw arrow
					this.paitnArrowHead(g, points.get(points.size() - 1), points.get(points.size() - 2), Color.black);

				} else {
					// QuadCurve2D q = new QuadCurve2D.Float();
					// dx = startY - endY;
					// dy = (startY + endY) / 2;
					// q.setCurve(startX, startY, dx, dy, endX, endY);
					// g.draw(q);
					// // this.paitnArrowHead(g, points.get(points.size() - 1),
					// points.get(points.size() - 2), Color.red);
					g.drawLine((int) startX, (int) startY, (int) endX, (int) endY);
					// draw arrow
					this.paitnArrowHead(g, points.get(points.size() - 1), points.get(points.size() - 2), Color.BLACK);
				}

			}
		}

		// if (points.size() > 1) {
		// this.paitnArrowHead(g, points.get(points.size() - 1),
		// points.get(points.size() - 2), Color.red);
		// }
		// if (drawn) {
		// g.drawString(weight + "", weightPoint.x, weightPoint.y);
		//
		// }
		// g.setColor(Color.BLACK);

	}

	public void paitnArrowHead(Graphics2D g, Point start, Point end, Color color) {
		g.setColor(color);
		double dy = start.y - end.y;
		double dx = start.x - end.x;
		double theta = Math.atan2(dy, dx);
		double tempX = start.x, tempY = start.y;
		// // the same vertical line
		// if (start.x < end.x && start.y == end.y) {
		// tempX = start.x + 10;
		// } else if (start.x > end.x && start.y == end.y) {
		// tempX = start.x - 10;
		//
		// }
		// // the same horizontal line
		// else if (start.y < end.y && start.x == end.x) {
		// tempY = start.y + 10;
		// } else if (start.y > end.y && start.x == end.x) {
		// tempY = start.y - 10;
		//
		// }
		// // line with angle
		// else {

		int rCosTheta = (int) (10 * Math.cos(theta));
		tempX = start.x - rCosTheta;
		tempY = -rCosTheta * dy / dx + start.y;
		// }
		// Angle of the arrow = 30 in degrees should be added to the angle o the
		// line
		// and subtracted
		double XX[] = new double[2];
		double YY[] = new double[2];
		double x, y, arrowTheta = theta + phi;
		for (int j = 0; j < 2; j++) {
			x = tempX - arrowLength * Math.cos(arrowTheta);
			y = tempY - arrowLength * Math.sin(arrowTheta);
			XX[j] = x;
			YY[j] = y;
			// g.draw(new Line2D.Double(tempX, tempY, x, y));
			arrowTheta = theta - phi;
		}
		Path2D.Double path;
		path = new Path2D.Double();
		path.moveTo(tempX, tempY);
		path.lineTo(XX[0], YY[0]);
		path.lineTo(XX[1], YY[1]);
		g.fill(path);
		// write weight
		g.setColor(Color.black);
		g.setFont(font);
		g.drawString(weight + "", (int) tempX+10 , (int) tempY+10);

	}

	public ArrayList<Point> getPoints() {
		return this.points;
	}

	public void addPoint(Point newNode) {
		this.points.add(newNode);
	}

	public int getPointNumber() {
		return this.points.size();
	}

	public void setStartNodeIndex(int index) {
		this.startNodeIndex = index;
	}

	public int getStartNodeIndex() {
		return startNodeIndex;
	}

	public void setEndNodeIndex(int index) {
		this.endNodeIndex = index;

	}

	public int getEndNodeIndex() {
		return endNodeIndex;
	}

	public int getLastPointX() {
		return points.get(points.size() - 1).x;
	}

	public int getLastPointY() {
		return points.get(points.size() - 1).y;
	}
}
