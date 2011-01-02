/**
 * A 'help' command. Prints some help for the user.
 * 
 * @author Emil Berg
 * @version 1.00
 */
public class HelpCommand extends Command
{
    // The commandWords, so the help command can display all of them.
    private CommandWords commandWords;
    
    /**
     * Creates a Command of type Help.
     * @param description The description of the command.
     * @param commandWords The possible command words in the game.
     */
    public HelpCommand(String description, CommandWords commandWords) {
        super(description);
        this.commandWords = commandWords;
    }
    
    /**
     * Displays some help text for the user.
     * @param player The player.
     * @return true if the game should end.
     */
    public boolean apply(Player player) {
        System.out.println("You are lost in the evil witch's ginger bread house.");
        System.out.println("You can't find Hans.");
        System.out.println("Print 'help' for help.");
        System.out.println("Print 'describe <command>' to describe a command.");
        System.out.println("Possible commands");
        commandWords.showAll();
        return false;
    }
}
