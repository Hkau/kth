package game;

import java.util.Set;
import java.util.HashMap;

/**
 * class Room
 * 
 * A "Room" represents one location in the scenery of the game. It is connected
 * to other rooms via exits. For each existing exit, the room stores a reference
 * to the neighboring room. Also keeps track of the items in the room with an
 * ItemCollection.
 * 
 * @author Michael Kolling and David J. Barnes, modified heavily by Anton Erholt
 * @version 2010.11.24
 */

public class Room {
	private String description;
	private HashMap<String, Room> exits; // stores exits of this room.
	private String name;
	public ItemCollection items;

	/**
	 * Create a room described "description". Initially, it has no exits.
	 * "description" is something like "a kitchen" or "an open court yard".
	 * 
	 * @param description
	 *            The room's description.
	 */
	public Room(String name, String description) {
		this.description = description;
		this.name = name;
		exits = new HashMap<String, Room>();
		items = new ItemCollection();
	}

	/**
	 * Returns the name of the room.
	 */
	public String getName() {
		return name;
	}

	/**
	 * Define an exit from this room.
	 * 
	 * @param direction
	 *            The direction of the exit.
	 * @param neighbor
	 *            The room to which the exit leads.
	 */
	public void setExit(String direction, Room neighbor) {
		exits.put(direction, neighbor);
	}

	/**
	 * @return The short description of the room (the one that was defined in
	 *         the constructor).
	 */
	public String getShortDescription() {
		return description;
	}

	/**
	 * Return a description of the room in the form: You are in the kitchen.
	 * Exits: north west
	 * 
	 * @return A long description of this room
	 */
	public String getLongDescription() {
		return "You are " + description + "\n" + getExitString();
	}

	/**
	 * Return a string describing the room's exits, for example
	 * "Exits: north west". Returns an empty String if the exits doesn't exist.
	 * 
	 * @return Details of the room's exits.
	 */
	private String getExitString() {
		String returnString = "Exits:";
		Set<String> keys = exits.keySet();
		
		if(keys.isEmpty()) {
			return "";
		}
		
		for (String exit : keys) {
			returnString += " " + exit;
		}
		return returnString;
	}

	/**
	 * Return the room that is reached if we go from this room in direction
	 * "direction". If there is no room in that direction, return null.
	 * 
	 * @param direction
	 *            The exit's direction.
	 * @return The room in the given direction.
	 */
	public Room getExit(String direction) {
		return exits.get(direction);
	}
}
