import java.util.*;
import java.awt.*;

public class Game 
{
/**
 * Startar spelet.
 */
    public static void main(String[] args){
        Game game = new Game();
        game.play();
    }
    private Parser parser;
    private Point location;
    //Innehåller alla rum i spelet. Dessa nås med hjälp av motsvarande kommentar.
    private HashMap<Point, Room> rooms;
    /*
     * Det finns två riktningar, en intern; rummen ligger i ett koordinatsystem, detta förändras inte. och ett externt:
     * går du åt vänster så kommer du att titta åt vänster i nästa rum. Detta är för att öka känslan av att man är vilse.
     * Tittriktning hjälper till att hålla reda på vart du tittar i förhållande till
     * de interna riktningarna.
     */ 
    // 0 = vänster, 1 = framåt, 2 = höger och 3 = bakåt
    private int tittriktning;
    //de föremål du bär på. Dessa nås via sitt namn.
    private HashMap<String, Item> inventory;
    private CutWrist wound;
    /**
     * Create the game and create first map.l
     */
    public Game() 
    {
        //Läser inkommandon.
        parser = new Parser();
        //Starting position.
        location  = new Point(0,0);
        rooms = new HashMap<Point,Room>();
        inventory = new HashMap<String, Item>();
        //du tittar framåt.
        tittriktning = 1;
        wound = new CutWrist();
        //Nedan skapar jag första rummet. Se rumsklassen för mer info om hur innehållet fungerar.
        Door[] doors = {null, new Door(false), null, null}; //dörrar i först rummet
        HashMap<String, Item> items = new HashMap<String, Item>();//Föremål i första rummet.
        items.put("lightbulb", new SolidItem("lightbulb", "It's just hanging there."));
        HashSet<Item> hiddenItems = new HashSet<Item>();
        hiddenItems.add(new SolidItem("light switch", "may be used to light the room"));
        //första rummet har en egen konstruktor, där inget slumpas.
        Room firstRoom = new Room(
        "A small room with concrete floor and walls. \n A lightbulb is hanging from its powercord",
            true, doors, items, hiddenItems);
            //Lägger till första rummet i spelet, sen är det bara att köra!
        rooms.put(new Point(location),firstRoom);
        
    }
        /**
         * Endast en loop som hämtar kommandon och utför dem.
         */
    public void play() 
    {            
        printWelcome();

        // Enter the main command loop.  Here we repeatedly read commands and
        // execute them until the game is over.
        
        while (true) {
            Command command = parser.getCommand();
            //kör tills processCommand returnerar true
            if(processCommand(command))break;
        }
        System.out.println("Thank you for playing.  Good bye.");
    }

    /**
     * Print out the opening message for the player.
     */
    private void printWelcome()
    {
        System.out.println();
        System.out.println("...As you slowly gain consciousness you feel the cold, hard surface you are");
        System.out.println("currently lying on. You are feeling cold.");
        System.out.println("Suddenly you realize that you have no idea where you are, and with a twitch");
        System.out.println("you are widely awake. The only light source in the room is a tiny interstice");
        System.out.println("between the floor and subject seems to be a door.");
        System.out.println("You rise from the floor and feel a little dizzy.");
        System.out.println();
        //Skriver vad som syns i första rummet.
        System.out.println(rooms.get(location).getDescription(tittriktning));
    }

    /**
     * Given a command, process (that is: execute) the command.
     * @param command The command to be processed.
     * @return true If the command ends the game, false otherwise.
     */
    private boolean processCommand(Command command) 
    {
        String commandWord = command.getCommandWord();
        if(commandWord == null){
            System.out.println("I don't know that command");
            return false;
        }
        //Om vi kommit ned hit så finns det ett andra kommando.
        if(commandWord.equals("quit")) return true;
        //Commandot var tydligen inte "quit"
        if(commandWord.equals("help")) printHelp(command);
        else if(commandWord.equals("go")) goRoom(command);
        else if(commandWord.equals("search")) search(command);
        else if(commandWord.equals("use")) use(command);
        else if(commandWord.equals("examine")) examine(command);
        //Alla look-kommandon motsvarar "examine room" fullständigt.
        else if(commandWord.equals("look")) examine(new Command("examine", "room"));
        else if(commandWord.equals("pick up")) pickUp(command);
        //Kommer vi ända hit så känns inte kommandot igen.
        else System.out.println("I don't know that command.");
        /*
         * wound.turn() anropas efter varje kommando. Den returnerar true om du gått skadad för länge
         * - och avslutar därmed sepelet.
         * Annars räknar den ner. Om du inte är skadad gör den ingenting.
         */
        return wound.turn();
    }

    // implementations of user commands:

    /**
     * Print out some help information.
     * Here we print some stupid, cryptic message and a list of the 
     * command words.
     */
    private void printHelp(Command command) 
    {
        //allmänhjäp.
        if(!command.hasSecondWord()){
        System.out.println("You are lost. You are alone. You want out.");
        System.out.println();
        System.out.println("Your command words are:");
        System.out.println("   go, quit, help, examine, search, use, pick up");
        System.out.println("\nTo get help for a certain command, type \"help <command>\". For instance \"help examine\"");
        return;
        }
        //nedan skriver den ut om du bett om hjälp för ett specifikt kommando.
        String helpSubject = command.getSecondWord();
        if(helpSubject.equals("go")){
            System.out.println("Move to an adjecent room. There has to be an unlocked door in the direction");
            System.out.println("Syntax: \"go <direction>\", directions are left, right, forward, and back.");
            return;
        }
        if(helpSubject.equals("examine")){
            System.out.println("Prints a detailed description of the room/item.");
            System.out.println("Syntax: \"examine 'room'/<item>\" ");
            return;
        }
        if(helpSubject.equals("search")){
            System.out.println("Seraches the room for hidden items");
            System.out.println("Syntax: \"search room\"");
            return;
        }
        if(helpSubject.equals("use")){
            System.out.println("Uses an item. Note that all items are not useable.");
            System.out.println("Syntax: \"use <item>\"");
            return;
        }
        if(helpSubject.equals("pick up")){
            System.out.println("Pick up an item and put it in your inventory");
            System.out.println("Syntax: \"pick up <item>\"");
            return;
        }
        
    }

    /** 
     * Try to go to one direction. If there is an exit, enter
     * the new room, otherwise print an error message.
     */
    private void goRoom(Command command) 
    {
        if(!command.hasSecondWord()) {
            // if there is no second word, we don't know where to go...
            System.out.println("You need to specify where you want to go.");
            return;
        }
        String directionString = command.getSecondWord();
        //Om den är fem efter ifsatserna så han man inte angett en giltig riktning.
        int direction = 5;
        if(directionString.equals("left")) direction = (tittriktning + 3) % 4;
        if(directionString.equals("forward")) direction = tittriktning;
        if(directionString.equals("right")) direction = (tittriktning + 1) % 4;
        if(directionString.equals("backwards") ||directionString.equals("backward") || directionString.equals("back"))
            direction = (tittriktning + 2) % 4;
        //Om riktningen är ogilting: whina och gör inget mer.
        if(direction == 5){
            System.out.println("Invalid go-command.\nYour go commands are: forward, back, left and right.");
            return;
        }
        //Hämta dörren i riktningen och undersök den.
        if(rooms.get(location).getDoor(direction) != null){
            if(rooms.get(location).getDoor(direction).getStatus()){
                System.out.println("Door is locked...");
                return;
            }
            else{
                //Om rummet saknas så måste vi lägga till detta i rooms. För att göra detta måste koordinaten modifieras.
                switch(direction){
                    case 0: location.x--; break;
                    case 1: location.y--; break;
                    case 2: location.x++; break;
                    case 3: location.y++; break;
                    //ifall det omöjliga skulle inträffa...
                    default: return;
                }
                //om rummet du försöker gå till inte tidigare är besökt så slumpas ett nytt rum.
                if(!rooms.containsKey(location)){
                    Room newRoom = new Room(new Point(location), rooms);
                    rooms.put(new Point(location), newRoom);
                    
                }
                //Du tittar nu i den riktning du gick.
                tittriktning = direction;
                System.out.println("You enter a new room.");
                //Och här får du se vad det nya rummet innehåller.
                System.out.println(rooms.get(location).getDescription(tittriktning));
                //Här finns det en viss chans att dörren går i baklås.
                if(Math.random() <= 0.05){
                    System.out.println("You hear a sound behind you.");
                    /* 
                     * Modulusoperatorn nedan är krånglig och dyker upp lite varstans i koden. Dess uppgift är att ta reda på vilken dörr internt du
                     * menar när du skriver en extern riktning. Den fungerar enligt följande:
                     * (5 - tittriktning) % 4 bestämmer hur mycket riktningen måste modifieras för att överensstämma med de interna riktningarna.
                     * 3an mostvarar bakåt internt. Och det är ju dörren du kom igenom som skall gå i baklås.
                     * Sista modulusoperatorn ser bara till att du inte överskrider 3 när du adderar med tre.
                     */
                    rooms.get(location).getDoor(((5-tittriktning)% 4 + 3)%4).setStatus(true);
                }
                return;
            }
        }
        //Om det saknas en dörr så får du följande error:
        else
            System.out.println("You can't go that way");
    }
    
    private void search(Command command){
        if(!command.hasSecondWord()) {
            // if there is no second word, we don't know subject to search...
            System.out.println("You need to specify subject you want to search.");
            return;
        }
        String subject = command.getSecondWord();
        if(subject.equals("inventory")) examine(command);//det fins inga dolda items i inventory, alltså gör den samma som examine inventory.
        //Sök igenom rummet.
        else if(subject.equals("room"))
            rooms.get(location).searchRoom(this);
        else{
            //Här kan jag eventuellt lägga till search <item>... Just nu whinar den bara.
            System.out.println("You can only search room or inventory.");
        }
    }
    
    private void use(Command command){
        if(!command.hasSecondWord()) {
            // if there is no second word, we don't know subject to use...
            System.out.println("You need to specify subject you want to use.");
            return;
        }
        String itemName = command.getSecondWord();
        //Om föremålet du vill använda finns i din inventory så använder vi det och är nöjda sen.
        if(inventory.containsKey(itemName)){
            //use() returnerar true om föremålet går sönder efter användning.
            if(inventory.get(itemName).use(this)) inventory.remove(itemName);
            //Om föremålet inte finns i inventory så söker vi igenom rummet. returnerar en boolean som talar om ifall föremålet fanns.
        }else if(!rooms.get(location).useItem(itemName, this))
            System.out.println("You couldn't find such item... Check your spelling and try searching the room.");
        
    }
    
    private void examine(Command command){
        if(!command.hasSecondWord()) {
            // if there is no second word, we don't know subject to examine...
            System.out.println("You need to specify subject you want to examine.");
            return;
        }
        String subject = command.getSecondWord();
        //Examine room skriver om rumsbeskrivningen.
        if(subject.equals("room"))
            System.out.println(rooms.get(location).getDescription(tittriktning));
            //Här kollar vi vad vi har i vår inventory. Listar allt i inventory med en iterator.
        else if(subject.equals("inventory")){
            Iterator<Item> it = inventory.values().iterator();
            System.out.println("You are currently carrying:");
            while(it.hasNext()){
                System.out.println("- " + it.next().getName());
            }
        }
        else{
            String description;
            //om vi söker beskrivningen av ett föremål kollar vi först i inventory
            if(inventory.containsKey(subject)){
                description = inventory.get(subject).getDescription();
                System.out.println(description);
            }else if(!rooms.get(location).examineItem(subject))//och annars i rummet. Om den returnar false(dvs. föremålet inte fanns) så whinar den.
            System.out.println("You couldn't find such item...");
        }       
    }
    
    private void pickUp(Command command){
        if(!command.hasSecondWord()) {
            // if there is no second word, we don't know subject to pick up...
            System.out.println("You need to specify subject you want to pick up.");
            return;
        }
        //removeItem tar bort föremålet från rummet och returnerar borttaget föremål.
        Item subject = rooms.get(location).removeItem(command.getSecondWord());
        //Om det inte fanns kommer removeItem() ha returnat null, och då whinar den.
        if(subject == null)
            System.out.println("You couldn't find such item...");
        else{
            //Om vi fann ett föremål, kollar vi att det inte är ett solidt föremål, för dessa får man inte plocka upp.
            if(!(subject instanceof SolidItem)){
                //vi lägger till föremålet i inventory, talar om detta för spelaren och upprepar sedan rummsbeskrivningen för
                //att förtydliga att föremålet är flyttat.
                inventory.put(subject.getName(), subject);
                System.out.println("\nYou pick up the " + subject.getName() + " and it can now be found in your inventory.");
                System.out.println(rooms.get(location).getDescription(tittriktning));
            }//Nedan : Om föremålet inte kan plockas upp.
            else System.out.println("You can't pick that item up");
        }
            
    }
    /**
     * Return tittriktning.
     */
    public int getTittriktning(){
        return tittriktning;
    }
    /**
     * Returns the room matching your current location.
     */
    public Room getCurrentRoom(){
        return rooms.get(location);
    }
    /**
     * Moves you to the desired location and prints the description of the new room.
     * A non existing room will cause the program to crash.
     */
    public void teleport(Point where){
        location = where;
        System.out.println(rooms.get(location).getDescription(tittriktning));
    }
    /**
     * Wounds or heals the player, depending on 'status' parameter.
     */
    public void wound(boolean status){
       if(status)wound.get();
       else wound.heal();
    }
    /**
     * returns your inventory.
     */
    public HashMap<String, Item> getInventory(){
        return inventory;
    }
}
