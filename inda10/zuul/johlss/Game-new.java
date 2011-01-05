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
 * 
 * 
 * 
 * 
 * Hela klassen Game är ändrad.
 */

public class Game 
{
    private Parser parser;
    private Player player;
    private Room currentRoom;
    private Room chargedRoom;
    private int numberOfCommands;
    private boolean finished;
        
    /**
     * Creates the game and initialise its internal map.
     */
    public Game() 
    {
        createPlayer();
        createRoomsAndItems();
        parser = new Parser();
    }
    
    /**
     * Creates the player.
     */
    private void createPlayer() {
        player = new Player();
    }

    /**
     * Creates all the rooms and link their exits together, creates items in the rooms.
     */
    private void createRoomsAndItems() {
        Room townSquare, giftShop, fleastinkHotel, room605, darkAlley, harbor, submarine, pub, abandonedFactory, sewers, airport;
      
        // create the rooms
        townSquare = new Room("in town square");
        giftShop = new Room("in a gift shop");
        fleastinkHotel = new Room("in 'the Fleastink Hotel'");
        room605 = new Room("in hotel room number 605");
        darkAlley = new Room("in a very very dark alley");
        harbor = new Room("in the town's harbor");
        submarine = new Room("in a submarine");
        pub = new Room("in an old rundown pub");
        abandonedFactory = new Room("in an old abandoned factory");
        sewers = new Room("in the smelly sewers");
        airport = new Room("in an airport");
       
        // initialise room exits
        townSquare.setExit("north", fleastinkHotel);
        townSquare.setExit("east", giftShop);
        townSquare.setExit("south", darkAlley);
        townSquare.setExit("west", airport);

        giftShop.setExit("west", townSquare);

        fleastinkHotel.setExit("south", townSquare);
        fleastinkHotel.setExit("east", room605);
        
        room605.setExit("west", fleastinkHotel);

        darkAlley.setExit("north", townSquare);
        darkAlley.setExit("south", abandonedFactory);
        darkAlley.setExit("east", pub);
        darkAlley.setExit("west", harbor);
        darkAlley.setExit("down", sewers); //Det finns ingen väg upp igen, en trapdoor! För att ta sig upp måste man ha laddat beamern innan.

        harbor.setExit("east", darkAlley);
        harbor.setExit("west", submarine);
        
        submarine.setExit("east", harbor);
        
        pub.setExit("west", darkAlley);
        
        abandonedFactory.setExit("north", darkAlley);
        
        airport.setExit("east", townSquare);
        
        

        currentRoom = townSquare;  // start game outside
        
        //locks rooms
        submarine.lockRoom();
        airport.lockRoom();
        
        Item ticket, beamer, silverkey;
       
        
        //create the items
        ticket = new Item("a ticket that can be used at the local airport.", 10);
        silverkey = new Item("a shiny key, perhaps it opens a door? Maybe to some kind of underwater transportational device?", 50);
        beamer = new Item("a beamer. High-voltage Transportational Device 3000", 500);
        
        //initialise the locations of items
        sewers.getInventory().setItem("ticket", ticket);
        submarine.getInventory().setItem("beamer", beamer);
        abandonedFactory.getInventory().setItem("silverkey", silverkey);
        
        numberOfCommands = 0;
        
    }
    
    /**
     *  Main play routine.  Loops until end of play.
     */
    public void play() 
    {            
        printWelcome();

        // Enter the main command loop.  Here we repeatedly read commands and
        // execute them until the game is over.
                
        finished = false;
        while (! finished) {
            Command command = parser.getCommand();
            processCommand(command);
            addToNumberOfCommands();
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
        System.out.println("Type 'help' if you need help.");
        System.out.println();
        System.out.println(currentRoom.getLongDescription());
    }

    /**
     * Given a command, process (that is: execute) the command.
     * @param command The command to be processed.
     * @return true If the command ends the game, false otherwise.
     */
    private void processCommand(Command command) 
    {
        if(command.isUnknown()) {
            System.out.println("I don't know what you mean...");
            return;
        }

        String commandWord = command.getCommandWord();
        if (commandWord.equals("help")) {
            printHelp();
        }
        else if (commandWord.equals("go")) {
            goRoom(command);
        }
        else if (commandWord.equals("quit")) {
            quit(command);
        }
        else if (commandWord.equals("examine")) {
            examine(command);
        }
        else if (commandWord.equals("take")) {
            take(command);
        }
        else if (commandWord.equals("drop")) {
            drop(command);
        }
        else if (commandWord.equals("charge")) {
            charge(command);
        }
        else if (commandWord.equals("fire")) {
            fire(command);
        }
        else if (commandWord.equals("use")) {
            use(command);
        }
        // else command not recognised.
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
        System.out.println("around in this strange town.");
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
            System.out.println("You can't go that way.");
        }
        else {
            if(nextRoom.isLocked()) {
                if(currentRoom.getShortDescription().equals("in the town's harbor")) {
                    System.out.println("The door is locked! Perhaps you can find a key...?");
                }
                if(currentRoom.getShortDescription().equals("in town square")) {
                    System.out.println("A guard stops you and says 'The airport is off limits! Unless you have a ticket...'");
                }
            }
            else {
                currentRoom = nextRoom;
                System.out.println(currentRoom.getLongDescription());
                //KOLLA LEMMING, H�R KAN MAN VINNA!!
                if(currentRoom.getShortDescription().equals("in an airport")) {
                    System.out.println("Congratulations! You have finished the game.");
                    finished = true; 
                }
            }
        }
    }
    
    /**
     * Use är en metod där man använder en item (just nu silverkey eller ticket) för
     * att öppna ett "rum" (just nu submarine och airport).
     * 
     * 
     * Command "use" was entered, method checks if second command 
     * has been entered, and if second command can be used. 
     */
    private void use(Command command) {
        if(!command.hasSecondWord()) {
            // if there is no second word, we don't know what to charge...
            System.out.println("Use what?");
            return;
        } 
        String itemName = command.getSecondWord();
        Item item = player.getInventory().getItem(itemName);
        if(!itemExists(item)) {
            return;
        }
        if(currentRoom.getShortDescription().equals("in the town's harbor") && itemName.equals("silverkey")) {

            Room roomToUnlock = currentRoom.getExit("west");
            roomToUnlock.unlockRoom();
            System.out.println("The door is now unlocked!");
            return;
        }
        if(currentRoom.getShortDescription().equals("in town square") && itemName.equals("ticket")) {
            Room roomToUnlock = currentRoom.getExit("west");
            roomToUnlock.unlockRoom();
            System.out.println("That ticket is valid! You may enter the airport.");
            return;
        }
        System.out.println("You can't use that here!");
        
    }

    /** 
     * "Quit" was entered. Check the rest of the command to see
     * whether we really quit the game.
     * @return true, if this command quits the game, false otherwise.
     */
    private void quit(Command command) 
    {
        if(command.hasSecondWord()) {
            System.out.println("Quit what?");
        }
        else {
            finished = true;
        }
    }
    
    /**
     * Charge är en metod som i nuläget enbart används för beamern. 
     * 
     * 
     * 
     * Command "charge" was entered, method checks if second command 
     * has been entered, and if second command can be charged. 
     */
    private void charge(Command command) {
        
        if(!command.hasSecondWord()) {
            // if there is no second word, we don't know what to charge...
            System.out.println("Charge what?");
            return;
        } 
        String itemName = command.getSecondWord();
        if(!itemName.equals("beamer")) {
            System.out.println("You can't charge that!");
        }
        else {
            Item item = player.getInventory().getItem(itemName);
            if(!itemExists(item)) {
                return;
            }
            chargedRoom = currentRoom;
            System.out.println("The beamer is now charged!");
        }
    }
    
    
   /**
    * Fire är en metod som i nuläget enbart används för beamern. 
    * 
    * 
    * Command "fire" was entered, method checks if second command
    * has been entered, and if second command can be fired.
    */
    private void fire(Command command) {
        
        if(!command.hasSecondWord()) {
            // if there is no second word, we don't know what to fire...
            System.out.println("Fire what?");
            return;
        } 
        String itemName = command.getSecondWord();
        if(itemName.equals("beamer")) {
            Item item = player.getInventory().getItem(itemName);
            if(!itemExists(item)) {
                return;
            }
            
            if (chargedRoom == null) {
                System.out.println("You havn't charged the beamer yet!");
            }
            else {
                currentRoom = chargedRoom;
                System.out.println("ZAP! ZAP! You have been transported.");
                System.out.println(currentRoom.getLongDescription());
            }
        }
        else {
            System.out.println("You can't fire that!");
        }
    }
    
    /**
     * Command "examine" has been entered, method checks if second command
     * has been entered, and if second command has been entered prints 
     * the description of examined item. 
     */
    private void examine(Command command) {
        if(!command.hasSecondWord()) {
            // if there is no second word, we don't know what to examine...
            System.out.println("Examine what?");
            return;
        }
        
        String itemName = command.getSecondWord();
        Item item = currentRoom.getInventory().getItem(itemName);
        
        if(!itemExists(item)) {
            return;
        }
        
        System.out.println("It's " + currentRoom.getInventory().getItemDescription(itemName));
    }
    
    /**
     * Command "take" has been entered, method checks if second command 
     * has been entered, and moves the item given in second command 
     * from the rooms inventory to the players inventory.
     */
    private void take(Command command) {
        if(!command.hasSecondWord()) {
            // if there is no second word, we don't know what to take...
            System.out.println("Take what?");
            return;
        }
        
        String itemName = command.getSecondWord();
        Item item = currentRoom.getInventory().removeItem(itemName);
        
        if(!itemExists(item)) {
            return;
        }
        
        player.getInventory().setItem(itemName, item);
        System.out.println("You took the " + itemName);
    }
    
    /**
     * Command "drop" has been entered, method checks if second command 
     * has been entered, and moves the item given in second command 
     * from the players inventory to the rooms inventory.
     */
    private void drop(Command command) {
        if(!command.hasSecondWord()) {
            // if there is no second word, we don't know what to drop...
            System.out.println("Drop what?");
            return;
        }

        String itemName = command.getSecondWord();
        Item item = player.getInventory().removeItem(itemName);
        
        if(!itemExists(item)) {
            return;
        }
        
        currentRoom.getInventory().setItem(itemName, item);
        System.out.println("You dropped the " + itemName);
        
    }
    
    /**
     * Checks if item exists.
     */
    private boolean itemExists(Item item) {
        if(item == null) {
            System.out.println("There is no such item.");
            return false;
        }
        return true;
    }
    
    /**
     * Här är en av fyra implementerade grejer från boken.
     * Här är metoden som anger hur många commands man får använda innan man förlorar. 
     * Antalet använda commands sparas i fältet numberOfCommands.
     */
    public void addToNumberOfCommands() {
        numberOfCommands = numberOfCommands + 1;
        if(numberOfCommands >= 100) {
            System.out.println();
            System.out.println("You have died of radiation exposure. Poor you!");
            finished = true;
        }
    }
}
