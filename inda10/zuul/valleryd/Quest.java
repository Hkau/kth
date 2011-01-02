import java.util.ArrayList;
/**
 * Class containing information about the quest
 */
public class Quest
{
    boolean finished = false;
    String type;
    Parser parser = new Parser();

    //Depending on type, these conditions will vary from quest to quest
    int timeCondition;
    NPC followerCondition;
    String riddle;
    String correctAnswer;
    Item itemCondition;

    /**
     * Constructor for objects of class Quest
     * @param int: What time the player needs to exceed to complete the quest
     */
    public Quest(int timeCondition)
    {
        type = "time";
        this.timeCondition = timeCondition;
    }

    /**
     * Constructor for objects of class Quest
     * @param NPC: Which NPC needs to be following to complete the quest
     */
    public Quest(NPC followerCondition)
    {
        type = "follower";
        this.followerCondition = followerCondition;
    }

    /**
     * Constructor for objects of class Quest
     * This is used for quests where NPC asks the player something, wanting a correct answer
     * @param String: The riddle to be asked
     * @param String: The correct answer for the question
     */
    public Quest(String riddle, String correctAnswer)
    {
        type = "riddle";
        this.riddle = riddle;
        this.correctAnswer = correctAnswer;
    }

    /**
     * Constructor for objects of class Quest
     * This is the constructor used for item quests
     * @param Item: The item needed in the inventory to complete quest
     */
    public Quest(Item itemCondition)
    {
        type = "item";
        this.itemCondition = itemCondition;
    }

    /**
     * Checks if the quest is done
     * @return boolean: True if the quest is finished, false if not
     */
    public boolean getQuestFinished() {
        return finished;
    }

    /**
     * This method tries to complete a quest of the type Riddle
     */
    public void tryToFinishRiddleQuest() {
            System.out.println(riddle);
            String answer = parser.getInput();
            if (answer.equals(correctAnswer)){
                System.out.println("Du svarade rätt!");
                finished = true;
            }
            else {
                System.out.println("Det var inte helt rätt...");
                finished = false;
            }
    }

    /**
     * This method tries to complete a quest of the type Time
     * @param int: The time (what hour it is) right now in the game
     */
    public void tryToFinishTimeQuest(int time) {
        if (time >= timeCondition)
            finished = true;
        else
            finished = false;
    }

    /**
     * This method tries to complete a quest of the type Follower
     * @param ArrayList<NPC>: The followers the player has collected so far
     */
    public void tryToFinishFollowerQuest(ArrayList<NPC> currentFollowers) {
        if (currentFollowers.contains(followerCondition)){

            System.out.println("'Där är " + followerCondition.getNPCName() + " ju!'");
            finished = true;
        }
        else
            finished = false;
    }

    /**
     * This method tries to complete a quest of the type Item
     * @param ArrayList<Item>: The items the player has collected so far
     */
    public void tryToFinishItemQuest(ArrayList<Item> currentInventory){
        if (currentInventory.contains(itemCondition)) {
            System.out.println("Du överlämnar " + itemCondition.getDescription() +".");
            finished = true;
        }
        else
            finished = false;
    }

    /**
     * Checks which type the quest is
     * @return String: What type the quest is
     */
    public String getType() {
        return type;
    }
}
