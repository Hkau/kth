import java.util.HashMap;
import org.w3c.dom.Node;

/**
 * Super with the addition of nodes to be parsed on Enter and onExit
 */
public class Room extends Super {
    private Node onEnter;
    private Node onExit;

    public Room(String description, Node onEnter, Node onExit) {
        this.description = description;
        this.onEnter = onEnter;
        this.onExit = onExit;
    }
    public Node getOnEnter() {
        return onEnter;
    }
    public Node getOnExit() {
        return onExit;
    }
}