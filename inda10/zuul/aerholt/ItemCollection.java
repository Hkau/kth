package game;

import java.util.ArrayList;
import java.util.Iterator;

/**
 * class ItemCollection
 * Handles items, inventories and such by keeping them in an ArrayList.
 * Sets and gets items. 
 * Removes items. 
 * You name it... With items!
 * 
 * @author Anton Erholt
 * @version 2010.11.27
 */
public class ItemCollection {

	private ArrayList<Item> items;
	
	/**
	 * Constructor.
	 */
	public ItemCollection(){	
		items = new ArrayList<Item>();
	}
	
	/**
	 * Checks if an item exists in the collection.
	 * @param itemName The name of the item to be checked for.
	 * @return true if it exists, otherwise false
	 */
	public boolean itemExists(String itemName) {
		for(Item item: items) {
			if(item.getName().equals(itemName)) {
				return true;
			}
		}
		return false;
	}
	
	 /**
     * Removes an item from the collection.
     * @param itemToRemove the item to be removed.
     */
    public void removeItem(Item itemToRemove){
        if(items.contains(itemToRemove)){
            Iterator<Item> itr = items.iterator();            
            while(itr.hasNext()){
                if(itemToRemove.equals(itr.next())){
                    itr.remove();
                }                
            }
        }
    }
    
    /**
     * Gets an item from the handler. Does not remove the item.
     * @return null if not found, else the wanted item.
     */
    public Item getItem(Item itemToGet) {
    	for(Item item:items) {
        	if(item.equals(itemToGet))
        		return item;        	
    	}
    	return null;
    }
    
    /**
     * Gets an item by its name. Does not remove the item.
     * @return null if it's not found, else the wanted item.
     */
    public Item getItemByName(String itemName) {
    	for(Item item:items) {
    		if(item.getName().equals(itemName)) {
    			return item;
    		}
    	}
    	return null;
    }
    
    /**
     * Adds an item to the handler.
     */
    public void addItem(Item item){
        items.add(item);
    }    
    
    /**
     * Gets the name of the items as a String. Returns an empty String if there are no items.
     * @return itemString A String with the current items kept. 
     */
    public String getItemNamesString(){
    	String itemString = new String();
    	itemString ="";
    	
    	for(Item item:items) {
    		itemString += item.getName() + " ";
    	}
    	return itemString;
    }
	
}
