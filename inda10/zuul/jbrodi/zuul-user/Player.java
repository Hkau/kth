import java.util.Stack;
import java.util.HashMap;
import java.util.Set;

/**
 * Holds the information about the player.
 * 
 * @author Johan Brodin
 * @version 1.0
 */
public class Player
{
	
	private Room currentRoom;
	private Room safeRoom=null;;
	private Stack<Room> previousRooms;
	private HashMap<String, Item> inventory;
	private double strength;
	final static int LIFTCAPACITY = 10;
	private int roundsLeft;




	/**
	 * Constructor for objects of class Player
	 */
	public Player(Room currentRoom)
	{
		this.currentRoom = currentRoom;
		previousRooms = new Stack<Room>();
		inventory = new HashMap<String, Item>();
		strength = 10;
		roundsLeft = 30;
	}
	/**
	 * @return The last room the player visited.
	 */
	public Room getLastRoom() {
		return previousRooms.peek();
	}
	/**
	 * @return The number of rounds the player got left.
	 */
	public int getRoundsLeft() {
		return roundsLeft;
	}
	/** 
	 * Set the Room for the beamer
	 * @param safeRoom The room which the beamer should return to
	 */
	public void setSafeRoom(Room safeRoom) {
		this.safeRoom = safeRoom;
	}
	/**
	 * 
	 * @return The room which the beamer should return to
	 */
	public Room getSafeRoom() {
		return safeRoom;
	}
	/**
	 * Adds an item to the players inventory
	 * @param item The item to be added
	 */
	public void addItemToInventory(Item item) {
		inventory.put(item.getName(), item);
	}
	/**
	 * Removes and item from the players inventory
	 * @param item The item to be removed
	 */
	public void removeItemFromInventory(String item) {
		inventory.remove(item);
	}
	/**
	 * Returns an item from the players inventory
	 * @param item Item to be returned
	 * @return The item from the players inventory
	 */
	public Item getItem(String item) {
		return inventory.get(item);
	}
	/**
	 * Returns an string containing information regarding the inventory
	 * @return An string representing the inventory
	 */
	public String getInventoryString() {
		String inventoryString = "Inventory:";
		if(!inventory.isEmpty()){

			Set<String> itemNames = inventory.keySet(); 
			for(String itemName : itemNames){
				inventoryString += "\n  " + inventory.get(itemName).getDescriptionString();
			}

			inventoryString += "\nCarrying " + getTotalWeightInventory() + " of the maximum " + getMaxInventoryWeight();

			return inventoryString;
		}      
		else
			return inventoryString += "\n Is Empty";  

	}
	/**
	 * Calculates the total weight of the players inventory
	 * @return The weight of the inventory
	 */
	public double getTotalWeightInventory() {
		double totalWeight = 0;
		if(!inventory.isEmpty()){
			Set<String> itemNames = inventory.keySet();
			for(String itemName : itemNames){
				totalWeight += inventory.get(itemName).getWeight();
			}
		}
		return totalWeight;
	}
	/**
	 * Calculates and returns the maximum weight the player can carry
	 * @return The total weight that can be lifted
	 */
	public double getMaxInventoryWeight()
	{
		return strength * LIFTCAPACITY;
	}
	/**
	 * Increases the strength of the player
	 * @param increment The increment of the players strength
	 */
	public void increaseStrength(double increment)
	{
		this.strength += increment;
	}
	/**
	 * 
	 * @return The room which the player is currently in
	 */
	public Room getCurrentRoom()
	{
		return currentRoom;
	}
	/**
	 * Check if an item exits in the players inventory or not
	 * @param itemName The item that should be looked for
	 * @return True if the item exits, false if not
	 */
	public boolean isInIventory(String itemName)
	{
		return inventory.containsKey(itemName);
	}
	/**
	 * Moves the player to another room
	 * @param nextRoom The room to player should be moved to
	 */
	public void movePlayer(Room nextRoom) {   
		previousRooms.push(currentRoom);
		currentRoom = nextRoom;
		sucsessfullRound();
	}
	/**
	 * Uses the Beamer to teleport the Player into another room
	 */
	public void useBeamer() {
		previousRooms.clear();
		currentRoom=safeRoom;
		sucsessfullRound();
	}
	/**
	 * Reduces the number of rounds the player got left
	 */
	public void sucsessfullRound() {
		roundsLeft--;
	}
	/**
	 * Calculates if the player can carry a given item or not
	 * @param item The item to be evaluated
	 * @return true if the player can carry the item, false if not
	 */
	public boolean isAbleToCarry(Item item) {
		if (item.getWeight()+getTotalWeightInventory() <= getMaxInventoryWeight()) {
			return true;
		}
		return false;
	}
	/**
	 * Move the player to the latest room
	 * @return true if possible, false if not
	 */
	public boolean moveToLastRoom()
	{
		if (!previousRooms.empty())
		{
			Room lastRoom = previousRooms.peek();
			String exit = currentRoom.getConnectionExit(lastRoom);

			if (currentRoom.isLocked(exit))
			{
				System.out.println("You cant go back, this door is closed!");
				return true;
			}

			currentRoom = previousRooms.pop();          
			return true;
		}
		else
			return false;
	}
	public void teleport(Room teleportRoom)
	{
		currentRoom = teleportRoom;
		previousRooms.clear();
	}

}
