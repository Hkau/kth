/**
 * A 'take' command that lets the player pick up items.
 * 
 * @author Emil Berg
 * @version 1.00
 */
public class TakeCommand extends Command
{
    /**
     * Creates a Command of type Take.
     * @param description The description of the command.
     */
    public TakeCommand(String description) {
        super(description);
    }

    /**
     * Takes something up from the ground and puts it in the players inventory. If there is
     * no second command word, it prints a suitable message. If there is no such item in that
     * room, it also prints a suitable message.
     * @param player The player that picks the item up.
     * @return true if the game should end.
     */
    public boolean apply(Player player) {
        if(!hasSecondWord()) {
            // if there is no second word, we don't know what to take...
            System.out.println("Take what?");
            return false;
        }
        
        if (player.pickUpItem(getSecondWord())) {
            System.out.println("You got the " + getSecondWord() + "!");
        } else {
            System.out.println("There is no " + getSecondWord() + " in this room.");
        }
        
        return false;
    }
}
