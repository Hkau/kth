import java.util.HashMap;
/**
 *  This class is the main class in the game Charlie the Unicorn, which is based on the second Charlie the Unicorn video.
 * 
 *  To play this game, create an instance of this class and call the "play"
 *  method.
 * 
 *  This main class creates and initialises all the others: it creates all
 *  rooms, creates the parser and starts the game.  It also evaluates and
 *  executes the commands that the parser returns.
 * 
 * @author  Louise Hansson (Originally by Michael Kolling and David J. Barnes)
 * @version 2010.12.02
 */
public class Game 
{
    private Parser parser;
    private Room currentRoom;
    private Inventory inventory;
    
    HashMap<String, Items> items;
    HashMap<Room, Items> locationOfItems;
    HashMap<String, Room> rooms;
    
    private boolean zIsAvailable;
    private boolean bookStoreOwnerIsAvailable;
    private boolean hasTalkedWithZ;
    private boolean wantToQuit;
    
    //Npcs
    private Npc blueUnicorn, pinkUnicorn, Z, bookStoreOwner;
    //Items
    private Items amulet, dictionary, bigStone, jewel, banana, boardingPass;
    //Rooms
    private Room meddow, amuletRoom, bookStore, zRoom, trainRoom, temple, trap;
        
    /**
     * Create the game and initialise its internal map.
     */
    public Game() 
    {
        parser = new Parser();
        inventory = new Inventory();
        items = new HashMap<String, Items>();
        locationOfItems = new HashMap<Room, Items>();
        zIsAvailable = false;
        bookStoreOwnerIsAvailable = false;
        hasTalkedWithZ = false;
        wantToQuit = false;
        createMaps();
    }

    /**
     * The starter for the program.
     * @param   args
     */
    public static void main(String[] args)
    {
        Game game = new Game();
        game.play();
    }
    
    /**
     * Create all the rooms and link their exits together.
     * Create all the items and add them to hashmaps, one with their names as keys, and one with thier location as key.
     * Create all the npcs.
     * Create a hashmap that links special rooms with special npcs.
     */
    private void createMaps()
    {
        // create the rooms
        meddow = new Room("the meddow where you live");
        amuletRoom = new Room("the room where a certain amulet is said to be found");
        bookStore = new Room("a random bookstore in the middle of the forest");
        zRoom = new Room("a part of the forest with a big Z in the middle");
        trainRoom = new Room("the place with the Chu-chu shoe,that is really a train... that looks like a sneaker.");
        temple = new Room("the temple of the Banana King");
        trap = new Room("a room without doors");
        
        // initialise room exits
        meddow.setExit("west", zRoom);
        meddow.setExit("north", amuletRoom);
        amuletRoom.setExit("west", bookStore);
        amuletRoom.setExit("south", meddow);
        amuletRoom.setExit("north", trap);
        bookStore.setExit("east", amuletRoom);
        zRoom.setExit("south", trainRoom);
        zRoom.setExit("east", meddow);
        trainRoom.setExit("north", zRoom);
        trainRoom.setExit("west", temple);
        
        currentRoom = meddow;  // start game in the meddow

        //create the items.
        amulet = new Items(1, "Amulet");   
        dictionary = new Items(5, "Dictionary");
        bigStone = new Items(7, "Big_Stone");
        jewel = new Items(5, "Jewel");
        banana = new Items(2, "Banana");
        boardingPass = new Items(1, "Boarding_Pass"); 
        
        //add items to hashmap
        addToItemList(amulet.getName(), amulet);
        addToItemList(dictionary.getName(), dictionary);
        addToItemList(bigStone.getName(), bigStone);
        addToItemList(jewel.getName(), jewel);
        addToItemList(banana.getName(), banana);
        addToItemList(boardingPass.getName(), boardingPass);
        
        //add items to hashmap with location as key.
        locationOfItems.put(amuletRoom, amulet);
        locationOfItems.put(bookStore, dictionary);
        locationOfItems.put(zRoom, jewel);
        locationOfItems.put(trainRoom, bigStone);
        locationOfItems.put(temple, banana);
        locationOfItems.put(meddow, boardingPass);
        
        //create the npcs.
        blueUnicorn = new Npc("Blue_Unicorn", "A blue unicorn that stalks you");
        pinkUnicorn = new Npc("Pink_Unicorn", "A pink unicorn that stalks you");
        Z = new Npc("Z", "A very big red Z that talks");
        bookStoreOwner = new Npc("Bookstore_Owner", "A bookstore owner");
    }
    
    /**
    *  Main play routine.  Loops until end of play.
    */
    public void play() 
    {            
        printWelcome();
        printNpcsInRoom(currentRoom);
        printItems();
        
        // Enter the main command loop.  Here we repeatedly read commands and
        // execute them until the game is over.    
        boolean finished = false;
        while (! finished) 
        {
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
        System.out.println("You are Charlie the Unicorn, a boring white unicorn that lacks a sense of humor.");
        System.out.println("You're in a meddow minding your own business - watching tv, when suddenly two unicorns come swimming towards you in diving suits.");
        System.out.println("But oh no, they're getting sucked in to the vortex on your back!");
        System.out.println("...or not, but to close the vortex and prevent it from letting out a");
        System.out.println(" thousand years of darkness you have to find the mysterious Amulet and bring it to the banana king.");
        System.out.println("Good luck!");
        System.out.println();
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
        CommandWord commandWord = command.getCommandWord();

        if(commandWord == CommandWord.UNKNOWN) 
        {
            System.out.println("I don't know what you mean...");
            return false;
        }

        if (commandWord == CommandWord.HELP) 
        {
            printHelp();
        }
        else if (commandWord == CommandWord.GO) 
        {
            goRoom(command);
            if(currentRoom == temple)
            {
                wantToQuit = true;
            }
        }
        else if (commandWord == CommandWord.QUIT) 
        {
            wantToQuit = quit(command);
        }
        else if(commandWord == CommandWord.CARRY)
        {
            carry(command);
        }
        else if(commandWord == CommandWord.DROP)
        {
            drop(command);
        }
        else if(commandWord == CommandWord.TALK)
        {
            talkTo(command);
        }
        // else command not recognised.
        return wantToQuit;
    }

    /**
     * Print out some help information.
     * Here we print some stupid, cryptic message and a list of the 
     * command words.
     */
    private void printHelp() 
    {
        System.out.println("You are lost. You are alone. You wander");
        System.out.println("around in the forest.");
        System.out.println();
        System.out.println("Your command words are:");
        parser.showCommands();
    }

    /** 
     * Try to go to one direction. If there is an exit, enter the new
     * room, otherwise print an error message. 
     * Also check if the door is locked.
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
        if (nextRoom == null) 
        {
            System.out.println("A mystic force is preventing you from going this way");
        }
        else 
        {
            if(currentRoom == amuletRoom && nextRoom == meddow)
            {
                if(inventory.backpack.contains(amulet))
                {
                    printFreeToPass();
                    moveRoom(nextRoom);
                }
                else
                {
                    System.out.println("This door is closed! To pass you must get the "+amulet.getName()+".");
                }
            }
            else if(currentRoom == meddow && nextRoom == zRoom)
            {
                if(inventory.backpack.contains(amulet))
                {
                    printFreeToPass();
                    moveRoom(nextRoom);
                }
                else
                {
                    System.out.println("This door is closed, you must find a special item to pass through here.");
                }
            }
            else if(currentRoom == zRoom && nextRoom == trainRoom)
            {
                if(inventory.backpack.contains(dictionary))
                {
                    if(hasTalkedWithZ)
                    {
                        moveRoom(nextRoom);
                    }
                    else
                    {
                        printMustTalkWithZ();
                    }
                }
                else
                {
                    printMustTalkWithZ();
                }
            }
            else if(currentRoom == trainRoom && nextRoom == temple)
            {
                if(inventory.backpack.contains(boardingPass))
                {
                    System.out.println("You have a "+boardingPass.getName()+"! Have a safe trip with the Chu-chu shoe!");
                    moveRoom(nextRoom); 
                }
                else
                {
                    System.out.println("You can't ride the Chu-chu shoe if you don't have a "+boardingPass.getName()+"! Go find it!");
                }
            }
            else if(nextRoom == trap)
            {
                System.out.println("It's a trap!");
                moveRoom(nextRoom);
            }
            else
            {
                moveRoom(nextRoom);
            }
        }
        if(currentRoom == temple)
        {
            System.out.println("Congratulations! You made it to the Temple of the Banana King and you are carrying the Magical Amulet!");
            System.out.println("But it was actually you who were the Banana King all along, anyway, well done.");
            System.out.println("You saved us, so now go and put a banana in your ear~");
            System.out.println("You won :)");
        }
    }
    
    /**
     * Print a message to inform the user that they can't pass through here.
     */
    private void printMustTalkWithZ()
    {
        System.out.println("In order to pass through here you must talk with the "+Z.getName()+".");
    }
    
    /**
     * Print a message to inform the user that they are free to pass.
     */
    private void printFreeToPass()
    {
        System.out.println("You are carrying the "+amulet.getName()+" and are free to pass.");
    }
    
    /**
     * Move to the next room, print out the description of the room
     * Print the npcs and the item in the room.
     * @param   nextRoom A room
     */
    private void moveRoom(Room nextRoom)
    {
        currentRoom = nextRoom;
        System.out.println(currentRoom.getLongDescription());
        printNpcsInRoom(currentRoom);
        printItems();
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
    
    /**
     * Put an item in your backpack and remove the item from the room.
     * @param   command A command
     */
    private void carry(Command command)
    {
        if(!command.hasSecondWord())
        {
            System.out.println("Carry what?");
            return;
        }
        else
        {
            String name = command.getSecondWord();
            Items item = getItem(name);
            if(currentRoom == bookStore)
            {
                if(locationOfItems.get(currentRoom) != null && locationOfItems.get(currentRoom) == item && item != dictionary)
                {
                    if(inventory.checkIfTooHeavy(item))
                    {
                        printDropSomething();
                    }
                    else
                    {
                        moveItem(item);
                    }
                }
                else
                {
                    printNoneHere("item");
                }
            }
            else
            {
                if(locationOfItems.get(currentRoom) != null && locationOfItems.get(currentRoom) == item)
                {
                    if(inventory.checkIfTooHeavy(item))
                    {
                         printDropSomething();
                    }
                    else
                    {
                         moveItem(item);    
                    }
                }
                else
                {
                    printNoneHere("item");
                }
            }
        }
    }
    
    /**
     * Add an item to the backpack, remove it from the current room.
     * Print out a message.
     * @param   item The item that should be moved
     */
    private void moveItem(Items item)
    {
        inventory.addToBackpack(item);
        locationOfItems.remove(currentRoom);
        items.remove(currentRoom);
        System.out.println("The item has been put in your backpack.");
    }
    
    /**
     * Print a message saying you have to drop something
     */
    private void printDropSomething()
    {
        System.out.println("Your backpack is too heavy, you must drop something in order to carry this item");
    }
    
    /**
     * Print an error message.
     */
    private void printNoneHere(String thing)
    {
        System.out.println("There is no such "+thing+" here.");
    }
    
    /**
     * Remove an item from your backpack, and add it to the room you're in.
     * @param   command A command
     */
    private void drop(Command command)
    {
        String name = command.getSecondWord();
        Items item = getItem(name);
        if(!command.hasSecondWord())
        {
            System.out.println("Drop what?");
            return;
        }
        else
        {
            if(locationOfItems.get(currentRoom) == null && inventory.backpack.contains(item))
            {
                inventory.dropItem(item);
                items.put(name, item);
                locationOfItems.put(currentRoom, item);
                System.out.println("You have dropped the item.");
            }
            else
            {
                if(currentRoom == bookStore)
                {
                    System.out.println("Don't throw things in the bookstore!");
                }
                else
                {
                    System.out.println("You can only have one item in each room. Either there's already an item here, or you don't have this item in your backpack.");
                }
            }
        }
    }
    
    /**
     * Check if the npc  you want to talk to is in the room, and print a response.
     * @param   command A command
     */
    private void talkTo(Command command)
    {
        String name = command.getSecondWord();
        if(!command.hasSecondWord())
        {
            System.out.println("Talk to who?");
            return;
        }
        else
        {
            if(name.equals(pinkUnicorn.getName()) || name.equals(blueUnicorn.getName()))
            {
                pinkUnicorn.printRandomResponse();
            }
            else if(name.equals(Z.getName()) && zIsAvailable)
            {
                if(inventory.backpack.contains(dictionary))
                {
                    Z.printEnglishInstructions();
                    hasTalkedWithZ = true;
                }
                else
                {
                    Z.printSpanishInstructions();
                }
            }
            else if(name.equals(bookStoreOwner.getName()) && bookStoreOwnerIsAvailable)
            {
                bookStoreOwner.printOwnerResponse();
                parser.getYesOrNo(inventory.backpack, items);
            }
            else
            {
                printNoneHere("npc");
            }
        } 
    }
    
    /**
     * Add items to a hashmap with the name of the item as key.
     * @param   A name, An item
     */
    private void addToItemList(String name, Items item)
    {
        items.put(name, item);
    }
    
    /**
     * Print the npcs that are currently in the room.
     * @param   location The current room
     */
    private void printNpcsInRoom(Room location)
    {
        System.out.print("Npcs in room: "+pinkUnicorn.getName()+" ("+pinkUnicorn.getDescription()+"), "+blueUnicorn.getName()+" ("+blueUnicorn.getDescription()+")");
        if(currentRoom == zRoom)
        {
            zIsAvailable = true;
            System.out.println(", "+Z.getName()+" ("+Z.getDescription()+")");
        }
        else
        {
            zIsAvailable = false;
        }
        
        if(currentRoom == bookStore)
        {
            bookStoreOwnerIsAvailable = true;
            System.out.print(", "+bookStoreOwner.getName()+" ("+bookStoreOwner.getDescription()+")");
        }
        else
        {
            bookStoreOwnerIsAvailable = false;
        }
        System.out.println();
    }
    
    /**
     * Add the item to a hashmap with the location of the item as key.
     * @param   An item, A room
     */
    private void addToLocationOfItems(Items item, Room location)
    {
        locationOfItems.put(location, item);
    }
    
    /**
     * Return the item by using the name of the item as key in the hashmap.
     * @return  An item
     */
    public Items getItem(String name)
    {
        return items.get(name);
    }
    
    /**
     * Return the item by using the location of the item as key in the hashmap.
     * @return  An item
     */
    public Items getItems(Room location)
    {
        return locationOfItems.get(location);
    }
    
    /**
     * Print the item in the room.
     * If there is no item in the room, print "none".
     */
    private void printItems()
    {
        System.out.print("Item in this Room: ");
        if(locationOfItems.get(currentRoom) != null && currentRoom != bookStore)
        {
            System.out.println(locationOfItems.get(currentRoom).getName());
        }
        else
        {
            System.out.println("none");
        }
    }
}
