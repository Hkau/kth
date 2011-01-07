import java.util.HashMap;
import org.w3c.dom.Node;

/**
 * Super with the addition of some nodes (which are not quite implemented though)
 */
public class Item extends Super {
    private Node onPickup;
    private Node onDrop;
    private Node onEquip;
    private Node onUnequip;
    private boolean equippable;
    
    public Item (String description, Node onPickup, Node onDrop,
                 Node onEquip, Node onUnquip) {
        this.description = description;
        this.onPickup = onPickup;
        this.onDrop = onDrop;
        this.onEquip = onEquip;
        this.onUnequip = onUnequip;
    }
    public Node getOnPickup() {
        return onPickup;
    }
    public Node getOnDrop() {
        return onDrop;
    }
    public Node getOnEquip() {
        return onEquip;
    }
    public Node getOnUnequip() {
        return onUnequip;
    }
    public boolean equippable() {
        return equippable;
    }
}