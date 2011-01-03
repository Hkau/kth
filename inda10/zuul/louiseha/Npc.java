import java.util.Random;
/**
 * The Npc class makes npcs and generates responses.
 * 
 * @author Louise Hansson
 * @version 2010.12.02
 */
public class Npc
{
    private String name;
    private String description;
    private String[] response;
    private Random responseGenerator;
    private boolean available;
    
    /**
     * Construct a npc.
     * @ param    name  the name of the item
     */
    public Npc(String name, String description)
    {
        this.description = description;
        this.name = name;
        responseGenerator = new Random();
        response = new String[7];
        setResponse();
    }
    
    /**
     * Add the unicorns' repsonses to an array.
     */
    private void setResponse()
    {
        response[0] = "Chaaarlie! Chaaaaa...";
        response[1] = "Do you remember when we went to Candy Mountain, Charlie?";
        response[2] = "Chaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaarlie";
        response[3] = "Glub, glub. Scuba diving is fun!";
        response[4] = "Watch out for the fugle fish!";
        response[5] = "Sparkle, sparkle!";
        response[6] = "The banana king is counting on us!";   
    }
    
    /**
     * Print the line that the bookstore owner will say every time the player talks with them.
     */
    public void printOwnerResponse()
    {
        System.out.println("Hello! I sell books, wonderful books! Would you like to buy a book? Maybe a Dictionary? If you have something of worth we can trade! Would you like to do that? (Yes/No)");
    }
    
    /**
     * Return the name of the npc.
     * @return  name    the name of the npc
     */
    public String getName()
    {
        return name;
    }
    
    public String getDescription()
    {
        return description;
    }
    
    /**
     * Print a random response for the unicorns.
     */
    public void printRandomResponse()
    {
        int index = responseGenerator.nextInt(7);
        System.out.println(response[index]);
    }
    
    /**
     * Print the Z's Spanish reply.
     */
    public void printSpanishInstructions()
    {
        System.out.println("Hola! Usted necesita encontrar el diccionario a fin de avanzar más allá de mí. Venga a hablar conmigo otra vez cuando usted lo encuentra! Vamos a fiesta de tortugas en la noche!");
    }
    
    /**
     * Print the Z's English reply.
     */
    public void printEnglishInstructions()
    {
        System.out.println("Hello! Congratulations, you found a dictionary. Now that you can understand me you are free to pass through to the next room. Good luck!");
    }
}
