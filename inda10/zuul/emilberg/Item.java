/**
 * Describes an item.
 * Parent class.
 * 
 * To add a new item, let the class inherit from this class, and preferably with the
 * the name <Item>Item, where <Item> is the item you want to add. Give the item a 
 * use method, which will be called when the player uses the item. 
 * 
 * @author Emil Berg
 * @version 1.00
 */
public abstract class Item
{
    // The items name.
    private String name;
    // The text to display when the item is used.
    private String useText;
    // The description of the item
    private String description;

    /**
     * Creates an item.
     * @param name The name of the item.
     * @param description The description of the item.
     * @param useText The text to display when an item is used.
     */
    public Item(String name, String description, String useText) {
        this.name = name;
        this.description = description;
        this.useText = useText;
    }
    
    /**
     * Makes the player use the item. The method is supposed to be overridden.
     * @param player The player that uses the item.
     */
    public abstract String use(Player player);
    
    /**
     * Returns the items name.
     */
    public String getName() {
        return name;
    }
    
    /**
     * @return the description of the item.
     */
    public String getDescription() {
        return description;
    }
    
    /**
     * @return The text to show when used.
     */
    public String getUseText() {
        return useText;
    }
}
