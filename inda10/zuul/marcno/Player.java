import java.util.ArrayList;
import java.util.Iterator;

/**
 * Class that takes care of the player, it's position, inventory etc.
 */
public class Player {
	
    private Room currentRoom;
	ArrayList<Key> keyBag;
	private Room loadedBeamRoom;
	
	public Player()
	{
		//currentRoom = new Room("loadingiroom");
		keyBag = new ArrayList<Key>();
	}

    /** 
     * Try to go to one direction. If there is an exit, enter the new
     * room, otherwise print an error message.
     */
    public void goRoom(Command command) 
    {
        if(!command.hasSecondWord()) {
            // if there is no second word, we don't know where to go...
            System.out.println("Go where?");
            return;
        }

        String direction = command.getSecondWord();

        // Try to leave current room.
        Room nextRoom = currentRoom.getExit(direction);

        if (nextRoom == null) {
            System.out.println("You can't go there.");
        }
        else {
            currentRoom = nextRoom;
            System.out.println(currentRoom.getLongDescription());
        }
    }

	/**
	 * @return room wich player currently occupies
	 */
    public Room getCurrentRoom()
    {
    	return currentRoom;
    }
    
	/**
     * @param room room you want player to occupy
	 */
    public void setCurrentRoom(Room room)
    {
    	currentRoom = room;
    }
    
	/**
	 * @return string consisting of names of keys currently in players bag.
	 */
    private String getBagKeyString()
    {
    	String returnString = "Key:";
    	for (Key key : keyBag) {
    		returnString += " " + key.getName();
    	}
    	return returnString;
    }
    
	/**
     * @param key add key to bag
	 */
    private void addBagKey(Key key)
    {
    	keyBag.add(key);
    }
    
	/**
	 * @return list of items in bag (the only item availible for now is keys)
	 */
    public String getBagItemString()
    {
    	return getBagKeyString();
    }
   
	/**
	 * @param command put key in bag and remove it from room, that is pickup a key.
	 */ 
    public void pickupKey(Command command)
    {
    	String keyName = command.getSecondWord();
    	if (keyName == null)
    		return;
    	
    	Key key = currentRoom.getKey(keyName);
    	if (key == null)
    		return;
    	currentRoom.removeKey(key);
    	addBagKey(key);
    	
    }
    
	/**
	 * @param command remove key from bag and put in room, that is drop a key.
	 */ 
    public void dropKey(Command command)
    {
    	String keyName = command.getSecondWord();
    	if (keyName == null)
    		return;
    	
    	Iterator<Key> it = keyBag.iterator();
    	while (it.hasNext()) {
    		Key currentKey = it.next();
    		if (currentKey.getName().equals(keyName)) {
    			currentRoom.addKey(currentKey);
    			it.remove();
    		}
    	}
    }
    
	/**
	 * @param command unlock door if correct key is in bag
	 */ 
    public void unlockRoom(Command command)
    {
    	String direction = command.getSecondWord();
    	if (direction == null)
    		return;
    	
    	Iterator<Key> it = keyBag.iterator();
    	while (it.hasNext()) {
    		Key currentKey = it.next();
    		if (currentRoom.unlockLock(direction, currentKey)) {
    			it.remove();
    			break;
    		}
    	}
    }
   
	/**
	 *	Load beamer
	 */ 
    public void beamLoad()
    {  	
    	loadedBeamRoom = currentRoom;
    }
    
	/**
	 *	Fire beamer
	 */ 
    public void beamFire()
    {
    	if(loadedBeamRoom != null)
    		currentRoom = loadedBeamRoom;
    }
}
