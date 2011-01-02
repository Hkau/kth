import java.util.HashMap;
import java.util.ArrayList;
/**
 *  This class is the main class of the "World of Zuul" application.
 *  "World of Zuul" is a very simple, text based adventure game.
 *
 *  To play this game, create an instance of this class and call the "play"
 *  method.
 *
 *  This main class creates and initialises all the others: it creates all
 *  rooms, creates the parser and starts the game.  It also evaluates and
 *  executes the commands that the parser returns.
 *
 * @author  Matilda Valleryd
 * @version 2010.12.2
 */

import java.util.Random;

public class Game
{
    private Parser parser;
    private Room currentRoom;
    private NPC currentNPC;
    private Item currentItem;
    private Quest currentQuest;
    //These ints are used to keep track of what time it is in the game, hour++ for each 12 time++. Time icreases when you move to a different room.
    private int time=0, hour=10;

    private static final int TIME_TO_FINISH = 18; //Time runs out and the game ends when the "hour" reaches this number
    private static final int NUMBER_TO_WIN = 10; //Number of followers needed to win the game

    private boolean finished = false;

    Player player;

    //Map over the NPCs and items
    private HashMap<Room, NPC> currentNPCRooms;
    private HashMap<Room, Item> thereIsStuffHere; //I couldn't think of a better name, it's the map over items

    //Declair the rooms, NPCs, Items and Quests
    Room tsnabben, tunnelbanan, dodsstjarnan, esc, gratiskyl, utanforesc;
    Room borggarden, d, e, gra, nymble, karbokhandeln, q, v, k, biblioteket;
    Room oskarsbacke, f, mattetornet, ksektion, mediaclsektion, vandplan;
    Room fonsterGlugg, brun;
    NPC valle, louise, lemming, snilsson, hime, hultis, smallhacker;
    NPC david, koronen, schlyter, erik, tess, gulleric;
    Item canOfCola, passerkort, projektor, bulle;
    Quest followerValle, followerHultis;
    Quest itemCola, itemBulle, itemProjektor, itemKort;
    Quest timeLater, timeAfter4;
    Quest riddleInda, riddleFF;


    /**
     * Create the game and initialise its internal map.
     */
    public Game()
    {
        player = new Player();
        currentNPCRooms = new HashMap<Room, NPC>();
        thereIsStuffHere = new HashMap<Room, Item>();
        createRooms();
        createItems();
        createQuests();
        createNCPs();
        createFollowerQuests(); //NPCs needed to create these quests
        createNPCsUsingFollowerQuests();
        parser = new Parser();

        //Set the room in which the game starts and gets eventual NPC and items in that room
        currentRoom = oskarsbacke;  // The game starts here
        currentNPC = getCurrentNPC();
        currentItem = getCurrentItem();
        if (currentNPC != null)
            currentQuest = currentNPC.getQuest();

    }



    /**
     *  Main play routine.  Loops until end of play.
     */
    public void play()
    {
        printWelcome();

        // Enter the main command loop.  Here we repeatedly read commands and
        // execute them until the game is over.

        while (! finished) {
            Command command = parser.getCommand();
            finished = processCommand(command);
        }
        printQuitMessage();
    }

    /**
     * Print out the opening message for the player.
     */
    private void printWelcome()
    {
        System.out.println();
        System.out.println("Välkommen till Zuul: Valle edition.");
        System.out.println("Skriv '" + CommandWord.HELP + "' om du behöver hjälp.");
        System.out.println();
        System.out.println("Du har precis anlänt till skolan när du får de hemska");
        System.out.println("nyheterna att Henrik Eriksson kommer tvångspesnioneras");
        System.out.println("ikväll kl 18. Du tror att om du får ihop tillräckligt");
        System.out.println("många till en demonstration och ta med dig dessa till");
        System.out.println("dödsstjärnan så kan du förhindra detta. Må kraften vara med dig.");
        System.out.println();
        System.out.println(currentRoom.getLongDescription());
    }

    /**
     * Given a command, process (that is: execute) the command.
     * @param command The command to be processed.
     * @return true If the command ends the game, false otherwise.
     */
    private boolean processCommand(Command command)
    {
        boolean wantToQuit = false;

        CommandWord commandWord = command.getCommandWord();

        if(commandWord == CommandWord.UNKNOWN) {
            System.out.println("Jag förstår inte vad du menar...");
            return false;
        }

        if (commandWord == CommandWord.HELP) {
            printHelp();
        }
        else if (commandWord == CommandWord.GO) {
            goRoom(command);
        }
        else if (commandWord == CommandWord.QUIT) {
            wantToQuit = quit(command);
        }
        else if (commandWord == CommandWord.TAKE) {
            take();
        }
        else if (commandWord == CommandWord.TALK) {
            talk();
        }
        else if (commandWord == CommandWord.WAIT) {
            waitForIt();
        }

        // else command not recognised.
        return wantToQuit;
    }



    //METHODS RESPONDING TO COMMANDS
    /**
     * Print out some help information.
     * Here we print some stupid, cryptic message and a list of the
     * command words.
     */
    private void printHelp()
    {
        System.out.println("Du vandrar runt på campus och försöker");
        System.out.println("värva personer till en demonstration.");
        System.out.println();
        System.out.println("Dina commandon är");
        parser.showCommands();
    }

    /**
     * Try to go to one direction. If there is an exit, enter the new
     * room, otherwise print an error message.
     * Responds to the GO-method
     */
    private void goRoom(Command command)
    {
        if(!command.hasSecondWord()) {
            // if there is no second word, we don't know where to go...
            System.out.println("Gå vart?");
            return;
        }

        String direction = command.getSecondWord();

        // Try to leave current room.
        Room nextRoom = currentRoom.getExit(direction);


        if (nextRoom == null) {
            System.out.println("Det finns ingen dörr där!");
        }
        else {
            currentRoom = nextRoom;
            System.out.println(currentRoom.getLongDescription());
            time++;
            if ((time%12) == 0) {
                hour++;
                if (hour == TIME_TO_FINISH)
                    lose();
                System.out.println("Nu är klockan " + hour + " och du har med dig " + player.getNumberOfFollowers() + " anhängare." );
            }
        }

        //Item in this room:
        currentItem = getCurrentItem();
        if (currentItem != null) {
            System.out.println("Här finns " + currentItem.getDescription() + ".");
        }

        //NPC in this room
        currentNPC = getCurrentNPC();
        if (currentNPC != null) {
            System.out.println(currentNPC.getNPCDescription() + ".");
        }

    }

    /**
     * Talk with NPC
     * Responds to the TALK-method
     */
    private void talk() {
        //Check if there is someone to talk to, then check if the quest exists and if so, tries to complete it
        if (currentNPC != null) {
            if (currentNPC.checkForQuest()) {
                tryToFinishCurrentQuest();
            }
            else { //If Quest is null
                //Prints a message indicating the NPC wanting to join the demonstration then calls the follow()-method to make it so
                System.out.println(currentNPC.getNPCDescriptionAfter());
                follow();
            }
        }
            else
                System.out.println("Det finns ingen här att tala med...");

    }

    /**
     * Method for waiting util 18 a-clock
     * Responds to the WAIT-method
     */
    private void waitForIt() {
        //Checking if player is sure he/she wants to wait
        System.out.println("Är du säker på att du vill vänta till kl 18? Du kommer hamna automatiskt vid dödsstjärnan. (j/n)");
        String reply = parser.getInput();

        if (reply.equals("n")) {
            //If not, just remind where you are
            System.out.println(currentRoom.getLongDescription() + ".");
        }
        if (reply.equals("j")) {
            if (player.getNumberOfFollowers() >= NUMBER_TO_WIN)
                win();
            else
                lose();
        }
    }

    /**
     * Placing items in the inventory
     * Responds to the "TAKE"-command
     */
    private void take() {
        if (currentItem != null){
            System.out.println("Du tar " + currentItem.getDescription());
            player.setItemToInventory(currentItem);
            setItemRoom(currentRoom, null);
        }
        else
        System.out.println("Det finns inget här att ta...");
    }




    //CREATIONMETHODS
    /**
     * Create all the rooms and link their exits together.
     */
    private void createRooms()
    {

        // create the rooms
       tsnabben = new Room("på t-snabben. I Franks kostar 10 kr. Finns massa gott här");
       tunnelbanan = new Room("utanför tunnelbanespärrarna");
       dodsstjarnan = new Room("vid dödsstjärnan");
       esc  = new Room("hemma");
       utanforesc = new Room("i oskarsbacke utanför esc");
       borggarden = new Room("på borggården");
       d = new Room("i D-huset");
       e = new Room("i E-huset");
       gra  = new Room("i grå");
       nymble  = new Room("på nymble");
       karbokhandeln = new Room("i kårbokhandeln");
       q = new Room("vid Q-huset");
       v = new Room("vid väg och vatten");
       k = new Room("Vid kemi");
       biblioteket = new Room("i biblioteket. Försök att vara tyst");
       oskarsbacke = new Room("i oskarsbacke utanför biblioteket");
       f = new Room("vid fysik. Fysikerna är på Alba Nova tror du");
       mattetornet = new Room("i mattetornet. Du är trött i benen");
       ksektion = new Room("i kemis sektionslokal");
       mediaclsektion = new Room("i medias och CLs sektionslokal");
       vandplan  = new Room("på vändplan");
       gratiskyl = new Room("i gratiskylen");
       fonsterGlugg = new Room ("i en fönsterglugg där projektorn borde stå men det gör den inte");
       brun = new Room ("i datorsalen Brun");


        // initialise room exits
        tsnabben.setExit("ut", tunnelbanan);
        tunnelbanan.setExit("söder", tsnabben);
        tunnelbanan.setExit("öst", dodsstjarnan);
        tunnelbanan.setExit("norr", utanforesc);
        dodsstjarnan.setExit("väst", tunnelbanan);
        dodsstjarnan.setExit("norr", borggarden);
        esc.setExit("ut", utanforesc );
        esc.setExit("in", gratiskyl);
        esc.setExit("framåt", fonsterGlugg);
        utanforesc.setExit("in", esc);
        utanforesc.setExit("norr", oskarsbacke);
        utanforesc.setExit("öst", e);
        utanforesc.setExit("väst", nymble);
        utanforesc.setExit("söder", tunnelbanan);
        borggarden.setExit("norr", f);
        borggarden.setExit("söder", dodsstjarnan);
        borggarden.setExit("öst", d);
        borggarden.setExit("väst", e);
        d.setExit("norr", mattetornet);
        d.setExit("väst", borggarden);
        e.setExit("upp", brun );
        e.setExit("öst", borggarden);
        e.setExit("väst", utanforesc);
        gra.setExit("ner", brun);
        nymble.setExit("in", karbokhandeln);
        nymble.setExit("norr", q);
        nymble.setExit("öst", utanforesc);
        karbokhandeln.setExit("ut", nymble);
        q.setExit("söder", nymble);
        q.setExit("norr", v);
        v.setExit("söder", q);
        v.setExit("öst", vandplan);
        v.setExit("in", mediaclsektion);
        k.setExit("in", ksektion);
        k.setExit("söder", f);
        k.setExit("väst", vandplan);
        biblioteket.setExit("ut", oskarsbacke);
        oskarsbacke.setExit("in", biblioteket);
        oskarsbacke.setExit("söder", utanforesc);
        oskarsbacke.setExit("norr", vandplan);
        oskarsbacke.setExit("öst", f);
        f.setExit("öst", mattetornet);
        f.setExit("väst", oskarsbacke);
        f.setExit("norr", k);
        f.setExit("söder", borggarden);
        mattetornet.setExit("väst", f);
        mattetornet.setExit("söder", d);
        ksektion.setExit("ut", k);
        mediaclsektion.setExit("ut", v);
        vandplan.setExit("söder", oskarsbacke);
        vandplan.setExit("öst", k);
        vandplan.setExit("väst", v);
        fonsterGlugg.setExit("bakåt", esc);
        brun.setExit("upp", gra);
        brun.setExit("ner", e);

    }

    /**
     * Creates NPCs and places them on the map
     */
    private void createNCPs() {

        //Create the NPCs
        //NPCparameters: String name, String description, String descriptionBeforQuestIsFinished, String descriptionWhenQuestIsFinished/null, Quest questNeedingToBeFullfilled
        valle = new NPC("Valle", "Vid kylen står Valle och ser besviken ut.", "'Cola Zeron är slut! Och jag som trodde ingen annan drack det...'", "'Okej, nu är jag glad, då bangar jag aldrig!'", itemCola);
        lemming = new NPC("Lemming", "Där står Lemming med en kopp te", null, "'Henrik Eriksson är kung. Klar man är på.'", null);
        snilsson = new NPC ("Snilsson", "Snilsson, indaläraren, går längs tavlan med en svamp i varje hand", "'Om du kan svara på en indafråga först!'", "'Okej, då går vi.'", riddleInda);
        hime = new NPC("Hime", "Utanför Delphi står Hime och trampar", "'Jag tappade mitt passerkort under mottagningen men delphi har aldrig öppet! Jag kan inte förljag med utan mitt kort.'", "'Yay, mitt kort! Låt oss gå.'", itemKort);
        hultis = new NPC("Hultis", "Det sitter en långhårig kille och kodar här. Du känner igen honom som Hultis.", null, "'Henrik Eriksson? Jag hänger på!'", null);
        david = new NPC("David", "Här hittar duodetrecesiumsen David som ser glad ut.", "'Jag följer bara med om du vet detta!'", "'Okej, du är reko!'", riddleFF);
        koronen = new NPC("QA Koronen", "QA Koronen är också här och ser ytterst förvirrad ut.", "'Jag kan inte hitta projektorn! Det är jättevikigt, annars blir det ingen filmkväll!'", "'Najs, jag följer med då.'", itemProjektor);
        schlyter = new NPC("Schlyter", "Schlyter sitter här och kodar som vanligt.", "'Jag är lite upptagen. Kom tillbaka efter 4.'", "'Okej, jag är klar nu.'", timeAfter4);
        erik = new NPC("Erik", "Här sitter Erik aka Wildfyre och pluggar.", "'Jag följer med om jag får en lussebulle.' :3", "*mums*", itemBulle);
        tess = new NPC("Tess", "Söta Tess sitter här och pluggar.", null, "'Jag har aldrig haft honom i något men jag vill gärna ha det i framtiden så jag följer med.'", null);
        gulleric = new NPC("Gulleric", "I kön står Gulleric.", "'Jag måste köa klart först...'", "'Jag klarade tentan!' :D 'Nu kan vi gå.'", timeLater);


        //Places the NPCs
        setNPCRoom(tsnabben, valle);
        setNPCRoom(vandplan, lemming);
        setNPCRoom(d, snilsson);
        setNPCRoom(e, hime);
        setNPCRoom(gra, hultis);
        setNPCRoom(borggarden, david);
        setNPCRoom(v, koronen);
        setNPCRoom(brun, schlyter);
        setNPCRoom(fonsterGlugg, erik);
        setNPCRoom(biblioteket, tess);
        setNPCRoom(mattetornet, gulleric);
    }

    /**
     * Creates and places the items on the map
     */
    private void createItems() {
        //create the items
        canOfCola = new Item("en burk 33 cl Cocla-Cola Zero");
        passerkort = new Item("ett blått magnetkort. Du har ett likadant");
        bulle = new Item("en rykande färsk lussebulle");
        projektor = new Item("en rätt dyr projektor att visa film på");


        //Places the items
        setItemRoom(esc, canOfCola);
        setItemRoom(q, passerkort);
        setItemRoom(tsnabben, bulle);
        setItemRoom(ksektion, projektor);
    }

    /**
     * Creates the quests
     */
    private void createQuests() {
        Random randomGenerator = new Random();
        int randomNumber = randomGenerator.nextInt(5);
        String[] indaQuestions = {"'Vad kallas den loop där man har en räknare och ett villkor?'","'Vilken datatyp är detta? 1337.'", "'Borde fält vara privata eller publika?'", "'Vilken typ av variabel bör skrivas i caps?'", "'Vad saknas i denna syntax? int[] numbers = int[5]'"};
        String[] indaAnswers = {"for","int", "privata", "konstanter", "new"};

        //Creates the quests
        //Itemquestparameters: Item itemNeededToFinishQuest
        //Timequestparameters: int timeToExceesToFinishQuest
        //Followerquestparameters: NPC npcNeededAsFollower
        //Riddlequestparameters: String Riddle, String correctAnswer
        itemCola = new Quest(canOfCola);
        itemBulle = new Quest(bulle);
        itemKort = new Quest(passerkort);
        itemProjektor = new Quest(projektor);

        timeLater = new Quest(13);
        timeAfter4 = new Quest(16);
        riddleInda = new Quest (indaQuestions[randomNumber], indaAnswers[randomNumber]);
        riddleFF = new Quest ("'Vem är den onda snubben i FF6?'", "Kefka");
    }

    /**
     * Creates the quests with NPCs as a condition
     */
    private void createFollowerQuests() {
        followerValle = new Quest(valle);
        followerHultis = new Quest(hultis);
    }

    /**
     * Creates the NPCs using follower quests
     */
    private void createNPCsUsingFollowerQuests() {
        louise = new NPC ("Louise", "Där står Louise, den söta duodetrecesiman med det ljusa, korta håret och hänger.", "Hon väntar på Valle som köper cola men om Valle går så går hon.", "'Jag går dit Valle går.' <3", followerValle);
        smallhacker= new NPC("Smallhacker", "Smallhacker sitter vid ett bort och pillar på något.", "'Behöver fråga Hultis en sak. ... '", "...", followerHultis);

        setNPCRoom(tunnelbanan, louise);
        setNPCRoom(nymble, smallhacker);
    }



    //HELPMETHODS
    /**
     * Method letting NPCs join the demonstration
     */
    private void follow() {
        player.addFollower(currentNPC);
        setNPCRoom(currentRoom, null);
    }

    /**
     * Generates a random quit message
     */
    private void printQuitMessage() {
        Random generator = new Random();
        String[] messages = {"Dags att plugga? GL HF, tack för att du spelade", "Stick då.", "Hej då!", "Du ger upp och din avatar dör i obeskrivlig smärta", "/quit"};
        System.out.println(messages[generator.nextInt(5)]);
    }

    /**
     *
     */
    private void tryToFinishCurrentQuest() {
        currentQuest = currentNPC.getQuest();
                if (currentQuest.getQuestFinished() == false) {
                    String questType = currentQuest.getType();

                    if(!questType.equals("riddle")) // If the type is "time" it does not print out the beforesting
                        System.out.println(currentNPC.getNPCDescriptionBefore() );

                    if (questType.equals("time")) {
                        currentQuest.tryToFinishTimeQuest(hour);
                        if (currentQuest.getQuestFinished() == false)
                            System.out.println(currentNPC.getNPCDescriptionBefore());
                        }

                    if (questType.equals("riddle"))
                        currentQuest.tryToFinishRiddleQuest();

                    if (questType.equals("follower")) {
                        ArrayList<NPC> currentFollowers = player.getCurrentFollowers();
                        currentQuest.tryToFinishFollowerQuest(currentFollowers);
                    }

                    if (questType.equals("item")) {
                        ArrayList<Item> currentInventory = player.getCurrentInventory();
                        currentQuest.tryToFinishItemQuest(currentInventory);
                    }
                }

                //Checks again if quest is complete
                if (currentQuest.getQuestFinished() == true) {
                     System.out.println(currentNPC.getNPCDescriptionAfter());
                     follow();
                }

    }



    //METODS FOR FINISHING THE GAME
    /**
     * Method called when time runs out or the number of followers is insufficient
     */
    private void lose() {
        System.out.println("Tiden är ute! Det kommer ingen och demonstrerar. Henrik Eriksson fäller en tår och går i pension.");
        System.out.println("Du förlorade om det var otydligt.");
        finished = true;
    }

    /**
     * Method called when the game is win
     */
    private void win() {
        System.out.println("När Henrik Eriksson tillsammans med rektorn passerar dödsstjärnan");
        System.out.println("möts de av " + player.getNumberOfFollowers() + " demonstranter som");
        System.out.println("alla vädjar att Henrik ska få stanna på KTH ett år till. Henrik");
        System.out.println("blir jätteglad när han ser att ");
        player.printFollowers();
        System.out.println("har samlats här idag för hans skull. De beslutar att låta");
        System.out.println("honom stanna kvar! Grattis!");
        System.out.println("Du vann om du inte förstod det...");
        finished = true;
    }

    /**
     * "Quit" was entered. Check the rest of the command to see
     * whether we really quit the game.
     * @return true, if this command quits the game, false otherwise.
     */
    private boolean quit(Command command)
    {
        if(command.hasSecondWord()) {
            System.out.println("Sluta vad?");
            return false;
        }
        else {
            return true;  // signal that we want to quit
        }
    }



    //METHODS FOR PLACING OBJECTS
    /**
     * Place the NPCs
     * @param Room: The room where we wish to place the NPC
     * @param NPC: The NPC we wish to place
     */
    private void setNPCRoom(Room room, NPC npc) {
        currentNPCRooms.put(room, npc);
    }

    /**
     * Places the items
     * @param Room: The room where we wish to place the item
     * @param Item: The item we wish to place
     */
    private void setItemRoom (Room room, Item item) {
        thereIsStuffHere.put(room, item);
    }



    //METHODS FOR GETTING OBJECTS
    /**
     * Get the NPC in this room
     * @return Room: The current room of the player
     */
    private NPC getCurrentNPC () {
        return currentNPCRooms.get(currentRoom);
    }

    /**
     * Get the item in this room
     * @return Item: The item in the current room
     */
    private Item getCurrentItem() {
        return thereIsStuffHere.get(currentRoom);
    }
}
