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
 * Incorporates a transporter room, a one-way trap door, a chargeable beamer and a timelimit based on moves.
 * 
 * @author  Michael Kolling and David J. Barnes
 * @version 2008.03.30
 */
import java.util.*;

public class Game 
{
    private Parser parser;
    private Room currentRoom;
    private Room beamerPosition;
    private ArrayList<Room> existingRooms;
        
    /**
     * Create the game and initialise its internal map.
     */
    public Game() 
    {
        createRooms();
        parser = new Parser();
        
    }

    /**
     * Create all the rooms and link their exits together.
     */
    private void createRooms()
    {
        Room outside, theatre, pub, lab, office, lost;
      
        // create the rooms
        outside = new Room("outside the main entrance of the university");
        theatre = new Room("theatre");
        pub = new Room("in the campus pub");
        lab = new Room("in a computing lab");
        office = new Room("in the computing admin office");
        lost = new Room("utterly lost - because there is always a back entrance to a pub, \n but most of the time you have no idea where it leads to.\n \n ...or how to get out of there.");
        
        existingRooms = new ArrayList<Room>();
        existingRooms.add(outside);
        existingRooms.add(theatre);
        existingRooms.add(pub);
        existingRooms.add(lab);
        existingRooms.add(office);
        
        // initialise room exits
        outside.setExit("east", theatre);
        outside.setExit("south", lab);
        outside.setExit("west", pub);

        theatre.setExit("west", outside); // this room has been remodeled into a warpzone/transporter room

        pub.setExit("east", outside);
        pub.setExit("west", lost);  // oneway door... with no other doors to choose from. :3 beamer charged?

        lab.setExit("north", outside);
        lab.setExit("east", office);

        office.setExit("west", lab);

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
        int amountOfMovesConsumed = 0; // timelimit added.
        int amountOfMovesAllowed = 13;
        while (! finished) {
            Command command = parser.getCommand();
            finished = processCommand(command);
            if(amountOfMovesConsumed == amountOfMovesAllowed) //test if timelimit is up
            {
                System.out.println("You are now so late and confused that you simply give up.");
                finished = true;
            }
            amountOfMovesConsumed ++;
        }
        System.out.println("Thank you for wasting a few pointless moments of your life in Zuul.  Good bye.");
    }

    /**
     * Print out the opening message for the player.
     */
    private void printWelcome()
    {
        System.out.println();
        System.out.println("Welcome to the World of Zuul!");
        System.out.println("World of Zuul is a new, incredibly pointless adventure game. \n Don't forget to charge your beamer.");
        System.out.println("Type '" + CommandWord.HELP + "' if you need help.");
        System.out.println();
        System.out.println(currentRoom.getLongDescription());
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
            System.out.println("Sure you are tired, but speak some sense why don't you?");
            return false;
        }

        if (commandWord == CommandWord.HELP) {
            printHelp();
        }
        else if (commandWord == CommandWord.GO) {
            goRoom(command);
        }
        else if (commandWord == CommandWord.QUIT) {
            wantToQuit = quit(command);
        }
        else if (commandWord == CommandWord.CHARGE) { //set the beamer
            beamerPosition = currentRoom;
        }
        else if (commandWord == CommandWord.FIRE) { //use beamer
            if (beamerPosition == null)
            {
                System.out.println("You have to charge the beamer for it to work, you know."); //in case you forgot to charge it
            }
            else{
                beamerRoom();
            }
        }
        // else command not recognised.
        return wantToQuit;
    }

    // implementations of user commands:

    /**
     * Print out some help information.
     * Here we print some stupid, cryptic message and a list of the 
     * command words.
     */
    private void printHelp() 
    {
        System.out.println("You are lost. You are alone. (You have no use for a story.)");
        System.out.println(" You wander around at the university.");
        System.out.println();
        System.out.println("To help you get out of tricky situations ");
        System.out.println("you have brought your trusty beamer.");
        System.out.println("Don't forget to charge it!");
        System.out.println();
        System.out.println("Your command words are:");
        parser.showCommands();
    }

    /** 
     * Try to go to one direction. If there is an exit, enter the new
     * room, otherwise print an error message.
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
        Room nextRoom = currentRoom.getExit(direction);
        

        if (nextRoom == null) {
            System.out.println("The door is a lie!");
        }
        else {
            if(nextRoom.getShortDescription().equals("theatre")) // transporter room defined by the shortdescription
            {
                System.out.println("You passed trough a lecture theatre but the advanced metaphysics class made you" +"\n" +"so confused that you ended up somwhere else entirely");
                Random x = new Random();
                nextRoom = existingRooms.get(x.nextInt(existingRooms.size())); // checks the nr of defined rooms (exclusive), randomize one of them and change your location to said place 
            }
            currentRoom = nextRoom;
            System.out.println(currentRoom.getLongDescription());
        }
    }
    /** 
     * Redirects the position of the player to the place where you last charged the beamer tool.
     * 
     */
    private void beamerRoom() 
    {
        currentRoom = beamerPosition;
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
