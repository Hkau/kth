import java.util.HashMap;
import org.w3c.dom.Node;

/**
 * This Super class has everything you might need.
 * The player, every item and every room can in turn contain
 * these: variables, commands, items and a description.
 */
public class Super {
    protected HashMap<String, String> variables = new HashMap<String, String>();
    protected HashMap<String, Node> commands = new HashMap<String, Node>();
    protected HashMap<String, Item> items = new HashMap<String, Item>();
    protected String description;
    
    public String getDescription() {
        return description;
    }
    
    
    public void setVariable(String variableName, String value) {
        variables.put(variableName, value);
    }
    public String getVariable(String variableName) {
        return variables.get(variableName);
    }
    
    
    public void addCommand(String commandName, Node run) {
        commands.put(commandName, run);
    }
    public void removeCommand(String commandName) {
        commands.remove(commandName);
    }
    public HashMap<String, Node> getCommands() {
        HashMap<String, Node> allCommands = new HashMap<String, Node>();
        allCommands.putAll(commands);
        for (Item item : items.values())
            allCommands.putAll(item.getCommands());
        return allCommands;
    }
    public Node getCommand(String commandName) {
        Node command = getCommands().get(commandName);
        if (command == null)
            System.err.println("Command '"+commandName+"' not recognized in "+description);
        return command;
    }
    
    
    public void addItem(String itemName, Item item) {
        items.put(itemName, item);
    }
    public void removeItem(String itemName) {
        items.remove(itemName);
    }
    public HashMap<String, Item> getItems() {
        return items;
    }
    public Item getItem(String itemName) {
        Item item = getItems().get(itemName);
        if (item == null)
            System.err.println("Item '"+itemName+"' not recognized in "+description);
        return item;
    }
}