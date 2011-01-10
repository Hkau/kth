
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
    private String Name;

    /**
     * Constructor for objects of class Character
     */
    public Character(String name, Room startRoom, String greeting)
    {
        charCurrentRoom = startRoom;
        hello = greeting;
        Name = name;
    }
    public String GetName(){
        return Name;    
    }
    public Room room(){
        return charCurrentRoom;
    }
    public String talk(){
        return hello;
        
    }
}
