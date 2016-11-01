package view;

import java.awt.Point;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

import javax.swing.JOptionPane;

public class MouseMotion implements MouseMotionListener, MouseListener {
	private Point point;
	private int dis;
	public static Branch branch = null;
	public static Point movedPoint;

	@Override
	public void mouseClicked(MouseEvent e) {
		point = new Point();
		dis = DrawPanel.distance;
		point = e.getPoint();

		if (CommandPanel.addNode) {
			if (point.x % dis >= dis / 2 && point.y % dis >= dis / 2) {
				int x = point.x / dis;
				int y = point.y / dis;
				point.x = x * dis + dis / 2;
				point.y = y * dis + dis / 2;

				String nodeName = JOptionPane.showInputDialog("Enter the Node Name : ");
				if (DrawPanel.isUniqueVariableName(nodeName) && !DrawPanel.isNodeExist(point) && nodeName != null) {
					DrawPanel.nodes.add(point);
					DrawPanel.nodesNames.add(nodeName);
					CommandPanel.controller.addNode();
				} else {
					JOptionPane.showMessageDialog(null, "Then Node is not valid");
				}
				CommandPanel.addNode = false;

			}
		} else if (CommandPanel.addBranch) {
			if (point.x % dis >= dis/2 && point.y % dis >= dis/2) {
				int x = point.x / dis;
				int y = point.y / dis;
				point.x = x * dis + dis / 2;
				point.y = y * dis + dis / 2;
				if (branch == null) {
					branch = new Branch();
				}
				branch.addPoint(new Point(x*dis + 3 * dis / 4, y*dis + 3 * dis / 4));

				if (branch.getPointNumber() >1) {
					if (DrawPanel.isNodeExist(point)) {
						String branchValue = JOptionPane.showInputDialog("Please enter branch gain value: ");
						boolean valid = false;
						for (int i = 0; i < branchValue.length(); i++) {
							if (branchValue.charAt(i) >= '0' || branchValue.charAt(i) <= '9') {
								valid = true;
							} else {
								valid = false;
								break;
							}
						}
						if (valid) {
							branch.setEndNodeIndex(DrawPanel.getNodeIndex(point));
							double weight = Double.parseDouble(branchValue);
							branch.setWeight(weight);
							DrawPanel.branches.add(branch);
							CommandPanel.addBranch = false;
							CommandPanel.controller.addBranch(branch.getStartNodeIndex(), branch.getEndNodeIndex(),
									weight);
							movedPoint = null;
							branch = null;
							CommandPanel.resetBranchButton();
							ScrollablePanel.draw.drawing();


						}


					}

					else {
						JOptionPane.showMessageDialog(null, "Error : enable to add branch");
						CommandPanel.resetBranchButton();
						CommandPanel.addBranch = false;
						branch = null;
						movedPoint = null;
						CommandPanel.resetBranchButton();

					}
				} else {
					if (DrawPanel.isNodeExist(point)) {
						branch.setStartNodeIndex(DrawPanel.getNodeIndex(point));
						ScrollablePanel.draw.drawing();

					} else {
						JOptionPane.showMessageDialog(null, "Error : enable to add branch");
						CommandPanel.resetBranchButton();
						CommandPanel.addBranch = false;
						branch = null;
						movedPoint = null;
						CommandPanel.resetBranchButton();

					}
				}
			}


		}
		ScrollablePanel.draw.drawing();
	}

	@Override
	public void mouseMoved(MouseEvent e) {
		if (branch != null && CommandPanel.addBranch && branch.getPointNumber() >= 1) {
			movedPoint = e.getPoint();
			ScrollablePanel.resetDrawPanel();
		}

	}

	public static void reset() {
		branch = null;
	}

	// Non
	@Override
	public void mouseEntered(MouseEvent e) {

	}

	@Override
	public void mouseExited(MouseEvent e) {

	}

	@Override
	public void mousePressed(MouseEvent e) {

	}

	@Override
	public void mouseReleased(MouseEvent e) {

	}

	@Override
	public void mouseDragged(MouseEvent e) {

	}

}
