import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JTextField;
import javax.swing.SwingConstants;


public class GUI extends JFrame implements ActionListener {
	// MEMBER VARIBLES
	
	// Frame settings
	private static final long serialVersionUID = 1L;
	private final int WIDTH = 1024;
	private final int HEIGHT = 768;
	private final int MAX_BUF = 32;
	
	// Input Elements
	private JButton connectButton;
	private JButton undoButton;
	private JButton moveButton;
	private JButton easyButton, mediumButton, hardButton;
	private JPasswordField passwordField;
	private JTextField hostNameField;
	private JTextField portNumField;
	private JTextField fromPosField;
	private JTextField toPosField;
	
	// Panels
	JPanel connectionPanel;
	JPanel passwordPanel;
	JPanel difficultyPanel;
	JPanel boardPanel;
	
	// Objects
	Client client;
	private Cell[][] cells = new Cell[10][10];
	private Color col1 = new Color(124, 55, 32);
	private Color col2 = new Color(255, 196, 77);
	
	// Password reading control
	boolean firstPwdLoop = true;
	
	// MEMBER METHODS
	public GUI() {
		client = new Client();
		
		Toolkit userConfig = Toolkit.getDefaultToolkit();
		Dimension userScreenResolution = userConfig.getScreenSize();
		
		setBounds(userScreenResolution.width / 2 - WIDTH / 2,
				  userScreenResolution.height / 2 - HEIGHT / 2, 
				  WIDTH, HEIGHT);
		
		setTitle("Breakthrough");
		setDefaultCloseOperation(EXIT_ON_CLOSE);
		setResizable(false);
		setVisible(true);
		
		loadConnectionSetup();
		validate();
	}
	
	private void loadConnectionSetup() {
		JLabel hostNameLabel = new JLabel("Host Name");
		JLabel portNumLabel = new JLabel("Port Number");
		
		hostNameField = new JTextField(MAX_BUF);
		portNumField  = new JTextField(MAX_BUF);
		
		connectButton = new JButton("CONNECT");
		connectButton.addActionListener(this);
		
		connectionPanel = new JPanel();
		connectionPanel.add(hostNameLabel);
		connectionPanel.add(hostNameField);
		connectionPanel.add(portNumLabel);
		connectionPanel.add(portNumField);
		connectionPanel.add(connectButton);
		
		connectionPanel.setVisible(true);
		add(connectionPanel, BorderLayout.CENTER);
	}
	
	private void loadPasswordVerification() {
		getContentPane().remove(connectionPanel);
		
		JLabel passwordLabel = new JLabel("Password");
		
		passwordField = new JPasswordField(MAX_BUF);
		passwordField.addActionListener(this);
		
		passwordPanel = new JPanel();
		passwordPanel.add(passwordLabel);
		passwordPanel.add(passwordField);
		
		getContentPane().add(passwordPanel, BorderLayout.CENTER);
	}
	
	private void loadDifficultySetup() {
		getContentPane().remove(passwordPanel);
		
		easyButton = new JButton("EASY");
		easyButton.addActionListener(this);
		mediumButton = new JButton("MEDIUM");
		mediumButton.addActionListener(this);
		hardButton =  new JButton("HARD");
		hardButton.addActionListener(this);
		
		difficultyPanel	= new JPanel();
		difficultyPanel.add(easyButton);
		difficultyPanel.add(mediumButton);
		difficultyPanel.add(hardButton);
		
		getContentPane().add(difficultyPanel, BorderLayout.CENTER);
	}
	
	private void loadBoard(String boardContent) {
		boardPanel = new JPanel(new GridLayout(10,10,0,0));
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
			{
				boardPanel.add(cells[i][j] = new Cell());
			}

		add(boardPanel, BorderLayout.CENTER);
		
		for (int i = 0; i < 8; i++)			//color squares
			for (int j = 0; j < 8; j++)
			{
				cells[i][j].borderSet();
				if (i%2 == 0 && j%2 == 0)
				{
					cells[i][j].setBackground(col1);
				}
				else if (i%2 == 0 && j%2 != 0)
				{
					cells[i][j].setBackground(col2);
				}
				else if (i%2 != 0 && j%2 == 0)
				{
					cells[i][j].setBackground(col2);
				}
				else if (i%2 != 0 && j%2 != 0)
				{
					cells[i][j].setBackground(col1);
				}
			}
		
			for (int i = 0; i < 8; i++)
			{
				int i2 = i*8;
				for (int j = 0; j < 8; j++)
				{
					String str = Character.toString(boardContent.charAt(i2+j));
					if (str.equals("1"))
						cells[i][j].setpiece("1");
					else if (str.equals("2"))
						cells[i][j].setpiece("2");
					else if (str.equals("3"))
						cells[i][j].setpiece("3");
				}
			}	
			
			for (int i = 0; i < 8; i++)
			{
				JLabel boardNum = new JLabel(Integer.toString(i));	// Board column numbers
				boardNum.setHorizontalTextPosition(JLabel.CENTER);
				boardNum.setFont(new Font("Serif", Font.BOLD, 20));
				cells[i][8].add(boardNum);
			}
			
			String[] BoardRow = {"A", "B", "C", "D", "E", "F", "G", "H"};
			for (int i = 0; i < 8; i++)
			{
				JLabel boardNum = new JLabel(BoardRow[i],JLabel.CENTER);	// Board row letters
				boardNum.setHorizontalTextPosition(SwingConstants.LEFT);
				boardNum.setFont(new Font("Serif", Font.BOLD, 20));
				cells[8][i].add(boardNum);
			}
			
			
//			undoButton = new JButton("Undo");					//Undo button
//			cells[2][9].add(undoButton);
//			undoButton.addActionListener(this);
			
			fromPosField = new JTextField(2);					// take substring of first two characters
			JLabel fromText = new JLabel("Move From:");
			cells[3][9].add(fromText);
			cells[3][9].add(fromPosField);
			fromPosField.addActionListener(this);
			
			toPosField = new JTextField(2);						// take substring of first two characters
			JLabel toText = new JLabel("Move To:");
			cells[4][9].add(toText);
			cells[4][9].add(toPosField);
			toPosField.addActionListener(this);
			
			moveButton = new JButton("Move");
			cells[5][9].add(moveButton);
			moveButton.addActionListener(this);
	}	
	
	private void redrawBoard(String boardContent) {
		for (int i = 0; i < 8; i++)
		{
			int i2 = i*8;
			for (int j = 0; j < 8; j++)
			{
				String str = Character.toString(boardContent.charAt(i2+j));
				if (str.equals("1"))
					cells[i][j].setpiece("1");
				else if (str.equals("2"))
					cells[i][j].setpiece("2");
				else if (str.equals("3"))
					cells[i][j].setpiece("3");
			}
		}	
	}
	
	private void loadGameScreen(String boardContent) {
		getContentPane().remove(difficultyPanel);
		loadBoard(boardContent);
		getContentPane().add(boardPanel, BorderLayout.CENTER);
	}
	
	// EVENT-HANDLING
	@Override
	public void actionPerformed(ActionEvent event) {
		// Connection
		if (event.getSource() == connectButton) {
			String hostName = hostNameField.getText();
			String portNum = portNumField.getText();
			
			if(client.connect(hostName, portNum)) {
				loadPasswordVerification();
				validate();
			} else {
				String message = "Unable to connect. Try again.";
				JOptionPane.showMessageDialog(null, message);
			}
		}
		
		// Password
		if (event.getSource() == passwordField) {
			String password = event.getActionCommand();
			
			System.out.println(password);
			
			String reply;
			if(firstPwdLoop) {
				reply = client.read(); // Reads 'PASSWORD'
				firstPwdLoop = false;
			}
			
			reply = client.sendRequest(password); // Sends password over
			
			if (reply.contains("WELCOME")) {
				loadDifficultySetup();
				validate();
			} else {
				String message = "Incorrect password. Try again.";
				JOptionPane.showMessageDialog(null, message);
			}
		}
		
		// Difficulty
		if (event.getSource() == easyButton) {
			String reply;
			reply = client.sendRequest("HUMAN-AI EASY");
			reply = client.sendRequest("DISPLAY");
			reply = client.read();
			
			loadGameScreen(reply);
			validate();
		}
		
		if (event.getSource() == mediumButton) {
			String reply;
			reply = client.sendRequest("HUMAN-AI MEDIUM");
			reply = client.sendRequest("DISPLAY");
			reply = client.read();
			
			loadGameScreen(reply);
			validate();
		}
		
		if (event.getSource() == hardButton) {
			String reply;
			reply = client.sendRequest("HUMAN-AI HARD");
			reply = client.sendRequest("DISPLAY");
			reply = client.read();
			
			loadGameScreen(reply);
			validate();
		}
		
		// Undo
		if (event.getSource() == undoButton) {
			String reply;
			reply = client.sendRequest("UNDO");
			
			if(reply.contains("UNDO OK")) {
				// Read the board again
				reply = client.read();
				// Redraw the board
				redrawBoard(reply);
			} else {
				String message = "Unable to undo.";
				JOptionPane.showMessageDialog(null, message);
			}
		}
		
		// Move
		if (event.getSource() == moveButton) {
			// Get client input movement
			String sourcePos = fromPosField.getText();
			String destPos = toPosField.getText();
			
			// Server will return feedback for correctness of input
			String reply = client.sendRequest(sourcePos + " to " + destPos);
			// Then it returns the board after the movement
			String boardContent = client.read();
			
			// Redraw the board
			redrawBoard(boardContent);
			
			// If movement was correct, read AI's move
			if(reply.contains("MOVE OK")) {
				// Now server returns the movement chosen by AI
				reply = client.read();
				// Lastly, the board with AI's move updated
				reply = client.read();
				
				// Redraw the board
				redrawBoard(reply);
			}
		}
	}
	
	// MAIN
	public static void main(String[] args) {
		GUI gui = new GUI();
	}
}
