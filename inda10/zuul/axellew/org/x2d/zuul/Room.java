package org.x2d.zuul;
import java.util.Set;
import java.util.HashMap;
import java.util.Iterator;
import java.util.*;
import java.io.*;
/**
 * Class Room - a room in an adventure game.
 *
 * A "Room" represents one location in the scenery of the game.  It is 
 * connected to other rooms via doors.  For each existing exit, the room 
 * stores a reference to the door.
 * 
 * The room also has items and doors which the player can interact with.
 */
public class Room implements Serializable {
    private String description;
    private HashSet<Items> items = new HashSet<Items>(2);
    private HashMap<String, Door> exits;        // stores exits of this room.
    private HashMap<String, Character> characters;

    /**
     * Create a room described "description". Initially, it has
     * no exits. "description" is something like "a kitchen" or
     * "an open court yard".
     * @param description The room's description.
     */
    public Room(String description) {
        this.description = description;
        exits = new HashMap<String, Door>();
        characters = new HashMap<String, Character>();
    }

    /**
     * Define an exit from this room. If neighbor is not null then
     * it will add the door to that room too.
     * @param direction The direction of the exit.
     * @param neighbor  The room to which the exit leads.
     */
    public void setEntrance(String direction, Room neighbor, Door door) {
        ArrayList<String> directions = new ArrayList<String>(4);
        directions.add(Game.Directions.NORTH.getValue());
        directions.add(Game.Directions.EAST.getValue());
        directions.add(Game.Directions.SOUTH.getValue());
        directions.add(Game.Directions.WEST.getValue());
        door.setRoom1(this);
        exits.put(direction, door);
        if (neighbor!=null) {
            //Gets the other side's exit direction, so south return north and so on
            String oppositeDirection = directions.get((directions.indexOf(direction)+2)%4);
            neighbor.setExit(oppositeDirection, door);
        }
    }
    
    /**
     * Define an exit from this room. The difference from setEntrance is that this
     * only sets a door to an direction and does not care where the door goes.
     * @param direction The direction of the exit.
     * @param door The door that should be in that direction.
     */
    public void setExit(String direction, Door door) {
        if (door!=null) {
            door.setRoom2(this);
            exits.put(direction, door);
        } else {
            exits.remove(direction);
        }
        
    }

    /**
     * @return The short description of the room
     * (the one that was defined in the constructor).
     */
    public String getShortDescription() {
        return description;
    }

    /**
     * Return a description of the room
     * @return A long description of this room
     */
    public String getLongDescription() {
        StringBuilder tmpString = new StringBuilder();
        tmpString.append(description).append("\n");
        Collection<Items> items = getItems();
        if (items.size() != 0) {
            tmpString.append(String.format("Det finns %d föremål i rummet:\n", items.size()));
            for (Items item : getItems()) {
                Item itemObject = item.getItem();
                tmpString.append(" - ").append(itemObject.getName()).append(": ");
                tmpString.append(itemObject.getDescription()).append("\n");
            }
        }
        Collection<Character> characters = getCharacters();
        if (characters.size() != 0) {
            tmpString.append(String.format("There finns %d person%s/djur i rummet:\n", characters.size(), (characters.size()==1)?"":"er"));
            for (Character character : characters) {
                tmpString.append(" - ").append(character.getName());
                if (character.isFirstTime()) {
                    tmpString.append(" säger: ").append(character.getFirstTimeText());
                    character.setFirstTime(false);
                }
                tmpString.append("\n");
            }
        }
        tmpString.append(getExitString());
        return tmpString.toString();
    }

    /**
     * Return a string describing the room's exits, for example
     * "Exits: north west".
     * @return Details of the room's exits.
     */
    private String getExitString() {
        StringBuilder returnString = new StringBuilder("Utgångar:");
        Set<String> keys = exits.keySet();
        if (keys.size()!=0) {
            for(String exit : keys) {
                returnString.append(" ").append(exit);
            }
        } else {
            returnString.append(" inga");
        }
        return returnString.toString();
    }
    
    /**
     * Gets a door from this room by direction.
     *
     * @param direction The direction.
     * @return the door or null if it doesn't exist
     */
    public Door getDoor(String direction) {
        return exits.get(direction);
    }
    
    /**
     * Adds an item to this room.
     * @param item The item
     */
    public void addItem(Items item) {
        items.add(item);
    }
    
    /**
     * Removes an item from this room.
     * @param item The item
     */
    public void removeItem(Items item) {
        items.remove(item);
    }
    
    /**
     * Gets an item by its name
     * @return The item.
     */
    public boolean hasItem(Items item) {
        return items.contains(item);
    }
    
    /**
     * Gets an collection with all the items in this room
     *
     * @return All the items in this room
     */
    public Collection<Items> getItems() {
        return items;
    }
    
    /**
     * Gets all the directions where there are doors.
     * 
     * @return The directions.
     */
    public String[] getExits() {
        return exits.keySet().toArray(new String[0]);
    }
    
    /**
     * Adds a character to this room.
     *
     * @param c The character
     */
    public void addCharacter(Character c) {
        characters.put(c.getName(), c);
        c.setCurrentRoom(this);
    }
    
    /**
     * Removes a character from this room.
     *
     * @param c The character
     */
    public void removeCharacter(Character c) {
        characters.remove(c.getName());
    }
    
    /**
     * Gets a character by his/her name
     * @return The item.
     */
    public Character getCharacter(String name) {
        return characters.get(java.lang.Character.toUpperCase(name.charAt(0))+name.substring(1));
    }
    
    /**
     * Gets an collection with all the characters in this room
     *
     * @return All the characters in this room
     */
    public Collection<Character> getCharacters() {
        return characters.values();
    }
}

