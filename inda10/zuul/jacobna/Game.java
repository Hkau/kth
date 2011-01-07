import java.util.Scanner;
import java.awt.Point;
import java.util.HashMap;
import java.util.ArrayList;
import java.util.Random;
/**
 *  Detta �r World of Zuuls mainklass. Spelet g�r ut p� att f�nga alla "Fairies"
 *  som skapas p� slumpm�ssiga punkter p� kartan. Man har tillg�ng till en karta
 *  som uppdateras varje g�ng spelaren r�r p� sig i n�gon riktning. F�r att f�nga
 *  en "Farie" beh�ver man endast g� p� dess tile. De r�r dock p� sig slumpm�ssigt,
 *  vilket f�rsv�rar inf�ngadet av dem.
 * 
 *  F�r att spela spelet skapar man ett Game-objekt och kallar p� metoden play.
 * 
 *  Den h�r klassen skapar och uppdaterar alla andra klasser. Ingenting annat beh�ver
 *  k�ras f�r att spelet ska fungera. H�r finns vissa metoder som tar hand om data som
 *  endast �r relevanta f�r just den h�r spelinstansen och vad den g�r ut p�. S�som s�ttet
 *  spelare r�r p� sig, hur man f�ngar "Fairies" etc..
 * 
 * @author  Jacob Norlin Andersson
 * @version 2010.12.03
 */

public class Game 
{
    private Parser parser;
    private Map world;
    private Tile[][] MapGrid;
    private Actor player;
    private ArrayList<Command> commandArray;
    private ArrayList<NPC> NPCList;
    private String[] exitwords = {"east","north","west","south"};
    final private int TIMELIMIT = 50; //Antalet g�nger man f�r g� innan man f�rlorar.
    final private int CAPTURELIMIT = 7; //antalet du m�ste f�nga f�r att vinna.
    final private int CHANCETOMOVE = 50; //% chans som de man ska f�nga har att r�ra p� sig.
    final private String source = "nytuffkarta.png";
    int time = 0;
    int captures = 0; //Antalet du f�ngat.
    boolean finished = false;
    
    public static void main(String [ ] args){
        Game newGame = new Game();
    }  
        
    /**
     * Skapar spelet, v�rlden, spelaren och startar sj�lva spelet.
     */
    public Game() 
    {
        world= new Map(source);
        parser = new Parser();
        MapGrid = world.createMap();
    }



    /**
     *  Sj�lva spelmetoden. K�rs tills n�got f�rlustkrav uppfyllts.
     */
    public void play(){  
        Scanner input = new Scanner(System.in);
        System.out.println(">ENTER YOUR NAME PLEASE:");
        String playerName = input.nextLine();
        player = new Actor(playerName,0,0);
        printWelcome();
        createNPCS();
        createItems();
        defaultItems();//de saker vi b�rjar med.
 
        while (! finished) {
            if(checkWin() == true || checkLoss(time) == true){
                continue;
            }
            Command command = parser.getCommand();
            finished = processCommand(command);
            checkLoss(time);
            
        }
        System.out.println("Thank you for playing.  Good bye.");
    }
    
    /**
     * Kollar om antalet g�nger spelaren har r�rt p� sig �r lika med
     * det maximala antalet r�relser som �r till�tna.
     * @param time Hur m�nga r�relser spelaren gjort.
     */
    public boolean checkLoss(int time){
        if(time == TIMELIMIT){
            finished = true;
            System.out.println("You did not catch all the fairies in time. Sorry but you lost");
            return finished;
        }
        return false;
    }
    
    /**
     * Kontrollerar om spelaren har uppn�tt vinstvillkoret.
     */
    public boolean checkWin(){
        if(captures==CAPTURELIMIT){
            finished = true;
            System.out.println("Congratulations on catching all the Fairies. You have won the game, and can leave Zuul");
            return finished;
        }
        return false;
    }

    /**
     * Skriver ut v�lkomsmeddelandet.
     */
    private void printWelcome()
    {
        System.out.println();
        System.out.println("Welcome to the World of Zuul, "+player.getName()+".");
        System.out.println("Type '" + CommandWord.HELP + "' if you need help.");
        System.out.println("You can go in the directions: ");
        for(String s : exitwords){System.out.print(s+", ");}
        System.out.println();
    }
    
    /**
     * Skapar alla items p� kartan. Positionen och hur itemet ser ut
     * �r h�rdkodat. Allts� best�mmer skaparen av spelet var spelaren kan
     * hitta itemet, samt vad det �r.
     */
    public void createItems(){
        Item beamer = new Beamer(player,"beamer", "Teleports you to where you placed the beamer", 100);
        MapGrid[1][1].setItem(beamer); //placera grunkan.
    }
    
    public void defaultItems(){
        Item map = new Item(player,"Map","A magical map that shows everything in the world");
        player.addItemToInventory(map);
    }
    
    /**
     * Skapar alla datorstyrda karakt�rer p� kartan.
     * Varje karakt�r skapas p� slumpm�ssiga punkter p� kartan.
     */
    private void createNPCS(){
        NPCList = new ArrayList<NPC>();
        Random rand = new Random();
        int xMax = world.getxMax();
        int yMax = world.getyMax();
        for(int i = 0; i<CAPTURELIMIT; i++){
            int x = rand.nextInt(xMax);
            int y = rand.nextInt(yMax);
            NPC target = new NPC("Fairy",x,y,"Oh no you caught me!");
            NPCList.add(target);
        }
        
    }
    
    private void killNPC(NPC foundNPC){
        int i = 0;
        for(Object o : NPCList){
            NPC tempNPC = (NPC)o;
            if(tempNPC == foundNPC){
                NPCList.remove(i);
                return;
            }
            i++;
        }
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
            System.out.println("I don't know what you mean...");
            
            return false;
        }

        if (commandWord == CommandWord.HELP) {
            printHelp();
        }
        else if (commandWord == CommandWord.GO) {
            goRoom(command,player);
           
        }
        else if (commandWord == CommandWord.QUIT) {
            wantToQuit = quit(command);
        }
        else if (commandWord == CommandWord.USE) {
            player.useItem(command);
        }
        else if (commandWord == CommandWord.SHOW) {
            show(command);
        }
        // else command not recognised.
        return wantToQuit;
    }

 

    

    /**
     * Print out some help information.
     * Here we print some stupid, cryptic message and a list of the 
     * command words.
     */
    private void printHelp() 
    {
        System.out.println();
        System.out.println("You are in the World of Zuul.");
        System.out.println("The only way to escape is to catch all the fairies,");
        System.out.println("but you only have "+TIMELIMIT+" turns to do so.");
        System.out.println("You forfeit a turn everytime you walk.");
        System.out.println();
        System.out.println("These commands are available to you, it is up to you to figure out the rest.");
        parser.showCommands();
    }

    /** 
     * Flyttar p� spelaren om platsen(tilen) den f�rs�ker g� till finns eller �r m�jlig att g� p�.
     * Metoden behandlar allt som h�nder n�r kommandot "go" skrivs in. Den skriver ut alla information
     * som spelare beh�ver. D�ribland spelarens koordinater samt en karta. I metoden k�rs �ven metoden
     * f�r att flytta p� NPCerna, eftersom de g�r n�r spelaren f�rs�ker g�.
     * 
     * @param command Kommandot som ger vilket h�ll spelaren vill g� �t.
     * @param actor Den spelare som flyttas.
     */
    public void goRoom(Command command, Actor actor) 
    {
        //Det kanske vore vettigare att ha hela den h�r metoden i Actor-klassen men jag �r inte s�ker, s� den f�r ligga kvar h�r just nu.
        
        if(!command.hasSecondWord()) {
            // if there is no second word, we don't know where to go...
            System.out.println("Go where?");
            return;
        }
        
        moveNPCS(); // flytta p� alla npcr varje g�ng du g�r
        int i = 0;
        for(String s : exitwords){
            if(command.getSecondWord().equals(s)){
                
                Tile currentTile = actor.getCurrentTile(MapGrid);
                HashMap exits = currentTile.getExitMap();
                Point newPos = currentTile.getExitDirection(command.getSecondWord());
                int currentX = actor.getX();
                int currentY = actor.getY();
                int xMax = world.getxMax();
                int yMax = world.getyMax();
                int nextX = currentX+(int)newPos.getX(); // dit vi ska g�.
                int nextY = currentY+(int)newPos.getY();
                Tile nextTile = currentTile; //tilen vi vill g� till.
                boolean nextTileWalkable;
                
                
                if(nextX < 0 || nextY <0 || nextX>=xMax || nextY>=yMax){//Kolla s� vi inte g�r n�nting utanf�r banan
                    nextTileWalkable = false;
                }
                else{
                    nextTile = MapGrid[nextX][nextY];
                    nextTileWalkable = nextTile.getWalkable();//kan man g� p� tilen?
                }
        
                if (nextTileWalkable == false) {
                    System.out.println("You cannot go there!");//nej det kunde vi inte.
                }
                else {
                    
                    System.out.println("Your current coordinates are: ("+nextX+","+nextY+") and you are in a "+nextTile.getTypeName()+" tile. These are you surroundings: ");
//                     for(String direction : exitwords){
//                         int tempX=(int)nextTile.getExitDirection(direction).getX()+nextX;
//                         int tempY=(int)nextTile.getExitDirection(direction).getY()+nextY;
//                         System.out.print(direction+": ");
//                         if(tempX <= 0 || tempY <=0 || tempX>=xMax || tempY>=yMax){//Kolla s� vi inte g�r n�nting utanf�r banan. Inte code-reuse, g�r inte riktig samma sak.
//                             System.out.print("The unwalkable void");
//                         }
//                         else{
//                             System.out.print(MapGrid[tempX][tempY].getTypeName());
//                         }
//                         System.out.println();
//                     }
                    
                    takeItemOnTile(nextTile);
                    ifNpcOnTile(nextTile); //Om det �r en NPC p� rutan k�rs interaktionen mellan spelaren och NPCn.
                    
                    time++; // Om man lyckas g� f�r man en mindre tur p� sig att klara spelet.
                    actor.moveActor(nextX,nextY);
                    showMap(); //Rita kartan s� man vet var man �r.
                    
                   
                }
                return;
            }
            
            else if(i >= 3){
                System.out.println("I dont understand where you want to go.");
                i = 0;
            }
            i++;
        }
    

    }
    
    /**
     * Flyttar p� alla datorstyrda karakt�rer.
     */
    public void moveNPCS(){
        Random rand = new Random();
        int i = rand.nextInt(100);
        if(i<=CHANCETOMOVE){//chans att r�ra p� sig.
            for(Object t : NPCList){
                NPC NPC = (NPC)t;
                NPC.moveNPC(world.getxMax(),world.getyMax());
            }
        }
        
        
    }
    
    /**
     * Om det finns en datorstyrd karakt�r p� en plats kommer interaktionen mellan den och spelaren att k�ras.
     * Om det inte finns n�got p� rutan h�nder ingenting.
     * @param playerTile Den tile spelaren just nu st�r p�.
     */
    private void ifNpcOnTile(Tile playerTile){
        //System.out.println("Funktionen k�rs");
        for(Object t : NPCList){
            NPC foundNPC = (NPC)t;
            if(foundNPC.getCurrentTile(MapGrid) == playerTile){
                System.out.println("\nYou have captured a Fairy!");
                foundNPC.startDialog();
                killNPC(foundNPC); // D�da npcn n�r du f�ngar den.
                //System.out.println("Funktion k�rd");
                captures++;//L�gg till att du f�ngar en fiende..
                return;
            }
        }
    }
    
    /**
     * Tar upp item till spelaren om det finns p� tilen den st�r p�.
     * Om det inte finns n�got h�nder ingenting.
     * @param whichTile Vilken tile som kollas p�.
     */
    private void takeItemOnTile(Tile whichTile){
        if(whichTile.hasItem() == true){
            Item tileItem = whichTile.getItem();//Vilket item ligger p� tilen?
            //skriv ut lite junk.
            System.out.println("You have found a "+tileItem.getName()+"!");
            System.out.println(tileItem.getDescription());
            player.addItemToInventory(tileItem);
            whichTile.removeItem();//ta bort itemet s� vi inte kan ta upp det fler g�nger.
        }
    }
    
    /**
     * Metod till kommandot show.
     * Kommer visa det som spelaren ange ska visas. Om det inte finns skrivs ett felmeddelande ut.
     * @param command Vad som ska visas.
     */
    private void show(Command command){
        ArrayList<String> validCommands = new ArrayList<String>(); //De till�tna andrakommandona.
        validCommands.add("inventory");
        validCommands.add("map");
        String secondWord = command.getSecondWord();
        
        if(!command.hasSecondWord() || !validCommands.contains(secondWord)) {
            System.out.println("Show what?");
            return;
        }
        
        if (secondWord.equals("inventory")){
            player.showInventory();
        }
        else if(secondWord.equals("map")){
            showMap();
   
        }
    }

    /**
     * Skriver ut en karta �ver v�rlden som visar vilka typer av tiles som finns,
     * samt om det st�r n�gon enhet p� en tile.
     */
    public void showMap(){
        //Bildens dimensioner.
        int xMax = world.getxMax();
        int yMax = world.getyMax();
        int area = xMax*yMax;
                
        int[][] intGrid = world.getMapArray();//2D arrayen som inneh�ller alla f�rger.
        String[][] A1 = new String[xMax][yMax]; //En 2D array av typ string som kommer skrivas ut till slut.
        //D�r vi b�rjar l�sa.
        int x = 0;
        int y = 0;
        for(int i=0; i<area;i++){
            if(x>=xMax){ // Ny rad n�r vi f�r slut p� x-bitar.
                y+=1;
                x=0;
                System.out.println();
            }
            
            for(Object t : NPCList){//Vi kollar om n�gon NPC delar position med en tile.
                NPC NPC = (NPC)t;
                int tempX = NPC.getX();
                int tempY = NPC.getY();
                if(tempX == x && tempY == y){
                    A1[x][y] = "F ";
                    break;
                }
                else if(player.getX() == x && player.getY() == y){
                    A1[x][y] = "X ";
                }
                //Ange vilken bokstav som korresponerar mot vilken f�rg.
                else if(intGrid[x][y] == -16735512){
                A1[x][y] = "W ";
                }
                else if(intGrid[x][y] == -14503604){
                    A1[x][y] = "G ";
                }
                else if(intGrid[x][y] == -4621737){
                    A1[x][y] = "D ";
                }
        
            }
            
            System.out.print(A1[x][y]);//skriv ut det som vi kom fram till.
            x++;//N�sta pixel!
            }
            System.out.println();
            System.out.println("F = Fairy, W = Water, G = Grass, X = Player");//Map legend.
    }    
       
        
        



    /** 
     * "Quit" was entered. Check the rest of the command to see
     * whether we really quit the game.
     * @return true, if this command quits the game, false otherwise.
     */
    private boolean quit(Command command) 
    {
        if(command.hasSecondWord()) {
            System.out.println("Quit what?");
            return false;
        }
        else {
            return true;  // signal that we want to quit
        }
    }
}
