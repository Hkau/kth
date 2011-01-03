package org.x2d.zuul;
import java.io.*;
/**
 * Character class. Stores information about one NPC in the world.
 * Subclasses should override methodes when needed to create new
 * functionality for a NPC. Here comes and example of how it can be used:
<pre>
public class MyCharacter extends Character {
    public MyCharacter() {
        super("my char name", "something the character says");
    }
    &#0064;Override
    public void talk(Game g, Player p) {
        //Something that should happen when the player talks to this
        //character.
    }
}
</pre>
 */
public abstract class Character implements Serializable {
    private Room currentRoom;
    private String name;
    private String textFirstTime;
    private Item wantedItem;
    private boolean walkRandomly = false;
    private boolean isFirstTime = true;
    
    /** 
     * Creates a new Character.
     * @param name The character's name.
     * @param firstTime A text that will be displayed the first time the player
     * meets this NPC.
     */
    public Character(String name, String firstTime) {
        this.name = name;
        this.textFirstTime = firstTime;
    }
    
    /**
     * Method that contains all the dialog and special things that should
     * happen when the player talks with this NPC
     *
     * @param g The game.
     * @param p The player.
     */
    public abstract void talk(Game g, Player p);
    
    /**
     * Gets the room which the NPC is in. 
     *
     * @return The room.
     */
    public Room getCurrentRoom() {
        return currentRoom;
    }
    
    /**
     * Sets the room which the character is in. 
     *
     * @param room The room.
     */
    public void setCurrentRoom(Room room) {
        currentRoom = room;
    }
    
    /**
     * Gets the character's name.
     *
     * @return The character's name.
     */
    public String getName() {
        return name;
    }
    
    /**
     * Gets the item the character wants. If he retrives this item something will happen.
     *
     * @return The item the character want or <code>null</code> if the character doesn't want an item.
     */
    public Item getWantedItem() {
        return wantedItem;
    }
    
    /**
     * Sets the item the character wants. If he retrives this item something will happen.
     *
     * @param wantedItem Item that the character want.
     */
    public void setWantedItem(Item wantedItem) {
        this.wantedItem = wantedItem;
    }
    
    /**
     * Sets if the character should walk around on the map.
     *
     * @param isWalkingRandomly <code>true</code> if the character should walk around on the map else <code>false</code>.
     */
    public void setWalkRandomly(boolean isWalkingRandomly) {
        walkRandomly = isWalkingRandomly;
    }
    
    /**
     * Gets if the character should walk around on the map.
     *
     * @return <code>true</code> if the character should walk around on the map else <code>false</code>.
     */
    public boolean isWalkingRandomly() {
        return walkRandomly;
    }

    /**
     * Gets the text that will be displayed the first time the player meets this npc.
     *
     * @return The text that will be displayed the first time the player meets this npc.
     */
    public String getFirstTimeText() {
        setFirstTime(false);
        return textFirstTime;
    }

    /**
     * Gets if this is the first time the player meets this NPC.
     *
     * @return <code>true</code> if this is the first time else <code>false</code>.
     */
    public boolean isFirstTime() {
        return isFirstTime;
    }
    
    /**
     * Sets if this is the first time the player meets this NPC.
     *
     * @param isFirstTime <code>true</code> if the character should walk around on the map else <code>false</code>.
     */
    public void setFirstTime(boolean isFirstTime) {
        this.isFirstTime = isFirstTime;
    }
}
