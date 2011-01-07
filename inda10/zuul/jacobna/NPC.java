import java.util.Random;
/**
 * NPC �r en subklass till Actor och anv�nds f�r att simulera
 * datorstyrda spelare. Den inneh�ller n�gra separata funktioner
 * som att skriva ut dialog och r�ra sig slumpm�ssigt som spelaren
 * inte har n�got behov att g�ra.
 * 
 * @author Jacob Norlin Andersson 
 * @version 2010.12.02
 */
public class NPC extends Actor
{

    private String dialog;
    private boolean met = false; // har den tr�ffat n�gon?
    /**
     * Skapar en NPC p� angiven plats med ett namn samt vad NPCn kan s�ga n�r den
     * tr�ffar spelaren.
     * 
     * @param name Namnet p� NPCn
     * @param startX Den x-koordinat som NPCn skapas p�.
     * @param startY Den y-koordinat som NPCn skapas p�.
     * @param dialog En str�ng med vad NPCn s�ger till den den m�ter.
     */
    public NPC(String name, int startX, int startY, String dialog){
        super(name,startX,startY);
        this.dialog = dialog;
    }
    
    /**
     * Den simulerar att NPCn g�r i en slumpm�ssig riktning �st,v�st,
     * norr eller syd. Om NPCn f�rs�ker g� utanf�r kartan kommer 
     * den inte att f�rflyttas.
     */
    public void moveNPC(int xMax,int yMax){
        Random rand = new Random();
        int x = rand.nextInt(3);
        int y = rand.nextInt(3);
        
        //jag hittade inget s�tt att slumpa mellen -1 och 1 s� det blev s�h�r.
        //Den slumpar mellan 0 och 2 och s�tter v�rden utifr�n dem.
        if(x==0){x = -1;}
        else if(x==1){x = 0;}
        else if(x==2){x = 1;}
        if(y==0){y = -1;}
        else if(y==1){y = 0;}
        else if(y==2){y = 1;}
        //System.out.println(x+" "+y);
        
        //s�tt ny position
        int nextX = x+getX();
        int nextY = y+getY();
        //System.out.println(nextX+" "+nextY);
        if(nextX <= 0 || nextY <= 0 || nextX>=xMax || nextY>=yMax){
            return;
        }
        else{
            moveActor(nextX,nextY); //Flytta p� NPCn.
        }
    }
    
    /**
     * Skriver ut dialogstr�ngen som anges i konstruktorn.
     */
    public void startDialog(){
        if(met == false){
            System.out.println(getName()+": "+dialog);
            met = true;
        }
    }
       
}


