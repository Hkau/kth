package game;


/**
 * class ScreenPrinter
 * Displays user output on the screen.
 * 
 * @author Anton Erholt 
 * @version 2010.11.25
 */
public class ScreenPrinter
{

    /**
     * Constructor for objects of class ScreenPrinter
     */
    public ScreenPrinter()
    {
        // Empty constructor
    }
    
    /**
     * Prints the message to the console with a line-break(CR) afterwards.
     */
    public void println(String message){
        System.out.println(message);
    }
    
    /**
     * Prints the message to the console.
     */
    public void print(String message){
        System.out.print(message); 
    }
}
