package view;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.EventQueue;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;

public class MainInterface extends JFrame {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private JPanel contentPane;
	public static void main(String args[]) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					MainInterface frame = new MainInterface();
					frame.setVisible(true);

				} catch (Exception e) {
					e.printStackTrace();

				}
			}

		});
	}

	public MainInterface() {
		setTitle("Signal Flow Graph");
		setDefaultCloseOperation(EXIT_ON_CLOSE);
		setBounds(100, 100, 1200, 600);
		Dimension minSize = new Dimension(500,350);
		setMinimumSize(minSize);
		 contentPane = new JPanel();
		 contentPane.setBorder(new EmptyBorder(5,5,5,5));
		 setContentPane(contentPane);
		 contentPane.setLayout(new BorderLayout(0,0));
		 
		CommandPanel commandPanel = new CommandPanel();
		contentPane.add(commandPanel,BorderLayout.NORTH);
		
		ScrollablePanel drawPanel = new ScrollablePanel();
		contentPane.add(drawPanel, BorderLayout.CENTER);
		

	}

}
