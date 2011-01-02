/**
 * A 'go' command. Let's the player move to an adjacent room.
 * 
 * @author Emil Berg
 * @version 1.00
 */
public class GoCommand extends Command
{
    /**
     * Creates a Command of type Go.
     * @param description The description of the command.
     */
    public GoCommand(String description) {
        super(description);
    }

    /**
     * Lets the player move to a room if the second command word is valid. If the
     * command word isn't valid it prints out a suitable message. If there is no 
     * room in that direction it also print out a suitable message.
     * @param player The player to move.
     * @return true if the game should end.
     */
    public boolean apply(Player player) {
        if(!hasSecondWord()) {
            // if there is no second word, we don't know where to go...
            System.out.println("Go where?");
            return false;
        }

        String direction = getSecondWord();
        
        Exit exit = player.getRoom().getExit(ConvertStringToExitDirection(direction));
        
        if (exit == null) {
            System.out.println("That's impossible.");
        }
        else {
            // If the player could move through the exit
            if (player.move(exit)) {
                // If the player has moved too far.
                if (player.getMovesLeft() <= 0) {
                    System.out.println("Your legs are tired. You sit down and wait for death. ");
                    return true;
                }
                System.out.println(exit.getWalkThroughMessage());
                System.out.println(exit.getRoom().getLongDescription());
            } else {
                System.out.println("The door is locked!");
            }
        }
        
        return false;
    }
    
    /**
     * Converts a String to the suitable Exitdireciton, for instance
     * "east" --> Room.ExitDirection.EAST.
     * @param direction the direction String
     * @return the direction converted to Room.ExitDirection, or null
     */
    private Room.ExitDirection ConvertStringToExitDirection(String direction) {
        Room.ExitDirection exitDir;
        try {
            // converts String to enum
            exitDir = Room.ExitDirection.valueOf(direction.toUpperCase());
        }
        catch(Exception exception) {
            return null;
        }
        
        return exitDir;
    }
}
