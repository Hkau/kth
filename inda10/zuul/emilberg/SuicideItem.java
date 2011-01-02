/**
 * Describes an item that kills the player if used.
 * 
 * @author Emil Berg
 * @version 1.00
 */
public class SuicideItem extends Item
{
    /**
     * Creates an item that can kill the player. 
     * @param name The name of the item.
     * @param description The description of the item.
     * @param useText The text to display when the player is killed by the item.
     **/
    public SuicideItem(String name, String description, String useText) {
        super(name, description, useText);
    }

    /**
     * Returns some suicide text.
     * @param player The user of the item.
     * @return A String with the text which should be displayed when the item is used.
     */
    public String use(Player player) {
        return (getUseText() + "\nYou are now dead. Good night sweet prince.\nAnd flight of angels sing thee to thy rest.");
    }
}
