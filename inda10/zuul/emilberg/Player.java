import java.util.ArrayList;

/**
 * Describes a player.
 * 
 * @author Emil Berg
 * @version 1.00
 */
public class Player
{
    // The players name.
    private String name;
    // list with all items.
    private ArrayList<Item> items;
    // The room the player is in.
    private Room currentRoom;
    // The number of moves between rooms the player can make before the player dies.
    private int movesLeft;
    
    /**
     * Creates a player.
     * @param name The players name.
     * @param totalNumberOfMoves The total number of moves the player will be able
     * to make before the player dies.
     */
    public Player(String name, int totalNumberOfMoves)
    {
        items = new ArrayList<Item>();
        this.name = name;
        this.movesLeft = totalNumberOfMoves;
    }
    
    /**
     * Returns the players name.
     */
    public String getName() {
        return name;
    }
    
    /**
     * Eats an item.
     * @param item The item to eat.
     * @return The item if it was edible, or null if it didn't exist or was unedible.
     */
    public Item eat(String itemName) {
        Item item = getItem(itemName);
        
        if (item == null) {
            return null;
        }
        
        // Controls if the item is food.
        if (item instanceof FoodItem) {
            FoodItem food = (FoodItem)item;
            movesLeft += food.getEnergy();
            return removeItem(itemName);
        }
        
        return null;
    }
    
    /**
     * Picks up an item.
     * @param item The item to pick up.
     * @return True if the item exists.
     */
    public boolean pickUpItem(String itemName) {
        Item i = currentRoom.takeItem(itemName);
        if (i == null) {
            return false;
        }
        
        items.add(i);
        
        return true;
    }
    
    /**
     * Removes an item from the player.
     * @param item The name of the item to remove.
     * @return The removed item, or null if it didn't exist.
     */
    public Item removeItem(String itemName) {
        return items.remove(items.indexOf(getItem(itemName)));
    }
    
    /**
     * Moves the player through an exit.
     * @param exit The exit to move through.
     * @return True if it was possible.
     */
    public boolean move(Exit exit) {
        if (exit.opened()) {
            currentRoom = exit.getRoom();
            movesLeft--;
            return true;
        }
        
        return false;
    }
    
    /**
     * Teleports the player to a room.
     * @param room The room to teleport to.
     */
    public void teleport(Room room) {
        currentRoom = room;
    }
    
    /**
     * Returns the room the player is in.
     * @return The current room.
     */
    public Room getRoom() {
        return currentRoom;
    }
    
    /**
     * Controls if the player has the item.
     * @param itemName The name of the item.
     * @return The item, or null if it doesn't exist.
     */
    public Item getItem(String itemName) {
        for (Item item : items) {
            if (item.getName().equals(itemName)) {
                return item;
            }
        }
        return null;
    }
    
    /**
     * @return The unlocked exit, or null if it couldn't be opened.
     */
    public Exit unlockDoor(Exit exit) {
        Item key = exit.getKey();
        
        if (key != null) {
            exit.unlock();
            removeItem(key.getName());
            return exit;
        }
        return null;
    }
    
    /**
     * Returns the players inventory.
     */
    public String listInventory() {
        
        if (items.size() == 0) {
            return "There are no items in your inventory.";
        }
        
        String inventoryString = "\n";
        for (Item item : items) {
            inventoryString += item.getName() + "\n";
        }
        
        return inventoryString;
    }
    
    /**
     * @return The number of moves between rooms made by the player.
     */
    public int getMovesLeft() {
        return movesLeft;
    }
}
