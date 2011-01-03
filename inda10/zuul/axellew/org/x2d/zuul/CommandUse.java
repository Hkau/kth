package org.x2d.zuul;
import java.util.*;
/**
 * Command word use. This command is used when the player is trying to use an item.
 */
public class CommandUse extends CommandWord {
    public CommandUse(String commandName) {
        super(commandName);
    }

    @Override
    public String[] getTargets(Game game) {
        Collection<Items> pItems = game.getPlayer().getItems();
        Collection<Items> rItems = game.getPlayer().getCurrentRoom().getItems();
        ArrayList<String> items = new ArrayList<String>();
        checkUsable(items, pItems);
        checkUsable(items, rItems);
        return items.toArray(new String[0]);
    }
    
    //getTarget help method.
    private void checkUsable(ArrayList<String> items, Collection<Items> itemCollection) {
        for (Items i : itemCollection) {
		    if (i.getItem().isUsable()) {
		        items.add(i.getItem().getName());
		    }
        }
    }
    /**
     * @param target The name of the item which the player should try to use.
     */
    @Override
    public void executeCommand(Game game, String target) {
        Items item = Items.getItem(target);
        if (game.getPlayer().getCurrentRoom().hasItem(item)) {
            if (game.getPlayer().hasItem(item)) {
                System.out.println(String.format("Det finns inget föremål med namnet '%s' i det här rummet eller i din ryggsäck.", target));
                return;
            }
        }
        if (!item.getItem().isUsable()) {
            System.out.println("Det går inte att använda "+target);
            return;
        }
        item.getItem().use(game);
        
    }
    @Override
    public boolean mustHaveTarget() {
        return true;
    }

}
