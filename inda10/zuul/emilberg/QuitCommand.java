/**
 * A quit command that lets the user quit the game.
 * 
 * @author Emil Berg
 * @version 1.00
 */
public class QuitCommand extends Command
{
    /**
     * Creates a Command of type Quit.
     * @param description The description of the command.
     */
    public QuitCommand(String description) {
        super(description);
    }
    
    /**
     * Quits the game. If there is a second command word, it prints a message telling
     * the user to only write 'quit' if he/she wants to quit.
     * @param player The player.
     * @return true if the game should end.
     */
    public boolean apply(Player player) {
        if (hasSecondWord()) {
            System.out.println("Write 'quit' to quit.");
            return false;
        }
        return true;
    }
}
