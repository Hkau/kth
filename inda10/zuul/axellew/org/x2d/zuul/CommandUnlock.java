package org.x2d.zuul;
import java.util.*;
/**
 * Command word unlock. This command is used when the player is trying to unlock a door.
 */
public class CommandUnlock extends CommandWord {
    public CommandUnlock(String commandName) {
        super(commandName);
    }
    
   @Override
    public String[] getTargets(Game game) {
        Room cRoom = game.getPlayer().getCurrentRoom();
        String[] exits = cRoom.getExits();
        ArrayList<String> tmpArray = new ArrayList<String>();
        for (String exit : exits) {
            Door door = cRoom.getDoor(exit);
            Items unlockItem = door.getUnlockItem();
            if (door.isLocked()&&unlockItem!=null&&game.getPlayer().hasItem(unlockItem)) {
                tmpArray.add(exit);
            }
        }
        return tmpArray.toArray(new String[0]);
    }
    
    /**
     * @param target The name of the direction which the player should try to unlock a door in.
     */
    @Override
    public void executeCommand(Game game, String target) {
        Door door = game.getPlayer().getCurrentRoom().getDoor(target);
        if (door==null) {
            System.out.println("Det finns inget att låsa upp åt "+target);
            return;
        }
        System.out.println(String.format("Dörren i %s har blivit upplåst", target));
        door.setLocked(false);
    }
    @Override
    public boolean mustHaveTarget() {
        return true;
    }

}
