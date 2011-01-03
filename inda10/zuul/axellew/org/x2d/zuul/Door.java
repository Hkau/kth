package org.x2d.zuul;
import java.io.*;
/**
 * This class represents a door going between two rooms.
 */
public class Door implements Serializable
{
    private boolean isLocked;
    private Items unlockItem;
    private Room room1, room2;
    
    /**
     * Constructor for objects of class Door
     *
     * @param room1 The first room.
     * @param room2 The second room.
     * @param isLocked If the door is locked or not.
     */
    public Door(Room room1, Room room2, boolean isLocked)
    {
        this.room1 = room1;
        this.room2 = room2;
        this.isLocked = isLocked;
    }
    /**
     * Constructor for objects of class Door
     */
    public Door() {
        this(null, null, false);
    }
    
    /**
     * Constructor for objects of class Door
     *
     * @param isLocked If the door is locked or not.
     */
    public Door(boolean isLocked) {
        this(null, null, isLocked);
    }
    
    /**
     * Sets an item which is needed to unlock this door.
     *
     * @param item The item.
     */
    public void setUnlockItem(Items item) {
        unlockItem = item;
    }
    
    /**
     * Gets the item which is needed to unlock this door.
     *
     * @return item The item.
     */
    public Items getUnlockItem() {
        return unlockItem;
    }
    
    /**
     * Sets if this door should be locked or not.
     *
     * @param isLocked Should be <code>true</code> if the door should be locked else <code>false</code>.
     */
    public void setLocked(boolean isLocked) {
        this.isLocked = isLocked;
    }
    
    /**
     * Gets if this door should be locked or not.
     *
     * @return <code>true</code> if the door is locked else <code>false</code>.
     */
    public boolean isLocked() {
        return isLocked;
    }
    
    /**
     * Returns the exit room.
     *
     * @param entrance If this is room1, then room2 is returned, and if it's room2 then room1 is returned.
     * @return The exit room.
     */
    public Room getExit(Room entrance) {
        return (entrance==room1)?room2:room1;
    }
    
    /**
     * Sets the first room.
     * 
     * @param r1 The first room.
     */
    public void setRoom1(Room r1) {
        this.room1 = r1;
    }
    
    /**
     * Sets the second room.
     * 
     * @param r2 The second room.
     */
    public void setRoom2(Room r2) {
        this.room2 = r2;
    }

}
