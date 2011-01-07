
import java.util.ArrayList;

/**
 * Inventory class to store several objects of the class Item 
 * 
 * @author Carl
 * @version 1.0
 */
public class Inventory {
	/**
	 *	Will save all of the different items in one ArrayList
	 */
	private ArrayList<Item> inv;
	/**
	 * creates a empty inventory
	 */
	public Inventory() {
		inv = new ArrayList<Item>();
	}
	/**
	 * Adds a item to the inventory
	 * @param newItem
	 */
	public void addItem(Item newItem){
		inv.add(newItem);
	}
	/**
	 * returns the size of the inventoty
	 * @return
	 */
	public int size(){
		return inv.size();
	}
	/**
	 * 	Returns a a Item, will NOT delete it after
	 * 	use method deleteItem for that
	 * 
	 * @param title
	 * @return the Item, if no match then null
	 */
	public Item getItem(String title){
		int index = findIndex(title);
		if(index != -1){
			return inv.get(index);
		}
		else{
			System.err.println("Error: Could not find a match for " + title);
			return null;
		}
	}
	/**
	 * Delete one item in the inventory
	 * @param title
	 */
	public void deleteItem(String title){
		int index = findIndex(title);
		if(index != -1){
			inv.remove(index);
		}
		else{
			System.err.println("Error: Could not find a match for " + title);
		}
	}
	/**
	 * returns a item by the index in the inventory
	 * @param i
	 * @return
	 */
	public Item getItemByIndex(int i){
		return inv.get(0);
	}
	/**
	 * Search the index for a Title, Will only find the first if there
	 * are more then one!
	 * 
	 * @param titel
	 * @return the index of the the title, if not match then -1 
	 */
	private int findIndex(String title){
		for(int r = 0; r < inv.size();r++){
			String temp = inv.get(r).getTitle();
			if(title.equals(temp)){
				return r;
			}
		}
		return (-1);
	}
	/**
	 * @return Ex: "Title", "Descriptions"	
	 */
	public String toString(){
		String temp="";
		if(inv.size() != 0){
			for(int r = 0; r < inv.size();r++){
				if(r == 0){
					temp = inv.get(r).getTitle()+", "+inv.get(r).getDescription();
				}
				else{
					temp = temp + "\n" + inv.get(r).getTitle()+", "+inv.get(r).getDescription();	
				}
			}
		}
		else{
			temp = "-No Items-";
		}
		return temp;
	}	
	/**
	 * Create a new Item and store it in the inventory!
	 * 
	 * @param title
	 * @param description
	 */
	public void createNewItem(String title, String description){
		inv.add(new Item(title,description));
	}
}
