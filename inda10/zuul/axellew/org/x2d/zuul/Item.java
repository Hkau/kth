package org.x2d.zuul;
import java.io.*;
/**
 * Abstract class representing an item. Methods should be overridden
 * if the new class should do scriptet stuff. As an example:
<pre>
new Item("some name", "a description of the item") {
    &#0064;Override
    public boolean isUsable() {
        return true;
    }
    &#0064;Override
    public void use(Game g) {
        //Something that should happen when this item is used.
    }
};
</pre>
 */
public abstract class Item implements Serializable
{
    private String name;
    private String description;
    private String text;
    private boolean isTakable;
    
    /**
     * Constructor for objects of class Item
     * @param name The name of the item.
     * @param description A short description of the item.
     */
    public Item(String name, String description) {
        this(name, description, null, false);
    }
    
    /**
     * Constructor for objects of class Item
     * @param name The name of the item.
     * @param description A short description of the item.
     * @param text A text that should be used when the item is read.
     * @param isTakable Should be <code>true</code> if the item could be put into the backpack else <code>false</code>
     */
    public Item(String name, String description, String text, boolean isTakable) {
        this.name = name;
        this.description = description;
        this.text = text;
        this.isTakable = isTakable;
    }
    
    /**
     * Checks if it's possible to read from this item
     * @return <code>true</code> if there is text on the item else <code>false</code>
     */
    public boolean isReadable() {
        return (getText()!=null);
    }
    
    /** 
     * Gets this item's description
     *
     * @return The description.
     */
    public String getDescription() {
        return description;
    }
    
    /** 
     * Gets this item's name
     *
     * @return The name.
     */
    public String getName() {
        return name;
    }
    
    /**
     * Checks if it's possible to pickup this item
     * @return <code>true</code> if there is text on the item else <code>false</code>
     */
    public boolean isTakable() {
        return isTakable;
    }

    /**
     * Gets the text from this item
     * @return The text or null if there is none.
     */
    public String getText() {
        return text;
    }
    
    /**
     * This method should be called when the item is used.
     *
     * @param g The game which this item should interact with
     */
    public abstract void use(Game g);
    
    /**
     * Checks if it's possible to use this item. This should return true if there is an use method.
     * @return <code>true</code> if it is possible to use this item else <code>false</code>
     */
    public abstract boolean isUsable();
}

