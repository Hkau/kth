/**
 * Representations for all the valid command words for the game
 * along with a string in a particular language.
 */
public enum CommandWord
{
    // A value for each command word along with its
    // corresponding user interface string.

    //LÄGGA TILL EGNA?
    GO("gå"), QUIT("sluta"), HELP("hjälp"), UNKNOWN("?"), TAKE ("ta"), TALK("tala"), WAIT("vänta");

    // The command string.
    private String commandString;

    /**
     * Initialise with the corresponding command word.
     * @param commandWord The command string.
     */
    CommandWord(String commandString)
    {
        this.commandString = commandString;
    }

    /**
     * @return The command word as a string.
     */
    public String toString()
    {
        return commandString;
    }
}
