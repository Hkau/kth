import java.util.ArrayList;
import java.util.Random;
import java.util.Set;
import java.util.HashMap;
//import java.util.Iterator;

/**
 * Class Room - a room in an adventure game.
 *
 * This class is part of the "World of Zuul" application. 
 * "World of Zuul" is a very simple, text based adventure game.  
 *
 * A "Room" represents one location in the scenery of the game.  It is 
 * connected to other rooms via exits.  For each existing exit, the room 
 * stores a reference to the neighboring room.
 * 
 * @author  Michael Kolling and David J. Barnes
 * @version 2008.03.30
 */

public class Room 
{
    private String description;
    // stores exits of this room.
    private HashMap<String, Room> exits; 
    //stores the hiden exits
    private ArrayList<String> hidenExits;
    private Random r;
    private boolean teleportRoom;
    private boolean locked;
    //the "key" to unlock the room if its locked, usally the title of the item
    private String key;
    //a iventory to store items
    private Inventory inven;

    /**
     * Create a room described "description". Initially, it has
     * no exits. "description" is something like "a kitchen" or
     * "an open court yard".
     * @param description The room's description.
     */
    public Room(String description,String key) 
    {
    	r = new Random();
        this.description = description;
        this.key= key;
        exits = new HashMap<String, Room>();
        setTeleportRoom(false);
        setLocked(false);    
        inven = new Inventory();
        hidenExits = new ArrayList<String>();
    }
    public Room(String description) 
    {
    	r = new Random();
        this.description = description;
        this.key= null;
        exits = new HashMap<String, Room>();
        setTeleportRoom(false);
        setLocked(false);    
        inven = new Inventory();
        hidenExits = new ArrayList<String>();
    }

    /**
     * Define an exit from this room.
     * @param direction The direction of the exit.
     * @param neighbor  The room to which the exit leads.
     */
    public void setExit(String direction, Room neighbor) 
    {
        exits.put(direction, neighbor);
    }

    /**
     * @return The short description of the room
     * (the one that was defined in the constructor).
     */
    public String getShortDescription()
    {
        return description;
    }

    /**
     * Return a description of the room in the form:
     *     You are in the kitchen.
     *     Exits: north west
     * @return A long description of this room
     */
    public String getLongDescription()
    {
        return "You are " + description + ".\n" + getExitString();
    }

    /**
     * Return a string describing the room's exits, for example
     * "Exits: north west".
     * @return Details of the room's exits.
     */
    private String getExitString(){
        String returnString = "Exits:";
        Set<String> keys = exits.keySet();

        for(String exit : keys) {
        		if(!hidenExits.contains(exit)){
        			returnString += " " + exit;
        		}
        } 
        return returnString;
    }

    /**
     * Return the room that is reached if we go from this room in direction
     * "direction". If there is no room in that direction, return null.
     * @param direction The exit's direction.
     * @return The room in the given direction.
     */
    public Room getExit(String direction) 
    {
        return exits.get(direction);
    }
    public Room getRandomRoom(ArrayList<Room> allRoomes){
    	return allRoomes.get(r.nextInt(allRoomes.size()));
    }

	/**
	 * @param teleportRoom the teleportRoom to set
	 */
	public void setTeleportRoom(boolean teleportRoom) {
		this.teleportRoom = teleportRoom;
	}
	/**
	 * @return the teleportRoom
	 */
	public boolean isTeleportRoom() {
		return teleportRoom;
	}
	/**
	 * @param locked the locked to set
	 */
	public void setLocked(boolean locked) {
		this.locked = locked;
	}
	/**
	 * @return the locked
	 */
	public boolean isLocked() {
		return locked;
	}
	/**
	 * Sets the "password" for the room
	 * @param key the key to set
	 */
	public void setKey(String key) {
		this.key = key;
	}
	/**
	 * tries to unlock a room with the inparameter
	 * @param inKey
	 */
	public void unlock(String inKey){
		if(inKey.equals(key)){
			setLocked(false);
		}
		else{
			setLocked(true);
		}
	}
	/**
	 * adds a item to the inventory
	 * @param title
	 * @param description
	 */
	public void addInventory(String title,String description){
		inven.createNewItem(title, description);
	}
	/**
	 * adds a item to the inventry
	 * @param newItem
	 */
	public void addItem(Item newItem){
		inven.addItem(newItem);
	}
	/**
	 * returns a item from the inventoty by the title of it
	 * @param title
	 * @return
	 */
	public Item getItem(String title){
		return inven.getItem(title);
	}
	/**
	 * 
	 */
	public void deleteItem(String title){
		inven.deleteItem(title);
	}
	/**
	 * lists the inventory in the room
	 */
	public void list(){
		System.out.println("Theese items are in the room: ");
		System.out.println(inven.toString());
	}
	/**
	 * Hides a exit so that it wont be listed as a exit.
	 * @param exit
	 */
	public void hideExit(String exit){
		hidenExits.add(exit);		
	}
}

