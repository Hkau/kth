import java.util.*;
/**
 * Föremål som förutom sina ärvda varibaler även håller koll på var den passar och inte passar.
 * När du testar på en ny dörr, så slumpar den om den passar, och minns sedan detta.
 */
public class Key extends Tool{
    //Här i finns testade dörrar samt resultatet.
    private HashMap<Door, Boolean> doors;
    public Key(String name, String description){
        super(name, description);
        doors = new HashMap<Door, Boolean>();
    }
    /**
     * Skriver över Item.use(). Returnerar alltid false, eftersom nycklar inte går sönder.
     */
    public boolean use(Game game){
        System.out.println("with which door?");
        System.out.print(">> ");
        String what; int direction = 5;
        while(true){
            //Hämtar användarens svar.
            what = Parser.getWholeLine();
            if(what != null){
                if(what.equals("left") || what.equals("left door")) direction = 0;
                else if(what.equals("forward") || what.equals("forwards")) direction = 1;
                else if(what.equals("right") || what.equals("right door")) direction = 2;
                else if(what.equals("back") || what.equals("backward door")) direction = 3;
                //Om man svarar något annat:
                else if(direction == 5){
                    System.out.println("I don't understand direction.");
                    return false;
                }
                //Om jag har ett vettigt svar så kommer jag ner hit, och då bryter jag loopen.
                break;
            }
        }
        //Här är dörren du testar på.
        Door currentDoor = game.getCurrentRoom().getDoor(((5 - game.getTittriktning()) % 4 + direction) %4);
        //Om den inte finns:
        if(currentDoor == null){
            System.out.println("There is no door in that direction.");
            return false;
        }
        //Om du redan testat på den dörren, så kommer du få samma effekt igen.
        if(doors.containsKey(currentDoor)){
            if(doors.get(currentDoor).booleanValue())
                    currentDoor.setStatus(!currentDoor.getStatus());
            else
                System.out.println("The key doesn't fit this door...");
        }
        else{//Annars har du 20% chans att nyckeln passar.
            if(Math.random() <= 0.2){
                doors.put(currentDoor, new Boolean(true));
                currentDoor.setStatus(!currentDoor.getStatus());
            }
            else{
                //dörren associeras med false och nyckeln kommer aldrig fungera.
                doors.put(currentDoor, new Boolean(false));
                System.out.println("The key doesn't fit this door...");
            }
        }
        return false;
    }
}
