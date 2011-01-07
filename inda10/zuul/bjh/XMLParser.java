import java.io.File;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.w3c.dom.NamedNodeMap;
import java.util.HashSet;
import java.util.Arrays;

/**
 * Translates XML-files into nodes and parses them.
 */
public class XMLParser {
    private static final short UNKNOWN = 0;
    private static final short TEXT = 1;
    private static final short WHITESPACES = 2;
    private static final short COMMENT = 3;
    private static final short NODES = 4;
    private static final short EMPTY = 5;
    private static final HashSet<String> knownTags = new HashSet<String>(
        Arrays.asList(
            "root","run","onEnter","onExit","description","name",
            "value","operator","operand1","operand2","return",
            "command","then","else","room"
        )
    );
    private Node defaultOnEnterRoom;
    private Node defaultOnExitRoom;
    private Node defaultOnPickupItem;
    private Node defaultOnDropItem;
    private Node defaultOnEquipItem;
    private Node defaultOnUnequipItem;
    private Game game;
    
    public XMLParser(Game game) {
        this.game = game;
    }
    
    /**
     * Takes a file, creates a node-tree from it and parses through it.
     * @param fileName file name
     */
    public void parseFile(String fileName) {
        Document doc = null;
        try {
            File file = new File(fileName);
            DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
            DocumentBuilder db = dbf.newDocumentBuilder();
            doc = db.parse(file);
            doc.getDocumentElement().normalize();
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        Node root = getChild(doc, "root");
        parse(root);
    }
    /**
     * Parses through a node, ignoring the node's own name.
     */
    public String run(Node inputNode) {
        return parseChildNodes(inputNode);
    }
    /**
     * This is where the magic happens, parses through a node,
     * and based upon node-name, expects certain sub-nodes and
     * parses them too.
     * 
     * @param inputNode The node (with sub-nodes) to be parsed
     * @return Either a combined String from every sub-node, or the string from a subnode with the name return
     */
    private String parse(Node inputNode) {
        switch (contents(inputNode)) {
            case WHITESPACES:
            case COMMENT:
            //case EMPTY:
            case UNKNOWN:
                return "";
            case TEXT:
                return inputNode.getTextContent();
        }
        String name = inputNode.getNodeName();
        
        
        
        
        if (name.equals("print"))
            System.out.print(parseChildNodes(inputNode));
        
        else if (name.equals("println"))
            System.out.println(parseChildNodes(inputNode));
        
        else if (name.equals("if")) {
            String condition = parseChild(inputNode, "condition");
            if (condition.equals("true"))
                return parseChild(inputNode, "then");
            else
                return parseChild(inputNode, "else");
        }
        
        else if (name.equals("condition")) {
            String operand1 = parseChild(inputNode, "operand1");
            String operand2 = parseChild(inputNode, "operand2");
            String operator = parseChild(inputNode, "operator");
            if (operator.equals("==")) {
                if (operand1.equals(operand2))
                    return "true";
                return "false";
            } else if (operator.equals("!=")) {
                if (operand1.equals(operand2))
                    return "false";
                return "true";
            }
        }
        
        else if (name.equals("import"))
            parseFile(parseChildNodes(inputNode));
        
        else if (name.equals("add") || name.equals("set")) {
            String what = getAttribute(inputNode, "what");
            String in = getAttribute(inputNode, "in");
            String named = getAttribute(inputNode, "named", false);
            String key = getAttribute(inputNode, "key");
            Super target = getSuper(in, named);
            if (what.equals("variable"))
                target.setVariable(key, parseChildNodes(inputNode));
            else if (what.equals("command"))
                target.addCommand(key, inputNode);
            else if (what.equals("item")) {
                target.addItem(key, game.getAnyItem(key));
            }
            else
                System.err.println("<set|add what='"+what+"' ...> not recognized");
        }
        
        else if (name.equals("get")) {
            String what = getAttribute(inputNode, "what");
            String in = getAttribute(inputNode, "in");
            String named = getAttribute(inputNode, "named", false);
            String key = getAttribute(inputNode, "key");
            Super target = getSuper(in, named);
            if (what.equals("variable"))
                return target.getVariable(key);
            else
                System.err.println("<get what='"+what+"' ...> not recognized");
        }
        
        else if (name.equals("remove")) {
            String what = getAttribute(inputNode, "what");
            String in = getAttribute(inputNode, "in");
            String named = getAttribute(inputNode, "named", false);
            String key = getAttribute(inputNode, "key");
            Super target = getSuper(in, named);
            if (what.equals("variable"))
                target.setVariable(key, null);
            else if (what.equals("command"))
                target.removeCommand(key);
            else if (what.equals("item")) {
                target.removeItem(key);
            }
            else
                System.err.println("<set|add what='"+what+"' ...> not recognized");
        }
        
        else if(name.equals("newItem")) {
            String key = getAttribute(inputNode, "key");
            String description = parseChild(inputNode, "description");
            Node onPickup = getChild(inputNode, "onPickup", false);
            if (onPickup == null)
                onPickup = defaultOnPickupItem;
            Node onDrop = getChild(inputNode, "onDrop", false);
            if (onDrop == null)
                onDrop = defaultOnDropItem;
            Node onEquip = getChild(inputNode, "onEquip", false);
            if (onEquip == null)
                onEquip = defaultOnEquipItem;
            Node onUnequip = getChild(inputNode, "onUnequip", false);
            if (onUnequip == null)
                onUnequip = defaultOnUnequipItem;
            game.newItem(key, description, onPickup, onDrop, onEquip, onUnequip);
        }
        else if (name.equals("getCurrentRoomDescription")) {
            return game.getCurrentRoomDescription();
        }
        else if (name.equals("getCurrentRoomKey")) {
            return game.getCurrentRoomKey();
        }
        else if (name.equals("getCurrentRoomItemList")) {
            return game.getCurrentRoomItemList(parseChildNodes(inputNode));
        }
        
        else if (name.equals("getCommandList")) {
            return game.getCommandList();
        }
        else if (name.equals("runCommand")) {
            String command = parseChildNodes(inputNode);
            game.runCommand(command);
        }
        else if (name.equals("addRoom")) {
            String roomName = getAttribute(inputNode, "key");
            String description = parseChild(inputNode, "description");
            Node onEnter = getChild(inputNode, "onEnter", false);
            Node onExit = getChild(inputNode, "onExit", false);
            if (onEnter == null)
                onEnter = defaultOnEnterRoom;
            if (onExit == null)
                onExit = defaultOnExitRoom;
            game.addRoom(roomName, description, onEnter, onExit);
        }
        else if (name.equals("onStart")) {
            game.setOnStart(inputNode);
        }
        else if (name.equals("defaultOnEnterRoom"))
            defaultOnEnterRoom = inputNode;
        
        else if (name.equals("defaultOnExitRoom"))
            defaultOnExitRoom = inputNode;
        
        else if (name.equals("defaultOnPickupItem"))
            defaultOnPickupItem = inputNode;
        
        else if (name.equals("defaultOnDropItem"))
            defaultOnDropItem = inputNode;
        
        else if (name.equals("defaultOnEquipItem"))
            defaultOnEquipItem = inputNode;
        
        else if (name.equals("defaultOnUnequipItem"))
            defaultOnUnequipItem = inputNode;
        
        else if (name.equals("onUnknownCommand"))
            game.setOnUnknownCommand(inputNode);
        
        else if (name.equals("onPrompt"))
            game.setOnPrompt(inputNode);
        
        else if (name.equals("prompt"))
            return game.prompt();
        
        else if (name.equals("enter"))
            game.enter(parseChildNodes(inputNode));
        
        else if (name.equals("quit"))
            game.quit();
        
        else if (knownTags.contains(name))
            return parseChildNodes(inputNode);
        
        else
            System.err.println("Unknown tag <"+name+">");
        
        
        
        return "";
    }
    private short contents(Node node) {
        //System.err.println(node.getNodeName());
        if (node.getNodeValue() == null)
            return EMPTY;
        if (node.getNodeType() == Node.TEXT_NODE) {
            if (node.getTextContent().matches("\\s*"))
                return WHITESPACES;
            return TEXT;
        }
        if (node.getNodeType() == Node.COMMENT_NODE)
            return COMMENT;
        if (node.hasChildNodes())
            return NODES;
        System.err.println(node.getNodeType());
        System.err.println("Unknown contents of node "+node.getNodeValue());
        return UNKNOWN;
    }
    private String parseChildNodes(Node inputNode) {
        String returnString = "";
        NodeList childNodes = inputNode.getChildNodes();
        for (int i=0; i < childNodes.getLength(); i++) {
            Node childNode = childNodes.item(i);
            if (!childNode.getNodeName().equals("return"))
                returnString += parse(childNode);
        }
        Node returnNode = getChild(inputNode, "return", false);
        if (returnNode != null)
            return parse(returnNode);
        return returnString;
    }
    private String parseChild(Node inputNode) {
        return parse(getChild(inputNode));
    }
    private String parseChild(Node inputNode, String name) {
        return parse(getChild(inputNode, name));
    }
    private Node getChild(Node inputNode) {
        NodeList childNodes = inputNode.getChildNodes();
        for (int i=0; i < childNodes.getLength(); i++) {
            Node childNode = childNodes.item(i);
            if (contents(childNode) == NODES)
                return childNode;
        }
        return inputNode.getFirstChild();
    }
    private Node getChild(Node inputNode, String name) {
        return getChild(inputNode, name, true);
    }
    private Node getChild(Node inputNode, String name, boolean showException) {
        NodeList childNodes = inputNode.getChildNodes();
        for (int i=0; i < childNodes.getLength(); i++) {
            Node childNode = childNodes.item(i);
            if (childNode.getNodeName().equals(name))
                return childNode;
        }
        if(showException)
            System.err.println("Expected sub-tag <"+name+"> in <"+inputNode.getNodeName()+">");
        return null;
    }
    private String getAttribute(Node inputNode, String attributeName) {
        return getAttribute(inputNode, attributeName, true);
    }
    private String getAttribute(Node inputNode, String attributeName, boolean showException) {
        Node attribute = inputNode.getAttributes().getNamedItem(attributeName);
        
        if (attribute != null)
            return attribute.getNodeValue();
        if(showException)
            System.err.println("Expected an attribute '"+attributeName+"' in <"+
                                inputNode.getNodeName()+">");
        return null;
    }
    private Super getSuper(String type, String name) {
        if (type.equals("global") || type.equals("player"))
            return game;
        else if (type.equals("room"))
            return game.getRoom(name);
        else if (type.equals("item"))
            return game.getAnyItem(name);
        else
            System.err.println("<get> or <set> in=\""+type+"\" ...> found, expected global, player, room or item");
        return null;
    }
}