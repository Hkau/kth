import java.util.HashMap;

/**
 * Class Item - an item in an adventure game.
 * 
 * @author johlss
 * @version 2010.11.24
 */
public class Item
{
    private String description;
    private int weigth;

    /**
     * Creates an item with a description and weight. 
     */
    public Item(String description, int weigth) {
        this.description = description;
        this.weigth = weigth;
    }
    
    /**
     * Returns a description of an item.
     */
    public String getDescription() {
        return description;
    }
}
