import java.util.ArrayList;
import java.util.Random;
/**
 *  This class is the main class of the "World of Zuul" application. 
 *  "World of Zuul" is a very simple, text based adventure game.  Users 
 *  can walk around some scenery. That's all. It should really be extended 
 *  to make it more interesting!
 * 
 *  To play this game, create an instance of this class and call the "play"
 *  method.
 * 
 *  This main class creates and initialises all the others: it creates all
 *  rooms, creates the parser and starts the game.  It also evaluates and
 *  executes the commands that the parser returns.
 * 
 * @author  Michael Kolling and David J. Barnes
 * @version 2008.03.30
 */

public class Game 
{
	public static void main (String[] args) { 
		Game obj = new Game();
		obj.play();
	}
	
	private Parser parser;
	private Player player;
	private ArrayList<Room> transpirationRooms;



	/**
	 * Create the game and initialise its internal map.
	 */
	public Game() 
	{
		transpirationRooms = new ArrayList<Room>();
		createWorld();
		parser = new Parser();
		
	}

	/**
	 * Create all the rooms and link their exits together.
	 */
	private void createWorld()
	{
		Room outside, theatre, pub, lab, office, teleportRoom;


		
		// create the rooms
		outside = new Room("outside the main entrance of the university");
		theatre = new Room("in a lecture theatre");
		pub = new Room("in the campus pub");
		lab = new Room("in a computing lab");
		office = new Room("in the computing admin office");
		teleportRoom = new Room("Test");
		
		// Enter the rooms into a list
		transpirationRooms.add(pub);
		transpirationRooms.add(theatre);
		transpirationRooms.add(outside);
		transpirationRooms.add(lab);
		transpirationRooms.add(office);
		
		teleportRoom.setTeleportation(true);
		
		outside.setExits("east", theatre);
		outside.setExits("south", lab);
		outside.setExits("west", pub);	
		outside.addItem("Statue", "a great statue of some dude", 102);
		outside.getItem("Statue").setLootable(false);
		outside.addItem("Bindstone", "A magic stone that have great teleportation skills!", 1);
		outside.getItem("Bindstone").setSpecialEffect("beamer");
		outside.addNpc("Stranger", "Hello there mate, could you spare me some coins?");
		

		theatre.setExits("west", outside);
		
		pub.setExits("east", outside);
		pub.setExits("secretdoor", teleportRoom);
		pub.lockExit("east", true);
		pub.addItem("cookie", "A magic cookie!", 10);
		pub.getItem("cookie").setEatable(true);
		pub.getItem("cookie").setSpecialEffect("cookie");

		lab.addItem("Coins", "a bag of some coins... maybe fit for a stranger?", 10);
		lab.getItem("Coins").setSpecialEffect("coinstostranger");
		lab.getItem("Coins").setRestrictedRoom(outside);
		lab.setExits("north", outside);
		lab.setExits("east", office);
		lab.addItem("Comp1", "a computer", 25);
		lab.addItem("Comp2", "another computer", 35);
		office.addItem("Key", "a key that fits to the door in the pub!", 5);
		office.getItem("Key").setSpecialEffect("openeastexit");
		office.getItem("Key").setRestrictedRoom(pub);
		office.setExits("west", lab);


		//Player is set outside
		player = new Player(outside);

		
	}

	/**
	 *  Main play routine.  Loops until end of play.
	 */
	public void play() 
	{            
		printWelcome();

		// Enter the main command loop.  Here we repeatedly read commands and
		// execute them until the game is over.

		boolean finished = false;
		while (! finished) {
			//Checks if the player is out of rounds!
			if (player.getRoundsLeft()<= 0) {
				System.out.println("You are out of rounds and are forced to quit the game!");
				break;
			}
			Command command = parser.getCommand();
			finished = processCommand(command);
		}

		System.out.println("Thank you for playing.  Good bye.");
	}

	/**
	 * Prints the Welcome messeges
	 */
	private void printWelcome()
	{
		System.out.println();
		System.out.println("Welcome to the World of Zuul!");
		System.out.println("World of Zuul is a new, incredibly boring adventure game.");
		System.out.println("You have " + player.getRoundsLeft() + " rounds to complete the game.");
		System.out.println("Type '" + CommandWord.HELP + "' if you need help.");
		System.out.println();
		System.out.println(player.getCurrentRoom().getLongDescription());
		System.out.println();

	}

	/**
	 * Given a command, process (that is: execute) the command.
	 * @param command The command to be processed.
	 * @return true If the command ends the game, false otherwise.
	 */
	private boolean processCommand(Command command) 
	{
		boolean wantToQuit = false;
		CommandWord commandWord = command.getCommandWord();

		if(commandWord == CommandWord.UNKNOWN) {
			System.out.println("I don't know what you mean...");
			return false;
		}

		if (commandWord == CommandWord.HELP)
			printHelp();
		else if (commandWord == CommandWord.GO)
			goRoom(command);
		else if (commandWord == CommandWord.LOOK)
			look();
		else if (commandWord == CommandWord.TAKE)
			take(command);
		else if (commandWord == CommandWord.DROP)
			drop(command);
		else if (commandWord == CommandWord.USE)
			use(command);
		else if (commandWord == CommandWord.EAT)
			eat(command);
		else if (commandWord == CommandWord.INVENTORY)
			showInventory();
		else if (commandWord == CommandWord.BACK)
			goBack();
		else if (commandWord == CommandWord.CHARGEBEAMER)
			chargeBeamer();
		else if (commandWord == CommandWord.USEBEAMER)
			use(new Command(commandWord, "Bindstone"));   
		else if (commandWord == CommandWord.QUIT)
			wantToQuit = quit(command);

		return wantToQuit;
	}
	/**
	 * Print out some help information.
	 * Some basic information, number of rounds left to play and valid commands.
	 */
	private void printHelp() 
	{
		System.out.println("You are lost. You are alone. You wander");
		System.out.println("around at the university.");
		System.out.println();
		System.out.println("You currently got " + player.getRoundsLeft() + " number of rounds left before the game is over!");
		System.out.println("Your command words are:");
		System.out.println(parser.getAllCommands());
	}
	/**
	 * Charges the beamer.
	 * If the player has the beamer it saves the current location for later teleportation.
	 */
	private void chargeBeamer() {
		if (!player.isInIventory("Bindstone")) {
			System.out.println("You dont have the bindstone!");
			return;
		}
		player.setSafeRoom(player.getCurrentRoom());
		System.out.println("Your stone is now bound to this room!");


	}
	/**
	 * Uses an item in the users inventory.
	 * 
	 * @param useableItem The item to be used
	 */
	private void use(Command useableItem) {
		//Check if the player has chosen  an item.
		if(!useableItem.hasSecondWord()) {
			System.out.println("Use what?");
			return;
		}
		String item = useableItem.getSecondWord();  
		//Check if the item exist in the inventory
		if (!player.isInIventory(item)) {
			System.out.println("You dont have this item!");
			return;
		}
		//Checks if the item has a magical effect.
		if (player.getItem(item).getSpecialEffect()!=null)
		{
			//Checks if the item can be used in the current location
			if (player.getItem(item).getRestrictedRoom()== player.getCurrentRoom())
			{
				//Check if the items special effect is to open the east exit
				if (player.getItem(item).getSpecialEffect().equals("openeastexit"))
				{
					player.getCurrentRoom().lockExit("east", false);
					System.out.println("You open the east exit from the pub!");
					return;
				}
				//Check if the items special effect is to be given to the stranger
				else if (player.getItem(item).getSpecialEffect().equals("coinstostranger"))
				{
					player.getCurrentRoom().getNpc("Stranger").addItemToInventory(player.getItem(item));
					player.removeItemFromInventory(item);
					player.getCurrentRoom().getNpc("Stranger").setGreeting("*music playing* and party and party and party and party and party  *stranger dancing*");
					System.out.println("Stranger: 'Thanks for the coins! You know what you should do? Go to the pub and enter a secret door - What you will find in there is awesome!");
					return;
				}
				
			}
			//Checks if the items is the beamer.
			else if (player.getItem(item).getSpecialEffect().equals("beamer")) {
				//If so check if a safe room (the room to be beamed) to exists
				if (player.getSafeRoom()==null) {
					System.out.println("You have not bound your stone to any room!");
					return;
				}
				player.useBeamer();
				System.out.println("You have successfully used your " + item + "!");
				System.out.println(player.getCurrentRoom().getLongDescription());
				return;

			}

		}
	


		System.out.println("You use the item, but nothing seams to happen!");

	}
	/**
	 * To pick up an item from a room
	 * @param command The item to be picked up
	 */
	private void take(Command command) {
		if(!command.hasSecondWord()) {
			System.out.println("Take what?");
			return;
		}

		String item = command.getSecondWord();

		Room currentRoom = player.getCurrentRoom();
		Item itemToBePicked = currentRoom.getItem(item);
		//Check if the item exists
		if (itemToBePicked==null) {
			System.out.println("This item doesn’t exist in this room");
			return;
		}
		//Check if items is allowd to be looted
		if (!currentRoom.isItemLootable(itemToBePicked)) {
			System.out.println("You are not allowed to take this item");
			return;
		}
		if (!player.isAbleToCarry(currentRoom.getItem(item)))
		{
			System.out.println("This item is to heavey for you to pick up!");
			return;
		}

		currentRoom.removeItem(item);
		player.addItemToInventory(itemToBePicked);

		System.out.println("You successfully looted " + item);
	}
	/**
	 * Drops an item from the players inventory in the current room
	 * @param command The item that should be dropped
	 */
	private void drop(Command command) {
		if(!command.hasSecondWord()) {
			System.out.println("Drop what?");
			return;
		} 
		String item = command.getSecondWord();
		Room currentRoom = player.getCurrentRoom();
		//Check if the item exists
		if (!player.isInIventory(item)) {
			System.out.println("You dont have this item!");
			return;
		}
		currentRoom.addItem(player.getItem(item));
		player.removeItemFromInventory(item);
		System.out.println("You successfully dropped " + item);

	}
	/**
	 * Displays the players inventory
	 */
	private void showInventory() {
		System.out.println(player.getInventoryString());
	}
	/**
	 * Eat a object in the players inventory
	 * @param command An item to be eaten
	 */
	private void eat(Command command) {
		//Checks if there is a second word or not!
		if(!command.hasSecondWord()) {
			System.out.println("Eat what?");
			return;
		}
		String item = command.getSecondWord();
		//Check if the player has this item
		if (!player.isInIventory(item)) {
			System.out.println("You don’t have this item!");
			return;
		}
		//Check if the player is allowed to eat this item
		if (!player.getItem(item).isEatable()){
			System.out.println("You cant eat this item!");
			return;
		}
		//Check if the item has a special effect
		if (player.getItem(item).getSpecialEffect()!=null)
		{
			//If the specialleffect is "cookie", then the players strength increase
			if (player.getItem(item).getSpecialEffect().equals("cookie"))
			{
				player.increaseStrength(20);
				player.removeItemFromInventory(item);
				System.out.println("You eat a magic cookie and feel much stronger!");

				return;
			}
		}
		//If the player sucsesfully eat a item which hasent a magic effect
		System.out.println("You have eaten now and your tummy is full!");

	}
	/**
	 * Looks around in the room
	 */
	private void look() {
		System.out.println(player.getCurrentRoom().getLongDescription());
		System.out.println();
	}
	/**
	 * Moves the player backwards.
	 */
	private void goBack() {

		if (player.moveToLastRoom())
		{         
			System.out.println(player.getCurrentRoom().getLongDescription());
		}
		else
		{
			System.out.println("You cant go back from here");
		}
	}

	/** 
	 * Try to go to one direction. If there is an exit, enter
	 * the new room, otherwise print an error message.
	 * @param coomand The chossen exit 
	 */
	private void goRoom(Command command) 
	{
		if(!command.hasSecondWord()) {
			// if there is no second word, we don't know where to go...
			System.out.println("Go where?");
			return;
		}

		String direction = command.getSecondWord();

		// Try to leave current room.
		Room roomToLeave = player.getCurrentRoom();
		Room nextRoom = roomToLeave.getExit(direction);


		if (nextRoom == null) {
			System.out.println("There is no exit!");
		}
		else if (roomToLeave.isLocked(direction))
		{
			System.out.println("That exit is locked!");
		}
		else {
			player.movePlayer(nextRoom);
			
			if (nextRoom.isLocked(nextRoom.getConnectionExit(roomToLeave)))
				System.out.println("The exit you just passed was locked behind you!");
			if (player.getCurrentRoom().getTeleportaion())
			{
				Random rand = new Random();
				player.teleport(transpirationRooms.get(rand.nextInt(transpirationRooms.size())));
				System.out.println("The room you entered had a teleportationfield and you are n... blub!");
			}
			
			System.out.println(player.getCurrentRoom().getLongDescription());
			System.out.println();
		}
	}

	/** 
	 * "Quit" was entered. Check the rest of the command to see
	 * whether we really quit the game.
	 * @param Command the Quit command.
	 * @return true, if this command quits the game, false otherwise.
	 */
	private boolean quit(Command command) 
	{
		if(command.hasSecondWord()) {
			System.out.println("Quit what?");
			return false;
		}
		else {
			return true;  // signal that we want to quit
		}
	}
}
