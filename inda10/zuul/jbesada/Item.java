
/**
 * Class Item - an item in the game
 * 
 * Items can both be located in a room or held by the player.
 *
 */
public class Item
{
    private String description;
    private String identification;
    /**
     * Create an item
     * @param itemDescription A description of the item.
     * @param itemIdentification The identy of the item, e.g. ball, car or key.
     */
    public Item(String itemDescription,String itemIdentification){
        description = itemDescription;
        identification = itemIdentification;
    }
    public String getDescription(){
        return description;
    }
    /**
     * Return a full sentence describing the item.
     * @return A long description of the item.
     */
    public String getLongDescription(){
        return "You see " + description;
    }
    public String getIdentification(){
        return identification;
    }
}
