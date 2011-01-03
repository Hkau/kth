package org.x2d.zuul;
import java.util.*;
import java.io.*;
/**
 * Enum that has all the items used in the game. It has one single public method to get an item from it.
 * 
 */
public enum Items implements Serializable {
       
    STONE_OF_DELEN(new SimpleItem("sten av delen", "En skimrande sten med märkliga tecken.", "G&a", true)),
    STORE_ROOM_KEY(new SimpleItem("lagernyckel", "En vanlig nyckel.", null, true)),
    NOTE(new SimpleItem("lapp", "En gammal utsliten lapp.", "Meningen med liver är 42!", true)),
    SKELETON(new SimpleItem("skelett", "Ett skelett som ser ut att ha legat här i evigheter.")),
    FOOD(new SimpleItem("mat", "Ett bröd och lite ost.", null, true)),
    PRAYER_BEADS(new SimpleItem("bönband", "En rad med pärlor på ett snöre.", null, true)),
    WARNING_SIGN(new SimpleItem("varningsskylt", "En skylt med en döskalle på.", 
        "Farligt område! Beträds på egen risk!", false)),
    DOOR_SIGN(new SimpleItem("dörrskylt", "En skylt som sitter brevid dörren.",
        "Dörren går endast att öppna med hjälp magisk sten.", false)),
    EATEN_SANDWICH(new SimpleItem("uppäten macka", "En uppäten macka. Inte speciellt mycket mat på den.", null, true)),
    MENU(new SimpleItem("meny", "En lista med mat.", "Stekt kött 2öre\nBröd 1öre\nOst 2öre", false)),
    CHAIRS(new SimpleItem("stolar", "Ett antal stolar.")),
    TABLES(new SimpleItem("bord", "Ett antal bord.")),
    BENCH(new SimpleItem("bänk", "En bänk.")),
    BOXES(new SimpleItem("lådor", "Massor av lådor som står staplade längs väggarna.")),
    SEAL(new SimpleItem("sigill", "Ett brev med sigill från prästen.",
        "Jag intygar att personen som innehar detta brev får komma in i templet.", true)),
    HOLY_STONE(new Item("helig sten", "En stor sten med konstiga tecken på.") {
        public boolean isUsable() {
            return true;
        }
        public void use(Game g) {
            System.out.println(
                "Allting runtomkring dig blir suddigt och plötsligt ser du att du befinner dig i ett helt annat rum.");
            g.getPlayer().setCurrentRoom(g.getRoom(Game.Rooms.ENTRANCE));
        }
    });
    
    private static HashMap<String, Items> itemMap = new HashMap<String, Items>();
    static {
        for (Items item : Items.values()) {
             itemMap.put(item.getItem().getName(), item);
        }
    }
    private Item item; 
    private Items(Item item) {
        this.item = item;
    }
    
    /**
     * Gets an item by it's name.
     *
     * @param itemName The name.
     */
    public static Items getItem(String itemName) {
        return itemMap.get(itemName);
    }

	/**
	 * Gets the item this enum is representing.
	 * @return The item.
	 */
    public Item getItem() {
        return item;   
    }
}
