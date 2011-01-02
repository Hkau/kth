/**
 * A 'describe' command to describe a lot of things.
 * 
 * @author Emil Berg
 * @version 1.00
 */
public class DescribeCommand extends Command
{

    // The commandWords.
    private CommandWords commandWords;

    /**
     * Creates a Command of type Describe.
     * @param description The description of the command.
     * @param commandWords The possible command words in the game.
     */
    public DescribeCommand(String description, CommandWords commandWords) {
        super(description);
        this.commandWords = commandWords;
    }

    /**
     * Decribes an item in the room, an item in the players inventory, a room or a command.
     * If there is no second command word, it prints a suitable message. If there is no item
     * in the room, item in the inventory or command it prints a suitable message.
     * @param player The player.
     * @return true if the game should end.
     */
    public boolean apply(Player player) {
        if(!hasSecondWord()) {
            System.out.println("Describe what?");
            return false;
        }
        
        Command command = commandWords.get(getSecondWord());
        if (command != null) {
            System.out.println(command.getDescription());
            return false;
        }
        
        if (getSecondWord().equals("room")) {
            System.out.println(player.getRoom().getLongDescription());
        } else {
        
            Item itemPlayer = player.getItem(getSecondWord());
            Item itemRoom = player.getRoom().getItem(getSecondWord());
        
            if (itemPlayer == null && itemRoom == null) {
                System.out.println("There is no such item or command.");
            } else {
        
                if (itemPlayer != null) {
                    System.out.println(itemPlayer.getDescription());
                } else {
                    System.out.println(itemRoom.getDescription());
                }
            }
        }
        
        return false;
    }
}
