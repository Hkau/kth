package org.x2d.zuul;
/**
 * Command word go. This command tries to move the character in the chosen direction.
 */
public class CommandGo extends CommandWord {
    public CommandGo(String commandName) {
        super(commandName);
    }
    
    @Override
    public String[] getTargets(Game game) {
        return game.getPlayer().getCurrentRoom().getExits();
    }
    
    /**
     * @param target The direction the player should go.
     */
    @Override
    public void executeCommand(Game game, String target) {
        if (game.getPlayer().getCurrentRoom().getDoor(target)==null) {
            System.out.println("Det finns inget utgång åt det hållet.");
            return;
        }
        game.getPlayer().goRoom(target);
        if (game.getPlayer().getCurrentRoom() == game.getRoom(Game.Rooms.OUTDOOR)) {
            game.endGame(true);
        }
    }
    
    @Override
    public boolean mustHaveTarget() {
        return true;
    }

}
