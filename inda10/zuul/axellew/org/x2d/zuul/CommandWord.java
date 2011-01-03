package org.x2d.zuul;
/**
 * Command word class. This class should be used as a super class for
 * all command words. There are two kinds of commands. The first type does not need
 * a target. These commands only need to override the executeCommand(String target) method.
 * The second type does need a target and then the executeCommand(String target), mustHaveTarget()
 * and getTargets() must be overriden. Here comes an example:
<pre>
public class CommandMyCommand extends CommandWord {
    public CommandMyCommand(String commandName) {
        super(commandName);
    }
    &#0064;Override
    public String[] getTargets(Game game) {
        return new String[0]; //Should be a list of possible targets
    }
    &#0064;Override
    public void executeCommand(Game game, String target) {
        //Something that should happen when this command word is used
    }
}
</pre>
 */
public abstract class CommandWord
{
    // instance variables - replace the example below with your own
    private String command;

    /**
     * Constructor for objects of class CommandWord
     *
     * @param command The command.
     */
    public CommandWord(String command)
    {
        if (!command.matches("[\\wåäö]+")) {
            throw new IllegalArgumentException("A command word can only use [a-ö,0-9]");
        }
        this.command = command;
    }
    
    /**
     * Gets the command string.
     *
     * @return The command string.
     */
    public String getCommand() {
        return command;
    }
    
    /**
     * Should be overriden. It's here the command gets executed.
     *
     * @param game The game.
     * @param target The target.
     */
    public abstract void executeCommand(Game game, String target);
    
    /**
     * Returns an array with all the possible targets at the current time.
     * This method should be overriden if the command needs a target.
     *
     * @return An array wih all the current possible targets for this command.
     */
    public String[] getTargets(Game game) {
        return new String[0];
    }
    
    /**
     * Should be overriden to say if this command needs a target to work.
     *
     * @return If this command must have a target.
     */
    public boolean mustHaveTarget() {
        return false;
    }
}
