import java.util.Stack;
import java.util.ArrayList;
/**
 * The player class holds information about the player such as current room, previous rooms and all the carried items.
 */
public class Player
{
    // instance variables - replace the example below with your own
   private Room currentRoom;
   private Stack<Room> previousRooms;
   private ArrayList<Item> items;

    /**
     * Create a new player with no items and no previously visited rooms.
     */
    public Player()
    {
        previousRooms = new Stack<Room>();
        items = new ArrayList<Item>();
        
    }
    /**
     * Stores the current room in the stack
     */
    public void storeCurrentRoom(){
        previousRooms.push(currentRoom);
    }
    /**
     * Adds an item to the players inventory.
     * @param item The item to add to the inventory.
     */
    public void addItem(Item item){
        items.add(item);
    }
    /**
     * Removes an item to the players inventory.
     * @param index The index of the item in the items ArrayList.
     */
    public void removeItem(int index){
        items.remove(index);
    }
    public ArrayList<Item> getItems(){
        return items;
    }
    public Room getCurrentRoom(){
        return currentRoom;
    }
    /**
     * Returns the previously visited room, if there is one.
     * @return Previously visited room, null if the current room is the first visited room.
     */
    public Room getPreviousRoom(){
        if(!previousRooms.empty()){
            return previousRooms.peek();
        }else{
            return null;
        }
    }
    /**
     * Returns the previous room, if there is one, and deletes it from the stack
     * @return Previously visited room, null if the current room is the first visited room.
     */
    public Room popPreviousRoom(){
         if(!previousRooms.empty()){
            return previousRooms.pop();
        }else{
            return null;
        }
    }
   
    public void setCurrentRoom(Room room){
        currentRoom = room;
    }
    

  
}
