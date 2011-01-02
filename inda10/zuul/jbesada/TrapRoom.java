/*# EXERCISE 7.43 (TRAPDOOR)*/
/**
 * Class TrapRoom - Room subclass
 * This subclass does not overwrite any methods or values. Traprooms are rooms with trapdoors that can only be
 * crossed one way. Rooms that are instances of TrapRoom cant be exited using the 'back' command.
 */
public class TrapRoom extends Room {

    public TrapRoom(String roomDescription){
        super(roomDescription);
    }
}