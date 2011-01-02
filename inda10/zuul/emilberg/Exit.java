
/**
 * Describes an exit, which the player needs to move through when moving between rooms.
 * 
 * @author Emil Berg
 * @version 1.00
 */
public class Exit
{
    // The key to open the exit
    private Item key;
    // The room the exit points to
    private Room room;
    // If the exit is open
    private boolean open;
    // The message to display when the exit is passed through.
    private String walkThroughMessage;
    
    /**
     * Creates an exit from a room to another.
     * @param room The room the exit leads to.
     * @param key The key required to go open the exit. Use null if the exit
     * always should be opened and not require a key.
     * @param walkThroughMessage The message displayed when the player moves
     * through the exit.
     */
    public Exit(Room room, Item key, String walkThroughMessage) {
        // If there is a key, lock the door.
        if (key == null) {
            open = true;
        } else {
            open = false;
        }
        this.key = key;
        this.room = room;
        this.walkThroughMessage = walkThroughMessage;
    }
    
    /**
     * @return The room the exit points to.
     */
    public Room getRoom() {
        return room;
    }
    
    /**
     * @return True if the exit is opened.
     */
    public boolean opened() {
        return open;
    }
    
    /**
     * @return The key that opens the exit.
     */
    public Item getKey() {
        return key;
    }
    
    /**
     * Unlocks the door (Makes the exit possible to move through).
     */
    public void unlock() {
        open = true;
    }
    
    /**
     * @return The message displayed when you walk through the exit.
     */
    public String getWalkThroughMessage() {
        return walkThroughMessage;
    }
}
