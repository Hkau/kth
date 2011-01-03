import java.util.HashMap;

import java.util.Set;


/**
 * Class Room - a room in an adventure game.
 *
 * This class is part of the "World of Zuul" application. 
 * "World of Zuul" is a very simple, text based adventure game.  
 *
 * A "Room" represents one location in the scenery of the game.  It is 
 * connected to other rooms via exits.  The exits are labelled north, 
 * east, south, west.  For each direction, the room stores a reference
 * to the neighboring room, or null if there is no exit in that direction.
 * 
 * @author  Michael Kolling and David J. Barnes
 * @version 2008.03.30
 */
public class Room 
{
	private String description;
	private HashMap<String, Room> exits;
	private HashMap<String, Boolean> exitsLock;
	private HashMap<String, Item> items;
	private HashMap<String, Npc> characters;
	private boolean teleportaion;
	


	/**
	 * Create a room described "description". Initially, it has
	 * no exits. "description" is something like "a kitchen" or
	 * "an open court yard".
	 * @param description The room's description.
	 */
	public Room(String description) 
	{
		this.description = description;
		exits = new HashMap<String, Room>();
		items = new HashMap<String, Item>();
		exitsLock = new HashMap<String, Boolean>();
		characters = new HashMap<String, Npc>();
	}
	/**
	 * Return the room that is reached if we go from this room in direction
	 * "direction". If there is no room in that direction, return null.
	 * @param direction The exit's direction.
	 * @return The room in the given direction.
	 */
	public Room getExit(String direction) {
		return exits.get(direction);
	}
	public Npc getNpc(String name) {
		return characters.get(name);
	}
	/**
	 * Retrun if a given exit is locked or not.
	 * @param direction The exit's direction.
	 * @return True if exit is locked, false if exit is open
	 */
	public boolean isLocked(String direction) {
		if (direction!=null)
			return exitsLock.get(direction).booleanValue();
		else
			return false;
	}
	/**
	 * Locks or unlock a given exit
	 * @param direction The exit's direction.
	 * @param condition true if the door should be locked, false if unlocked
	 */
	public void lockExit(String direction, boolean condition) {
		exitsLock.put(direction, condition);
	}
	/**
	 * Return the name of a exit that is connected with a given neighbour
	 * @param neighbour the neigbour to be looked for
	 * @return the name of the exit that connects this room with the neighbour, 
	 * null if no conenction is found
	 */
	public String getConnectionExit(Room neighbour) {
		Set<String> exitKey = exits.keySet(); 
		for(String exitName : exitKey){
			if (exits.get(exitName) == neighbour)
				return exitName;
		}
		return null;
	}
	/**
	 * Set the current room to a teleportaion room
	 * @param condition True if the room is a teleportaion room, false if not
	 */
	public void setTeleportation(boolean condition)
	{
		teleportaion = condition;
	}
	/**
	 * Return the state of the varibel teleportaion
	 * @return True if the room is a teleportaion room, false if not
	 */
	public boolean getTeleportaion()
	{
		return teleportaion;
	}

	/**
	 * Return a description of the room's exist,
	 * for example, "Exits: north west".
	 * @return A description of the available exits.
	 */
	public String getExitString()
	{
		String returnString = "Exits:";
		Set<String> keys = exits.keySet();
		for (String exit: keys) {
			returnString += " " + exit;
		}
		return returnString;
	}
	/**
	 * Adds a item to the room
	 * @param name The name of the item
	 * @param description The description of the item
	 * @param weight The weight of the item
	 */
	public void addItem(String name, String description, double weight)
	{
		items.put(name, new Item(name, description, weight));
	}
	/**
	 * Adds a item to the room
	 * @param item The item to be added
	 */
	public void addItem(Item item) {
		items.put(item.getName(), item);
	}

	public void addNpc(String name, String description) {
		characters.put(name, new Npc(name, description));
	}

	/**
	 * Remove a item from the room
	 * @param item The name of the item to be removed
	 */
	public void removeItem(String item) {
		items.remove(item);
	}
	/**
	 * Returns an item the exits is the room
	 * @param itemName The name of the item that should be returned
	 * @return The item!
	 */
	public Item getItem(String itemName) {
		return items.get(itemName);
	}
	/**
	 * Define the exits from this room.
	 * to another room or is null (no exit there).
	 * @param direction The direction of the exit.
	 * @param neighbor The room in the fiven diretion.
	 */
	public void setExits(String direction, Room neighbor)
	{
		exits.put(direction, neighbor);
		exitsLock.put(direction, false);
	}

	/**
	 * @return The description of the room.
	 */
	public String getDescription()
	{
		return description;
	}
	
	/**
	 * Return a long description of this room, of the form:
	 *  You are in the kitchen.
	 *  Exits: north west
	 *  
	 * @return A description of the room, includning exits.
	*/
	public String getLongDescription()
	{   
		String returnString;

		returnString= "You are " + description + "."; 
		returnString += getItemString();
		returnString +="\n"+ getNpcString();
		returnString +="\n\n"+ getExitString();
		return returnString;
	}
	/**
	 * Returns a String contaning iformation regardning the items in the room
	 * @return A string with item information
	 */
	public String getItemString(){
		String itemString = "\nItems:";
		if(!items.isEmpty()){

			Set<String> itemNames = items.keySet(); 
			for(String itemName : itemNames){
				itemString += "\n  " + getItem(itemName).getDescriptionString();
			}
			return itemString;
		}      
		else
			return itemString + "\nNone!";  
	}
	/**
	 * Returns a String contaning iformation regardning the npcs in the room
	 * @return A string with npc information
	 */
	public String getNpcString(){
		String charactersString = "Characters:";
		if(!characters.isEmpty()){

			Set<String> npcNames = characters.keySet(); 
			for(String npcName : npcNames){
				charactersString += "\n  " + getNpc(npcName).getFullDescription();
			}
			return charactersString;
		}      
		else
			return charactersString + "\nNone!"; 
	}
	/**
	 * Checks if the player is allowed to loot a certaint item
	 * @param item The item to check
	 * @return true if the player is allowed, false if not
	 */
	public boolean isItemLootable(Item item)
	{
		return item.isLootable();
	}



}
