/**
 * The game parses through an XML-file with item and room definitions
 * and transfer them into a "map".
 * 
 * @author Björn Holm
 * @version 03-12-2010
 */
import java.util.Scanner;
import java.util.HashMap;
import org.w3c.dom.Node;
import java.util.Set;

public class Game extends Super {
    public static void main(String[] args) {
        Game game;
        if (args.length == 0) {
            String[] xml = {"map.xml"};
            game = new Game(xml);
        }
        else
            game = new Game(args);
        game.play();
    }
    
    private HashMap<String, Room> rooms = new HashMap<String, Room>();
    private HashMap<String, Item> allItems = new HashMap<String, Item>();
    private Scanner reader = new Scanner(System.in);
    private boolean playing = true;
    private String currentRoomKey;
    private Room currentRoom;
    private Node onPrompt;
    private Node onStart;
    private Node onEnterRoom;
    private Node onUnknownCommand;
    private XMLParser xmlParser;
    
    /**
     * Creates a game based on passed XML-files
     */
    public Game(String[] xmlFiles) {
        xmlParser = new XMLParser(this);
        for (String xmlFile : xmlFiles)
            xmlParser.parseFile(xmlFile);
    }
    
    /**
     * The (almost) infinite loop of the game.
     */
    public void play() {
        xmlParser.run(onStart);
        while (playing) {
            String commandName = xmlParser.run(onPrompt);
            //get commandName
            //check if it's a 'pickup ...' command
            if (commandName.length() > 8 && commandName.substring(0, 7).equals("pickup ")) {
                String itemName = commandName.substring(7);
                Item item = currentRoom.getItem(itemName);
                if (item == null)
                    xmlParser.run(onUnknownCommand);
                else {
                    currentRoom.removeItem(itemName);
                    addItem(itemName, item);
                }
                continue;
            }
            
            //else let the associated command defined in the XML run
            Node command = getAvailableCommands().get(commandName);
            if (command == null)
                xmlParser.run(onUnknownCommand);
            else
                xmlParser.run(command);
        }
    }
    
    
    //All of the following functions are used by the XMLparser to add items and such
    public String prompt() {
        return reader.nextLine();
    }
    public void quit() {
        playing = false;
    }
    public HashMap<String, Node> getAvailableCommands() {
        HashMap<String, Node> allCommands = new HashMap<String, Node>();
        allCommands.putAll(currentRoom.getCommands());
        allCommands.putAll(getCommands());
        return allCommands;
    }
    public String getCommandList() {
        return getNiceList(getAvailableCommands());
    }
    public String getCurrentRoomItemList(String separator) {
        String list = getNiceList(currentRoom.getItems());
        if (list == "")
            return "";
        return separator+"\n"+list;
    }
    public String getNiceList(HashMap hashMap) {
        String returnString = "";
        String[] keys = (String[])hashMap.keySet().toArray(new String[0]);
        sortByLength(keys);
        int elements = keys.length;
        if (elements == 0)
            return "";
        int[] columnWidths = {keys[elements-1].length()};
        int columnHeight = elements;
        for (int i=1; i <= elements; i++) {
            int combinedKeyLength = 0;
            int tempColumnHeight = (elements+i-1)/i;
            int[] tempColumnWidths = new int[i];
            for (int j=0; j < i; j++) {
                tempColumnWidths[j] = keys[Math.min(tempColumnHeight*(j+1), elements)-1].length();
                combinedKeyLength += tempColumnWidths[j]+4;
            }
            if (combinedKeyLength > 50)
                break;
            columnHeight = tempColumnHeight;
            columnWidths = tempColumnWidths;
        }
        for (int row=0; row < columnHeight; row++) {
            for (int column = 0; column < columnWidths.length; column++) {
                int element = column*columnHeight+row;
                if (element >= elements)
                    break;
                returnString += keys[element];
                for (int i=keys[element].length(); i < columnWidths[column]+4; i++)
                    returnString += " ";
            }
            returnString += "\n";
        }
        return returnString;
    }
    public Room getRoom(String roomName) {
        return rooms.get(roomName);
    }
    public String getCurrentRoomDescription() {
        return currentRoom.getDescription();
    }
    public String getCurrentRoomKey() {
        return currentRoomKey;
    }
    public HashMap<String, Item> getAllItems() {
        return allItems;
    }
    public void newItem(String key, String description, Node onPickup,
                        Node onDrop, Node onEquip, Node onUnequip) {
        allItems.put(key, new Item(description, onPickup, onDrop, onEquip, onUnequip));
    }
    public void addItem(String itemName, Item item) {
        items.put(itemName, item);
        xmlParser.run(item.getOnPickup());
    }
    public Item getAnyItem(String key) {
        return allItems.get(key);
    }
    public void setOnPrompt(Node run) {
        onPrompt = run;
    }
    public void setOnUnknownCommand(Node run) {
        onUnknownCommand = run;
    }
    public void addRoom(String key, String description, Node onEnter, Node onExit) {
        rooms.put(key, new Room(description, onEnter, onExit));
    }
    public void runCommand(String command) {
        xmlParser.run(getAvailableCommands().get(command));
    }
    public void enter(String roomName) {
        Room newRoom = rooms.get(roomName);
        if (newRoom == null)
            System.err.println("Room '"+roomName+"' not found");
        else {
            Room oldRoom = currentRoom;
            currentRoom = newRoom;
            currentRoomKey = roomName;
            if (oldRoom != null)
                xmlParser.run(oldRoom.getOnExit());
            xmlParser.run(currentRoom.getOnEnter());
        }
    }
    public void sortByLength(String[] a) {
        for (int i=1; i < a.length; i++) {
            int j=i;
            String s = a[j];
            int sLength = s.length();
            for (; j>0 && a[j-1].length() > sLength; j--)
                a[j] = a[j-1];
            a[j] = s;
        }
    }
    public void addRoomCommand(String roomName, String command, Node run) {
        Room room = rooms.get(roomName);
        if (room == null)
            System.err.println("room: '"+roomName+"' does not exist. (yet)");
        else
            room.addCommand(command, run);
    }
    public void setOnStart(Node run) {
        onStart = run;
    }
    public void setCurrentRoom(String roomName) {
        currentRoom = rooms.get(roomName);
    }
}