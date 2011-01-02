import java.util.ArrayList;

/**
 *  This class is the main class of the "The Adventures of Hans & Greta" application. 
 *  "The Adventures of Hans & Greta" is a very simple, text based adventure game. 
 * 
 *  This main class creates and initialises all the others: it creates all
 *  rooms, creates the parser and starts the game.  It also evaluates and
 *  executes the commands that the parser returns.
 * 
 * @author  Michael Kolling, David J. Barnes and Emil Berg
 * @version 1.00
 */

public class Game 
{
    /**
     * main method. Starting point for the whole game.
     */
    public static void main(String[] args) {
        Game game = new Game();
        game.play();
    }

    private Parser parser;
    // The player controlled by the user.
    private Player player;
        
    /**
     * Create the game and initialise its internal map. Give the game a player to control.
     */
    public Game() 
    {
        parser = new Parser();
        player = new Player("Erik", 3);
        createRooms();
    }

    /**
     * Create all the rooms and link their exits together.
     */
    private void createRooms()
    {
        Room outside, theatre, pub, lab, office, hell, win;
        
        KeyItem key = new KeyItem("Chocolate-key", "A key made of belgian chocolate.", "It melts in the lock.");
        FoodItem key2 = new FoodItem("Jelly-key", "A key made of red jelly.", "It tastes awesome. You can feel som energy returning.", 2);
        SuicideItem rope = new SuicideItem("Caramel-rope", "A red- and white colored caramel rope.", "You hang the rope in the ceiling and hang yourself.");
        SuicideItem pickaxe = new SuicideItem("Toffee-pickaxe", "A pickaxe made entirely of toffee.", "You impale your own head with the pickaxe.");
        SuicideItem gun = new SuicideItem("Liquorice-shotgun", "The Terminator 2 shotgun made by tasty liqurice.", "You put the gun towards your head and fire.");
        
        // create the rooms
        outside = new Room("outside the main entrance of the university");
        theatre = new Room("in a lecture theatre. You can spot a hatch in the ceiling");
        pub = new Room("in the campus pub");
        lab = new Room("in a computing lab");
        office = new Room("in the computing admin office");
        hell = new Room("in hell. The devil obstructs the north doorway");
        win = new Room("in win room");
        
        // initialise room exits
        outside.setExit(Room.ExitDirection.EAST, theatre, key, "");
        outside.setExit(Room.ExitDirection.SOUTH, lab);
        outside.setExit(Room.ExitDirection.WEST, pub, null, "You can hear the cave behind you collapse.");

        theatre.setExit(Room.ExitDirection.WEST, outside);

        lab.setExit(Room.ExitDirection.NORTH, outside);
        lab.setExit(Room.ExitDirection.EAST, office);

        office.setExit(Room.ExitDirection.WEST, lab);
        
        theatre.setExit(Room.ExitDirection.EAST, hell, null, "Flames cover the exit behind you. You can't head back.");
        
        hell.setExit(Room.ExitDirection.NORTH, win);
        
        outside.addItem(key);
        outside.addItem(key2);
        outside.addItem(rope);
        pub.addItem(pickaxe);
        theatre.addItem(gun);

        player.teleport(outside);
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
            Command command = parser.getCommand();
            if (command == null) {
                System.out.println("I don't know what you mean...");
            } else {
                finished = command.apply(player);
            }
        }
        System.out.println("Thank you for playing. Good bye.");
    }

    /**
     * Print out the opening message for the player.
     */
    private void printWelcome()
    {
        System.out.println();
        System.out.println("Welcome to The Adventures of Hans & Greta!");
        System.out.println("The adventures of Hans & Greta is a new, incredibly old adventure game.");
        System.out.println("Type 'help' if you need help.");
        System.out.println();
        System.out.println(player.getRoom().getLongDescription());
    }
}
