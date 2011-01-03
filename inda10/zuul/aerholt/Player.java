package game;


/**
 * Describes a player. 
 * Keeps track of:
 *  - health
 *  - hunger 
 *  - distance walked
 *  - name
 *  - location
 *  - inventory, with an ItemCollection
 * 
 * @author Anton Erholt 
 * @version 2010.11.23
 */
public class Player
{   
    private String name;
    private int health;
    private Room location;
    public ItemCollection inventory;
    
    /**
     * Constructor for objects of class Player
     * @param playerName The name of the player.
     * @param playerLocation The players starting location.
     */
    public Player(String playerName,Room startingLocation)
    {
        name = playerName;
        health = 100;
        location = startingLocation;
        inventory = new ItemCollection();
    }

    /**
     * Returns the healts as an int.
     */
    public int getHealth(){
        return health;
    }
    
    /**
     * Sets the health of the player to newHealth
     */
    public void setHealth(int newHealth){
        health = newHealth;
    }
    
    /**
     * Returns the player name.
     */
    public String getName(){
        return name;
    }
        
    /**
     * Returns the current location as a room.
     */
    public Room getLocation(){
        return location;
    }
        
    /**
     * Sets the player location.
     */
    public void setLocation(Room newLocation){
        location = newLocation;    
    }
    
}
