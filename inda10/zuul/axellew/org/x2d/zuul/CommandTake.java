package org.x2d.zuul;
import java.util.*;
/**
 * Command word take. This command is used when the player is trying to take an item and put it into
 * the backpack.
 */
public class CommandTake extends CommandWord {
    public CommandTake(String commandName) {
        super(commandName);
    }
    
    @Override
    public String[] getTargets(Game game) {
        Collection<Items> rItems = game.getPlayer().getCurrentRoom().getItems();
        ArrayList<String> items = new ArrayList<String>();
        for (Items i : rItems) {
            if (i.getItem().isTakable()) {
                items.add(i.getItem().getName());
            }
        }
        return items.toArray(new String[0]);
    }
    
    /**
     * @param target The name of the item which the player should try to take.
     */
    @Override
    public void executeCommand(Game game, String target) {
        Items item = Items.getItem(target);
        if (!game.getPlayer().getCurrentRoom().hasItem(item)) {
            System.out.println(String.format("Det finns inget föremål med namnet '%s' i det här rummet.", target));
            return;
        }
        if (!item.getItem().isTakable()) {
            System.out.println("Du kan inte ta med dig: "+target);
            return;
        }
        game.getPlayer().getCurrentRoom().removeItem(item);
        game.getPlayer().addItem(item);
    }
    @Override
    public boolean mustHaveTarget() {
        return true;
    }

}
