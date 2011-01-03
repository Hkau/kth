import java.util.HashMap;

/**
 * Här är en klass för att hålla koll på spelaren. 
 * En carrierLimit är tillagd men används inte i nuläget.
 * 
 * @author johlss 
 * @version 2010.11.24
 */
public class Player
{
    private int carrierLimit;
    private Inventory inventory;

    /**
     * Constructor for objects of class Player
     */
    public Player()
    {
        inventory = new Inventory();
        carrierLimit = 5000;
    }
    
    /**
     * Returns the inventory.
     */
    public Inventory getInventory() {
        return inventory;
    }
}
