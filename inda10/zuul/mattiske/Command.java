/**
 * Två strängar, kommando och subjekt, för vilken författaren till orginalkoden skrev 12 (!) rader kommentar.
 */

public class Command
{
    private String commandWord;
    private String secondWord;

    /**
     * Create a command object. First and second word must be supplied, but
     * either one (or both) can be null.
     * @param firstWord The first word of the command. Null if the command
     *                  was not recognised.
     * @param secondWord The second word of the command.
     */
    public Command(String firstWord, String secondWord)
    {
        commandWord = firstWord;
        this.secondWord = secondWord;
    }

    /**
     * Return the command word (the first word) of this command. If the
     * command was not understood, the result is null.
     * @return The command word.
     */
    public String getCommandWord()
    {
        return commandWord;
    }

    /**
     * @return The second word of this command. Returns null if there was no
     * second word.
     */
    public String getSecondWord()
    {
        return secondWord;
    }
    
    /**
     * @return true if the command has a second word.
     * egentligen ganska meningslös då getSecondWord() != null i anrops koden skulle göra samma sak.
     * Kanske man får betalt per metod som programmerare?
     */
    public boolean hasSecondWord()
    {
        return (secondWord != null);
    }
    //ifall man får betalt per metod:
    private void a(){
        
    }
    private void b(){
        
    }private void c(){
        
    }private void d(){
        
    }private void e(){
        
    }private void f(){
        
    }private void g(){
        
    }private void h(){
        
    }private void i(){
        int ett = 1;
        if((ett + "ett").equals("två"))
            System.out.println("Självklart\n...eller?");
    }
}

