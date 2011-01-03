package org.x2d.zuul;
import java.util.*;
/**
 * Command word read. Used to read items which isReadable() returns true.
 * The target paramenter should be the name of an item either in the
 * player's backpack or in the room.
 */
public class CommandRead extends CommandWord {
    public CommandRead(String commandName) {
        super(commandName);
    }
    
    @Override
    public String[] getTargets(Game game) {
        //Checks both the player's backpack and the current room.
        Collection<Items> pItems = game.getPlayer().getItems();
        Collection<Items> rItems = game.getPlayer().getCurrentRoom().getItems();
        ArrayList<String> items = new ArrayList<String>();
		checkReadable(items, pItems);
		checkReadable(items, rItems);
        return items.toArray(new String[0]);
    }
    
    //getTarget help method.
    private void checkReadable(ArrayList<String> items, Collection<Items> itemCollection) {
        for (Items i : itemCollection) {
		    if (i.getItem().isReadable()) {
		        items.add(i.getItem().getName());
		    }
        }
    }
    
    /**
     * @param target The name of the item which the player should try to read from.
     */
    @Override
    public void executeCommand(Game game, String target) {
        Items item = Items.getItem(target);        
        if (!game.getPlayer().hasItem(item)) {
            if (!game.getPlayer().getCurrentRoom().hasItem(item)) {
                System.out.println(String.format("Det finns inget föremål med namnet '%s' i det här rummet eller i din ryggsäck.", target));
                return;
            }
        }
        if (!item.getItem().isReadable()) {
            System.out.println("Det finns inget att läsa på: "+target);
            return;
        }
        System.out.println("Följande står skrivet:");
        System.out.println(item.getItem().getText());
        
    }
    @Override
    public boolean mustHaveTarget() {
        return true;
    }

}
