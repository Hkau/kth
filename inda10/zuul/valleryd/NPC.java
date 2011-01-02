import java.util.HashMap;
import java.util.Set;

/**
 * Containing information about the NPCs in this game
 * 
 * @author Matilda Valleryd 
 * @version 2010.12.01
 */
public class NPC {
    private String name;
    private String description;
    private String descriptionBefore;
    private String descriptionAfter;
    private Quest quest = null;
    

    /**
     * Constructor for objects of class NPC
     * @param String: The NPC name
     * @param String: The NPCs description
     * @param String: Description before the NPCs quest is finished
     * @param String: Description after the NPCs quest is finished
     * @param Quest: The NPCs quest
     */
    public NPC(String name, String description, String descriptionBefore, String descriptionAfter, Quest quest)
    {
        // initialise instance variables
        this.name = name;
        this.description = description;
        this.descriptionBefore = descriptionBefore;
        this.descriptionAfter = descriptionAfter;
        this.quest = quest;
    }
    
    /**
     * Get the name of the NPC
     * @return String: The NPCs name
     */
    public String getNPCName() {
        return name;
    }  
    
    /**
     * Get the description of the NPC
     * @return String: The NPCs description
     */
    public String getNPCDescription() {
        return description;
    }    
    
    /**
     * Get the description when the quest isn't finished
     * @return String: The NPCs description before the quest is finished
     */
    public String getNPCDescriptionBefore() {
        return descriptionBefore;
    }
    
    /**
     * Get the description when the quest is finished
     * @return String: The NPCs description after the quest is finished
     */
    public String getNPCDescriptionAfter() {
        return descriptionAfter;
    }
    
    /**
     * Checks if the NPC has a quest
     * @return boolean: True if the NPC has a quest, false otherwise
     */
    public boolean checkForQuest() {
        if (quest == null)
            return false;
        else
            return true;
    }
    
    /**
     * Get the quest
     * @return Quest: The NPCs quest
     */
    public Quest getQuest() {
        return quest;
    }

}
