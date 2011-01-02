/*# EXERCISE 7.46 (TRANSPORTER ROOM)*/
/**
 * TransporterRoom - Room subclass
 * A transporter room behaves a little bit differently from a normal room. Instead of
 * having specified neighbor rooms it puts the player in a random room when exited.
 */

public class TransporterRoom extends Room{
    /**
     * Creates a transporter room with the specified description.
     * @param roomDescription A description of the room.
     */
    public TransporterRoom(String roomDescription){
        super(roomDescription);
    }
    /**
     * Return a random room.
     * @direction The exit's direction.
     * @return A random room, if exit exists.
     */
    public Room getExit(String direction){
        if(exits.get(direction) != null){
            return randomRoom();
        } else {
            return null;
        }
    }
    /**
     * Choose a random room.
     * @return A random room.
     */
    private Room randomRoom(){
        int random = (int)(Math.random() * (rooms.size()-1));
        //Some rooms are excluded
        while(rooms.get(random) instanceof TransporterRoom || rooms.get(random) instanceof LockedRoom
        || rooms.get(random) instanceof TrapRoom){
            random = (int)(Math.random() * (rooms.size()-1));
        }
        return rooms.get(random);
    }
}