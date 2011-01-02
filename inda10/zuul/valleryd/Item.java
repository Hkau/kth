
/**
 * Class containing information about the items in this game
 * 
 * @author Matilda Valleryd
 * @version 2010.12.04
 */
public class Item
{
    String description;

    /**
     * Constructor for objects of class Item
     * @param String: The description of the item
     */
    public Item(String description)
    {
       this.description = description;
    }
    
    /**
     * Get the items description
     * @return String: The description of the item
     */
    public String getDescription() {
        return description;
    }
}
