import java.util.ArrayList;
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
    private Parser parser;
    private Room currentRoom;
    private ArrayList<Character> chars;
    private int time, characters;
    private Room savedRoom;
    private Room outside, theatre, pub, lab, office, storage, ventilation, cafe, death;
        
    /**
     * Create the game and initialise its internal map.
     */
    public Game() 
    {
        createRooms();
        chars = new ArrayList<Character>();
        createCharacter();
        parser = new Parser();
    }

    /**
     * Create all the rooms and link their exits together.
     */
    private void createRooms()
    {
      
        // create the rooms
        outside = new Room("You're outside the main entrance of the university");
        theatre = new Room("You're in a lecture theatre");
        pub = new Room("You're in the campus pub");
        lab = new Room("You're in a computing lab");
        office = new Room("You're in the computing admin office");
        storage = new Room("You're the door deadlocked when you entered the storageroom");
        ventilation = new Room("You crawl into the ventilation");
        cafe = new Room("You're in a small café");
        death = new Room("You ended up in the room of death, here you WILL DIE");
        
        
        // initialise room exits
        outside.setExit("east", theatre);
        outside.setExit("south", lab);
        outside.setExit("west", pub);

        theatre.setExit("west", outside);

        pub.setExit("east", outside);

        lab.setExit("north", outside);
        lab.setExit("east", office);
        lab.setExit("south", storage);

        office.setExit("west", lab);
        office.setExit("south", cafe);
        
        storage.setExit("up", ventilation); //7.43 trapdoor
        
        ventilation.setExit("east", cafe);
        ventilation.setExit("south", death);
        
        cafe.setExit("north", office);

        currentRoom = outside;  // start game outside
    }
    
    /**
     * Skapar karaktärer
     */
    
    private void createCharacter(){
        chars.add(new Character(pub, "Bartender: Looks like you've had a tough time..."));
    }
    /**
     *  Main play routine.  Loops until end of play.
     *  har en tids begränsning.
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
            if (time == 10 || currentRoom == death){
                finished = true;
            }
            if (savedRoom == null){
                savedRoom = currentRoom;
            }
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
        System.out.println("World of Zuul is a new, incredibly boring adventure game.");
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
            System.out.println("I don't know what you mean...");
            return false;
        }

        if (commandWord == CommandWord.HELP) {
            printHelp();
        }
        else if (commandWord == CommandWord.GO) {
            goRoom(command);
        } else if(commandWord == CommandWord.BEAMER){ 
            beamer(command);
        }
        else if (commandWord == CommandWord.QUIT) {
            wantToQuit = quit(command);
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
        System.out.println("You are lost. You are alone. You wander");
        System.out.println("around at the university.");
        System.out.println();
        System.out.println("Your command words are:");
        parser.showCommands();
    }

    /** 
     * Try to go to one direction. If there is an exit, enter the new
     * room, otherwise print an error message.
     */
    
    
    private void beamer(Command command) 
    {
        if(!command.hasSecondWord()) {
            // if there is no second word, we don't know what to do...
            System.out.println("Do what with the beamer?");
            return;
        }
        String secondWord = command.getSecondWord();
        
        if (secondWord.equals("save")) { //sparar rummet så du kan återvända med ett kommando
            savedRoom = currentRoom;
            System.out.println(currentRoom.getLongDescription());
        }
        else if(secondWord.equals("use")) {
            currentRoom = savedRoom;
            System.out.println(currentRoom.getLongDescription());
        } else {
            System.out.println("Do what with the beamer?");
        }
    }
    private void goRoom(Command command) 
    {
        int i;
        if(!command.hasSecondWord()) {
            // if there is no second word, we don't know where to go...
            System.out.println("Go where?");
            return;
        }

        String direction = command.getSecondWord();

        // Try to leave current room.
        Room nextRoom = currentRoom.getExit(direction);

        if (nextRoom == null) {
            System.out.println("There is no door!");
        }
        else {
            for(Character characters : chars){
                if (characters.room() == nextRoom){
                    System.out.println(characters.talk());
                }
            }
            currentRoom = nextRoom;
            System.out.println(currentRoom.getLongDescription());
            time = time +1; //time limit, begränsas av antalet steg. 7.42
        }
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
