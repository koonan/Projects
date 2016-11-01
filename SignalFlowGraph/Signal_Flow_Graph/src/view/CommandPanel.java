package view;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.SystemColor;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.List;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.border.BevelBorder;

public class CommandPanel extends JPanel {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	/**
	 * 
	 */
	private static JButton  btnForwardPaths,
			btnTransferFunction, btnAddNode, btnAddBranch;
	public static boolean addNode, addBranch;
	public static Controller controller = new Controller();
	private JLabel lblStartNode;
	private JTextField textStartNode, textEndNode;
	private JLabel lblEndNode;
	private JButton btnEnter;
	private String[] arr = { "individual loop", "nontouching" };
	private JButton btnNnontouchedLoops;
	private JButton btnDelta;
	private JButton btnDeltan;
	private String startNode, endNode;
	private boolean isValidInput;
	private JFrame frame ;

	public CommandPanel() {
		addBranch = false;
		addNode = false;
		setLayout(new BorderLayout(0, 0));

		JPanel panel = new JPanel();
		panel.setBackground(SystemColor.inactiveCaptionBorder);
		panel.setBorder(new BevelBorder(BevelBorder.RAISED,
				SystemColor.infoText, SystemColor.inactiveCaptionText,
				SystemColor.activeCaptionBorder, SystemColor.controlHighlight));
		add(panel, BorderLayout.CENTER);

		lblStartNode = new JLabel("Start Node: ");
		panel.add(lblStartNode);

		textStartNode = new JTextField();
		panel.add(textStartNode);
		textStartNode.setColumns(10);

		lblEndNode = new JLabel("End Node:");
		panel.add(lblEndNode);

		textEndNode = new JTextField();
		panel.add(textEndNode);
		textEndNode.setColumns(10);

		btnEnter = new JButton("Enter");

		panel.add(btnEnter);

		JPanel panel_1 = new JPanel();
		panel_1.setBorder(new BevelBorder(BevelBorder.LOWERED,
				SystemColor.controlText, new Color(160, 160, 160), new Color(
						67, 78, 84), SystemColor.controlHighlight));
		panel_1.setBackground(new Color(128, 128, 128));
		add(panel_1, BorderLayout.SOUTH);
		panel_1.setLayout(new FlowLayout(FlowLayout.CENTER, 5, 5));

		btnAddNode = new JButton("Add Node");
		panel_1.add(btnAddNode);
		btnAddNode.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				addNode = true;
				addBranch = false;
				ScrollablePanel.draw.drawing();
				ScrollablePanel.draw.setFocusable(true);
			}
		});

		btnAddBranch = new JButton("Add Branch");
		panel_1.add(btnAddBranch);

		btnAddBranch.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				if (!addBranch) {
					addBranch = true;
					addNode = false;
					ScrollablePanel.draw.drawing();
					ScrollablePanel.draw.setFocusable(true);
					btnAddBranch.setText("Cancel Branch");
				} else {
					addBranch = false;
					MouseMotion.reset();
					btnAddBranch.setText("Add Branch");
					ScrollablePanel.resetDrawPanel();
				}
			}
		});

		btnTransferFunction = new JButton("Transfer Function");

		panel_1.add(btnTransferFunction);

		btnForwardPaths = new JButton("Forward Paths");

		panel_1.add(btnForwardPaths);

		// btnIndividualLoops = new JButton("Individual Loops");

		// panel_1.add(btnIndividualLoops);

		btnNnontouchedLoops = new JButton(" Loops");

		panel_1.add(btnNnontouchedLoops);

		btnDelta = new JButton("Delta");
		panel_1.add(btnDelta);

		btnDeltan = new JButton("Delta-n");

		panel_1.add(btnDeltan);
		
		btnDeltan.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent arg0) {
				String deltan = controller.mason.findDeltan();
				JOptionPane.showMessageDialog(null, deltan,
						"deltan", JOptionPane.INFORMATION_MESSAGE);
				
			}
			
		});
		
		btnDelta.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent arg0) {
				String deltan = controller.mason.findDelta();
				JOptionPane.showMessageDialog(null, deltan,
						"delta", JOptionPane.INFORMATION_MESSAGE);
				
			}
			
		});
		
		btnTransferFunction.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent arg0) {
				 controller.getForwardPathsNames(startNode, endNode);
				 controller.mason.findLoop();
				 controller.mason.cycle.getCombination();
				 controller.mason.findDelta();
				 controller.mason.findDeltan();
							
				String deltan = controller.mason.findTF();
				JOptionPane.showMessageDialog(null, deltan,
						"transfer function", JOptionPane.INFORMATION_MESSAGE);
				
			}
			
		});


		btnNnontouchedLoops.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent arg0) {
				frame = new JFrame(); 
				frame.setVisible(true);
				frame.setTitle("loops ^_^");
				frame.setBounds(300,300, 750, 750);
				frame.getContentPane().setLayout(new GridLayout(50, 1));
				controller.mason.findLoop();

				@SuppressWarnings("rawtypes")
				List loop = controller.mason.cycle.getCombination();
				for (int i = 0; i < loop.size(); i++) {
					if (i == 0) {
						String numLoop = arr[0] + "\n";
						JLabel l = new JLabel(numLoop);
						frame.getContentPane().add(l);
					} else {
						String numLoop = i + 1 + " " + arr[1]+"\n";
						JLabel l = new JLabel(numLoop);
						frame.getContentPane().add(l);
					}
					@SuppressWarnings("rawtypes")
					List group = (List) loop.get(i);
					for (int j = 0; j < group.size(); j++) {
						
						@SuppressWarnings("rawtypes")
						List cycle = (List) group.get(j);
						String cycleName = " ";
						String end = null;
						for(int k = 0; k < cycle.size(); k++){
							String node = (String) cycle.get(k);
                            if(k == 0){
								end = node;
							}
							if (k < cycle.size() - 1 ) {
								if(node.compareTo(",") == 0 || i != 0){
									cycleName = cycleName + node + "  ";
								}
								else{
									cycleName = cycleName + controller.nodesNames.get(Integer.parseInt(node)) + "  ";
								}
								
							} else {
								if(i == 0){
									cycleName = cycleName  +controller.nodesNames.get(Integer.parseInt(node)) 
											+ "  " + controller.nodesNames.get(Integer.parseInt(end))  + "  :( loop " +j+" )";
								}
								else{
									cycleName = cycleName  + node  ;
								}
								
							}
						}
						JLabel l = new JLabel(cycleName+"\n");
						frame.getContentPane().add(l);
					}
				}

			}

		});
		btnForwardPaths.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (isValidInput) {
					addBranch = false;
					addNode = false;
					String forwardPaths = controller.getForwardPathsNames(
							startNode, endNode);
					JOptionPane.showMessageDialog(null, forwardPaths,
							"Forward Paths", JOptionPane.INFORMATION_MESSAGE);

				} else
					JOptionPane.showMessageDialog(null,
							"Unavilable Nodes.\nStart Node: " + startNode
									+ "\nEnd Node: " + endNode, "Error",
							JOptionPane.ERROR_MESSAGE);
			}
		});

		btnEnter.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent arg0) {
				startNode = textStartNode.getText();
				endNode = textEndNode.getText();
				if (nodeExist(startNode, endNode)) {
					JOptionPane.showMessageDialog(null, "Start Node: "
							+ startNode + "\nEnd Node: " + endNode);
					isValidInput = true;
				} else {
					startNode = "";
					endNode = "";
					textStartNode.setText("");
					textEndNode.setText("");
					JOptionPane.showMessageDialog(null, "Unavilable Nodes.",
							"Error", JOptionPane.ERROR_MESSAGE);
					isValidInput = false;

				}

			}
		});

	}

	public boolean nodeExist(String startNode, String endNode) {
		return (DrawPanel.nodesNames.contains(startNode))
				&& (DrawPanel.nodesNames.contains(endNode));
	}

	public static void resetBranchButton() {
		btnAddBranch.setText("Add Branch");
	}
	
}