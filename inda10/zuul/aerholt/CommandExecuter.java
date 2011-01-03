package game;

import java.util.Random;

/**
 * class CommandExecuter Executes the commands sent to it.
 * 
 * @author Anton Erholt
 * @version 2010.11.25
 */
public class CommandExecuter {
	// private Command command;

	private ScreenPrinter screenPrinter = new ScreenPrinter();
	private World world;
	private CommandWords cWords = new CommandWords();
	private Random randGen = new Random();
	private boolean wantToQuit = false;

	/**
	 * Constructor for objects of class CommandExecuter
	 * 
	 * @param world
	 *            The world on which to execute the commands.
	 */
	public CommandExecuter(World world) {
		this.world = world;
	}

	/**
	 * Given a command, process (that is: execute) the command.
	 * 
	 * @param command
	 *            The command to be processed.
	 * @return true If the command ends the game, false otherwise.
	 */
	public boolean runCommand(Command command) {
		String commandWord = command.getCommandWord();

		// TODO Switch? :)

		if (commandWord.equals("help")) {
			printHelp();
		} else if (commandWord.equals("go")) {
			goRoom(command);
		} else if (commandWord.equals("look")) {
			look();
		} else if (commandWord.equals("take")) {
			take(command.getSecondWord());
		} else if (commandWord.equals("inventory")) {
			showInventory();
		} else if (commandWord.equals("use")) {
			use(command.getSecondWord());
		}
		return wantToQuit;
	}

	// implementations of user commands:

	/**
	 * Print out some help information. Here we print some stupid, cryptic
	 * message and a list of the command words.
	 */
	private void printHelp() {
		screenPrinter.println("So, you need help? " + "Remember to 'look' for items.");
		screenPrinter.println("");
		screenPrinter.println("The number commands are used for choices later in the game.");
		screenPrinter.println("Your command words are:");
		showCommands();
	}

	/**
	 * Shows all available commands.
	 */
	private void showCommands() {
		cWords.showAll();
	}

	/**
	 * Try to go to one direction. If there is an exit, enter the new room,
	 * otherwise print an error message. This method takes care of the
	 * event-handling. I.e. it checks if an exit's locked( and lets the player
	 * 'enter the locked room' and get returned to the previous). This could
	 * easier have been done with an event handler added to each room.
	 */
	private void goRoom(Command command) {
		if (!command.hasSecondWord()) {
			// if there is no second word, we don't know where to go...
			screenPrinter.println("Go where?");
			return;
		}

		String direction = command.getSecondWord();
		Room currentRoom = world.player.getLocation();

		// Try to leave current room.
		Room nextRoom = world.player.getLocation().getExit(direction);

		if (nextRoom == null) {
			screenPrinter.println("There is no exit that way!");
			return;
		}

		world.player.setLocation(nextRoom);
		firstEntry();

		if (triesEasyWay()) {
			return;
		}

		// Handle if a room is locked!
		if (world.player.getLocation().getName().equals("locked")) {
			screenPrinter.println("You are "
					+ world.player.getLocation().getShortDescription());
			world.player.setLocation(currentRoom);
			return;
		}

		// Handle the rock,scissor,paper game
		if (world.player.getLocation().getName().equals("stairs")) {
			playMiniGame();
		}

		lowerPlayerHP();
		screenPrinter.println(world.player.getLocation().getLongDescription());

		// Handle if player has won the game.
		if (world.player.getLocation().getName().equals("outside")
				&& world.player.getHealth() < 100) {
			screenPrinter.print("Well done " + world.player.getName()
					+ "! You have won the game! :) ");
			wantToQuit = true;
			return;
		}

		checkPlayerHP();

	}

	/**
	 * Allows the player to play a minigame of rock-paper-scissors versus a ghost.
	 * 
	 */
	private void playMiniGame() {
		
		checkPlayerHP();
		screenPrinter.println("As you ascend, an evil ghost appears before you!");
		screenPrinter.println("");
		screenPrinter.println("	I am the evil ghost!");
		if (!(world.player.inventory.itemExists("rock")
				&& world.player.inventory.itemExists("paper") && world.player.inventory
				.itemExists("scissors"))) {
			screenPrinter.println("	Hrrmpfh! You are not a worthy opponent. "
					+ "Get back and find what you need to face me!");
			screenPrinter.println("You return back down.");
			world.player.setLocation(world.getRoom("hall"));
			return;
		}

		screenPrinter.println("	Very well! "
				+ "We shall play the game. You will bet your life on it!");

		Parser gameParser = new Parser();
		String choice, ghostChoice;

		while (true) {

			screenPrinter.println("Pick:");
			screenPrinter.println(" 1) rock");
			screenPrinter.println(" 2) paper");
			screenPrinter.println(" 3) scissors");
			while(true) {
				choice = gameParser.getCommand().getCommandWord();
				if(choice.equals("1")||choice.endsWith("2")||choice.equals("3")) {
					break;
				}
				screenPrinter.print("Choose a valid number!");
			}
			
			ghostChoice = (randGen.nextInt(3) + 1) + "";

			screenPrinter.println("3...");
			wait(500);
			screenPrinter.println("2...");
			wait(500);
			screenPrinter.println("1...");
			wait(500);
			screenPrinter.println("");
			
			choice = rockPaperOrScissors(choice);
			ghostChoice = rockPaperOrScissors(ghostChoice);
			
			screenPrinter.println("Player: " + choice + ", Ghost: " + ghostChoice);
			screenPrinter.println("");
			
			// Draw
			if (choice.equals(ghostChoice)) {
				screenPrinter.println("	Tie! We play again!");
				continue;
			}

			// Ghost wins
			if ((choice.equals("1") && ghostChoice.equals("2"))
					|| (choice.equals("2") && ghostChoice.equals("3"))
					|| (choice.equals("3") && ghostChoice.equals("1"))) {
				screenPrinter.println("	HAHA! I, the evil ghost won!"
						+ " You will pay with your life!");
				screenPrinter.println("You get punched down the stairs.");
				world.player.setHealth(world.player.getHealth() - 10);
				world.player.setLocation(world.getRoom("hall"));
				break;
			}
			// Player wins
			screenPrinter.println("	Allright! I'll let you through to the attic.");
			screenPrinter.println("	If you wanna get there again. YOU HAVE TO BEAT ME AGAIN!"
					+ " KYAHAHAHA!");

			// Initiate attic
			Room attic = new Room("attic",
					"in the large attic. You see lots and yet lots of rubbish"
							+ " all over the place.");
			attic.setExit("down", world.getRoom("hall"));

			Item key = new Item("key", "a mysterious key");
			attic.items.addItem(key);
			world.player.setLocation(attic);
			break;
		}

	}
	
	/**
	 * Helps the minigame display a correct winning String.
	 * @param choice either "1" ,"2" or "3".
	 */
	private String rockPaperOrScissors(String choice) {
		if(choice.equals("1")) {
			return "rock";
		}
		else if(choice.equals("2")) {
			return "paper";
		}
		return "scissors";
	}
	
	
	/**
	 * Waits for n number of milliseconds before continuing.
	 * @param n number of milliseconds to wait.
	 */
	private void wait(int n) {
		try {
			Thread.sleep(n); // Does nothing for n milliseconds
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * Shows the long description of the players current location.
	 */
	public void look() {
		screenPrinter.println(world.player.getLocation().getLongDescription());
		screenPrinter.println("Items: "
				+ world.player.getLocation().items.getItemNamesString());
	}

	/**
	 * Takes an item from a room and adds the item to the players inventory.
	 * Removes the item from the room.
	 * 
	 * @param itemName
	 *            The name of the item to be taken.
	 */
	public void take(String itemName) {
		if (world.player.getLocation().items.itemExists(itemName)) {
			Room location = world.player.getLocation();
			world.player.inventory.addItem(location.items.getItemByName(itemName));
			location.items.removeItem(location.items.getItemByName(itemName));
			screenPrinter.println("You now carry an item named " + itemName + ".");
			return;
		}
		screenPrinter.println("Take what?");
	}

	/**
	 * Prints the players inventory to screen.
	 */
	public void showInventory() {
		screenPrinter.println(world.player.inventory.getItemNamesString());
	}

	/**
	 * Uses an item in the current room.
	 * 
	 * - Key: Unlocks the door in the hall.
	 * 
	 * @param item
	 *            The name of item to be used
	 */
	public void use(String itemName) {
		if (itemName.equals("key")) {
			if (world.player.getLocation().getName().equals("hall")) {
				world.player.getLocation().setExit("south", world.getRoom("outside"));
				screenPrinter.println("Door unlocked!");
			}
		}
	}

	/**
	 * Checks if the players HP has fallen below 0. If it has, it quits the
	 * game.
	 */
	public void checkPlayerHP() {
		if (world.player.getHealth() < 1) {
			wantToQuit = true;
			screenPrinter.println("Oh dear, you got killed. GAME OVER! KYAHAHAHAHA!");
		}
	}

	/**
	 * Lowers the players HP randomly by 1-5 HP.
	 */
	public void lowerPlayerHP() {
		world.player.setHealth(world.player.getHealth() - (randGen.nextInt(4) + 1));
	}

	// Room events

	/**
	 * Run if player has entered the room for the first time.
	 */
	private void firstEntry() {
		// Handle if player enters the haunted house the first time.
		if (world.player.getLocation().getName().equals("hall")
				&& world.player.getHealth() == 100) {
			screenPrinter.println("The door slams shut behind you.");
			screenPrinter.println("KYAHAHA! I've put a curse on you and locked the door. "
					+ "You'll never get out of here! ]:)");
			screenPrinter.println("");
			screenPrinter.println("You feel a sting going through your body.");
		}
	}

	/**
	 * Handle if the player tries to run away from the haunted house.
	 * 
	 * @return true if the player tries to run away from the haunted house.
	 *         Returns false otherwise.
	 */
	public boolean triesEasyWay() {
		if (world.player.getLocation().getName().equals("road")) {
			screenPrinter.println(world.player.getLocation().getLongDescription());
			screenPrinter.println("You shouldn't have done that...");
			screenPrinter.println("SLAM! A truck runs you over.");
			world.player.setHealth(0);
			wantToQuit = true;
			return true;
		}
		return false;
	}

}
