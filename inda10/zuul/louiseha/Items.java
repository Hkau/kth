import java.util.ArrayList;
import java.util.HashMap;
/**
 * A class to keep track of the items in the game.
 * 
 * @author Louise Hansson 
 * @version 2010.11.29
 */
public class Items
{
    //The weight of the item.
    private int itemWeight;
    //The name of the item.
    private String name;

    /**
     * Construct an item.
     * @param   the weight of the item, the name of the item
     */
    public Items(int itemWeight, String name)
    {
        this.itemWeight = itemWeight;
        this.name = name;
    }

    /**
     * @return  itemWeight The weight of the item
     */
    public int getWeight()
    {
        return itemWeight;
    }
    
    /**
     * @return  name The name of the item
     */
    public String getName()
    {
        return name;
    }
}
