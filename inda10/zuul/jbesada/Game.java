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
    private Player player; 
    private static final int TIME_LIMIT = 15;
    private int time;
    /**
     * Create the game and initialise its internal map.
     */
    public Game() 
    {
        time = TIME_LIMIT;  
        player = new Player();
        createRooms();
        parser = new Parser();
    }

    /**
     * Create all the rooms and link their exits together.
     */
    private void createRooms()
    {
        Room outside, theatre, pub, lab, office;
        TrapRoom corridor;
        TransporterRoom transporter;
        LockedRoom secret;
        // create the rooms
        outside = new Room("outside the main entrance of the university");
        theatre = new Room("in a lecture theatre");
        pub = new Room("in the campus pub");
        lab = new Room("in a computing lab");
        office = new Room("in the computing admin office");
        corridor = new TrapRoom("in a dark corridor.\n" + 
        "As you enter the room the door behind you magically disappears");
        transporter = new TransporterRoom("in a small empty room. \n" +
        "As you take a step forward, the world around \n" + 
        "you suddenly starts spinning very fast. \n" + 
        "A couple of seconds passes before the spinning stops. \n" + 
        "You are still in the same room but somehow you feel like \n" + 
        "you have been relocated");
        secret = new LockedRoom("secret room");
        
        // initialise room exits
        outside.setExit("east", theatre);
        outside.setExit("south", lab);
        outside.setExit("west", pub);

        theatre.setExit("west", outside);
        theatre.addItem("a used papercup","papercup");

        pub.setExit("east", outside);
        pub.addItem("a rusty old key","key");

        lab.setExit("north", outside);
        lab.setExit("east", office);
        lab.setExit("south", corridor);

        office.setExit("west", lab);
        office.setExit("east", secret);

        corridor.setExit("south",transporter);
        
        transporter.setExit("north",corridor);
        
      
        
        
        player.setCurrentRoom(outside);  // start game outside
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
        System.out.println("Welcome to World of Zuul!");
        System.out.println("World of Zuul is a new, incredibly amazing adventure game.");
        System.out.println("The goal is to reach the secret room before the time expires");
        System.out.println("Type '" + CommandWord.HELP + "' if you need help.");
        System.out.println();
        System.out.println(player.getCurrentRoom().getLongDescription());
        printTime();
        
    }
    /*# EXERCISE 7.42 (TIME LIMIT)*/
    /**
     * Print out the time that is left.
     */
    private void printTime(){
        String mins = " minute";
        if(time > 1){
            mins += "s";
        }
        System.out.println("You have " + time + mins + " left to find the secret room");
    }
    /**
     * 'Ticks' the time, when time reaches 0 the game ends.
     */
    private void timeTick(){
        time--;
        if(time == 0){
            gameLose();
        }else{
            printTime();
        }
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
        } else if(commandWord == CommandWord.LOOK){
            look();
        } else if(commandWord == CommandWord.BACK){
            goBack();
        } else if(commandWord == CommandWord.TAKE){
            pickUp(command);
        } else if(commandWord == CommandWord.DROP){
            drop(command);
        } else if(commandWord == CommandWord.ITEMS){
            displayItems();
        }else if (commandWord == CommandWord.GO) {
            goRoom(command);
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
     * Prints out the room description again
     */
    private void look(){
        System.out.println(player.getCurrentRoom().getLongDescription());
        printTime();
    }
    /**
     * Returns the player to the previous room, if allowed, else print an
     * error message.
     */
    private void goBack(){
        if(player.getPreviousRoom() != null){
            //disables you from returning to or from a transporter room or 
            //through a trap door
            if(!(player.getPreviousRoom() instanceof TransporterRoom)
            && !(player.getCurrentRoom() instanceof TransporterRoom)
            && !(player.getCurrentRoom() instanceof TrapRoom)){
               player.setCurrentRoom(player.popPreviousRoom());
               System.out.println(player.getCurrentRoom().getLongDescription());
               timeTick();
            } else {
                System.out.println("You can't go back!");
            }
        }
    }
    /**
     * Try to pick up a specified item. If the item exists in the room place it in
     * the players inventory,else print an error message.
     */
    private void pickUp(Command command){
        if(!command.hasSecondWord()) {
            System.out.println("Take what?");
            return;
        }
        String takeItem = command.getSecondWord();
        ArrayList<Item> roomItems = player.getCurrentRoom().getItems();
        for(int i = roomItems.size()-1;i >= 0;i--){           
            if(roomItems.get(i).getIdentification().equals(takeItem)){
                System.out.println("You picked up the " + takeItem + ".");
                player.addItem(roomItems.get(i));
                player.getCurrentRoom().removeItem(i);
                return;
            }
        }
        System.out.println("There is no such item.");
    }
    /**
     * Removes a specified item from the players inventory and places it in the room,
     * if such item exists.
     */
    private void drop(Command command){
        if(!command.hasSecondWord()) {
            System.out.println("Drop what?");
            return;
        }
        String dropItem = command.getSecondWord();
        ArrayList<Item> playerItems = player.getItems();
        for(int i = playerItems.size()-1;i >= 0;i--){           
            if(playerItems.get(i).getIdentification().equals(dropItem)){
                System.out.println("You dropped the " + dropItem + ".");
                player.getCurrentRoom().dropItem(playerItems.get(i));
                player.removeItem(i);
                return;
            }
        }
        System.out.println("You have no such item.");
    }
    /**
     * Displays all the items in the players inventory.
     */
    private void displayItems(){
        String itemString = "You are carrying";
        if(player.getItems().size() > 0){
            for(Item item : player.getItems()){
                itemString += "\n" + item.getIdentification();
            }
        } else {
            itemString += " nothing.";
        }
        System.out.println(itemString);
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
        Room nextRoom = player.getCurrentRoom().getExit(direction);

        if (nextRoom == null) {
            System.out.println("There is no door!");
        } else if (nextRoom instanceof LockedRoom){
            if(nextRoom.getLocked()){
                if(nextRoom.attemptOpen(player.getItems())){
                    System.out.println("You have unlocked the door"); 
                    gameWin();
                }else{
                    System.out.println("The door is locked!");
                }
           }
        } else {
            player.storeCurrentRoom();
            player.setCurrentRoom(nextRoom);
            System.out.println(player.getCurrentRoom().getLongDescription());
            timeTick();
        }
        
    }
    /**
     * Print out the win message to the player and end the game.
     */
    private void gameWin(){
        System.out.println("Congratulations, you have found the secret room \n" +
        "and thereby completed the game!!!");
        endGame();
    }
     /**
     * Print out the lose message to the player and end the game.
     */
    private void gameLose(){
        System.out.println("Your time has run out...");
        endGame();
    }
    /**
     * Print out a farewell message and exit the game.
     */
    private void endGame(){
        System.out.println("The game will now end. Thank you for playing.  Good bye.");
       //small delay before exit
        try
        {
            Thread.sleep(1000);
        } 
        catch (InterruptedException e)
        {
            // ignoring exception at the moment
        }
        System.exit(0);
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
