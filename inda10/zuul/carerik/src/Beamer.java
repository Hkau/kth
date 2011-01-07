/**
 * Beamer class to use to teleport between roomes
 * @author Carl
 *
 */
public class Beamer extends Item {
	
	private Room savedRoom;
	private int maxUse;
	private int teleporterUsed;

	/**
	 * Create the beamer
	 * @param title
	 * @param description
	 * @param savedRoom
	 * @param maxUse
	 */
	public Beamer(String title, String description, Room savedRoom,int maxUse){
		super(title, description);
		this.setSavedRoom(savedRoom);
		this.setMaxUse(maxUse);
	}
	/**
	 * @param savedRoom the savedRoom to set
	 */
	public void setSavedRoom(Room savedRoom) {
		this.savedRoom = savedRoom;
	}
	/**
	 * @return the savedRoom
	 */
	public Room getSavedRoom() {
		if(teleporterUsed >= maxUse){
			return null;
		}
		teleporterUsed++;
		return savedRoom;
	}
	/**
	 * Check how times it have been used
	 * @return
	 */
	public boolean toManyUses(){
		if(teleporterUsed >= maxUse){
			return true;
		}
		return false;
	}
	/**
	 * set the max uses for the beamer
	 * @param maxUse the maxUse to set
	 */
	public void setMaxUse(int maxUse) {
		this.maxUse = maxUse;
	}
	/**
	 * Return the allowed max uses!
	 * @return the maxUse
	 */
	public int getMaxUse() {
		return maxUse;
	}
	
}
