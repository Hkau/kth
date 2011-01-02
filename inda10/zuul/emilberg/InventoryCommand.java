/**
 * An 'inventory' command that prints a  list of all the items in the players inventory.
 * 
 * @author Emil Berg
 * @version 1.00
 */
public class InventoryCommand extends Command
{
    /**
     * Creates a Command of type Inventory.
     * @param description The description of the command.
     */
    public InventoryCommand(String description) {
        super(description);
    }

    /**
     * displays all the items in the players inventory.
     * @param player The player.
     * @return true if the game should end.
     */
    public boolean apply(Player player) {
        if (hasSecondWord()) {
            System.out.println("Write 'inventory' to display the player's inventory.");
            return false;
        }
        System.out.println(player.listInventory());
        return false;
    }
}