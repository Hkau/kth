import java.util.Set;
import java.util.HashMap;
import java.util.Iterator;
import java.util.ArrayList;
import java.lang.Math;

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
    protected HashMap<String, Room> exits;  // stores exits of this room.
    protected boolean locked = false;
    protected static ArrayList<Room> rooms = new ArrayList<Room>(); //stores all rooms
    private ArrayList<Item> items;
    /**
     * Create a room described "description". Initially, it has
     * no exits. "description" is something like "a kitchen" or
     * "an open court yard".
     * @param description The room's description.
     */
    public Room(String roomDescription) 
    {
        description = roomDescription;
        exits = new HashMap<String, Room>();
        items = new ArrayList<Item>();
        rooms.add(this);
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
     * Adds a new item to this room.
     * @param description A description of the item
     * @param id The identy of the item, e.g. ball, car or key
     */
    public void addItem(String description,String id){
        items.add(new Item(description,id));
    }
    /**
     * Puts an already instantiated item in this room.
     * @param item The item to put in the room
     */
    public void dropItem(Item item){
        items.add(item);
    }
    /**
     * Removes the specified item from the room.
     * @param index The index of the item in the ArrayList-
     */
    public void removeItem(int index){
        items.remove(index);
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
        String rString = "You are " + description + ".\n";
        for(Item item : items){
            rString += item.getLongDescription() + ".\n";
        }
        rString += getExitString();
        return rString;
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
        return exits.get(direction);
    }
    /**
     * This function only serves a purpose in the LockedRoom subclass
     */
    public boolean attemptOpen(ArrayList<Item> items){
        return false;
    }
    public boolean getLocked(){
        return locked;
    }
    public ArrayList<Item> getItems(){
        return items;
    }
        
}

