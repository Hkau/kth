T
/**
 * Write a description of class Character here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */
public class Character
{
    // 7,48 character
    public Room charCurrentRoom;
    private String hello;

    /**
     * Constructor for objects of class Character
     */
    public Character(Room startRoom, String greeting)
    {
        charCurrentRoom = startRoom;
        hello = greeting;
    }
    public Room room(){
        return charCurrentRoom;
    }
    public String talk(){
        return hello;
        
    }
}
