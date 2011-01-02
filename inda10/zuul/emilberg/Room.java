import java.util.Set;
import java.util.HashMap;
import java.util.Iterator;
import java.util.ArrayList;

/**
 * Class Room - a room in an adventure game.
 *
 * A "Room" represents one location in the scenery of the game.  It is 
 * connected to other rooms via exits.  For each existing exit, the room 
 * stores a reference to the neighboring room.
 * 
 * @author  Michael Kolling, David J. Barnes and Emil Berg
 * @version 1.00
 */

public class Room 
{
    private String description;
    // This rooms exits.
    private HashMap<ExitDirection, Exit> exits;
    // This rooms items.
    private ArrayList<Item> items;
    
    // All the possible directions of the exits while changing room
    public static enum ExitDirection {
        NORTH,
        WEST,
        SOUTH,
        EAST
    }
    
    /**
     * Create a room described "description". Initially, it has
     * no exits. "description" is something like "a kitchen" or
     * "an open court yard".
     * @param description The room's description.
     */
    public Room(String description) 
    {
        this.description = description;
        exits = new HashMap<ExitDirection, Exit>();
        items = new ArrayList<Item>();
    }

    /**
     * Define an exit from this room.
     * @param direction The direction of the exit.
     * @param neighbor  The room to which the exit leads.
     */
    public void setExit(ExitDirection direction, Room neighbor) 
    {
        setExit(direction, neighbor, null, "");
    } 
    
    /**
     * Define an exit from this room.
     * @param direction The direction of the exit.
     * @param neighbor  The room to which the exit leads.
     */
    public void setExit(ExitDirection direction, Room neighbor, Item key, String walkThroughMessage) 
    {
        Exit exit = new Exit(neighbor, key, walkThroughMessage);
        exits.put(direction, exit);
    }
    
    /**
     * Adds an item to the room.
     * @param item The item to add.
     */
    public void addItem(Item item) {
        items.add(item);
    }
    
    /**
     * @return All the locked exits in the room.
     */
    public ArrayList<Exit> lockedExits() {
        
        ArrayList<Exit> lockedExits = new ArrayList<Exit>();
        
        for (Exit exit : exits.values()) {
            if (exit.opened() == false) {
                lockedExits.add(exit);
            }
        }
        
        return lockedExits;
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
        return "You are " + description + ".\n" + listItems() + "\n" + getExitString();
    }
    
    /**
     * Return a description of all the items in the players
     * inventory in the form:
     *      item1
     *      item2
     * @return A String listing all the items in a room.
     */
    public String listItems() {
        
        if (items.size() == 0) {
            return "";
        }
        
        String itemString = "There is a ";
        
        for (int i = 0; i < items.size(); i++) {
            itemString += items.get(i).getName();
            if (i == items.size() - 2) {
                itemString += " and a ";
            } else if (i < items.size() - 2) {
                itemString += ", ";
            }
        }
        
        itemString += " in the room.";
        
        return itemString;
    }

    /**
     * Return a string describing the room's exits, for example
     * "Exits: north west".
     * @return Details of the room's exits.
     */
    private String getExitString()
    {
        String returnString = "Exits:";
        Set<ExitDirection> keys = exits.keySet();
        for(ExitDirection exit : keys) {
            returnString += " " + exit.toString().toLowerCase();
        }
        
        if (exits.size() == 0) {
            returnString += " none";
        }
        
        return returnString;
    }

    /**
     * Return the exit that is reached if we go from this room in ExitDirection
     * "direction". If there is no exit in that direction, return null.
     * @param direction The exit's direction.
     * @return The exit in the given direction.
     */
    public Exit getExit(ExitDirection direction)
    {
        return exits.get(direction);
    }
    
    /**
     * Return the item with the given name, or null if the item
     * doesn't exist.
     * @param itemName The name of the item.
     */
    public Item getItem(String itemName) {
        for (Item item : items) {
            if (item.getName().equals(itemName)) {
                return item;
            }
        }
        
        return null;
    }
    
    /**
     * Removes the item with the given name from the room and
     * return it.
     * @return the item, or null if it doesn't exist.
     */
    public Item takeItem(String itemName) {
        int index = items.indexOf(getItem(itemName));
        if (index == -1) {
            return null;
        }
        return items.remove(index);
    }
}

