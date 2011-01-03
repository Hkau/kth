package org.x2d.zuul;
import java.util.*;
/**
 * This class is part of the "World of Zuul" application. 
 * "World of Zuul" is a very simple, text based adventure game.  
 * 
 * This parser reads user input and tries to interpret it as an "Adventure"
 * command. Every time it is called it reads a line from the terminal and
 * tries to interpret the line as a two word command. It returns the command
 * as an object of class Command.
 *
 * The parser has a set of known command words. It checks user input against
 * the known commands, and if the input is not one of the known commands, it
 * returns a command object that is marked as an unknown command.
 * 
 * @author  Michael Kolling and David J. Barnes
 * @version 2008.03.30
 */
public class Parser 
{
    private static final HashMap<String, CommandWord> validCommands = 
        new HashMap<String, CommandWord>(10);
    private Game game;
    private static Scanner reader = new Scanner(System.in);         // source of command input

    /**
     * Create a parser to read from the terminal window.
     */
    public Parser(Game game) 
    {
        this.game = game;        
        addCommandWord(new CommandGo("gå"));
        addCommandWord(new CommandUnlock("låsupp"));
        addCommandWord(new CommandWord("sluta") {
                public void executeCommand(Game game, String target) {
                   game.endGame(false);
                }
            });
        addCommandWord(new CommandHelp("hjälp"));
        addCommandWord(new CommandUse("använd"));
        addCommandWord(new CommandTake("ta"));
        addCommandWord(new CommandTalk("prata"));
        addCommandWord(new CommandRead("läs"));
        addCommandWord(new CommandSave("spara"));
        addCommandWord(new CommandLoad("ladda"));
        addCommandWord(new CommandList("lista"));
    }

    /**
     * Reads one line parses it for and commands. If a command is found then
     * it's executed.
     */
    public void getCommand() 
    {
        String inputLine;   // will hold the full input line
        String command = null;
        String target = null;

        inputLine = reader.nextLine().trim();
        System.out.println("> "+inputLine);
        int spacePos = inputLine.indexOf(' ');
        if (spacePos == -1) {
            command = inputLine;
        } else {
            command = inputLine.substring(0, spacePos);
            target = inputLine.substring(spacePos+1);
        }
        if (!isCommand(command)) {
            System.out.println("Okänd kommando: "+command);
            getCommand("hjälp").executeCommand(getGame(), null);
            return;
        }
        CommandWord cw = getCommand(command);
        if (cw.mustHaveTarget()&&target==null) {
            System.out.println("Det här kommandot kräver ett mål, vad vill du använda det på?");
            getCommand("hjälp").executeCommand(getGame(), command);
            return;
        }
        cw.executeCommand(getGame(), target);
    }
    
    /**
     * Gets the game currently using this parser.
     *
     * @return The game.
     */
    public Game getGame() {
        return game;
    }
    
    /**
     * Sets which game that is currently using this parser.
     *
     * @param game The game.
     */
    public void setGame(Game game) {
        this.game = game;
    }
    
    /**
     * Returns a list of all possible targets for a command.
     *
     * @param command The command's name
     * @return The list of possible targets or <code>null</code> if it's not a valid command
     */
    public String[] showTargets(String command) {
        if (isCommand(command)) {
            return getCommand(command).getTargets(getGame());
        }
        return null;
    }

    /**
     * Adds a command word to the list of commands.
     *
     * @param cw A CommandWord which should be added to this list of commands.
     */
    public void addCommandWord(CommandWord cw) {
        validCommands.put(cw.getCommand(), cw);
    }

    /**
     * Check whether a given String is a valid command word.
     *
     * @return <code>true</code> if it is, <code>false</code> if it isn't.
     */
    public boolean isCommand(String aString)
    {
        return (validCommands.get(aString)!=null);
    }
    
    /**
     * Gets a collection of the command words.
     *
     * @return A collection of all the command words. 
     */
    public Collection<CommandWord> getCommandWords() {
        return validCommands.values();
    }
    
    /**
     * Gets a single command word from the list of command words.
     *
     * @return The command word.
     */
    public CommandWord getCommand(String command) {
        return validCommands.get(command);
    }

}
