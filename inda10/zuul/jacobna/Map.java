import java.awt.image.*;
import java.io.File;
import javax.imageio.*;
import java.io.IOException;
/**
 * Map �r klassen som behandlar allt som har med spelarv�rlden att g�ra.
 * Klassen skapar v�rlden enligt en angiven bild som man sj�lv kan rita.
 * Den skapar d� en tile per pixel, och varje tile f�r en viss typ
 * beroende p� vilken f�rg pixeln har. V�rlden anges i slut�ndan av en
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
     * Den anv�nder ImageParser f�r att initera alla instansvariabler.
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
     * Metoden j�mf�r den givna f�rgen med de olika typernas f�rg
     * och returnerar vilken typ som korresponderar mot den givna
     * f�rgen.
     * 
     * @param RGB Pixelns f�rg.
     * @return F�rgens korresponerande typ.
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
     * Skapar en 2D array av tiles som utg�r sj�lva v�rlden.
     * 
     * @return En 2D-array inneh�llande alla tiles p� dess korrekta x,y position.
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
     * Metod f�r att komma �t 2D arrayen som utg�r v�rlden.
     */
    public Tile[][] getMapGrid(){
        return MapGrid;
    }
    
    /**
     * Metod f�r att komma �t bildens bredd.
     */
    public int getxMax(){
        return xMax;
    }
    
    /**
     * Metod f�r att komma �t bildens h�jd.
     */
    public int getyMax(){
        return yMax;
    }
    
    /**
     * Metod f�r att komma �t en 2D array som inneh�ller f�rgen p� varje pixel.
     */
    public int[][] getMapArray(){
        return MapArray;
    }

    
    



}
