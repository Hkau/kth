import java.util.HashMap;
import java.util.Set;
/**
 * A class which can create Npcs for the World of Zuul.
 * Can contain information regardning their name, inventory and dialog with the player 
 *
 * @author Johan Brodin
 * @version 1.0
 */
public class Npc
{
	private String name;
	private String greeting;
	private HashMap<String, Item> inventory;

	/**
	 * Constructor for objects of class Npc
	 */
	public Npc(String name, String greeting)
	{
		this.name = name;
		this.greeting = greeting;
		inventory = new HashMap<String, Item>();

	}
	/**
	 * @return The name of the character
	 */
	public String getName() {
		return name;
	}
	/**
	 * @return The description of the character
	 */
	public String getFullDescription(){
		return name + ": '" + greeting + "'";
	}
	/**
	 * Returns an item from the ncps inventory
	 * @param item The item which should be looked for
	 * @return The item from the npcs inventory
	 */
	public Item getItem(String item) {
		return inventory.get(item);
	}
	/**
	 * Adds an item to the npcs inventory
	 * @param item The item to be added
	 */
	public void addItemToInventory(Item item) {
		inventory.put(item.getName(), item);
	}
	/**
	 * Removes an item from the npc's inventory
	 * @param item The item to be removed
	 */
	public void removeItemFromInventory(String item) {
		inventory.remove(item);
	}
	/**
	 * Checks if an item exists in the npcs inventory or not
	 * @param itemName The item to look for
	 * @return True if it exists, false if not
	 */
	public boolean isInIventory(String itemName)
	{
		return inventory.containsKey(itemName);
	}
	/**
	 * Chang the greeting messeage of the Npc
	 * @param response The new greeting line
	 */
	public void setGreeting(String response)
	{
		this.greeting = response;
	}
	
}
