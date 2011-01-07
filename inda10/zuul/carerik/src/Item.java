
/**
 * Item class that makes a simple item
 * @author Carl
 *
 */
public class Item {
	
	private String title;
	private String description;

	/**
	 * Creates the item with a title and a discription
	 * @param title
	 * @param description
	 */
	public Item(String title, String description){
		this.description = description;
		this.title = title;
	}

	/**
	 * @param description the description to set
	 */
	public void setDescription(String description) {
		this.description = description;	
	}
	/**
	 * @return the description
	 */
	public String getDescription() {
		return description;
	}
	/**
	 * @param title the title to set
	 */
	public void setTitle(String title) {
		this.title = title;
	}
	/**
	 * @return the title
	 */
	public String getTitle() {
		return title;
	}
}


