package org.x2d.zuul;
import java.util.*;
/**
 * Command word list items. Lists all the items in the player's backpack.
 */
public class CommandList extends CommandWord {
    public CommandList(String commandName) {
        super(commandName);
    }

    /**
     * @param target Never used.
     */
    @Override
    public void executeCommand(Game game, String target) {
        System.out.println("Följande saker finns i din ryggsäck: ");
        Collection<Items> pItems = game.getPlayer().getItems();
        if (pItems.size()==0) {
            System.out.println(" - Inga");
            return;
        }
        for (Items item : pItems) {
            Item itemObject = item.getItem();
            System.out.println(" - "+itemObject.getName()+": "+itemObject.getDescription());
        }
    }

    @Override
    public boolean mustHaveTarget() {
        return false;
    }
}
