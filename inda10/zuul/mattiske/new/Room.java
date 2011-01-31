import java.util.*;
import java.awt.*;
/**
 * Klassen rum. Den är vad den heter. Ett rum. Den har 1-4 dörrar, föremål - dolda och inte dolda, det kan vara mörkt, och den beskriver sig själv.
 */
public class Room 
{
    private boolean dark;
    //tempDescription är sådant i rummet som varierar... T.ex föremål och dörrarnas positioner (externt).
    private String description, tempDescription;
    //Finns aldrig mer än 4 dörrar.
    private Door[] doors;
    private HashMap<String, Item> items;
    private HashSet<Item> hiddenItems;
    
    /**
     * Här är ett rum med allt förutbestämt. Användsför att skapa första rummet.
     */
    public Room(String description, boolean dark, Door[] doors, HashMap<String, Item> newItems, HashSet<Item> newHidden) 
    {
        this.doors = doors;
        this.description = description;
        this.dark = dark;
        items = newItems;
        hiddenItems = newHidden;
    }
    /**
     * Här skapar jag ett slumpmässigt rum. Inparametrarna är rummets koordinat och de övriga rummen.
     * Here be dragons.
     */
    public Room(Point location, HashMap<Point, Room> rooms){
        /*
         * Frågetecknet kallas "turnary operator". Nedanstående turnary fungerar precis som
         * if(Math.random() < 0.3){
         *     dark = true;
         *     }else{
         *         dark = false;
         *     }
         *     Alltså:
         *     <en variabel eller return som skall få ett värde> = <argument (true eller false)> ? <om true> : <om false>;
         */
        dark = Math.random() < 0.3 ? true : false;
        //Ser till att dörrarna överensstämmer med omgivningen och slumpar annars:
        doors = new Door[4];
        // 0 = vänster om rummet, 1 = framför om rummet, 2 = höger om rummet, 3 = under rummet. (internt)
        for(int i = 0; i < 4; i++){
            //fixar så att location pekar på rätt rum i x-led:
            if(i == 0 || i == 3){
                location.x--;
            }
            else location.x++;
            //och y-led:
            if(i <= 1) location.y -= i;
            else location.y++;
            if(rooms.containsKey(location))
                    //Om rummet finns kommer alltså att båda rummen ha SAMMA dörr. En ändring i det ena rummet kommer påverka det andra rummets dörr.'
                    //OBS! Om rummet bredvid saknar dörr kommer även det nya rummet sakna dörr i den riktningen.
                    doors[i] = rooms.get(location).getDoor((i + 2) % 4);
            else //Om det inte finns något rum brevid, finns det en chans att det finns en dörr ändå.
                if(Math.random() > 0.3)//30% chans att det finns en dörr och 30% chans att den dörren är låst.
                    doors[i] = Math.random() < 0.3 ? new Door(true): new Door(false);//Se ovan för förklaring av turnary.
        }
        items = new HashMap<String, Item>();
        hiddenItems = new HashSet<Item>();
        //Lägger till lite slumpmässiga dolda föremål.
        if(Math.random() <= 0.2) hiddenItems.add(new Item("light switch", "may be used to light the room"));
        if(Math.random() <= 0.05) hiddenItems.add(new Tool("plastic spoon", "ollad..."));
        if(Math.random() <= 0.1)hiddenItems.add(new Key("rusty key", "Where does it fit?"));
        if(Math.random() <= 0.1 && dark){
            //Det är inte helt ofarligt att ströva runt i mörka rum...
            hiddenItems.add(new Trap("bear trap", "It has hurt you badly."));
        }
    }
    

    /**
     * @return The descrition of the room. If dark, you wont see anything.
     */
    public String getDescription(Game game)
    {
        return !dark? generateDescription(game) : "It's too dark to see anything";
    }
    /**
     * returnerar en dörr med motsvarande internt index.
     */
    public Door getDoor(int direction){
        return doors[direction];
    }
    /**
     * Antal dörrar.
     */
    private int howManyDoors(){
        int a = 0;
        for(int i = 0; i < 4; i++)
            if(doors[i] != null) a++;
        return a;
    }
    /**
     * Om rummet är nytt läggs nya föremål till och sedan genereras en beskrivning av rummet.
     * Om rummet har beskrivning så listas bara dörrar och föremål.
     */
    private String generateDescription(Game game){
        if(description == null){//Om rummet inte har besökts tidigare behöver det föremål och beskrivning.
            Random r = new Random();//genererear slumptal
            /*
             * En switch-sats är som ett antal if-satser, fast med specifika cases för varje värde på arugumentet. default fångar upp resten.
             * switch(siffra){
             *     case 0: //gör något. break;
             *     case 1: //gör något. break;
             *     case 2: //gör något. break;
             *     default: //gör annars
             * }
             * Motsvarar alltså:
             * if(siffra == 0) //gör något
             * else if(siffra == 1) //gör något
             * else if(siffra == 2) //gör något
             * else //gör annars.
             */
            switch(r.nextInt(14)){
                case 0:
                    dark = true;//Specialfall där jag vill att rummet alltid är mörkt
                    //när man tänt det fungerar rummet som vanligt.
                    description = "There is a crucified, old man hanging on the wall. He is calmly following you with his eyes.";
                    items.put("old man", new SolidItem("old man","His tounge is cut out, and there is blood coming from his ears."));
                    items.put("cross", new SolidItem("cross", "A big wooden cross the man is nailed to."));
                    hiddenItems.add(new Item("jar","It contains a fluid and what appears to be the old mans tounge"));
                    return "It's too dark to see anything";//Eftersom rummet alltid är mörkt när man kommer in så kan jag returnera detta.
                case 1: description = "In the middle of the room there is an operating table, on top of it:\na man. His stomach is widely open. You hear a steady beeping sound,\nyou assume its his pulse. There is noone in the room.";
                    items.put("operating table", new SolidItem("operating table", "aside from some blood from the patient it seems rather clean."));
                    items.put("patient", new SolidItem("patient", "He is put to sleep. You see that he is actually two bodies sewn together."));
                    items.put("gas tank", new Item("gas tank","it has a mouthpiece connected too it. You guess that it puts people to sleep.")); break;
                case 2: description = "You are in a chapel. There are lines of benches and an altar in the front";
                    items.put("altar", new SolidItem("altar","There are two unlit candles on it"));
                    items.put("bench", new SolidItem("bench","Wooden bench."));
                    hiddenItems.add(new Item("holy bible","when you look inside you realize that it isn't the holy bible. \nIt's something completely else. There is something written in a language you don't understand."));
                    hiddenItems.add(new ExpiringItem("candle","if only you had something light it with...", 10));break;
                case 3: description = "There are flowers painted on the walls.";
                    items.put("bookshelf", new SolidItem("bookshelf", "Covers one wall entirely, full of books."));
                    items.put("red book", new Item("red book", "Book title: \"Conquer difficulties in life\""));
                    items.put("big blue book", new Item("big blue book", "Book title: \"Wish it. Want it. Get it.\""));
                    items.put("small black book", new Item("small black book", "Book title: \"What to expect when you are expecting to expect.\""));
                    items.put("brown book", new Item("brown book", "Book title: \"En lemmings guide to survival\""));
                    items.put("really big book", new Item("really big book", "Book title: \"The big book about penises\""));
                switch(r.nextInt(25)){
                    default: break;
                    case 0: items.put("screwdriver", new Tool("Screwdriver", "Seems useful")); break;
                    case 3: items.put("flashlight", new Item("flashlight", ""));break;
                    case 4: items.put("dead flower", new Item("dead flower", "seems useless"));break;
                    case 5:
                    case 6: items.put("broken glass", new Item("broken glass", "its all over the floor"));break;
                    case 7:
                    case 8: items.put("plastic spoon", new Item("plastic spoon", "looks fragile"));break;
                    case 9: hiddenItems.add(new Key("key", "found under the desk"));
                    case 10: items.put("cup of coffe", new Item("cup of coffe", "its standing on the desk. And it's still warm"));
                    case 11: items.put("office chair", new SolidItem("office chair", "in the corner behind the desktop"));
                    case 12: items.put("desk", new SolidItem("desk", "it's turned towards the corner"));break;
                    case 13:
                    case 14: items.put("what appears to be a piece of a note", new Item("what appears to be a piece of a note", "all you can see is \"121060\""));break;
                }break;
                case 4: description = "White floors, walls and ceiling. A single table in the middle of the room. On it, a blue pill.";
                    items.put("table", new SolidItem("table","Made of metal"));
                    items.put("blue pill", new Item("blue pill", "It's small and blue."));break;
                case 5: description = "A small closet full of old clothes and other useless stuff.";
                    // 20% chans att få något av följande:
                    if(r.nextInt(5) == 0)hiddenItems.add(new ExpiringItem("first aid kit", "Seems useful if you get hurt", 5));
                    if(r.nextInt(5) == 0)hiddenItems.add(new Tool("batteries", "use with electronic devices"));
                case 6: description = "Empty, white room, exept for a hole in the wall.";
                    items.put("hole", new SolidItem("hole", "It's about 10cm in diameter, use it to put your arm into it."));break;
                case 7: description = "The room has concrete walls painted pink.";
                switch(r.nextInt(25)){
                    default: break;
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4: items.put("posters", new Item("few posters", "They are on all four walls. You dont recognize them"));break;
                    case 5: items.put("rusty nail", new Item("rusty nail", "seems useless..."));break;
                    case 6: items.put("pocket radio", new Item("pocket radio", "you got no signal here")); break;
                    case 7:
                    case 8: items.put("table",new SolidItem("table","iron legs and plastic surface"));
                    case 9: items.put("chair", new SolidItem("chair", "standing in the corner"));
                    case 10: 
                    case 11:
                    case 12: items.put("chair", new SolidItem("chair", "standing in the corner"));
                    case 13:
                    case 14: items.put("table",new SolidItem("table","iron legs and plastic surface"));
                    case 15:
                    case 16: items.put("chair", new SolidItem("chair", "standing in the corner"));break;
                    
                    case 20: items.put("an old picture", new Item("an old picture", "the picture is taken outside your kitchen window. You are in the picture."));
                    case 21:
                    case 22:
                    case 23: items.put("small table", new SolidItem("small table","iron legs and plastic surface"));break;
                    case 24: items.put("a short piece of string", new Tool("a short piece of string", "seems plastic"));break;
                }break;
                // case 8:
                case 9: description = "The walls are covered with pictures of you. Someone has been following you\n for a long time...";break;
                case 10: description = "It appears to be a bathroom.";
                    switch(r.nextInt(10)){
                    default: break;
                    case 0: items.put("dead fish", new Item("dead fish", "found in the bathtub"));
                    case 1:
                    case 2: items.put("bathtub",new SolidItem("bathtub", "half filled with rotten water"));
                    case 3:
                    case 4: items.put("broken mirror", new SolidItem("broken mirror", "you cut your finger examining it"));
                    case 5:
                    case 6: items.put("sink", new SolidItem("sink", "it's old and dirty"));
                    case 7:
                    case 8: items.put("toilet", new SolidItem("toilet", "has a big crack in it. Seems unclean"));break;
                    case 9: items.put("body of a dead woman", new SolidItem("body of a dead woman.", "she is lying face down on the floor. She is wearing a light yellow dress, \n all covered in blood. Her pale skin indicates that she's \nbeen dead a long time. She has some really bad injuries on her back."));break;                   
                }break;
                case 11: description = "The room you've now entered is heavily furnitured with an antique style.\nA big carpet covers the whole floor. The room is lighted by a fireplace.";
                    items.put("paintings", new Item("pantings", "Differend paintings. Motives: An old king, a landscape, a battlefield and a big pineapple"));
                    items.put("picture", new Item("picture", "Appears to be drawn by a fiveyearold. Signed \"Lemming\n"));
                    items.put("map", new Item("map","It looks like earth, but it's wierdly altered.\nBoth sout and north america are missing and africa is split in half"));
                    items.put("table", new SolidItem("table", "A really big, dark wood table for about 12 people"));
                    items.put("ashtray", new Item("ashtray", "Made of marble"));
                    items.put("cigar", new Tool("cigar", "still lit, spreading a really nice smell in the room. Maybe you can use it to light stuff..."));
                    hiddenItems.add(new SolidItem("fireplace", "Looks really old and classy"));
                    items.put("chair" , new Item("chair", "rather small silk seats"));
                    items.put("armchair", new SolidItem("armchair", "looks comfortable. Faces the fireplace"));
                    items.put("curtains", new SolidItem("curtains", "Red, silk curtains with gold details hanging on both sides of the fireplace"));break;
                case 12: description = "The floor is all covered with blood, and there are stains on the walls";
                    items.put("teddybear", new Item("teddybear", "covered in blood. You recognize it as your very owchildhood toy."));break;
                case 13: description = "Looks like a classroom. There are lines of school benches and chairs.\nOn each chair there is a naked doll. At the end of the classroom there is a board and you can read \"I love the warmth of your bodies\"";
                    items.put("dolly", new Item("dolly", "Naked plastic baby."));
                    items.put("chair", new SolidItem("chair","regular chair"));
                    items.put("school bench", new SolidItem("school bench","It's a litte table with a lid"));
                    items.put("board", new SolidItem("board","\"I love the warmth of your bodies\""));break;
                default: description = "Just a regular hallway";
            }
        }
        //tempDescription inehåller sådan info som förändras varje gång.
        tempDescription = "\nItems: ";
        for(Item item : items.values()){
                tempDescription += "\n   - " + item.getName();
        }
        tempDescription += (howManyDoors() > 1) ? "\n There are doors": ".\n There is a door";
        for(int i = 0; i < 4; i++){
            if(doors[i] == null) continue;
            switch(game.translateToInternalDirection(i)){
                case 0: tempDescription += " left,"; break;
                case 1: tempDescription += " forward,"; break;
                case 2: tempDescription += " right,"; break;
                case 3: tempDescription += " back,"; break;
                default: System.out.println("Error in generateDescription() - doorloop");
            }
        }
        //Om jag rummet saknar beskrivning så returnerar jag inte tempDescription.
        return (description != null) ? description + tempDescription : null;
    }
    /**
     * Listar dolda föremål och flyttar dem till listan med kända föremål.
     */
    public void searchRoom(Game game){
        //Ifall det inte finns några dolda föremål.
        if(hiddenItems.size() == 0){
            System.out.println("You find nothing of interest");
            return;
        }
        Iterator<Item> it = hiddenItems.iterator();
        //en do-while fungerar som en while förutom att den inte testar argumentet första körningen. Eftersom jag redan kollat ovan så slipper jag första kollen.
        do{
            Item item = it.next();
            System.out.println("You find " + item.getName());
            items.put(item.getName(), item);
            //Om du skulle ha oturen att stöta på en fälla, så slutar du leta och använder fällan istället...
            if((item instanceof Trap) && dark){
                item.use(game);
                //Men du har hittat fällan, så nu känner du iaf till den :)
                hiddenItems.remove(item);
                return;
            }
        }while(it.hasNext());
        hiddenItems.clear();
        //Skriver ut olika beroende på om det finns en dörr eller flera.
        description = (howManyDoors() > 1) ? "\n There are doors": ".\n There is a door";
        //fungerar precis som i generateDescription() : om dörren != null så skriver den ut vilken riktning den har.
        for(int i = 0; i < 4; i++){
            if(doors[i] == null) continue;
            switch(game.translateToInternalDirection(i)){
                case 0: description += " left,"; break;
                case 1: description += " forward,"; break;
                case 2: description += " right,"; break;
                case 3: description += " back,"; break;
                default: System.out.println("Error in generateDescription() - doorloop");
            }
        }
        System.out.println(description);
    }
    /**
     * Returnerar ett item om det finns, annars null,
     */
    public Item getItem(String itemName){
        if(!items.containsKey(itemName)) return null;
        return items.get(itemName);
    }
    /**
     * Tar bort ett föremål och returnerar det om det finns. Annars returnerar den null.
     */
    public Item removeItem(String itemName){
        return items.remove(itemName);
    }
    /**
     * Använder ett föremål. Returnerar true om föremålet fanns, annars false.
     * Dessutom tar den bort föremålet om item.use() returnerar true.
     */
    public boolean useItem(String itemName, Game game){
        if(!items.containsKey(itemName)) return false;
        if(items.get(itemName).use(game)) removeItem(itemName);
        return true;
    }
    /**
     * Skriver ut ett föremåls beskrivning och returnerar true om det fanns.
     */
    public boolean examineItem(String itemName){
        if(!items.containsKey(itemName)) return false;
        System.out.println(items.get(itemName).getDescription());
        return true;
    }
    /**
     * Returnerar dark.
     */
    public boolean getDark(){
        return dark;
    }
    /**
     * Ändrar dark.
     */
    public void setDark(boolean newValue){
        dark = newValue;
    }
    /**
     * Används aldrig, för jag orkar inte ändra alla anrop på den andra setDark(), 
     * men den skriver ut den nya rumsbeskrivningen automatiskt när man tänder/släcker.
     */
    public void setDark(boolean newValue, Game game){
        dark = newValue;
        System.out.println(getDescription(game));
    }
}
