/**
 *  This class is the main class of the "World of Zuul" application. 
 *  "World of Zuul" is a simple, text based adventure game. The player 
 *  can walk around some scenery, until the time limit is up. Unless he is
 *  trapped, of course.
 * 
 *  To play this game, create an instance of this class and call the "play"
 *  method.
 * 
 *  This main class creates and initialises all the others: it creates all
 *  rooms, creates the parser and starts the game.  It also evaluates and
 *  executes the commands that the parser returns.
 * 
 * @author  Michael Kolling, David J. Barnes and Vladimir Grozman
 * @version 2010.12.02
 */

import java.util.Random;

public class Game 
{
    private Parser parser;
    private Room currentRoom, beamTo;
    public static final int TIME_LIMIT = 10;
    private int currentTime;
    
    public static void main(String[] args) {
        (new Game()).play();
    }
        
    /**
     * Create the game and initialise its internal map.
     */
    public Game() 
    {
        createRooms();
        parser = new Parser();
        currentTime = 0;
    }

    /**
     * Create all the rooms and link their exits together.
     */
    private void createRooms()
    {
        Room outside, theatre, pub, lab, office;
      
        // create the rooms
        outside = new Room("outside the main entrance of the university");
        theatre = new Room("in a lecture theatre");
        pub = new Room("in the campus pub");
        lab = new Room("in a computing lab");
        office = new Room("in the computing admin office");
        
        // initialise room exits
        outside.setExit("east", theatre, false);
        outside.setExit("south", lab, false);
        outside.setExit("west", pub, true);

        //theatre.setExit("west", outside, false);

        pub.setExit("east", outside, false);

        lab.setExit("north", outside, false);
        lab.setExit("east", office, false);

        office.setExit("west", lab, false);

        currentRoom = outside;  // start game outside
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
            finished = processCommand(command);
        }
        System.out.println("Thank you for playing.  Good bye.");
    }

    /**
     * Print out the opening message for the player.
     */
    private void printWelcome()
    {
        System.out.println();
        System.out.println("Welcome to the World of Zuul!");
        System.out.println("World of Zuul is an old, incredibly boring adventure game.");
        System.out.println("You need to complete your (non-existant) mission in " + TIME_LIMIT + " turns.");
        System.out.println("Type 'help' if you need help.");
        System.out.println();
        System.out.println(currentRoom.getLongDescription());
    }

    /**
     * Given a command, process (that is: execute) the command.
     * @param command The command to be processed.
     * @return true If the command ends the game, false otherwise.
     */
    private boolean processCommand(Command command) {
        if(command.isUnknown()) {
            System.out.println("I don't know what you mean...");
            return false;
        }

        String commandWord = command.getCommandWord();
        if (commandWord.equals("help")) {
            printHelp();
        }
        else if (commandWord.equals("go")) {
            if(tryToGo(command) && ++currentTime >= TIME_LIMIT) {
                System.out.println("THE TIME IS UP, SLAVE!!1mouahahaha");
                return true;
            }
        }
        else if (commandWord.equals("chargebeamer")) {
            if(beamTo == null) {
                beamTo = currentRoom;
                System.out.println("You have successfully charged the beamer.");
            }
            else System.out.println("You must not overcharge the beamer");
        }
        else if (commandWord.equals("firebeamer")) {
            if(beamTo == null) System.out.println("The beamer emits a quirky noise.");
            else {
                goRoom(beamTo);
                beamTo = null;
            }
        }
        else if (commandWord.equals("quit")) {
            return quit(command);
        }
        // else command not recognised.
        return false;
    }

    /**
     * Tries to execute the "go" command.
     * @param command The "go" command.
     * @return Whether the room has been entered successfully
     */
    private boolean tryToGo(Command command) {
        if(!command.hasSecondWord()) {
            System.out.println("Go where?"); // if there is no second word, we don't know where to go...
            return false;
        }
        
        Room nextRoom = currentRoom.getExit(command.getSecondWord());
        if(nextRoom == null) {
            System.out.println("There is no door!");
            return false;
        }
        
        if(currentRoom.isExitLocked(command.getSecondWord())) {
            System.out.println("This door is locked. To obtain a key, you need to solve");
            System.out.println("a difficult differential equation.");
            Random rnd = new Random();
            int a = rnd.nextInt(10) + 1, b = rnd.nextInt(10) + 1, c = 0;
            System.out.println("What is " + a + " + " + b + "?");
            try {c = Integer.parseInt(parser.getCommand().getSecondWord());}
            catch(NumberFormatException e) {}
            if(c == a + b) {
                currentRoom.unlockExit(command.getSecondWord());
                System.out.println("You have solved the difficult differential equation and enter the room.");
            }
            else {
                System.out.println("You are not worthy of entering this room.");
                return false;
            }
        }
        
        goRoom(nextRoom);
        return true;
    }

    /**
     * Print out some help information.
     * Here we print some stupid, cryptic message and a list of the 
     * command words.
     */
    private void printHelp() 
    {
        System.out.println("You are lost. You are alone. You wander");
        System.out.println("around at the university.");
        System.out.println();
        System.out.println("Your command words are:");
        parser.showCommands();
    }

    /** 
     * Try to go to one direction. If there is an exit, enter the new
     * room, otherwise print an error message. If the time limit is
     * reaced, exit game.
     */
    private void goRoom(Room nextRoom) 
    {
        currentRoom = nextRoom;
        System.out.println(currentRoom.getLongDescription());
    }

    /** 
     * "Quit" was entered. Check the rest of the command to see
     * whether we really quit the game.
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
