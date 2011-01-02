/**
 * This is a door, seen from a specific room.
 * It has a target room and can be locked.
 * 
 * @author Vladimir Grozman 
 * @version 2010.12.02
 */
public class Door
{

    private Room target;
    private boolean locked;
    
    public Door(Room targetRoom, boolean lockState)
    {
        target = targetRoom;
        locked = lockState;
    }
    
    /**
     * Returns target room
     */
    public Room getTarget() {return target;}
    
    /**
     * Returns whether door is locked
     */
    public boolean isLocked() {return locked;}
    
    /**
     * Sets whether door is locked
     */
    public void setLocked(boolean lockState) {locked = lockState;}

}