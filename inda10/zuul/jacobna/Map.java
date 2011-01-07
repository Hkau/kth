import java.awt.image.*;
import java.io.File;
import javax.imageio.*;
import java.io.IOException;
/**
 * Map är klassen som behandlar allt som har med spelarvärlden att göra.
 * Klassen skapar världen enligt en angiven bild som man själv kan rita.
 * Den skapar då en tile per pixel, och varje tile får en viss typ
 * beroende på vilken färg pixeln har. Världen anges i slutändan av en
 * 2D array av tiles.
 * 
 * @author Jacob Norlin Andersson
 * @version 2010.12.03
 */
public class Map
{
    // instance variables - replace the example below with your own
    private int xMax;
    private int yMax;
    private int area;
    private Tile[][] MapGrid;
    private int[][] MapArray;

    /**
     * Kontruktorn till Map
     * 
     * Den använder ImageParser för att initera alla instansvariabler.
     */
    public Map(String source)
    {
        ImageParser ip = new ImageParser(source);
        MapArray = ip.readSequence();
        yMax = MapArray.length;
        xMax = MapArray.length;
        area = xMax*yMax;
        //ip.print2Darray(MapArray);
        
    }
    
    /**
     * Metoden jämför den givna färgen med de olika typernas färg
     * och returnerar vilken typ som korresponderar mot den givna
     * färgen.
     * 
     * @param RGB Pixelns färg.
     * @return Färgens korresponerande typ.
     */
    private Type determineType(int RGB){
        Type type = null;
        if(RGB == -16735512){
            type = new Type(false,"Water");
            return type;
        }
        else if(RGB == -14503604){type = new Type(true,"Grass");return type;}
        else if(RGB == -4621737){type = new Type(true,"Dirt");return type;}
    return type;
    }
    
    /**
     * Skapar en 2D array av tiles som utgör själva världen.
     * 
     * @return En 2D-array innehållande alla tiles på dess korrekta x,y position.
     */
    public Tile[][] createMap(){
        int x = 0;
        int y = 0;
        MapGrid = new Tile[xMax][yMax];
        for(int i=0; i<area; i++){
            
            int color = MapArray[x][y];
            Type tileType = determineType(color);
            Tile newTile = new Tile(tileType, x,y, tileType.getName());//Skapa tilen.
            MapGrid[x][y]=newTile;
            x++;
            //System.out.println("x: "+x+" "+"y: "+y+" "+"area: "+area);
            if(x>=xMax){
                y+=1;
                x=0;
            }
            
        }
        return MapGrid;
    }
    
    /**
     * Metod för att komma åt 2D arrayen som utgör världen.
     */
    public Tile[][] getMapGrid(){
        return MapGrid;
    }
    
    /**
     * Metod för att komma åt bildens bredd.
     */
    public int getxMax(){
        return xMax;
    }
    
    /**
     * Metod för att komma åt bildens höjd.
     */
    public int getyMax(){
        return yMax;
    }
    
    /**
     * Metod för att komma åt en 2D array som innehåller färgen på varje pixel.
     */
    public int[][] getMapArray(){
        return MapArray;
    }

    
    



}
