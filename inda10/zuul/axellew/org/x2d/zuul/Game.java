package org.x2d.zuul;
import java.awt.*;
import java.util.*;
import java.io.*;
import org.x2d.console.*;

/**
 *  This class is the main class of the "World of Zuul" application. 
 *  "World of Zuul" is a very simple, text based adventure game.  Users 
 *  can walk around some scenery. That's all. It should really be extended 
 *  to make it more interesting!
 * 
 *  To play this game, create an instance of this class and call the "play"
 *  method.
 * 
 *  This main class creates and initialises all the others: it creates all
 *  rooms, creates the parser and starts the game.  It also evaluates and
 *  executes the commands that the parser returns.
 * 
 * @author  Michael Kolling and David J. Barnes
 * @version 2008.03.30
 */
public class Game 
{
    private Parser parser;
    private Player player;
    private boolean notFinished = true;
    private static HashSet<String> usedNames = new HashSet<String>();
    public static enum Rooms {
        OUTDOOR, ENTRANCE, CORRIDORE1, PIT, DINING_ROOM, KITCHEN, CORRIDORE2,
        CORRIDORE3, CORRIDORE4, STORE_ROOM, BRIDGE, GARDEN, TAVERN, TEMPLE, TELEPORT;
    }
    public static enum Directions {
        NORTH("norr"), SOUTH("söder"), WEST("väster"), EAST("öster");
        Directions(String value) {
            this.value = value;
        }
        private String value;
        public String getValue() {
            return value;
        }
    }
    private EnumMap<Rooms, Room> map = new EnumMap<Rooms, Room>(Rooms.class);
    private HashMap<String, Character> characters = new HashMap<String, Character>();
    
    /**
     * Starts a new game.
     */
    public static void main(String args[]) {
        Game g = new Game();
        g.play();
    }
        
    /**
     * Create the game and initialise its internal map.
     */
    public Game() 
    {
        new ConsoleGUI();
        createParser();
        createPlayer();
        createRooms();
    }

    /**
     * Create all the rooms and link their exits together.
     */
    private void createRooms()
    {
        Room outdoor, entrance, corridore1, pit, diningRoom, kitchen, corridore2, corridore3, corridore4, storeRoom;
        Room bridge, garden, tavern, temple, teleport;
        Door tmpDoor;
        //Creates rooms
        map.put(Rooms.ENTRANCE, entrance = new Room("Det är ett mörkt och dystert rum, endast upplyst av några facklor. Golv och väggar är gjorda av stora massiva stenar. Du hör musik komma från den södra dörren."));
        map.put(Rooms.OUTDOOR, outdoor = new Room("Du är nu utanför borgen. Solen lyser och allt du vet är att du aldrig vill återvänta till den mörka borgen."));
        map.put(Rooms.CORRIDORE1, corridore1 = new Room("Du kommer in i en gång som forsätter så långt du kan se, in i själva berget. Ser ut att vara en gammal övergiven gruvgång. Det är helt mörkt längre in i gången."));
        map.put(Rooms.PIT, pit = new Room("Du såg inget i mörkret och ramla ner i ett gammalt gruvschakt. Du känner efter åt alla håll men du hittar bara solid sten."));
        map.put(Rooms.DINING_ROOM, diningRoom = new Room("Det är musik och rörelse i rummet. Det sitter 4 personer vid det ena bordet som ser ut som de inte vill bli störda. Det sitter en ensam man vid ett av de andra och äter."));
        map.put(Rooms.KITCHEN, kitchen = new Room("Du har kommit in i ett kök. Vilka det än var som lagade maten så är de inte kvar längre men du känner lukten av mat som de har lämnat kvar."));
        map.put(Rooms.CORRIDORE2, corridore2 = new Room("Du hör musik som kommer från den norra dörren."));
        map.put(Rooms.CORRIDORE3, corridore3 = new Room("Det ligger mängder av obetydliga saker på golvet som ser ut att ha ramlat av diverse transporter. Det ser ut som de har kommit eller gått från den västra utgången."));
        map.put(Rooms.CORRIDORE4, corridore4 = new Room("Mängder av skräp på golvet, utöver det finns inget av betydelse."));
        map.put(Rooms.STORE_ROOM, storeRoom = new Room("Nu förstår du vad allt skräp i de tidigare gången kom ifrån. Du har kommit in i en lagerlokal där det finns massor av lådor och tunnor staplade längs väggarna."));
        map.put(Rooms.BRIDGE, bridge = new Room("Du har kommit ut och står på en bro. Den går över en å som går långt nedanför bron. Du kan skymta träd och grönska söder ut och du ser borgen bakom dig."));
        map.put(Rooms.GARDEN, garden = new Room("Du står i en park med en fontän i mitten. Väster ut ser du ett tempel och öster ut ser du ett värdshus."));        
        map.put(Rooms.TEMPLE, temple = new Room("Det är en lugn och tyst plats. Du har kommit in borgens tempel. Du känner att inget kan gå fel så länge du är inne i templet och att alla dina problem snart kommer att ordna sig."));        
        map.put(Rooms.TELEPORT, teleport = new Room("Du kommer längre in i templet och ser ett blått sken lysa mot en sten i mitten av rummet."));        
        map.put(Rooms.TAVERN, tavern = new Room("Du har kommit in i värdshuset. Det är en livlig miljö med massor av folk och ljudnivån är hög."));

        //Creates doors
        tmpDoor = new Door(true);
        tmpDoor.setUnlockItem(Items.STONE_OF_DELEN);
        entrance.setEntrance(Directions.NORTH.getValue(), outdoor, tmpDoor);
        entrance.setEntrance(Directions.WEST.getValue(), corridore1, new Door());
        entrance.setEntrance(Directions.SOUTH.getValue(), diningRoom, new Door());
        corridore1.setEntrance(Directions.WEST.getValue(), pit, new Door());
        pit.setExit(Directions.EAST.getValue(), null);
        diningRoom.setEntrance(Directions.WEST.getValue(), kitchen, new Door());
        diningRoom.setEntrance(Directions.SOUTH.getValue(), corridore2, new Door());
        kitchen.setEntrance(Directions.WEST.getValue(), null, new Door(true));
        tmpDoor = new Door(true);
        tmpDoor.setUnlockItem(Items.STORE_ROOM_KEY);
        corridore2.setEntrance(Directions.SOUTH.getValue(), corridore3, tmpDoor);
        corridore3.setEntrance(Directions.WEST.getValue(), corridore4, new Door());
        corridore3.setEntrance(Directions.SOUTH.getValue(), bridge, new Door());
        corridore4.setEntrance(Directions.WEST.getValue(), storeRoom, new Door());
        storeRoom.setEntrance(Directions.NORTH.getValue(), null, new Door(true));
        bridge.setEntrance(Directions.SOUTH.getValue(), garden, new Door());
        tmpDoor = new Door(true);
        tmpDoor.setUnlockItem(Items.SEAL);
        garden.setEntrance(Directions.WEST.getValue(), temple, tmpDoor);
        garden.setEntrance(Directions.EAST.getValue(), tavern, new Door());
        temple.setEntrance(Directions.WEST.getValue(), teleport, new Door());
        temple.setEntrance(Directions.SOUTH.getValue(), null, new Door(true));
        
        //Adds items and characters to the rooms
        entrance.addItem(Items.DOOR_SIGN);
        corridore1.addItem(Items.WARNING_SIGN);
        pit.addItem(Items.NOTE);
        pit.addItem(Items.SKELETON);
        diningRoom.addItem(Items.MENU);
        diningRoom.addItem(Items.CHAIRS);
        diningRoom.addItem(Items.TABLES);
        kitchen.addItem(Items.FOOD);
        kitchen.addItem(Items.TABLES);
        corridore2.addItem(Items.BENCH);
        Character beggar = new Beggar();
        characters.put(beggar.getName(), beggar);
        corridore2.addCharacter(beggar);
        storeRoom.addItem(Items.BOXES);
        storeRoom.addItem(Items.PRAYER_BEADS);
        garden.addItem(Items.BENCH);
        Character cat = new Cat();
        characters.put(cat.getName(), cat);
        garden.addCharacter(cat);
        Character templeGuard = new TempleGuard();
        characters.put(templeGuard.getName(), templeGuard);
        garden.addCharacter(templeGuard);
        temple.addItem(Items.STONE_OF_DELEN);
        teleport.addItem(Items.HOLY_STONE);
        tavern.addItem(Items.TABLES);
        tavern.addItem(Items.CHAIRS);
        Character priest = new Priest();
        characters.put(priest.getName(), priest);
        tavern.addCharacter(priest);
    }
    
    //Creates the parser and adds all the possible commands.
    private void createParser() {
        parser = new Parser(this);
    }
    
    //Creates the player.
    private void createPlayer() {
        player = new Player(generateName());
    }
    
    /**
     * Generates a name to be used for characters.
     *
     * @return A string with 3-8 chars which can be used as a character name.
     */
    public static String generateName() {
        String vocals = "aeiouy";
        String consonants = "bcdfghjklmnpqrstvwxz";
        int nameLength = (int)(Math.random()*5+3);
        StringBuilder name = new StringBuilder(nameLength);
        //Creates a random name but with two rules:
        //1: a vocal should not be followed by a second vocal
        //2: there can't be more than 2 consonants in a row
        for (int i=0;i<nameLength;i++) {
            boolean vocal;
            if (i>0) {
                if (vocals.indexOf(name.charAt(i-1))!=-1) {
                    vocal = false;
                } else if(i>1&&consonants.indexOf(name.charAt(i-1))!=-1) {
                    vocal = true;
                } else {
                    vocal = (Math.random()<0.3);
                }
            } else {
                vocal = (Math.random()<0.3);
            }
            if (vocal) {
                name.append(vocals.charAt((int)(Math.random()*vocals.length())));
            } else {
                name.append(consonants.charAt((int)(Math.random()*consonants.length())));
            }
        }
        name.setCharAt(0, java.lang.Character.toUpperCase(name.charAt(0)));

        String finalName = name.toString();
        //If the name is already used: generat a new
        if (usedNames.contains(finalName)) {
            return generateName();
        }
        usedNames.add(finalName);
        return finalName;
    }

    /**
     *  Main play routine. Loops until end of play.
     */
    public void play() 
    {            
        printWelcome();
        player.setCurrentRoom(getRoom(Rooms.ENTRANCE));
        while (notFinished) {
            parser.getCommand();
			moveCharacters();
        }
    }
    
    //Moves characters that are marked as walk randomly
    private void moveCharacters() { 
        for (Character c : characters.values()) {
            if (c.isWalkingRandomly() && Math.random()<0.2) {
                Room cRoom = c.getCurrentRoom();
                String[] exits = cRoom.getExits();
                ArrayList<Room> possibleRooms = new ArrayList<Room>(exits.length);
                for (int i=0;i<exits.length;i++) {
                    Room checkRoom = cRoom.getDoor(exits[i]).getExit(cRoom);
                    if (checkRoom != null && checkRoom != getRoom(Rooms.PIT)) {
                        possibleRooms.add(checkRoom);
                    }
                }
                if (possibleRooms.size()>0) {
                    Room newRoom = possibleRooms.get((int)(Math.random()*possibleRooms.size()));
                    cRoom.removeCharacter(c);
                    newRoom.addCharacter(c);
                    if (cRoom == getPlayer().getCurrentRoom()) {
                        System.out.println(String.format("%s gick iväg.", c.getName()));
                    } else if (newRoom == getPlayer().getCurrentRoom()) {
                        System.out.println(String.format("%s kom in i rummet.", c.getName()));
                    }
                }
            }
        }
    }

    /*
     * Print out the opening message for the player.
     */
    private void printWelcome()
    {
        System.out.print("Du har blivit insläpad i borgen men det visade sig vara ett missförstånd. Vakten har gett sig iväg. Du vill bara komma ut igen men dörren är låst.");
        System.out.println(String.format(" Ditt namn är %s", getPlayer().getName()));
        System.out.println("Skriv 'hjälp' ifall du behöver hjälp eller 'hjälp <kommando>' för att lista möjliga mål för ett kommando, t.ex: 'hjälp gå'.");
    }

    /**
     * Returns the player.
     *
     * @return The player.
     */
    public Player getPlayer() {
        return player;
    }
    
    /** 
     * Gets the parser
     *
     * @return The parser.
     */
    public Parser getParser() {
        return parser;
    }
    
    /** 
     * Gets a room from the map using the enum Rooms.
     *
     * @param name The enum linked to the room.
     * @return The room.
     */
    public Room getRoom(Rooms name) {
        return map.get(name);
    }
    
    /**
     * Ends the game.
     *
     * @param completed If <code>true</code> print a success message and if <code>false</code>
     * print a fail message.
     */
    public void endGame(boolean completed) {
        notFinished = false;
        if (completed) {
            System.out.println("Du klarade det, du kom ut ur borgen!");
        } else {
            System.out.println("Du har misslyckats!");
        }
    }
    
    /**
     * Saves the current game to a file so that it can be loaded later.
     *
     * @param f The file where the game will be saved.
     */
    
    public void saveGame(File f) {
        /*
         * Tries to save the game with an object output stream. Writes all the important classes
         * to the file so the information can be loaded later.
         */
        try {
            ObjectOutputStream out = new ObjectOutputStream(new FileOutputStream(f));
            out.writeObject(player);
            out.writeObject(map);
            out.writeObject(characters);
            out.flush();
            out.close(); 
        } catch (IOException e) {
            System.out.println("Misslyckades att spara filen med felmeddelandet: "+e.getMessage());
        }
    }
    
    @SuppressWarnings("unchecked")
    public void loadGame(File f) {
        /*
         * Tries to load the game from the file by using an object input stream.
         * It reads one object at a time and tries to cast it to the correct class.
         * If any of these casts or the reading would fail the game could not be loaded.
         */
        try {
            ObjectInputStream in = new ObjectInputStream(new FileInputStream(f));
            Player tmpPlayer = (Player)in.readObject();
            EnumMap<Rooms, Room> tmpMap = (EnumMap<Rooms, Room>)in.readObject();
            HashMap<String, Character> tmpCharacter = (HashMap<String, Character>)in.readObject();
            player = tmpPlayer;
            map = tmpMap;
            characters = tmpCharacter;
            in.close();
        } catch (IOException e) { //Problem with the stream
            System.out.println("Misslyckades att ladda filen med felmeddelandet: "+e.getMessage());
        } catch (ClassNotFoundException e) { //Problem reading a class from the stream
            System.out.println("Kan inte ladda sparat spel, troligen för att det är av en gammal version av spelet.");
        } catch (ClassCastException e) { //The wrong class was read from the stream.
            System.out.println("Kan inte ladda sparat spel, troligen för att det är av en gammal version av spelet.");
        }
    }
}
