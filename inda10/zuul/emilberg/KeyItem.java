import java.util.ArrayList;

/**
 * Describes an item of type key.
 * 
 * @author Emil Berg
 * @version 1.00
 */
public class KeyItem extends Item
{
    /**
     * Creates an item of type key. 
     * @param name The name of the key.
     * @param description The description of the key.
     * @param useText The text to display when a door is unlocked with the key.
     **/
    public KeyItem(String name, String description, String useText) {
        super(name, description, useText);
    }

    /**
     * Uses the key. Unlocks a door if possible and returns a suitable message.
     * @param player The user of the item.
     * @return A String with the text which should be displayed when the item is used.
     */
    public String use(Player player) {
        Exit openedExit = null;
                
        ArrayList<Exit> lockedExits = player.getRoom().lockedExits();
        for (Exit lockedExit : lockedExits) {
            openedExit = player.unlockDoor(lockedExit);
                    
            if (openedExit != null) {
                return getUseText();
            }
        }
                
        // If there was no door to unlock,
        if (openedExit == null) {
            return "You can't unlock any door with that key in this room.";
        }
        
        return "";
    }
}
