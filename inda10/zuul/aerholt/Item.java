package game;

/**
 * class Item Keeps track of an item and its state. - name - description
 * 
 * @author Anton Erholt
 * @version 2010.11.24
 */
public class Item {
	private String name;
	private String description;

	/**
	 * Constructor for objects of class Item.
	 */
	public Item(String itemName, String itemDescription) {
		name = itemName;
		description = itemDescription;
	}

	/**
	 * Returns the item description.
	 */
	public String getDescription() {
		return description;
	}

	/**
	 * Returns the name of the item.
	 */
	public String getName() {
		return name;
	}
}
