/**
 * A 'use' command that lets the user use an item.
 * 
 * @author Emil Berg
 * @version 1.00
 */
public class UseCommand extends Command
{
    /**
     * Creates a Command of type Use.
     * @param description The description of the command.
     */
    public UseCommand(String description) {
        super(description);
    }

    /**
     * Makes the player use an item. If there is no second command word, it prints
     * a suitable message. If there is no such item, it prints a sutiable message.
     * @param player The player that uses the item.
     * @return true if the game should end.
     */
    public boolean apply(Player player) {
        if(!hasSecondWord()) {
            // if there is no second word, we don't know what to use...
            System.out.println("Use what?");
            return false;
        }
        
        Item item = player.getItem(getSecondWord());
        
        if (item == null) {
            System.out.println("There is no such item.");
        } else {
            System.out.println(item.use(player));
            
            // Returns true, that is ends the game if the player used a suicide item.
            if (item instanceof SuicideItem) {
                return true;
            }
        }
        
        return false;
    }
}
