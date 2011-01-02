import java.util.ArrayList;
/**
 * This class contains the inventory and the NPCs following the player
 */
public class Player
{
    //Which NPCs are currently following the player
    ArrayList<NPC> followers;
    //The players inventory
    ArrayList<Item> inventory;

    /**
     * Constructor for objects of class Player
     */
    public Player()
    {
     followers = new ArrayList<NPC>();   
     inventory = new ArrayList<Item>();   
    }
    
    /**
     * Adds an NPC to followers
     * @param NPC: NPC to be added
     */
    public void addFollower(NPC npc) {
        followers.add(npc);
    }
    
    /**
     * Adds items to the inventory
     * @param Item: The item to be added
     */
    public void setItemToInventory(Item thing) {
        inventory.add(thing);
    }
    
    /**
     * Gets how many NPC are currently following the player
     * @return int: Number of NPC in followers
     */
    public int getNumberOfFollowers() {
        return followers.size();
    }
    
    /**
     * Returns the arraylist of followers
     * @return ArrayList<NPC>: The followers the player has gathered so far
     */
    public ArrayList<NPC> getCurrentFollowers() {
        return followers;
    }
    
    /**
     * Returns the arraylist of items currently carried
     * @return ArrayList<Item>: The items the player has gathered so far
     */
    public ArrayList<Item> getCurrentInventory() {
        return inventory;
    }
    
    /**
     * Prints out the names of the followers
     */
    public void printFollowers() {
        for (NPC following : followers) {
            System.out.println(following.getNPCName() + ",");
        }
    }
    
    /**
     * Checks if a certain NPC is following
     * @param NPC: What NPC to check for
     * @return boolean: True if the NPC is following, false otherwise
     */
    public boolean isNPCFollowing(NPC npc) {
        return followers.contains(npc);
    }

}
