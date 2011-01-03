package game;

import java.util.ArrayList;

/**
 * class World ¨
 * Keeps track of the haunted house of Zuul.
 *  - player
 *  - rooms 
 *  - items
 * 
 * @author Anton Erholt
 * @version 2010.11.24
 */
public class World {
	public Player player;
	private Room playerStartingLocation;
	private ArrayList<Room> rooms = new ArrayList<Room>();

	/**
	 * Constructor for World. Initiates the rooms and the player.
	 */
	public World(String playerName) {
		createRooms();
		player = new Player(playerName, playerStartingLocation);

		// For easy debugging of the rock,paper & scissors session
		if (Game.DEBUGGING) {
			Item rock, scissors, paper;

			rock = new Item("rock", "a fine rock");
			paper = new Item("paper", "the finest toiletpaper around");
			scissors = new Item("scissors", "a rugged pair of scissors");

			player.inventory.addItem(rock);
			player.inventory.addItem(paper);
			player.inventory.addItem(scissors);
			player.setLocation(rooms.get(2)); // sets location to hall
		}
	}

	
	
	/**
	 * Create all the rooms and link their exits together. Sets the player's
	 * startingLocation to outside.
	 */
	private void createRooms() {
		Room road, outside, hall, kitchen, livingRoom, bedRoom, hallway, toilet, locked, stairs;

		// create the rooms
		stairs = new Room("stairs", "climbing upstairs.");
		road = new Room("road", "trying to sneak away from the haunted house!");
		outside = new Room("outside",
				"outside, in front of the haunted house. A vast road lies to the south.");
		hall = new Room("hall", "in the hall of the house."
				+ " You see a stairway"
				+ " leading to the upper floor on your right."
				+ " There's a large room up ahead.");
		hallway = new Room("hallway",
				"in the hallway. Large portraits of weird people are staring at you! ");
		livingRoom = new Room("living room",
				"in the livingroom. A large table stands in the "
						+ "middle of the room.");
		kitchen = new Room("kitchen",
				"in the kitchen. There seems to be some noise coming from the "
						+ "fridge. It seems jammed.");
		bedRoom = new Room("bedroom",
				"in the bedroom. It's dark and cold in here.");
		toilet = new Room("toilet", "in the toilet. It's really messy. "
				+ "Dirt all over the floor and a rusty sink.");

		locked = new Room("locked", "stuck here! It's locked!");

		// initialize room exits

		outside.setExit("north", hall);
		outside.setExit("south", road);

		hall.setExit("up", stairs);
		hall.setExit("south", locked);
		hall.setExit("north", hallway);

		hallway.setExit("south", hall);
		hallway.setExit("north", livingRoom);

		livingRoom.setExit("south", hallway);
		livingRoom.setExit("east", kitchen);
		livingRoom.setExit("west", bedRoom);

		bedRoom.setExit("south", toilet);
		bedRoom.setExit("east", livingRoom);

		toilet.setExit("north", bedRoom);

		kitchen.setExit("west", livingRoom);

		// initialize room items
		Item rock, scissors, paper;

		rock = new Item("rock", "a fine rock");
		paper = new Item("paper", "the finest toiletpaper around");
		scissors = new Item("scissors", "a rugged pair of scissors");

		hallway.items.addItem(rock);
		toilet.items.addItem(paper);
		kitchen.items.addItem(scissors);

		// player starting location
		playerStartingLocation = outside;

		// append rooms to 'rooms'
		rooms.add(outside);
		rooms.add(bedRoom);
		rooms.add(hall);
		rooms.add(hallway);
		rooms.add(livingRoom);
		rooms.add(kitchen);
		rooms.add(toilet);
		rooms.add(road);
	}

	/**
	 * Gets a room by its name.
	 * 
	 * @param roomName
	 *            The name of the room.
	 * @return the wanted room. Returns null if non-existent.
	 */
	public Room getRoom(String roomName) {
		for (Room room : rooms) {
			if (room.getName().equals(roomName)) {
				return room;
			}
		}
		return null;
	}

}
