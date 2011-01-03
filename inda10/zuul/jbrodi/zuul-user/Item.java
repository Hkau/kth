
/**
 * Hold information about Items in the game World of Zuul.
 * 
 *
 * @author Johan Brodin
 * @version 1.0
 */
public class Item
{
    private double weight;
    private String name;
    private String description;
    private boolean lootable=true;
    private boolean eatable;
    private String specialEffect=null; 
    private Room restrictedRoom=null;

    /**
     * Constructor for objects of class Item
     */
    public Item(String name, String description, double weight)
    {
        this.name = name;
        this.description = description;
        this.weight = weight;    
    }
    /**
     * Givs the item an speciall effect
     * @param effect The effect given to the item
     */
    public void setSpecialEffect(String effect) {
        specialEffect = effect;
    }
    /**
     * Set the item to eatable or not
     * @param condition True if the player should be able to eat the item, false if not
     */
    public void setEatable(boolean condition) {
        eatable = condition;
    }
    /**
     * Set if the player should be able to pick up the item or not
     * @param condition True if able to pick it up, false if not
     */
    public void setLootable(boolean condition) {
        lootable = condition;
    }
    /**
     * Set if an item only should be useable in a certaint room
     * @param restricedRoom The room which the item should be used in
     */
    public void setRestrictedRoom(Room restricedRoom) {
        this.restrictedRoom=restricedRoom;
    }
    /**
     * Returns the restricted room of the item
     * @return A room or null if no restricted room is set
     */
    public Room getRestrictedRoom() {
        return restrictedRoom;
    }
    /**
     * Returns the special effect of this item
     * @return A string with the effect, null if no effect
     */
    public String getSpecialEffect() {
        return specialEffect;
    }
    /**
     * @return the weight of the item
     */
    public double getWeight() {
        return weight;
    }
    /**
     * Returns if the player is able to eat this item or not
     * @return True if eatable, false if not
     */
    public boolean isEatable() {
        return eatable;
    }
    /**
     * Returns the name of the item
     * @return A string with the name
     */
    public String getName() {
        return name;
    }
    /**
     * Returns if the player is able to loot this item or not
     * @return True if lootable, false if not
     */
    public boolean isLootable() {
        return lootable;
    }
    /**
     * Returns an description of the item.
     * @return A string with the description
     */
    public String getDescriptionString()
    {
        return name + " - " + description + ". Weight: " + weight;
    }

  
}
