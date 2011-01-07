import java.util.Set;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;

//import javax.swing.text.html.HTMLDocument.Iterator;
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
    private HashMap<String, Room> exits;        // stores exits of this room.
    private HashMap<String, Key> locks;
    private ArrayList<Key> keys;

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
        locks = new HashMap<String, Key>();
        keys = new ArrayList<Key>();
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
        return "You are " + description + ".\n" +
        	getExitString()	+ "\n" +
        	getLockString()	+ "\n" +
        	getKeyString()	+ "\n";
    }

    /**
     * Return a string describing the room's exits, for example
     * "Exits: north west".
     * @return Details of the room's exits.
     */
    private String getExitString()
    {
        String returnString = "Exits:";
        Set<String> keys = exits.keySet();
        for(String exit : keys) {
            returnString += " " + exit;
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
    	if (locks.containsKey(direction))
    		return null;
    	
        return exits.get(direction);
    }
    
    /**
     * Define a key from this room.
     * @param direction	The direction of the lock.
     * @param key		The key needed to open the lock
     */
    public void setLock(String direction, Key key) {
    	locks.put(direction, key);
    }
    
    /**
     * Return a string describing exit's locks
     * @return Details of exits locks
     */
    private String getLockString()
    {
    	String returnString = "Locks:";
    	Set<String> keys = locks.keySet();
    	for(String lock : keys) {
    		returnString += " " + lock;
    	}
    	return returnString;
    }
    
    /**
     * Get Key to a lock
     * @param direction
     * @return key to lock
     */
    private Key getKeyToLock(String direction)
    {
    	return locks.get(direction);
    }
   
    /**
     * Try to unlock lock
     * @param direction	Direction of lock
     * @param key		What key to use
     * @return 			If unlock successfull return true
     */
    public boolean unlockLock(String direction, Key key)
    {
    	if (getKeyToLock(direction).getName().equals(key.getName())) {
    		removeLock(direction);
    		return true;
    	}
    	return false;
    }
    
    /**
     * Remove a certain lock
     * @param direction
     */
    private void removeLock(String direction)
    {
    	locks.remove(direction);
    }
    
    /**
     * Return a string describing rooms keys
     * @return Details of room's keys
     */
    private String getKeyString()
    {
    	String returnString = "Keys:";
    	for (Key key: keys) {
    		returnString += " " + key.getName();
    	}
    	return returnString;
    }
    
    /**
     * Add key to room
     * @param key
     */
    public void addKey(Key key)
    {
    	keys.add(key);
    }
    
	/**
	 * If a key matches the string return it
	 * @param keyName string
	 * @return key that mathes string or null
	 */
    public Key getKey(String keyName)
    {
    	Iterator<Key> it = keys.iterator();
    	while (it.hasNext()) {
    		Key currentKey = it.next();
    		if (currentKey.getName().equals(keyName)) {
    			return currentKey;
    		}
    	}
    	return null;
    }
    
	/**
	 * If key is found remove it
	 * @param key remove the key
	 */
    public void removeKey(Key key)
    {
    	Iterator<Key> it = keys.iterator();
    	while (it.hasNext()) {
    		Key currentKey = it.next();
    		if (currentKey == key) {
    			it.remove();
    		}
    	}
    }
}

