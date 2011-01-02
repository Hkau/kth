import java.util.HashMap;

/**
 * This class is part of the "The Adventures of Hans & Greta" application. 
 * "The Adventures of Hans & Greta" is a very simple, text based adventure game.
 * 
 * This class holds an enumeration of all command words known to the game.
 * It is used to recognise commands as they are typed in.
 *
 * @author  Michael Kolling, David J. Barnes and Emil Berg
 * @version 1.00
 */

public class CommandWords
{
    // A mapping between a command word and the CommandWord
    // associated with it.
    private HashMap<String, Command> validCommands;

    /**
     * Constructor - initialise the commands.
     */
    public CommandWords()
    {
        validCommands = new HashMap<String, Command>();
        // Add all possible commands to the list.
        String goDescription = "Lets you go from a room to an adjacent room.\nThis command is used by writing 'Go <direction>', where <direction> is for instance east.";
        String helpDescription = "Prints a help message.\nUse the command by writing 'help'.";
        String takeDescription = "Picks up an item in a room.\nUse the command by writing 'take <item>', where <item> is the name of an item in the room.";
        String useDescription = "Uses an item in your inventory.\nUse the command by writing 'use <item>', where <item> is the name of the item in your inventory.";
        String describeDescription = "Describes a room, an item in a room, an item in your inventory or a command.\nUse the command by writing 'describe <object>', where <object> is 'room', the name of an item or the name of a command.";
        String inventoryDescription = "Displays your inventory.\nUse the command by writing 'inventory'.";
        String quitDescription = "Quits the game.\nUse the command by writing 'quit'.";
        
        validCommands.put("go", new GoCommand(goDescription));
        validCommands.put("help", new HelpCommand(helpDescription, this));
        validCommands.put("take", new TakeCommand(takeDescription));
        validCommands.put("use", new UseCommand(useDescription));
        validCommands.put("describe", new DescribeCommand(describeDescription, this));
        validCommands.put("inventory", new InventoryCommand(inventoryDescription));
        validCommands.put("quit", new QuitCommand(quitDescription));
    }
    
    /**
     * Check whether a given String is a valid command word. 
     * @return true if it is, false if it isn't.
     */
    public boolean isCommand(String aString)
    {
        return validCommands.containsKey(aString);
    }

    /**
     * Print all valid commands to System.out.
     */
    public void showAll() 
    {
        for(String command : validCommands.keySet()) {
            System.out.print(command + "  ");
        }
        System.out.println();
    }
    
    /**
     * @param commandName The name of the command to search for.
     * @return The suitable command.
     */
    public Command get(String commandName) {
        return validCommands.get(commandName);
    }
}
