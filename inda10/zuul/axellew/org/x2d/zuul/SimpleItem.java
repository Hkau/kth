package org.x2d.zuul;
/**
 * Version of Item does not require an use method.
 */
public class SimpleItem extends Item {
    /**
     * Constructor for objects of class Item
     * @param name The name of the item.
     * @param description The item's description
     */
    public SimpleItem(String name, String description) {
        super(name, description);
    }
    
    /**
     * Constructor for objects of class Item
     * @param name The name of the item.
     * @param description The item's description
     * @param text If there should be some text on the item that can
     * be read. If set to <code>null</code> there is nothing to read on the item.
     * @param isTakable Should be <code>true</code> if it is possible to take the
     * item else it should be <code>false</code>
     */
    public SimpleItem(String name, String description, String text, boolean isTakable) {
        super(name, description, text, isTakable);
    }
    
    @Override
    public void use(Game g) {
        //Can't be used
    }
    
    @Override
    public boolean isUsable() {
        return false;
    }

}
