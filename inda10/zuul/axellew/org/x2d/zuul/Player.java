package org.x2d.zuul;
import java.util.*;
import java.io.*;
/**
 * Class representing the player. The player has a backpack
 * which stores item.
 */
public class Player implements Serializable {
    private String name;
    private HashSet<Items> items = new HashSet<Items>(10);
    private Room currentRoom;
    
    /**
     * Constructor for objects of class Player
     * @param name The name of the player.
     */
    public Player(String name) {
        this.name = name;
    }
    
    /**
     * Gets the name of the player
     *
     * @return The player's name
     */
    public String getName() {
        return name;
    }
    
    /**
     * Sets the player's name
     *
     * @param name The player's new name.
     */
    public void setName(String name) {
        this.name = name;
    }
    
    /**
     * Adds an item to the players backpack
     *
     * @param item The item to add.
     */
    public void addItem(Items item) {
        items.add(item);
        System.out.println(String.format("Du tar imot %s och lägger i din ryggsäck.", item.getItem().getName()));
    }
    
    /**
     * Removes an item from the players backpack
     *
     * @param item The item to remove.
     */
    public void removeItem(Items item) {
        items.remove(item);
        System.out.println(String.format("Du plockar upp %s ur din räcksäck och använder.", item.getItem().getName()));
    }
    
    /**
     * Gets an item from the players backpack
     *
     * @param item The item
     * @return <code>true</code> if found else <code>false</code>
     */
    public boolean hasItem(Items item) {
        return items.contains(item);
    }
    
    /**
     * Gets a collection of all the items in the player's backpack
     *
     * @return A collection of all the items
     */
    public Collection<Items> getItems() {
        return items;
    }
    
    /**
     * Gets the room current which the player currently is in.
     *
     * @return The current room.
     */
    public Room getCurrentRoom() {
        return currentRoom;
    }
    
    /**
     * Sets the room current which the player currently is in.
     *
     * @param cRoom The the room which the player should now be in.
     */
    public void setCurrentRoom(Room cRoom) {
        currentRoom = cRoom;
        System.out.println();
        System.out.println(currentRoom.getLongDescription());
    }
    /** 
     * Try to go to one direction. If there is an exit, enter the new
     * room, otherwise print an error message.
     */
    public void goRoom(String direction) 
    {
        // Try to leave current room.
        Door door = getCurrentRoom().getDoor(direction);
        if (door.isLocked()) {
            Items unlockItem = door.getUnlockItem();
            if (unlockItem!=null) {
                System.out.println(String.format("Dörren är låst och du behöver: %s för att låsa upp dörren..", unlockItem.getItem().getName()));
            } else {
                System.out.println("Dörren är låst.");
            }
            return;
        }
        Room nextRoom = door.getExit(getCurrentRoom());
        setCurrentRoom(nextRoom);

    }
}
