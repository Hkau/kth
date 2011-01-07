import java.util.ArrayList;

/**
 *  A new game based on the World of Zuul but much better!
 * 
 *  To play this game, create an instance of this class and call the "play"
 *  method.
 * 
 *  This main class creates and initialises all the others: it creates all
 *  rooms, creates the parser and starts the game.  It also evaluates and
 *  executes the commands that the parser returns.
 * 
 * @author  Michael Kolling and David J. Barnes and now also Carl Eriksson
 * @version 2008.03.30
 */

public class Game 
{
    private Parser parser;
    private Room currentRoom;
    //creates a beamer
    private Beamer beam;
    //keep track of numbers of moves
    private int moveDone;
    //saves all the roomes in one list
    private ArrayList<Room> allRoomes;
    //create a instance of the inventoty class
    private Inventory inven;
    
    //create some items that will be used
    private Item masterKey;
    private Item wine;
    /**
     * Create the game and initialise its internal map.
     */
    public Game() 
    {
    	inven = new Inventory();
    	allRoomes = new ArrayList<Room>();
        createRooms();
        parser = new Parser();
        beam = new Beamer("A Teleporter","Use at your own risk!",null,4);
        moveDone = 0;          
    }
    /**
     * The starter for the program
     * @param args
     */
    public static void main(String[] args) {
		Game g = new Game();
		g.play();		
	}
    /**
     * Create all the rooms and link their exits together.
     */
    private void createRooms()
    {
    	//create the items
    	masterKey = new Item("Masterkey","This key will open any door");
    	wine = new Item("Winebottle","Too bad it's empty!");
    	//create some roomes
        Room start,startDown,finish,prison,lobby,kitchen,stairs,hall,bedroom,basement,wineBasment,teleportRoom;
      
        // create the rooms
        start = new Room("at the exit but there exit door seems to be locked.\nNow you need to find the key to get out");
        startDown = new Room("at the a secret room! Exlopre it!");
        finish = new Room("out! You have now won the game! Type 'quit' to stop playing");
        prison = new Room("at the prison cell, uber fail man!");
        lobby = new Room("at the lobby");
        kitchen = new Room("at the kitchen, nothing here to see execpt the door over there...");
        stairs = new Room("at the middle of stairs.");
        hall = new Room("at the upper hall");
        bedroom = new Room("the bedroom, better to have a look under that big bed");
        basement = new Room("at creepy basement, dont stay here for to long!");
        wineBasment = new Room("at the wineroom, so much wine and so lite time");
        teleportRoom = new Room("at a teleportroom");
        
        //sets the exits on the rooms
        start.setExit("north", finish);
        start.setExit("down", startDown);
        	start.hideExit("down");
        start.setExit("east",lobby);
        start.setExit("west", prison);
        
        startDown.setExit("up",start);
        
        lobby.setExit("west",start);
        lobby.setExit("east",kitchen);
        lobby.setExit("south", stairs);
        
        kitchen.setExit("west", lobby);
        kitchen.setExit("north", prison);
        
        stairs.setExit("north", lobby);
        stairs.setExit("up",hall);
        stairs.setExit("down", basement);
        
        hall.setExit("down",stairs);
        hall.setExit("east",bedroom);
        
        bedroom.setExit("west",hall);
        
        basement.setExit("up", stairs);
        basement.setExit("north", wineBasment);
        basement.setExit("east",teleportRoom);
        
        wineBasment.setExit("south", basement);
        
        //adds item to some roomes
        startDown.addItem(masterKey);
        wineBasment.addItem(wine);
        bedroom.addItem(masterKey); 
        
        //set properties for the finish room 
        finish.setLocked(true);
        finish.setKey("Masterkey");
        
        //sets teleport room to be the teleport room
        teleportRoom.setTeleportRoom(true);
        
        //adds all the roomes to the list
        allRoomes.add(start);
        allRoomes.add(prison);
        allRoomes.add(lobby);
        allRoomes.add(kitchen);
        allRoomes.add(stairs);
        allRoomes.add(hall);
        allRoomes.add(bedroom);
        allRoomes.add(basement);
        allRoomes.add(wineBasment);
          
        currentRoom = start;  // start game outside
    }

    /**
     *  Main play routine.  Loops until end of play.
     */
    public void play() 
    {         
    	int moves = 100;
        printWelcome();
        

        // Enter the main command loop.  Here we repeatedly read commands and
        // execute them until the game is over.
                
        boolean finished = false;
        while (!finished) {
            Command command = parser.getCommand();
            finished = processCommand(command);
            if(moveDone > moves){
            	finished = true;
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
        System.out.println("~~~~~~House of Boring~~~~~~");
        System.out.println();
        System.out.println("Make it out and you win!");
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
        }
        else if (commandWord == CommandWord.QUIT) {
            wantToQuit = quit(command);
        }
        else if (commandWord == CommandWord.BEAMER){
        	useTeleport(command);
        }
        else if(commandWord == CommandWord.UNLOCK){
        	unlockRoom(command);
        }
        else if(commandWord == CommandWord.LIST){
        	startList(command);
        }
        else if(commandWord == CommandWord.PICKUP){
        	pickUpItem(command);
        }
        // else command not recognized.
        return wantToQuit;
    }

    /**
     * Will pickup a item if possible in a room and add it in the player inventory
     * @param command
     */
    public void pickUpItem(Command command){
    	//if the user miss used the command
    	if(!command.hasSecondWord()) {
            // if there is no second word, we don't know where to go...
            System.out.println("Pickup what?");
            return;
        }
    	//picks the item up
    	String title = command.getSecondWord();
    	Item tempItem = currentRoom.getItem(title);
    	if(tempItem != null){
    		inven.addItem(tempItem);
    		currentRoom.deleteItem(title);
    		System.out.println("Added " + tempItem.getTitle() + " to the inventory");
    	}
    	else{
    	}
    }
    //list the stuffs in the room or in the player inventory
    public void startList(Command command){
    	//miss use of the command
    	if(!command.hasSecondWord()) {
            // if there is no second word, we don't know where to go...
            System.out.println("List what?");
            return;
        }
    	String secondWord = command.getSecondWord();
    	
    	//list the room
    	if(secondWord.equals("room")){
    		currentRoom.list();
    	}
    	//list the inventory
    	else if(secondWord.equals("inventory")){
    		System.out.println("Theese items are in your inventoty");
    		System.out.println(inven.toString());
    	}
    }
    
    /**
     * Unlock a room that is locked
     * @param command
     */
    public void unlockRoom(Command command){
    	//miss use of the command
    	if(!command.hasSecondWord()) {
            // if there is no second word, we don't know where to go...
            System.out.println("Unlock what?");
            return;
        }
    	
    	String direction = command.getSecondWord();
    	boolean open = false;

        //Try to open a room
        Room nextRoom = currentRoom.getExit(direction);
        if(nextRoom != null){
	        if(nextRoom.isLocked()){
	        	for(int r = 0; r < inven.size();r++){
	        		//try to unlock the room
	        		nextRoom.unlock(inven.getItemByIndex(r).getTitle());
	        		if(!nextRoom.isLocked()){
	        			System.out.println("The door is now unlocked!");
	        			open = true;
	        		}
	        	}
	        	if(!open){
	        		System.out.println("You don't have the correct key...");
	        	}
	        }
	        else{
	        	System.out.println("It's not locked");
	        }
        }
        else{
        	System.out.println("Ain't a door there!");
        }
    	
    }
    /**
     * Print out some help information.
     * Here we print some stupid, cryptic message and a list of the 
     * command words.
     */
    private void printHelp() 
    {
        System.out.println("You are lost. You are alone.");
        System.out.println("around in the House of Boring.");
        System.out.println("The beamer can only be used a limited of times");
        
        System.out.println();
        System.out.println("beamer: 'save' & 'use'");
        System.out.println("list: 'inventory' & 'room'");
        System.out.println();
        System.out.println("Your command words are:");
        parser.showCommands();
        
    }
    /**
     * Use your teleporter, either save a location or teleport to one.
     * @param command
     * @throws InterruptedException 
     */
    public void useTeleport(Command command){
    	//miss use of the command
    	if(!command.hasSecondWord()) {
            // if there is no second word, we don't know where to go...
            System.out.println("Teleport what?");
            return;
        }
    	
    	String usage = command.getSecondWord();
    	
    	//if the user wants to save the location
    	if (usage.equals("save")){
    		beam.setSavedRoom(currentRoom);
    		System.out.println("Location saved");
    	}
    	//if the user wants to teleport to the saved location
    	else if(usage.equals("use")){
    		Room teleRoom = beam.getSavedRoom();
    		if(teleRoom == null){
    			if(!beam.toManyUses()){
    				System.out.println("There is no room saved!");
    			}
    			else{
    				System.out.println("You have to charge your teleporter");
    			}
    		}
    		else{
    			//teleport.printStatusBar();
    			currentRoom = teleRoom;
    			System.out.println(currentRoom.getLongDescription());
    			
    		}
    	}
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
            System.out.println("There is no door!");
        }
        else {
        	//this will check if the room is a teleporter room
        	if(nextRoom.isTeleportRoom()){
        		currentRoom = nextRoom.getRandomRoom(allRoomes);
        		moveDone++;
        		System.out.println("Warning: Teleporing initiated!");
        		System.out.println(currentRoom.getLongDescription());
        	}
        	else{
        		//check if the next room is locked
        		if(!nextRoom.isLocked()){
        			currentRoom = nextRoom;
        			moveDone++;
        			System.out.println(currentRoom.getLongDescription());
        		}
        		else{
        			System.out.println("The room is locked");
        		}
        	}
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
