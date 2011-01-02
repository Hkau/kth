import java.util.Set;
import java.util.HashMap;
import java.util.Iterator;

/**
 * Class Room - a room in an adventure game.
 *
 * This class is part of the "World of Zuul" application. 
 * "World of Zuul" is a simple, text based adventure game.  
 *
 * A "Room" represents one location in the scenery of the game.  It is 
 * connected to other rooms via exits.  For each existing exit, the room 
 * stores a reference to the door to the neighboring room.
 * 
 * @author  Michael Kolling, David J. Barnes and Vladimir Grozman
 * @version 2010.12.02
 */

public class Room 
{
    private String description;
    private HashMap<String, Door> exits;        // stores exits of this room.

    /**
     * Create a room described "description". Initially, it has
     * no exits. "description" is something like "a kitchen" or
     * "an open court yard".
     * @param description The room's description.
     */
    public Room(String description) 
    {
        this.description = description;
        exits = new HashMap<String, Door>();
    }

    /**
     * Define an exit from this room.
     * @param direction The direction of the exit.
     * @param neighbor  The room to which the exit leads.
     * @param locked    Whether the exit is locked.
     */
    public void setExit(String direction, Room neighbor, boolean locked) 
    {
        exits.put(direction, new Door(neighbor, locked));
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
    private String getExitString()
    {
        String exitString = "";
        Set<String> keys = exits.keySet();
        for(String exit : keys) {
            exitString += " " + exit;
        }
        return (exitString.equals("") ? "There are no exits!!1 You are stuck here forever, have a good day, Sir." : "Exits:" + exitString);
    }

    /**
     * Return the room that is reached if we go from this room in direction
     * "direction". If there is no room in that direction, return null.
     * @param direction The exit's direction.
     * @return The room in the given direction.
     */
    public Room getExit(String direction) 
    {
        if(exits.get(direction) == null) return null;
        return exits.get(direction).getTarget();
    }
    
    /**
     * Returns whether specified exit is locked
     */
    public boolean isExitLocked(String direction) 
    {
        if(exits.get(direction) == null) return false;
        return exits.get(direction).isLocked();
    }
    
    /**
     * Unlocks specified exit
     */
    public void unlockExit(String direction) {
        if(exits.get(direction) != null) exits.get(direction).setLocked(false);
    }
    
    /**
     * Locks specified exit
     */
    public void lockExit(String direction) {
        if(exits.get(direction) != null) exits.get(direction).setLocked(true);
    }
}