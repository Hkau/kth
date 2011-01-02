import java.util.ArrayList;
/*# EXERCISE 7.45 (LOCKED DOOR)*/
/**
 * LockedRoom - Room subclass
 * Behaves like a normal room apart from that the player needs a key to unlock the door before the 
 * room can be entered.
 */
public class LockedRoom extends Room {
    /**
     * Creates a room with a locked door.
     * @param roomDescription A description of the room.
     */
    public LockedRoom(String roomDescription){
        super(roomDescription);
        locked = true;
    }
    /**
     * Checks if the player has the key to the door. If so, the door is unlocked.
     * @return True if unlocked, false if not.
     */
    public boolean attemptOpen(ArrayList<Item> items){
        for(Item item : items){    
            if(item.getIdentification().equals("key")){
                locked = false;               
                return true;
            }
        }
        return false;
    }
}