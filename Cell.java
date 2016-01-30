import java.awt.*;
import javax.swing.border.LineBorder;
import javax.swing.JPanel;
import java.awt.Graphics;

public class Cell extends JPanel
{
	private static final long serialVersionUID = 1L;
	private String piece;
	public Color cellColor;

	public Cell()
	{
		//setBorder(new LineBorder(Color.black,2));
	}
	
	public Cell(String s)
	{
		//setBorder(new LineBorder(Color.black,2));
		setpiece(s);
	}
	
	public void borderSet()
	{
		setBorder(new LineBorder(Color.black,2));
	}
	public String getpiece()
	{
		return piece;
	}
	
	public void setpiece(String s)
	{
		piece = s;
		repaint();
	}
	
	@Override
	protected void paintComponent(Graphics g)
	{
		super.paintComponent(g);	//"http://i.imgur.com/tBm9rSp.png"
		if (piece == "1")		// X
		{
			Graphics2D g2 = (Graphics2D)g;
			g2.setStroke(new BasicStroke(3));
			g2.drawLine(10, 10, getWidth() - 10, getHeight() - 10);
			g2.drawLine(getWidth() - 10, 10, 10, getHeight() - 10);
		}
		else if (piece == "2")	// O	"http://i.imgur.com/MTPLkOw.png"
		{
			Graphics2D g2 = (Graphics2D)g;
			g2.setStroke(new BasicStroke(3));
			g2.drawOval(10, 10, getWidth() - 20, getHeight() - 20);
		}
		else if (piece == "3")
		{
			Graphics2D g2 = (Graphics2D)g;
		}
	}
}