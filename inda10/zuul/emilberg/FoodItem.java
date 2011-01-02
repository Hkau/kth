/**
 * Describes some food.
 * 
 * @author Emil Berg
 * @version 1.00
 */
public class FoodItem extends Item
{
    // The amount of extra steps the player will get when he eats the item.
    private int energy;
    
    /**
     * Creates an item of type food. 
     * @param name The name of the food.
     * @param description The description of the food.
     * @param useText The text to display when the food is eaten by the player.
     **/
    public FoodItem(String name, String description, String useText, int energy) {
        super(name, description, useText);
        this.energy = energy;
    }

    /**
     * Eats the food. 
     * @param player The eater of the food,
     * @return A String with the text which should be displayed when the item is used.
     */
    public String use(Player player) {
        player.eat(getName());
        return getUseText();
    }
    
    /**
     * @return the energy given by the food.
     */
    public int getEnergy() {
        return energy;
    }
}
