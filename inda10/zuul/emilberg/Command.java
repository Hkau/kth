import java.util.ArrayList;

/**
 * This class is part of the "The Adventures of Hans and Greta" application. 
 * "The Adventures of Hans and Greta" is a very simple, text based adventure game.  
 *
 * This class holds information about a command that was issued by the user.
 * This is a parent class for the commands.
 *
 * If the command had only one word, then the second word is <null>.
 * 
 * To add a new command, let the class inherit from this class, and preferably with the
 * the name <Name>Command, where <Name> is the name of the command you want to add. 
 * Give the item an apply method, which will be called when the user issues the command.
 * 
 * 
 * @author  Michael Kolling, David J. Barnes and Emil Berg
 * @version 1.00
 */

public abstract class Command
{
    // The String after the command. For instance: Go west --> 
    // secondWord = "west".
    private String secondWord;
    // Describes the command.
    private String description;

    /**
     * Create a command object. 
     * @param description The description of the command. 
     **/
    public Command(String description)
    {
        this.secondWord = null;
        this.description = description;
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
     * Sets the word written after the commandWord.
     * @param secondWord The second word.
     */
    public void setSecondWord(String secondWord) {
        this.secondWord = secondWord;
    }

    /**
     * @return true if the command has a second word.
     */
    public boolean hasSecondWord()
    {
        return (secondWord != null);
    }
    
    /**
     * @return This commands description.
     */
    public String getDescription() {
        return description;
    }
    
    /**
     * This methid is overridden in the child-classes.
     */
    public abstract boolean apply(Player player);
}

