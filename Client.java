import java.net.*;
import java.io.*;

public class Client {
	private final int MAX_BUF = 256;
	private Socket clientSoc;
	private PrintWriter out;
	private BufferedReader in;
	private BufferedReader stdIn;
    
	private boolean displayIsOn = false;
    
    public Client() {}
    
    public boolean connect(String serverName, String portNum) {
    	try {
	        
        	//Create socket object
	        clientSoc = new Socket(serverName, Integer.parseInt(portNum));
	        
	        // Channel to send to server
	        out = new PrintWriter(clientSoc.getOutputStream(),true);
	        
	        // Channel to receive from server
	        in = new BufferedReader(
	        	new InputStreamReader(clientSoc.getInputStream())
	        );
	        
	        stdIn = new BufferedReader(new InputStreamReader(System.in));
	        
	        return true;
        
        } catch(IOException e) {
            return false;
        }
    }
    
    public void write(String userInput) {
    	out.println(userInput);
    }
    
    public String read() {
    	try {
    		String reply = in.readLine();
    		System.out.println("<" + reply + ">");
    		return reply;
	    } catch(IOException e) {
	        e.printStackTrace();
	        return "null";
	    }
    }
    
    public String sendRequest(String userInput) {
        try {
	        // Send user input to server
	        out.println(userInput);
	     
	        // Get reply from server
	        String reply = in.readLine();
	        
	        System.out.println("<" + reply + ">");
	        return reply;
        
        } catch(IOException e) {
            e.printStackTrace();
            return "";
        }
    }
    
    public void markPosition() {
    	try {
			stdIn.mark(MAX_BUF);
		} catch (IOException e) {
			e.printStackTrace();
		}
    }
    
    public void rewind() {
    	try {
			stdIn.reset();
		} catch (IOException e) {
			e.printStackTrace();
		}
    }
}
