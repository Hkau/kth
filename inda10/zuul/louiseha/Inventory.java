import java.util.ArrayList;
/**
 * The class Inventory keeps track of the inventory of the player.
 * 
 * @author Louise Hansson 
 * @version 2010.12.02
 */
public class Inventory
{
    ArrayList<Items> backpack;
    private int currentBackpackWeight;
    
    /**
    * Construct a Player objekt.
    **/
    public Inventory()
    {
        backpack = new ArrayList<Items>();
        currentBackpackWeight = 0;
    }
    
    /**
     * Add an item to the backpack.
     * @param    an item
     */
    public void addToBackpack(Items item)
    {
        backpack.add(item);
        currentBackpackWeight = item.getWeight() + currentBackpackWeight;
    }
    
    /**
     * Check if the item is too heavy to be put in the backpack.
     * @param   an item
     * @return  true or false
     */
    public boolean checkIfTooHeavy(Items item)
    {
        boolean trueOrFalse = true;
        if(item.getWeight() + currentBackpackWeight <= 10)
        {
            trueOrFalse = false;
        }
        return trueOrFalse;
    }
    
    /**
     * Drop an item from the backpack.
     * @param    an item
     */
    public void dropItem(Items item)
    {
        currentBackpackWeight -= item.getWeight();
        backpack.remove(item);
    }
}
