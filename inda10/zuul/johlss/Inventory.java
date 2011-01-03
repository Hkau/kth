import java.util.HashMap;
import java.util.Set;

/**
 * Inventory of items.
 * 
 * En helt ny klass som håller koll på alla grejer lite snyggt.
 * 
 * @author johlss
 * @version 2010.11.25
 */
public class Inventory
{
   private HashMap<String, Item> items;

    /**
     * Constructor for objects of class Inventory
     */
    public Inventory()
    {
        items = new HashMap<String, Item>();
    }
    
    /**
     * Returns a string of all the items.
     */
    public String getItemList() {
        if(items.isEmpty()) {
            return "";
        }
        String returnString = "You see these items:\n";
        Set<String> itemNames = items.keySet();
        for(String item : itemNames) {
            returnString += " " + item;
        }
        return returnString;
    }
    
    /**
     * Removes an item.
     */
    public Item removeItem(String itemName) {
        return items.remove(itemName);
    }
    
    /**
     * Returns an item in this room. 
     */
    public Item getItem(String itemName) {
        return items.get(itemName);
    } 
    
    /**
     * Returns a description of an item.
     */
    public String getItemDescription(String itemName) {
        return items.get(itemName).getDescription();
    }
    
    /**
     * Define items.
     */
    public void setItem(String name, Item item) {
        items.put(name, item);
    }
    

}
