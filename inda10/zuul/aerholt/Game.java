package game;

/**
 * This class is the main class of the "Haunted house of Zuul" application.
 * "Haunted house of Zuul" is a very simple, text based adventure game. You get trapped
 * in a spooky house and need to get out before the curse has drained out your
 * life!
 * 
 * To play this game, create an instance of this class and call the "play"
 * method or call the main method.
 * 
 * This main class creates and initializes all the others: it creates the world,
 * creates the parser and starts the game. It also evaluates and executes the
 * commands that the parser returns.
 * 
 * @author Michael Kolling and David J. Barnes, modified heavily by Anton Erholt
 * @version 2010.11.23
 */
public class Game {
	private Parser parser;
	private World world;
	private CommandExecuter commandExecuter;
	private ScreenPrinter screenPrinter = new ScreenPrinter();
	public static boolean DEBUGGING = false; // gives player some items and puts him in hall

	/**
	 * The awesome main method. Ensures you can run the game from out of the
	 * BlueJ/Eclipse environment.
	 */
	public static void main(String[] args) {
		Game g = new Game();
		g.play();
	}

	/**
	 * Create the game and initialize the world as well as the player data.
	 */
	public Game() {
		parser = new Parser();

		screenPrinter.print("Please enter your name:");
		String playerName = parser.getPlayerName();
		world = new World(playerName);
		commandExecuter = new CommandExecuter(world);
	}

	/**
	 * Main play routine. Loops until end of play.
	 */
	public void play() {
		printWelcome();
		// Enter the main command loop. Here we repeatedly read commands and
		// execute them until the game is over.

		while (true) {
			boolean wantToQuit = false;
			// print the prompt
			screenPrinter.print(world.player.getName() + "@"
					+ world.player.getLocation().getName() + " : "
					+ world.player.getHealth() + " hp> ");
			Command command = parser.getCommand();
			if (command.isUnknown()) {
				screenPrinter.println("Unknown command! Type 'help' for more info.");
				continue;
			}
			if (command.getCommandWord().equals("quit")) {
				break;
			}
			wantToQuit = commandExecuter.runCommand(command);
			if (wantToQuit == true) {
				break;
			}

		}
		screenPrinter.println("Thank you for playing.  Good bye.");
	}

	/**
	 * Print out the opening message for the player.
	 */
	private void printWelcome() {
		screenPrinter.println("");
		screenPrinter.println("Welcome to the haunted house of Zuul!");
		screenPrinter.println("The haunted house of Zuul is a new, incredibly awesome adventure game.");
		screenPrinter.println("If you ever need help, type 'help'.");		
		screenPrinter.println("");
		screenPrinter.println("Type 'look' to begin exploring.");
	}

}
