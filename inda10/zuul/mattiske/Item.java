import java.awt.*;
/**
 * Superklassen för alla föremål.
 * Generellt ett föremål som går att plocka upp, utan särskilda egenskaper.
 */
public class Item{
    private String name, description;
    public Item(String name, String description){
        this.name = name;
        this.description = description;
    }
    /**
     * Returnerar namnet.
     */
    public String getName(){
        return name;
    }
    /**
     * Returnerar beskrivningen.
     */
    public String getDescription(){
        return description;
    }
    
    /**
     * Använder ett föremål. Returnerar true om föremålet går sönder efter användning.
     * OBS! denna metod overridas av subklasser med andra funktionaliteter.
     */    
    public boolean use(Game game){
        //Nedan finns lite olika föremål, och dess användningar.
        if(name.equals("light switch")){
            //Släcker rummet.
            game.getCurrentRoom().setDark(!game.getCurrentRoom().getDark());
            System.out.println(game.getCurrentRoom().getDescription(game.getTittriktning()));
            return false;
        }
        if(name.equals("sink")){
            System.out.println("You twist the tap, but there is no water connected to it.");
            return false;
        }
        if(name.equals("toilet")){
            System.out.println("Nothing happens when you flush it.");
            return false;
        }
        if(name.endsWith("book")){
            System.out.println("You open the book. It really is what the title says.");
            return false;
        }
        if(name.equals("broken glass")){
            System.out.println("You don't want to touch it.");
            return false;
        }
        if(name.equals("cup of coffe")){
            System.out.println("You drink the coffe, it tastes good.");
            return false;
        }
        if(name.endsWith("chair")){
            System.out.println("You sit down on the chair");
            return false;
        }
        if(name.equals("bathtub")){
            System.out.println("You undress and slowly lie down in the bathtub.\nYou probalbely want to examine the bathtub now.");
            return false;
        }
        if(name.equals("body of a dead woman")){
            // "use body of a dead woman" - LOL
            System.out.println("No...you can't...Man you are sick!");
            return false;
        }
        if(name.equals("armchair")){
            System.out.println("You sit down in the armchair");
            return false;
        }
        if(name.equals("fireplace")){
            System.out.println("You can't unlight it, you have to wait for it to burn out.");
            return false;
        }
        if(name.equals("teddybear")){
            //Första användningen rengör du den.
            if(!description.equals("Almost clean teddybear. You had it when you were a kid")){
                System.out.println("You try to clean off some of the blood.");
                description = "Almost clean teddybear. You had it when you were a kid";
            }else
            //andra gången händer detta.
                System.out.println("You feel a little comforted");
                return false;
        }
        // Här är min teleporter. Sväljer du pillret hamnar du i första rummet igen.
        if(name.equals("blue pill")){
            System.out.println("You feel a little dizzy and within seconds you are unconcious.\nYou slowly wake up again...");
            game.teleport(new Point(0,0));
            return true;
        }
        //Den här fällan gör ONT!
        if(name.equals("bear trap")){
            System.out.println("You step on a bear trap, and get a really deep wound.\nThe wound needs to be taken care of before you loose to much blood");
            game.wound(true);
            return false;
        }
        //Vid användning får du en radio utan batterier och lösa batterier.
        if(name.equals("pocket radio")){
            System.out.println("You got no signal. However, you pick out the batteries.");
            game.getInventory().put("batteries", new Tool("batteries","Use with electric devices."));
            game.getInventory().put("pocket radio", new Item("pocket radio without batteries","Seems rather useless down here..."));
            return true;
        }
        //Mindre smart.
        if(name.equals("hole")){
            System.out.println("You put your hand into the hole. A flash of pain ripps through you,\nand when you pull out your arm, you dont have an arm.");
            game.wound(true);
            return false;
        }
        //Om föremålet inte har någon definierad använding.
        System.out.println("You cant use that item right now...");
        return false;
    }
}
