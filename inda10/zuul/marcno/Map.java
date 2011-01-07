/*
 * Map class, creates a certain map
 */
public class Map {
	
	private Room startRoom;
	
	public Map()
	{
		createMap();
	}
	
	/**
     * Create all the rooms and link their exits together.
     */
    void createMap()
    {
        Room outside, theatre, pub, lab, office;
      
        // create the rooms
        outside = new Room("outside the main entrance of the university");
        theatre = new Room("in a lecture theatre");
        pub = new Room("in the campus pub");
        lab = new Room("in a computing lab");
        office = new Room("in the computing admin office");
        
        // initialise room exits
        outside.setExit("east", theatre);
        outside.setExit("south", lab);
        outside.setExit("west", pub);
        outside.setLock("west", new Key("somekey"));
        outside.addKey(new Key("somekey"));
        theatre.setExit("west", outside);
        pub.setExit("east", outside);
        lab.setExit("north", outside);
        lab.setExit("east", office);
        lab.setLock("north", new Key("someotherkey"));
        office.setExit("west", lab);
        
        startRoom = outside;
    }
   
	/**
	 * @return the start room of the map
	 */ 
    public Room getStartRoom()
    {
    	return startRoom;
    }
}
