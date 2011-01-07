
/**
 * A NPC that will be able to speak and have it's own inventory
 * 
 * @author Carl
 * @version 0.1
 */
public class NPC {

	private Inventory inven;
	private String name;
	private String[] phrase;
	
	public NPC() {
		inven = new Inventory();
		phrase = new String[3];
	}
	/**
	 * @param name the name to set
	 */
	public void setName(String name) {
		this.name = name;
	}
	/**
	 * @return the name
	 */
	public String getName() {
		return name;
	}
	/**
	 * Get a specific item from the NPC inventory
	 * if there was no match it will return null
	 * 
	 * @param titel The title of the item
	 * @return The item that was requested
	 */
	public Item getNPCItem(String title){
		Item temp = inven.getItem(title);
		inven.deleteItem(title);
		return temp;
	}
	/**
	 * Will add one item to the NPC's inventory
	 * 
	 * @param newItem
	 */
	public void addNPCItem(Item newItem){
		inven.addItem(newItem);
	}
	/**
	 * Set what the npc can say
	 * 
	 * @param welcome
	 * @param answer
	 * @param bye
	 */
	public void setPhrase(String welcome, String answer, String bye){
		phrase[0] = welcome;
		phrase[1] = answer;
		phrase[2] = bye;
	}
	/**
	 * Get the phrases that are stored
	 * 
	 * @param pos 1 = welcome, 2 = answer, 3 = bye
	 * @return
	 */
	public String speak(int pos){
		if(pos >= 0 && pos <=3){
			return phrase[pos];
		}
		else{
			System.err.println("Wrong input parameter, null was returned");
			return null;
		}
	}
}
